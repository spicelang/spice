---
name: spice-test
description: Run the Spice test suite with `test/run-tests.py` (reference/integration tests in test/test-files, linter tests and the builtin #[test] unit tests of the compiler sources). Use when the user wants to run all tests, a specific suite or case, update reference files, check for memory errors with ASAN, collect coverage, or debug a failing test.
---

# Run Spice tests

`test/run-tests.py` is a standalone Python runner. It builds the compiler from `src/main.spice` with a given compiler
(`--build-compiler`, default: the released stage0 compiler, that `python fetch-stage0.py` downloads into `build/stage0/`,
version pinned in `.github/stage0-version`), or takes an already built one (`--compiler`), then runs all test cases against
it in parallel (`-j`, default: number of CPUs). It drives three kinds of tests:

- **Reference / integration tests** over the directories in `test/test-files/<group>/...`. Each test case dir has a
  `source.spice` plus reference files the runner compares against (see below).
- **Linter tests** (`LinterTests`), run against the `lint` subcommand.
- **Unit tests**: `BootstrapTests.BuiltinTests` builds the compiler sources in test build mode (`--build-mode test`, into
  `build/test-tmp/bootstrap-tests/`) and runs their builtin tests (`#[test]` functions, e.g. in `src/driver.spice`).

Artifacts go to `build/test-tmp/` (`--work-dir`), which is also the working directory of the compiler and the test programs
(it links `test-files/`).

## Running

```sh
# Build the compiler with the stage0 compiler, then run everything
python test/run-tests.py

# Reuse an already built compiler (much faster for iterating, see the spice-build skill)
python test/run-tests.py --compiler build/spice

# List every test name (best way to find an exact filter)
python test/run-tests.py --list

# Filter by suite or case (GoogleTest-style globbing, '-' starts the negative patterns)
python test/run-tests.py --compiler build/spice --filter='IRGeneratorTests.*'
python test/run-tests.py --compiler build/spice --filter='*ForLoop*-*Error*'
```

Reference-test suites (from `test/test-files/<group>`): `CommonTests`, `LexerTests`, `ParserTests`,
`SymbolTableBuilderTests`, `TypeCheckerTests`, `IRGeneratorTests`, `StdTests`, `BenchmarkTests`, `ExampleTests`.

## Flags

- `--update-refs` — overwrite the existing reference files of the selected cases with the actual output. Use after an
  intentional change to the compiler output, then review `git diff test/test-files`. To add a new reference, create the
  empty file first.
- `--asan` — build the compiled test programs with `--sanitizer address` and fail on any AddressSanitizer or LeakSanitizer
  report. Only `cout.out` and `exit-code.out` are compared, since the instrumentation changes the generated code. Tests
  requesting another sanitizer are skipped.
- `--coverage` — compile the test programs with Spice code coverage instrumentation, skipping all reference comparisons.
  Every compiler run works in its output dir, so the gcov data lands per test case under `<work-dir>/tests/`;
  `python coverage.py --only std` (expects `--work-dir build/test-tmp-coverage`) turns it into a std coverage report.
- `--instrument coverage` — build the compiler under test (and its builtin tests) with `-O0 --coverage` instead of
  `-O3 -lto`; `python coverage.py --only compiler` turns the data into an HTML report. `coverage.py` needs `LLVM_COV`
  pointing at `llvm-cov` and generates both reports without `--only`.
- `--instrument asan` — build them with `-O1 --sanitizer address`. Every case fails, for which the compiler prints an
  AddressSanitizer or LeakSanitizer report (weekly `ci-asan.yml` job).
- `--skip-sanitizer-tests` — skip tests exercising language sanitizers.
- `--is-github-actions` — skip cases and checks unsupported on CI (GDB tests, assembly refs, `skip-gh-actions`).
- `--build-only` — only build the compiler under test.
- `--list`, `-v/--verbose` (prints every compiler invocation and its output), `--timeout` (per process, default 1800 s).

## What a reference test checks

The runner invokes the compiler like a user would (`build [// TEST: args] --output ... source.spice`) and checks:
`syntax-tree.dot` and `dependency-graph.dot` (via `--dump-ast` / `--dump-dependency-graph`, taken from the console output),
`symbol-table.json`, `assembly.asm`, `type-registry.out` and `cache-stats.out` (via a separate run with `--dump-to-files`,
only if one of these refs exists; it uses the last opt level with an IR reference, e.g. `-O3` if `ir-code-O3.ll` exists),
`exception.out`, `warning.out`, the IR references (one run per opt level), `cout.out`, `exit-code.out` and `debug.out` (via
GDB). `run-builtin-tests` cases are built with the internal `--test-main` flag (test main without the test build mode). The
compiler prints compile errors and exits with a non-zero exit code; a panic (internal error) always fails the case.
Warnings are taken from the console output, and only the ones of the main source file are compared.

The IR comparison ignores `dso_local` markers: the compiler uses the LLVM C API, which cannot set them yet, while the refs
keep them.

## Environment

The runner sets `SPICE_STD_DIR` and `SPICE_BOOTSTRAP_DIR` to the checkout itself and derives `LLVM_LIB_DIR` /
`LLVM_INCLUDE_DIRS` (needed to link the LLVM bindings) from `LLVM_DIR` or `llvm-config`, unless set. The TPDE test cases
need the TPDE libraries: `TPDE_FLAGS`, or on Linux the ones `python setup-deps.py --tpde` installs into
`std/bindings/tpde/`, where the compiler finds them on its own. Without them, the TPDE test cases are skipped
(`TPDE_FLAGS=` disables them).

CI (`ci.yml`, all platforms) builds the compiler with the stage0 compiler, runs all test suites against it without a
filter, and bootstraps the compiler to a fixed point.

## Debugging a failing case

```sh
# See the exact compiler invocations and outputs of one case
python test/run-tests.py --compiler build/spice --filter='TypeCheckerTests.foo_bar' -v

# Re-run one of them under gdb, from the work dir (test case paths are relative to it)
cd build/test-tmp && gdb --args ../spice build --test-mode --ignore-cache ./test-files/typechecker/foo/bar/source.spice

# Memory errors in the compiler: run the case against an ASAN-instrumented compiler
python test/run-tests.py --instrument asan --filter='TypeCheckerTests.foo_bar'
```

## Reference files in a test-case directory

Common files the runner reads/compares (presence is optional per case):

- `source.spice` (+ `source1.spice`, …) — input program
- `cout.out` — expected stdout; `exception.out` / `warning.out` — expected errors/warnings
- `exit-code.out` — expected process exit code
- `ir-code.ll`, `ir-code-O2.ll`, `ir-code-O3.ll`, … — expected LLVM IR per opt level
- `assembly-linux-amd64.asm`, `assembly-linux-aarch64.asm` — expected assembly
- `symbol-table.json`, `type-registry.out` — expected symbol/type dumps
- `syntax-tree.dot` / `dependency-graph.dot` — expected graphs
- Platform/skip markers: `*-windows.*`, `*-macos.*`, `*-linux-aarch64.*`, `skip-windows`, `skip-macos`,
  `skip-gh-actions`, `skip-without-tpde`, `disabled`, `run-builtin-tests`, `cli-flags.txt`, `debug.gdb`

To produce these dumps manually for a single input, use the `spice-dump` skill.
