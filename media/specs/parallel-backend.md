# Parallel back end design document

The `--jobs`/`-j` cli option has existed for a long time, but was never wired up to anything: `CliOptions::compileJobCount`
was parsed and then ignored, so every build compiled strictly serially. This document describes how the back end is
parallelized across source files with a thread pool, and which shared state had to be made safe for it.

## Why the back end

The compiler pipeline (see [compile-stages.md](compile-stages.md)) has three phases per source file:

- **Front end** — lexer, parser, AST builder, import collector, symbol table builder
- **Middle end** — type checker (pre + post)
- **Back end** — IR generator, IR optimizer, object emitter

Only the back end is embarrassingly parallel across source files:

- Every `SourceFile` already owns its own `llvm::LLVMContext`, `llvm::IRBuilder`, `llvm::TargetMachine`, `llvm::Module`
  and `typeToLLVMTypeMapping`. Two source files therefore never touch the same LLVM data structure.
- References to symbols of other source files are emitted as *declarations* into the local module
  (`module->getOrInsertFunction` / `module->getOrInsertGlobal`). No source file reads the IR of another one.
- The AST, the symbol tables and the type registry are inputs to the back end, and are fully populated once the middle
  end is done.

The front end cannot be parallelized as easily, because the import collector discovers dependencies while it runs and
files feed symbols into each other. The middle end is a fixpoint iteration over a dependency graph that may contain
cycles. Both are worth a separate look; this change deliberately stops at the back end.

The old back end recursed over the dependency graph (`runBackEnd` calling itself for every dependency first). That
recursion looked like a data dependency but is not one — it only exists so that the graph is walked cycle-safely. It is
replaced by an explicit, deterministic worklist.

## Design

```
runBackEnd()                                          [main thread]
 ├─ collectBackEndSourceFiles()                       flatten the dependency graph, dependencies first
 ├─ for each source file: runBackEndForThisFile()     [worker threads, via ThreadPool]
 │    ├─ runIRGenerator()
 │    ├─ runDefaultIROptimizer()
 │    └─ runObjectEmitter()
 └─ for each source file: concludeCompilation()       [main thread, deterministic order]
      ├─ linker.addFileToLinkage(objectFilePath)
      └─ cacheManager.cacheSourceFile()
```

### New utilities

- `src/util/ThreadPool.h` — a fixed-size worker pool. Tasks are `void()` and the submitter blocks in `waitForAll()`.
  Two properties matter for the compiler:
  - Passes report failures by **throwing** (`LexerError`, `SemanticError`, `CompilerError`, ...). An exception escaping
    a worker thread would terminate the process, so tasks are wrapped and the exception is re-thrown on the waiting
    thread.
  - If several tasks fail, the exception of the **task that was submitted first** wins, so the reported error does not
    depend on the scheduling. Queued tasks that have not started yet are dropped after the first failure.
- `src/util/Concurrency.h` — `ParallelSection`, `ConditionalLock` and the shared `symbolRegistryMutex`.
  `ConditionalLock` only takes its mutex while a `ParallelSection` is active. The process-wide caches it protects sit on
  the hottest paths of the (single-threaded) front end and middle end, where an unconditional lock would be pure
  overhead; outside of a parallel section the lock degenerates to a relaxed atomic load plus a well-predicted branch.

The pool is owned by the `GlobalResourceManager` and created lazily, so a `-j 1` build never spawns a thread.

### Job count

`GlobalResourceManager::getCompileJobCount()` resolves the cli option:

| `-j` value | Result |
| --- | --- |
| `0` (default) | `std::thread::hardware_concurrency()`, or 1 if it cannot be determined |
| `1` | serial back end, no threads are spawned |
| `n > 1` | `n` workers |

The job count is additionally capped at the number of source files.

### When the back end stays serial

- **LTO** (`--lto`). All source files share `GlobalResourceManager::ltoContext` and `ltoModule`, and `BitcodeLinker`
  merges the modules of all files. This has to stay serial until every file gets its own context and the modules are
  moved into the LTO context explicitly.
- **Dump modes** (`-ir`, `-s`, `--dump-obj`). Their output goes to the console/output dir and its ordering is part of
  what the user reads. `--abort-after-dump` also expects to stop the pipeline at a well-defined point.
- **Single source file**, since there is nothing to overlap.

## Shared state

Everything the back end touches outside its own `SourceFile` was audited. The result:

### Safe as-is (read-only during the back end)

| State | Written by |
| --- | --- |
| `astNodeAlloc`, `nodeToNodeId` | AST builder |
| `compileTimeStringValues` | AST builder, type checker |
| `nextCustomTypeId` | AST builder (and already an atomic) |
| `sourceFiles`, ASTs, symbol tables, scopes | front end, middle end |
| `sourceLinkerFlags`, `sourceAdditionalSourcePaths` | symbol table builder |

### Made safe by this change

| State | Problem | Fix |
| --- | --- | --- |
| `TypeRegistry::types` | Deriving a type (`QualType::toPtr`, ...) can insert into the registry and invalidate concurrent lookups | `ConditionalLock` on a dedicated mutex |
| `FunctionManager::lookupCache` (+ counters) | `FunctionManager::lookup` is called by the IR generator (copy ctor lookup) | `ConditionalLock` on `symbolRegistryMutex` |
| `StructManager` / `InterfaceManager` matchers | `Type::toLLVMType` lowers struct/interface types through `QualType::getStruct`/`getInterface`, which mutate lookup caches, manifestation lists and `used` flags | `ConditionalLock` on `symbolRegistryMutex` (recursive, because struct matching recurses into struct and interface matching) |
| `TypeNameDisambiguator::claimedTypeIds` | Reached from `NameMangling::mangleType` | `ConditionalLock` on a dedicated mutex |
| `ExternalLinkerInterface::linkLibMath` | `OpRuleConversionManager` requests libm linkage while emitting operators | `std::atomic<bool>` |
| `GlobalResourceManager::abortCompilation` | Written by the dump logic, which runs inside the back end | `std::atomic<bool>` |
| `ExternalLinkerInterface::linkedFiles` | `runObjectEmitter` used to append to it, which would make the link order depend on the scheduling | Registration moved to `concludeCompilation`, which runs serially in dependency order |

Lock ordering is always `symbolRegistryMutex` → `TypeRegistry::typesMutex` /
`TypeNameDisambiguator::claimedTypeIdsMutex`; the inner locks never acquire the outer one, so there is no cycle.

## Determinism

A parallel build has to produce byte-identical output to a serial one:

- The **linker input order** is unchanged, because object files are registered in `concludeCompilation`, which the main
  thread drives over the same dependency-first list the old recursion produced.
- The **compilation cache** is written from the same serial pass.
- **Per-file compiler output** (`compilerOutput.irString`, `asmString`, timings, warnings) lives on the `SourceFile`, so
  it is unaffected.
- **Error reporting** is stable, see the `ThreadPool` note about the first failing task above.

### Known limitation

`TypeNameDisambiguator` hands out a suffix based on the order in which type names are *claimed*. In practice every
struct/interface name is claimed while the type checker builds signatures and diagnostics, so the back end only reads
back existing claims. A name claimed for the very first time during back-end name mangling would get a
scheduling-dependent index — consistent within one compilation, but possibly different between runs. Ranking claimants
by type id over the complete, front-end-known set of types would remove this residual nondeterminism; that is a
follow-up.

Independently of this change, two source files with the same file name in different directories map to the same object
file path (`outputDir / filePath.filename()`). That was already a last-one-wins bug serially; with a parallel back end
it becomes a concurrent write. It should be fixed by deriving the object file name from the cache key or the full path.

## Testing

- `test/unittest/UnitThreadPool.cpp` covers the pool (task execution, deterministic exception selection, reuse after a
  failure) and `ConditionalLock`/`ParallelSection`.
- The integration test runner compiles with `-j 1`. The test cases are already spread over processes by
  `gtest-parallel` (`spicetest_parallel` target), so an additional intra-process parallel back end would only
  oversubscribe the machine, and the reference outputs stay strictly deterministic this way.
- The races are best flushed out with the existing thread sanitizer build option
  (`-DSPICE_TSAN=ON`, see `Options.cmake`) on a project with many imports.
