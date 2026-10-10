---
name: spice-architecture
description: Orientation map of the Spice compiler codebase — the compilation pipeline (lexer → parser → CST → AST → import collector → symbol table → type checker → IR generator → optimizer → object emitter → linker), the key class/file for each stage, and the SourceFile orchestration sequence. Use when navigating the compiler, deciding where a change belongs, or understanding how a stage feeds the next.
---

# Spice compiler architecture

The self-hosted bootstrap compiler in Spice lives in `src/` and is the default
compiler. The host compiler in C++23 lives in `src-host/`; it builds the bootstrap
compiler and the two parallel each other file-for-file. The paths below point at the
host compiler; the bootstrap counterpart of `src-host/<stage>/FooBar.cpp` is
`src/<stage>/foo-bar.spice`.

## Entry & orchestration

- `src-host/main.cpp` → `compileProject()` builds the project and invokes the linker.
- `src-host/SourceFile.cpp` / `.h` orchestrates the per-file pipeline in three phases.
  Each stage is a `run*` method; this is the call sequence to read first:

  **Front end** (`runFrontEnd`): `runLexer` → `runParser` → (`runCSTVisualizer`) →
  `runASTBuilder` → (`runASTVisualizer`) → `runImportCollector` → `runSymbolTableBuilder`

  **Middle end** (`runMiddleEnd`): `runTypeCheckerPre` → `runTypeCheckerPost` →
  (`runDependencyGraphVisualizer`)

  **Back end** (`runBackEnd`): `runIRGenerator` → IR optimizer
  (`runDefaultIROptimizer`, or `runPreLinkIROptimizer`/`runBitcodeLinker`/`runPostLinkIROptimizer` for LTO)
  → `runObjectEmitter` → `concludeCompilation`

- `src-host/CompilerPass.h` — base class for passes (scope tracking, source-file handle).
- `src-host/global/GlobalResourceManager.*` — central owner of source files, caches,
  the type registry, and the linker. `CacheManager`, `RuntimeModuleManager`,
  `TypeRegistry` also live in `src-host/global/`.

## Stages → where the code is

| Stage | Dir / key class | Purpose |
|-------|-----------------|---------|
| Lex + parse | ANTLR-generated `SpiceLexer`/`SpiceParser` (from `src-host/Spice.g4`) | Source → token stream → CST. See `spice-language-feature` skill. |
| CST → AST | `src-host/ast/` — `ASTBuilder` | Builds the AST from the parser's CST |
| AST nodes / visitors | `src-host/ast/ASTNodes.h`, `AbstractASTVisitor.h`, `ParallelizableASTVisitor.h`, `ASTVisitor.h` | Node defs + visitor interfaces |
| Imports | `src-host/importcollector/` — `ImportCollector` | Resolve & recursively load dependencies |
| Symbols | `src-host/symboltablebuilder/` — `SymbolTableBuilder`, `Scope`, `SymbolTable` | Build scopes & symbol tables |
| Types | `src-host/typechecker/` — `TypeChecker` (`TC_MODE_PRE` then `TC_MODE_POST`), `QualType`, `Type` | Type inference/checking, generic instantiation, overload resolution |
| IR gen | `src-host/irgenerator/` — `IRGenerator` (a `ParallelizableASTVisitor`) | AST → LLVM IR |
| IR opt | `src-host/iroptimizer/` — `IROptimizer` | LLVM pass pipeline (incl. LTO) |
| Emit | `src-host/objectemitter/` — `ObjectEmitter` | LLVM module → object/asm |
| Link | `src-host/linker/` — `ExternalLinkerInterface` | Object files → executable/library |
| CLI | `src-host/driver/Driver.*` — `Driver`, `CliOptions` | Subcommands & flags (see `spice-run`/`spice-dump`) |
| Errors | `src-host/exception/`, `src-host/util/CompilerWarning.*` | Diagnostics (see `spice-diagnostics` skill) |
| Visualizers | `src-host/visualizer/` | CST/AST/dependency-graph dumps (the `--dump-*` flags) |
| Models | `src-host/model/` | Cross-stage data (functions, structs, generics, …) |

## Notes

- The type checker runs **twice** (pre = bottom-up to resolve generic
  instantiations; post = top-down, may re-visit). Many "why isn't my type
  resolved" issues are about which pass populates what.
- IR generation uses the *parallelizable* (const) visitor; other passes use the
  mutating `ASTVisitor`.
- `src-host/Spice.g4` is the source of truth for syntax; the parser is **generated at
  build time**, not checked in.
- `media/specs/` holds design notes for language features — useful background
  before changing semantics.
- To see any stage's output for a file, use the `spice-dump` skill
  (`-cst`, `-ast`, `--dump-symtab`, `--dump-types`, `-ir`, `-s`).
