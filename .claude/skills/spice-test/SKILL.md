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
`ExampleTests`, `BootstrapCompilerTests`.

## Custom test-runner flags

These are `spicetest`'s own flags (not GoogleTest):

- `--update-refs` — regenerate/overwrite reference files from current output.
  Use after an intentional change to compiler output, then review the diff.
- `--run-benchmarks` — also run benchmark cases and check baselines.
- `--leak-detection` — wrap tests in valgrind to detect leaks.
- `--skip-sanitizer-tests` — skip tests exercising language sanitizers.
- `--is-github-actions` — skip cases unsupported on CI.
- `--verbose` — extra runner debug output.

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
  `skip-gh-actions`, `run-builtin-tests`, `cli-flags.txt`

To produce these dumps manually for a single input, use the `spice-dump` skill.
