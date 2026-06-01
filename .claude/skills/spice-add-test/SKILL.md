---
name: spice-add-test
description: Add a reference/integration test case under test/test-files for the Spice compiler. Covers the directory layout (flat vs grouped suites), the exact reference filenames the runner compares against, control/skip marker files, platform-specific variants, and the --update-refs workflow to generate expected output. Use when adding or modifying compiler test cases.
---

# Add a Spice reference test

Reference tests live under `test/test-files/<suite>/...`. The runner
(`test/TestRunner.cpp`) discovers a test case per leaf directory, compiles its
`source.spice`, and compares each pipeline stage's output against any reference
file present. A reference file that doesn't exist is simply skipped — so a test
"opts in" to checks by including the corresponding files.

## 1. Pick the suite and directory shape

Some suites are **grouped** (`test-files/<suite>/<group>/<case>/`), others are
**flat** (`test-files/<suite>/<case>/`):

- Grouped (`useSubDirs=true`): `symboltablebuilder`, `typechecker`, `irgenerator`, `std`
- Flat: `common`, `lexer`, `parser`, `benchmark`, `examples`, `bootstrap-compiler`

The GoogleTest suite names are `SymbolTableBuilderTests`, `TypeCheckerTests`,
`IRGeneratorTests`, `StdTests`, `CommonTests`, `LexerTests`, `ParserTests`,
`BenchmarkTests`, `ExampleTests`, `BootstrapCompilerTests`. Match an existing
neighbor case for conventions.

## 2. Add `source.spice`

Every case needs `source.spice` (extra files `source1.spice`, `source2.spice`, …
and a `cli-flags.txt` whose first line is passed as runtime args to the built
executable are also supported).

## 3. Add the reference files you want checked

Exact names (from `test/util/TestUtil.h`):

| File | Checks |
|------|--------|
| `parse-tree.dot` | CST |
| `syntax-tree.dot` | AST |
| `symbol-table.json` | Symbol tables (after type checking) |
| `dependency-graph.dot` | Compile-unit dependency graph |
| `type-registry.out` | Registered types |
| `cache-stats.out` | Lookup-cache stats |
| `ir-code.ll` | LLVM IR at `-O0` |
| `ir-code-O1.ll` … `ir-code-O3.ll`, `ir-code-Os.ll`, `ir-code-Oz.ll` | IR at that opt level (runner loops O0–Oz) |
| `assembly.asm` | Emitted assembly (skipped on GitHub Actions) |
| `cout.out` | Program stdout (triggers a real compile+run) |
| `exit-code.out` | Process exit code (triggers a run) |
| `exception.out` | Expected compile error — presence means the test **must** error |
| `warning.out` | Expected compiler warnings |
| `debug.out` + `debug.gdb` | Expected gdb output + the gdb script to run |

Rules of thumb:
- **Success test:** include `cout.out` (and/or `exit-code.out`, `ir-code.ll`).
- **Error test:** include `exception.out` (the runner fails if no error is raised).
- **Warning test:** include `warning.out`.

## 4. Platform-specific variants

For any ref, the runner prefers, in order:
`<stem>-<os>-<arch>.<ext>` → `<stem>-<os>.<ext>` → `<stem>.<ext>`.
Examples: `assembly-linux-amd64.asm`, `assembly-linux-aarch64.asm`,
`ir-code-windows.ll`, `cout-macos.out`. (os ∈ linux/windows/macos, arch ∈ amd64/aarch64).

## 5. Control / skip marker files (empty files in the case dir)

- `disabled` — skip the case entirely
- `skip-gh-actions` — skip on CI
- `skip-windows`, `skip-macos` — skip on that OS
- `run-builtin-tests` — compile with the test entry point and run the file's own tests
- `debug.gdb` — gdb script (used with `debug.out`)

## 6. Generate expected output and run

Build the runner (`spice-test` skill), then let `--update-refs` fill in the
machine-generated refs (IR, asm, symbol table, …) from a `source.spice` you've
written, and **review the diff** before committing:

```sh
cmake --build cmake-build-debug --target spicetest
# Generate/refresh refs for just your new case(s)
cmake-build-debug/test/spicetest --gtest_filter='IRGeneratorTests*YourCase*' --update-refs
git diff test/test-files            # inspect what was generated
# Re-run without update to confirm it passes
cmake-build-debug/test/spicetest --gtest_filter='IRGeneratorTests*YourCase*'
```

Hand-write `cout.out`/`exception.out`/`exit-code.out` when you want to assert
specific expected behavior rather than snapshotting current behavior. Use
`--gtest_list_tests` to find the exact generated test name.
