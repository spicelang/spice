---
name: spice-dump
description: Dump compiler intermediate output (LLVM IR, assembly, AST, CST, symbol table, types) for a Spice source or test-input file using the `spice` CLI dump flags. Use when the user wants to inspect what the compiler generates for a `.spice` file, debug codegen, or regenerate reference dumps.
---

# Dump Spice compiler output

The `spice` compiler (built at `cmake-build-debug/src/spice`, see the
`spice-build` skill) can dump every stage of compilation via flags on the
`build` / `run` / `test` subcommands.

## Quick start

```sh
SPICE=cmake-build-debug/src/spice

# Dump LLVM IR for a source file (unoptimized)
$SPICE build -O0 -ir path/to/source.spice

# Dump assembly
$SPICE build -O0 -s path/to/source.spice

# Inspect IR for an existing test input
$SPICE build -O0 -ir test/test-files/irgenerator/for-loops/success-for-loop/source.spice
```

Many test-case dirs reference IR at `-O2`/`-O3` — pass the matching opt level to
reproduce those (e.g. `-O2 -ir` to compare against `ir-code-O2.ll`).

## Dump flags (build / run / test subcommands)

| Flag | Aliases | Dumps |
|------|---------|-------|
| `--dump-cst` | `-cst` | Concrete syntax tree (serialized + SVG) |
| `--dump-ast` | `-ast` | Abstract syntax tree (serialized + SVG) |
| `--dump-symtab` | | Serialized symbol tables |
| `--dump-types` | | All used types |
| `--dump-ir` | `-ir` | LLVM IR |
| `--dump-assembly` | `-asm`, `-s` | Assembly |
| `--dump-object-file` | | Object files |
| `--dump-dependency-graph` | | Compile-unit dependency graph |
| `--dump-cache-stats` | | Internal lookup-cache stats |

Control flags (build subcommand):

- `--dump-to-files` — write dumps to files instead of stdout.
- `--abort-after-dump` — stop after the first requested dump (skip the rest of
  compilation/linking). Handy for fast IR/AST inspection without producing a binary.

Useful companions:

- Optimization: `-O0`/`-O1`/`-O2`/`-O3`/`-Os`/`-Oz` (IR/asm differ per level).
- `-d` / `--debug-output` — verbose compiler debug output.
- `--target`, `--target-arch`, `--target-os` — cross-target IR/assembly
  (e.g. assembly refs are kept per target like `assembly-linux-aarch64.asm`).
- `--build-mode test -ir` (or the `test` subcommand) — dump IR for the test
  harness variant of a file.

## Common recipes

```sh
SPICE=cmake-build-debug/src/spice
SRC=test/test-files/irgenerator/for-loops/success-for-loop/source.spice

# Just the IR, nothing else, fast
$SPICE build -O0 -ir --abort-after-dump $SRC

# IR at O2 (matches ir-code-O2.ll references)
$SPICE build -O2 -ir --abort-after-dump $SRC

# AST + CST as SVG/text
$SPICE build -ast -cst --abort-after-dump $SRC

# Symbol table and type registry
$SPICE build --dump-symtab --dump-types --abort-after-dump $SRC

# Cross-target assembly
$SPICE build -O2 -s --target-arch aarch64 --target-os linux --abort-after-dump $SRC
```

## Relation to reference tests

The dump filenames above correspond to reference files in
`test/test-files/<group>/<case>/` (e.g. `ir-code.ll`, `ir-code-O2.ll`,
`assembly-linux-amd64.asm`, `symbol-table.json`, `type-registry.out`,
`syntax-tree.dot`). To regenerate those en masse after an intended compiler
change, prefer `spicetest --update-refs` (see the `spice-test` skill) rather
than hand-copying dumps. Use the manual dumps here for ad-hoc inspection and
debugging of a single file.

Run `$SPICE --help` (or `$SPICE build --help`) to see the full, current option list.
