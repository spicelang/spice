---
name: spice-test
description: Run the Spice `spicetest` suite (GoogleTest-based integration + unit tests). Use when the user wants to run all tests, a specific suite or case, update reference files, check for memory leaks with valgrind, or debug a failing test.
---

# Run Spice tests

`spicetest` is a GoogleTest binary built from `test/`. It drives two kinds of
tests:

- **Reference / integration tests** parameterized over directories in
  `test/test-files/<group>/...`. Each test case dir has a `source.spice` plus
  reference files the runner compares against (see below).
- **Unit tests** in `test/unittest/` (e.g. `BlockAllocatorTest`, `CommonUtilTest`).

Build it first (see the `spice-build` skill):
`cmake --build cmake-build-debug --target spicetest`.

The binary lands at `cmake-build-debug/test/spicetest`.

## Running

```sh
# All tests
cmake-build-debug/test/spicetest

# List every test name (best way to find an exact filter)
cmake-build-debug/test/spicetest --gtest_list_tests

# Filter by suite or case (GoogleTest globbing)
cmake-build-debug/test/spicetest --gtest_filter='IRGeneratorTests*'
cmake-build-debug/test/spicetest --gtest_filter='CommonUtilTest.*'
cmake-build-debug/test/spicetest --gtest_filter='*ForLoop*'
```

Reference-test suites (instantiated from `test/test-files/<group>`):
`CommonTests`, `LexerTests`, `ParserTests`, `SymbolTableBuilderTests`,
`TypeCheckerTests`, `IRGeneratorTests`, `StdTests`, `BenchmarkTests`,
`ExampleTests`.

## Custom test-runner flags

These are `spicetest`'s own flags (not GoogleTest):

- `--update-refs` — regenerate/overwrite reference files from current output.
  Use after an intentional change to compiler output, then review the diff.
- `--run-benchmarks` — also run benchmark cases and check baselines.
- `--asan` — build the compiled test programs with `--sanitizer address` and
  fail on any AddressSanitizer or LeakSanitizer report. Only `cout.out` and
  `exit-code.out` are compared, since the instrumentation changes the generated
  code. Tests requesting another sanitizer are skipped.
- `--skip-sanitizer-tests` — skip tests exercising language sanitizers.
- `--is-github-actions` — skip cases unsupported on CI.
- `--verbose` — extra runner debug output.

`spicetest` only tests the host compiler. The bootstrap compiler has its own
runner, see below.

## Bootstrap compiler tests (`test/run-tests.py`)

The bootstrap (self-hosted) compiler is tested by a standalone Python runner,
independent of the C++ code, so it keeps working once the host compiler is gone.
It builds the bootstrap compiler from `src/main.spice` with a given compiler
(`--build-compiler`, default: the host compiler found in `build/`,
`cmake-build-release/` or `cmake-build-debug/`), or takes an already built one
(`--compiler`), then runs all reference-test suites against it in parallel
(`-j`, default: number of CPUs). It uses the same test names as `spicetest`
(e.g. `ParserTests.parser_errorExtraneousInput`) and GoogleTest-style filters
(`--filter`, alias `--gtest_filter`). Artifacts go to `build/test-tmp/`
(`--work-dir`), which is also the working directory of the compiler and the
test programs (it links `test-files/`).

It invokes the compiler like a user would (`build [// TEST: args] --output ...
source.spice`) and checks the same references as the host runner:
`syntax-tree.dot` and `dependency-graph.dot` (via `--dump-ast` /
`--dump-dependency-graph`, taken from the console output), `symbol-table.json`,
`assembly.asm`, `type-registry.out` and `cache-stats.out` (via a separate run with
`--dump-to-files`, only if one of these refs exists; it uses the last opt level
with an IR reference, e.g. `-O3` if `ir-code-O3.ll` exists), `exception.out`,
`warning.out`, the IR references (one run per opt level), `cout.out`,
`exit-code.out` and `debug.out` (via GDB). `run-builtin-tests` cases are built
with the internal `--test-main` flag (test main without the test build mode).
The compiler prints compile errors like the host and exits with a non-zero exit
code; a panic (internal error) always fails the case. Warnings are taken from the
console output, and only the ones of the main source file are compared.
`LinterTests` run against the `lint` subcommand. `BootstrapTests.BuiltinTests`
builds the compiler sources in test build mode (`--build-mode test`, into
`test-tmp/bootstrap-tests/`) and runs their builtin tests (`#[test]` functions,
e.g. in `src/driver.spice`).

It sets `SPICE_STD_DIR` and `SPICE_BOOTSTRAP_DIR` to the checkout itself and
derives `LLVM_LIB_DIR` / `LLVM_INCLUDE_DIRS` (needed to link the LLVM bindings)
from `LLVM_DIR` or `llvm-config`, unless set. The TPDE test cases need the TPDE
libraries: `TPDE_FLAGS`, or on Linux the ones `python setup-deps.py --tpde`
installs into `std/bindings/tpde/`, where the compilers find them on their own.
Without them, the TPDE test cases are skipped (`TPDE_FLAGS=` disables them).

Flags:

- `--update-refs` — only updates bootstrap refs (e.g. `ir-code-bootstrap.ll`,
  create the empty file first), so host refs are never overwritten.
- `--asan` / `--coverage` / `--is-github-actions` / `--skip-sanitizer-tests` —
  like for `spicetest`, applied to the compiled test programs.
- `--instrument coverage` — build the bootstrap compiler (and its builtin tests)
  with `-O0 --coverage` instead of `-O3 -lto`. Its gcov data lands in
  `build/test-tmp/bootstrap-compiler/`; `coverage-bootstrap.py` (run from
  `build/`, `LLVM_COV` pointing at `llvm-cov`) turns it into an HTML report.
- `--instrument asan` — build them with `-O1 --sanitizer address`. Every case
  fails, for which the compiler prints an AddressSanitizer or LeakSanitizer
  report (weekly `ci-asan.yml` job).
- `--build-only` — only build the bootstrap compiler.
- `--list`, `-v/--verbose`, `--timeout` (per process, default 1800 s).

Cases where the outputs of both compilers differ follow the bootstrap compiler
(it is the default) and get a `skip-host` marker file instead of an exclusion.
CI (`ci.yml`, all platforms) runs all test suites against it, without a filter.

```sh
python test/run-tests.py                                    # build with the host compiler, run all
python test/run-tests.py --build-compiler build/src-host/spice --filter='ParserTests.*'
python test/run-tests.py --compiler build/test-tmp/bootstrap-compiler/spice --filter='*Union*'
cmake --build cmake-build-debug --target spicetest_bootstrap  # same, builds the host compiler first
```

```sh
# Update refs for one suite after an intended change, then inspect git diff
cmake-build-debug/test/spicetest --gtest_filter='IRGeneratorTests*' --update-refs
git diff test/test-files
```

## Memory-leak / debugging

```sh
# Valgrind on a focused set (full run is slow)
valgrind --leak-check=full cmake-build-debug/test/spicetest --gtest_filter='ParserTests*'

# Or the dedicated build target
cmake --build cmake-build-debug --target spicetest_leakcheck

# Debug a failing case under gdb
gdb --args cmake-build-debug/test/spicetest --gtest_filter='TypeCheckerTests*'
```

## Reference files in a test-case directory

Common files the runner reads/compares (presence is optional per case):

- `source.spice` (+ `source1.spice`, …) — input program
- `cout.out` — expected stdout; `exception.out` / `warning.out` — expected errors/warnings
- `exit-code.out` — expected process exit code
- `ir-code.ll`, `ir-code-O2.ll`, `ir-code-O3.ll`, … — expected LLVM IR per opt level
- `assembly-linux-amd64.asm`, `assembly-linux-aarch64.asm` — expected assembly
- `symbol-table.json`, `type-registry.out` — expected symbol/type dumps
- `syntax-tree.dot` / `parse-tree.dot` / `dependency-graph.dot` — expected graphs
- Platform/skip markers: `*-windows.*`, `*-macos.*`, `skip-windows`,
  `skip-gh-actions`, `skip-bootstrap`, `skip-host`, `skip-without-tpde`, `run-builtin-tests`, `cli-flags.txt`

To produce these dumps manually for a single input, use the `spice-dump` skill.
