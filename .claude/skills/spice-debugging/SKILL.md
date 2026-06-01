---
name: spice-debugging
description: Find, diagnose, and fix bugs in the Spice compiler or in a compiled Spice program using GDB, objdump, sanitizers (for both the host C++ compiler and the emitted Spice program via the built-in `--sanitizer` flag), the compiler dump flags, and valgrind. Use when investigating a crash, miscompilation, memory error, or undefined behavior, or when triaging a failing test. Whenever a bug is fixed, this skill also requires extending or adding a test that covers the problematic code path and any related uncovered paths.
---

# Spice — Debug & Reduce Bugs

Uses the compiler at `cmake-build-debug/src/spice` and the test runner at
`cmake-build-debug/test/spicetest` (build them via the `spice-build` skill):

```sh
SPICE=cmake-build-debug/src/spice
SPICETEST=cmake-build-debug/test/spicetest
```

## Two layers can be buggy — identify which first

1. **The host program** — the `spice` compiler / `spicetest` runner, written in
   C++. Bugs here are C++ crashes, leaks, and undefined behavior. Debug them with
   GDB, valgrind, and host-compiler (C++) sanitizers.
2. **The emitted Spice program** — the native binary the compiler produces from a
   `.spice` file. Bugs here are miscompilations or runtime faults in generated
   code. Debug them with the compiler's dump flags, the **built-in
   `--sanitizer`** instrumentation, GDB, and objdump.

A segfault in a compiled Spice program is usually a **codegen bug in the host
compiler**, not a bug in the user's Spice source — keep that distinction in mind.

## 1. Reproduce first

Get the smallest reliable repro before debugging.

- Failing reference test: re-run just that case
  `$SPICETEST --gtest_filter='*<Case>*'` (see the `spice-test` skill).
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
| `--dump-cst` (`-cst`) | Parser produced the wrong tree |
| `--dump-ast` (`-ast`) | AST building / desugaring is wrong |
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
# Host program (compiler / test runner) crash
gdb --args $SPICE build <file.spice>
# then: run, bt, bt full, frame N, print <expr>, list

# A single failing test case
gdb --args $SPICETEST --gtest_filter='*<Case>*'

# The emitted Spice program — compile with debug info (-g) and a known path
$SPICE build -g -o /tmp/prog <file.spice>
gdb --args /tmp/prog <args>
```

`-g` / `--debug-info` makes the compiler emit DWARF debug info for the produced
binary. Useful inside GDB: `bt`, `break <file>:<line>`, `watch <expr>`,
`catch throw` (stop on C++ exceptions in the compiler), `info locals`.

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

### b) Host compiler / test runner (C++)

The CMake build doesn't wire C++ sanitizers in by default; enable them in a
dedicated build dir (see `spice-build`):

```bash
cmake -S . -B cmake-build-asan -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build cmake-build-asan --target spice spicetest
cmake-build-asan/test/spicetest --gtest_filter='*<Case>*'
```

Use `address` for heap/stack/use-after-free, `undefined` for UB, `thread` for
data races (build TSan separately — it is incompatible with ASan). Tune at
runtime: `ASAN_OPTIONS=detect_leaks=1:abort_on_error=1`,
`UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1`.

## 6. Valgrind

For memory errors and leaks in the host program. There are dedicated targets and
a runner flag (there is **no** `--valgrind` flag — use these):

```bash
# Runner flag: wrap each test in valgrind to detect leaks
$SPICETEST --leak-detection --gtest_filter='*<Case>*'

# CMake convenience targets
cmake --build cmake-build-debug --target spicetest_leakcheck   # tests under valgrind
cmake --build cmake-build-debug --target spice_leakcheck       # compiler under valgrind

# Ad-hoc run of the compiler or an emitted program
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
  $SPICE build <file.spice>
```

The runner invokes valgrind as
`valgrind -q --leak-check=full --num-callers=100 --error-exitcode=1`, so any leak
or invalid access fails the case. `--track-origins=yes` is worth the slowdown
when chasing uninitialized reads. ASan (faster, catches some stack/global issues)
and valgrind (no rebuild needed) overlap — reach for whichever is already set up.

## 7. Choosing a tool

| Symptom | Reach for |
|---------|-----------|
| Compiler/test crash, need a stack trace | GDB (`catch throw`, `bt full`) |
| Heap corruption / use-after-free / leak in the compiler | host ASan, or valgrind |
| Undefined behavior in the compiler | host UBSan |
| Data race in the compiler | host TSan |
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
   the relevant suite under valgrind (`$SPICETEST --leak-detection
   --gtest_filter='...'`) so the new case is also checked for memory issues. If
   the bug was a runtime fault in generated code, also re-run the repro under the
   matching `--sanitizer`.

Note in the commit/PR which path the bug was on and which additional paths the
new tests now cover.

## Related skills

- `spice-build` — Debug / sanitizer / leakcheck builds of the compiler.
- `spice-run` — compile/run a `.spice` file, including `-g` and `--sanitizer`.
- `spice-dump` — full reference for the dump flags.
- `spice-test` / `spice-add-test` — run, add, and update reference tests.
- `spice-architecture` — map a symptom to the compiler stage that owns it.
- `spice-diagnostics` — when the right fix is a new error/warning.
