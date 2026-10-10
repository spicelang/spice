---
name: spice-debugging
description: Find, diagnose, and fix bugs in the Spice compiler or in a compiled Spice program using GDB, objdump, sanitizers (for both the compiler itself and the emitted Spice program, via the built-in `--sanitizer` flag), the compiler dump flags, and valgrind. Use when investigating a crash, miscompilation, memory error, or undefined behavior, or when triaging a failing test. Whenever a bug is fixed, this skill also requires extending or adding a test that covers the problematic code path and any related uncovered paths.
---

# Spice — Debug & Reduce Bugs

Uses the compiler at `build/spice` and the test runner `test/run-tests.py` (build the compiler via the `spice-build`
skill, `python build.py --build-type Debug` for debug info):

```sh
SPICE=build/spice
```

## Two layers can be buggy — identify which first

1. **The compiler** — the `spice` executable, written in Spice (`src/`). Bugs here are crashes, panics, leaks and
   miscompilations of the compiler itself. Debug them with GDB, valgrind, and an ASAN-instrumented compiler. Remember that
   the compiler is itself a compiled Spice program: a crash in it can also be a codegen bug of the compiler that built it.
2. **The emitted Spice program** — the native binary the compiler produces from a
   `.spice` file. Bugs here are miscompilations or runtime faults in generated
   code. Debug them with the compiler's dump flags, the **built-in
   `--sanitizer`** instrumentation, GDB, and objdump.

A segfault in a compiled Spice program can be either: a **codegen bug in the
compiler**, or a genuine bug in the **user's Spice source** (e.g. a null
dereference, out-of-bounds access, or use-after-free written in Spice). Don't
assume one or the other — determine which it is. The dumps and `--sanitizer`
help here: if the IR/assembly faithfully implements what the Spice source says
and the fault is an invalid operation the source actually requests, it's a
source bug; if the emitted code diverges from the source's intent, it's a
codegen bug.

## 1. Reproduce first

Get the smallest reliable repro before debugging.

- Failing reference test: re-run just that case with the compiler invocations printed
  `python test/run-tests.py --compiler $SPICE --filter='*<Case>*' -v` (see the `spice-test` skill).
- Standalone file: `$SPICE build <file.spice>` / `$SPICE run <file.spice>` (see
  the `spice-run` skill).
- Shrink the `.spice` input until the symptom is minimal — this almost always
  localizes which compiler stage is at fault (see `spice-architecture`).

## 2. Inspect compiler output (dumps)

When the emitted program misbehaves, look at what each stage produced. Confirm
the current flag list with `$SPICE build --help`. See the `spice-dump` skill for
the full reference; the debugging-relevant flags:

| Flag (aliases) | Inspect it for |
|----------------|----------------|
| `--dump-ast` (`-ast`) | Parser produced the wrong tree / desugaring is wrong |
| `--dump-symtab` | Wrong symbol resolution, scoping |
| `--dump-types` | Type registration / mangling issues |
| `--dump-ir` (`-ir`) | Wrong LLVM IR — most codegen bugs surface here |
| `--dump-assembly` (`-asm`, `-s`) | IR is right but lowering/asm is wrong |
| `--dump-object-file` | Inspect the object before linking |
| `--dump-dependency-graph` | Import / compile-unit ordering bugs |

Companions worth knowing while debugging:

- `--dump-to-files` writes dumps to files instead of stdout (easier to diff).
- `--abort-after-dump` stops right after the dump (fast IR/AST inspection).
- `-d` / `--debug-output` adds verbose compiler-internal tracing.
- `--disable-verifier` is *only* for narrowing down where invalid IR is produced
  — never leave it on for a fix; the verifier is what catches the bug.
- Pass the matching opt level (`-O0`..`-O3`, `-Os`, `-Oz`) so the dump matches
  the reference you are comparing against.

Reference files to diff against live in `test/test-files/<group>/<case>/`, e.g.
`syntax-tree.dot`, `symbol-table.json`, `type-registry.out`, `ir-code.ll`,
`ir-code-O2.ll`, `assembly-linux-amd64.asm`, `cout.out`, `exception.out`,
`warning.out`, `exit-code.out`. See the `spice-dump` / `spice-add-test` skills.

## 3. GDB

Build with debug info so symbols and line numbers are present.

```bash
# Compiler crash (debug build: python build.py --build-type Debug)
gdb --args $SPICE build <file.spice>
# then: run, bt, bt full, frame N, print <expr>, list

# A single failing test case: take the invocation from 'run-tests.py -v' and run it from build/test-tmp
cd build/test-tmp && gdb --args ../spice build --test-mode --ignore-cache ./test-files/<suite>/<case>/source.spice

# The emitted Spice program — compile with debug info (-g) and a known path
$SPICE build -g -o /tmp/prog <file.spice>
gdb --args /tmp/prog <args>
```

`-g` / `--debug-info` makes the compiler emit DWARF debug info for the produced
binary. Useful inside GDB: `bt`, `break <file>:<line>`, `watch <expr>`, `info locals`. A compiler panic exits via
`exit()`, so `break exit` stops there with the stack intact.

## 4. objdump / disassembly

When the emitted program misbehaves at the machine-code level and
`--dump-assembly` isn't enough, inspect the actual binary/object:

```bash
objdump -d -S /tmp/prog      # disassemble with interleaved source (needs -g)
objdump -drwC <object>.o      # disassemble + demangle
objdump -t <object>.o         # symbol table
objdump -r <object>.o         # relocations
objdump -h /tmp/prog          # sections
```

Cross-check `objdump -d` against `$SPICE build -s` to spot where lowering
diverges from the IR. Use `--dump-object-file` to get the object to inspect.

## 5. Sanitizers

### a) Emitted Spice program — built-in `--sanitizer`

The compiler can instrument the generated code directly. This is the first
choice for runtime faults in a compiled Spice program:

```bash
$SPICE run --sanitizer=address <file.spice>   # use-after-free, OOB, leaks
$SPICE run --sanitizer=thread  <file.spice>   # data races
$SPICE run --sanitizer=memory  <file.spice>   # uninitialized reads (Linux only)
$SPICE run --sanitizer=type    <file.spice>   # type-cast / aliasing violations
```

Valid values: `none` (default), `address`, `thread`, `memory`, `type`. Notes
baked into the driver: `memory` is **Linux-only**; `address`/`memory` rely on
lifetime markers and `type` on TBAA metadata, which the compiler enables
automatically (`--use-lifetime-markers`, `--use-tbaa-metadata`). Combine with
`-g` for readable sanitizer stack traces. If the sanitizer fires, the runtime
fault is real; if it stays silent but the program is still wrong, suspect a
logic miscompilation and go back to the IR/assembly dumps.

### b) The compiler itself

The compiler can be built with the same instrumentation. The test runner does it with `--instrument asan`
(`-O1 --sanitizer address`) and fails every case, for which the compiler prints an AddressSanitizer or LeakSanitizer report:

```bash
python test/run-tests.py --instrument asan --filter='*<Case>*'
# The instrumented compiler stays at build/test-tmp/compiler/spice for manual runs
```

Or build it by hand with the stage0 compiler, e.g. with the thread sanitizer for races in the parallel back end:

```bash
SPICE_STD_DIR=$PWD/std SPICE_BOOTSTRAP_DIR=$PWD/src \
  build/stage0/spice build -O1 -g --sanitizer thread --ignore-cache --output build/spice-tsan src/main.spice
```

## 6. Valgrind

For memory errors and leaks in the compiler or an emitted program (there is **no** `--valgrind` flag). For memory errors
and leaks in the compiled test programs, use the `--asan` runner flag instead, which builds them with
`--sanitizer address`:

```bash
# Runner flag: build and run each test program with AddressSanitizer
python test/run-tests.py --compiler $SPICE --asan --filter='*<Case>*'

# Ad-hoc run of the compiler or an emitted program
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
  $SPICE build <file.spice>
```

With `--asan`, the runner fails a case on any AddressSanitizer or LeakSanitizer report of the compiled program. For
valgrind, `--track-origins=yes` is worth the slowdown when chasing uninitialized reads, which ASan does not detect. ASan
(faster, catches stack/global issues) and valgrind (no rebuild needed) overlap — reach for whichever is already set up.

## 7. Choosing a tool

| Symptom | Reach for |
|---------|-----------|
| Compiler crash or panic, need a stack trace | GDB (`bt full`), the panic's own stack trace |
| Heap corruption / use-after-free / leak in the compiler | `run-tests.py --instrument asan`, or valgrind |
| Data race in the compiler | compiler built with `--sanitizer thread` |
| Crash / bad values in a compiled Spice program | `--sanitizer=address` (+`-g`), then dumps → objdump/GDB |
| Wrong intermediate representation | the `--dump-*` flags |

## 8. After fixing a bug — REQUIRED: extend test coverage

**A fix is not complete until it is covered by a test.** Every time you fix a
bug, you MUST:

1. **Add or extend a test that fails before the fix and passes after it**,
   exercising the exact code path that was broken. Use the minimal repro from
   step 1 as the basis — see `spice-add-test` for the directory layout and
   reference-file conventions, and `spice-test` for running and `--update-refs`.
2. **Cover the related, previously-untested paths too** — the sibling branches,
   edge cases, and error paths around the bug (e.g. the empty/zero/overflow case,
   the alternate operand order, the failure/diagnostic path). Bugs cluster; a
   missed branch usually has untested neighbors.
3. **Verify the new test catches the regression**: confirm it fails on the
   pre-fix code (temporarily revert the fix if needed) and passes after, then run
   the relevant suite with AddressSanitizer (`python test/run-tests.py --compiler $SPICE --asan
   --filter='...'`) so the new case is also checked for memory issues. If
   the bug was a runtime fault in generated code, also re-run the repro under the
   matching `--sanitizer`.

Note in the commit/PR which path the bug was on and which additional paths the
new tests now cover.

## Related skills

- `spice-build` — Debug builds of the compiler.
- `spice-run` — compile/run a `.spice` file, including `-g` and `--sanitizer`.
- `spice-dump` — full reference for the dump flags.
- `spice-test` / `spice-add-test` — run, add, and update reference tests.
- `spice-architecture` — map a symptom to the compiler stage that owns it.
- `spice-diagnostics` — when the right fix is a new error/warning.
