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
- `--leak-detection` — wrap tests in valgrind to detect leaks.
- `--skip-sanitizer-tests` — skip tests exercising language sanitizers.
- `--is-github-actions` — skip cases unsupported on CI.
- `--verbose` — extra runner debug output.
- `--bootstrap` — bootstrap mode: first build the bootstrap compiler
  (`src-bootstrap/main.spice`) with the in-process host compiler into
  `test-tmp/bootstrap-compiler/`, then run the reference-test suites against it
  (see below).
- `--bootstrap-compiler=<path>` — bootstrap mode with an already built bootstrap
  compiler, skipping the build.
- `--bootstrap-build-only` — only build the bootstrap compiler into
  `test-tmp/bootstrap-compiler/`, without running any test case. Used to build it
  once before running the test cases in parallel against it.
- `--bootstrap-coverage` — bootstrap mode with a bootstrap compiler built with
  `-O0 --coverage` (instead of `-O3 -lto`). Its gcov data lands in
  `test-tmp/bootstrap-compiler/`; `coverage-bootstrap.py` (run from the build
  dir, `LLVM_COV` pointing at `llvm-cov`) turns it into an HTML report.

## Bootstrap mode

`--bootstrap` runs each reference test case by invoking the bootstrap compiler
like a user would (`build [// TEST: args] --output ... source.spice`), run from
`test/` like the normal mode. It needs `SPICE_STD_DIR`, `SPICE_BOOTSTRAP_DIR`
and `LLVM_LIB_DIR` (the build links the LLVM bindings); the
`spicetest_bootstrap` CMake target sets them. Building takes about a minute.
The `spicetest_bootstrap_parallel` target builds the bootstrap compiler once
(`--bootstrap-build-only`), then runs the test cases against it in parallel via
gtest-parallel (`--bootstrap-compiler`), like `spicetest_parallel` does for the
host compiler. CI uses the parallel target.

It is functionally equivalent to the host mode and checks the same references:
`syntax-tree.dot` and `dependency-graph.dot` (via `--dump-ast` /
`--dump-dependency-graph`, taken from the console output), `symbol-table.json`,
`assembly.asm`, `type-registry.out` and `cache-stats.out` (via a separate run with
`--dump-to-files`, only if one of these refs exists; like the host runner, it uses
the last opt level with an IR reference, e.g. `-O3` if `ir-code-O3.ll` exists), `exception.out`,
`warning.out`, the IR references (one run per opt level), `cout.out`,
`exit-code.out` and `debug.out` (via GDB). `run-builtin-tests` cases are built
with the internal `--test-main` flag of the bootstrap compiler (test main without
the test build mode, like the host runner). `--leak-detection` runs the compiled
programs under valgrind, `--coverage` instruments them via the `--coverage` flag
of the bootstrap compiler, and `--is-github-actions` skips the assembly and GDB
checks, all like in host mode. The bootstrap compiler reports errors via a
panic, so the message is taken from the panic output, with test paths rewritten
to `./` like the host prints them. Every expected error must be raised. Warnings
are taken from the console output, and only the ones of the main source file
are compared, like the host test runner does. `LinterTests` run in this mode
only. CI (`ci.yml`, all platforms) runs this mode for all reference test suites,
minus a list of known-failing cases in `BOOTSTRAP_TEST_FILTER*`.
`--update-refs` only updates bootstrap refs (e.g. `ir-code-bootstrap.ll`, create
the empty file first), so host refs are never overwritten with bootstrap output.

```sh
cmake --build cmake-build-debug --target spicetest_bootstrap
# or in parallel (one process per test case):
cmake --build cmake-build-debug --target spicetest_bootstrap_parallel
# or, iterating on a pre-built bootstrap compiler:
cd test && SPICE_STD_DIR=$PWD/../std ../cmake-build-debug/test/spicetest \
  --bootstrap-compiler=$PWD/test-tmp/bootstrap-compiler/spice --gtest_filter='ParserTests*'
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
