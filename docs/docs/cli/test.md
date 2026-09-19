---
title: Test subcommand
tags:
  - Command Line Interface
  - Test
---

The `test` subcommand can be used to run individual tests or the whole test suite.

## Usage
=== "Long form"
    Use the `test` subcommand by executing:
    ```sh
    $ spice test [options] <test-source-file>
    ```
=== "Short form"
    Use the `test` subcommand by executing:
    ```sh
    $ spice t [options] <test-source-file>
    ```

## Options
You can apply following options to the `test` subcommand:

| Option       | Long                      | Description                                                                                                                                                                                                                                                                                                         |
|--------------|---------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                                                                                                                                                                                                                                                |
| `-cst`       | `--dump-cst`              | Dump CST as serialized string and SVG image                                                                                                                                                                                                                                                                         |
| `-ast`       | `--dump-ast`              | Dump AST as serialized string and SVG image                                                                                                                                                                                                                                                                         |
| -            | `--dump-symtab`           | Dump serialized symbol tables                                                                                                                                                                                                                                                                                       |
| -            | `--dump-types`            | Dump all used types                                                                                                                                                                                                                                                                                                 |
| -            | `--dump-cache-stats`      | Dump stats for compiler-internal lookup caches                                                                                                                                                                                                                                                                      |
| `-ir`        | `--dump-ir`               | Dump LLVM-IR                                                                                                                                                                                                                                                                                                        |
| `-s`, `-asm` | `--dump-assembly`         | Dump Assembly code                                                                                                                                                                                                                                                                                                  |
| -            | `--dump-object-file`      | Dump object files                                                                                                                                                                                                                                                                                                   |
| -            | `--dump-dependency-graph` | Dump compile unit dependency graph                                                                                                                                                                                                                                                                                  |
| `-j <n>`     | `--jobs <n>`              | Set number of jobs to parallelize compilation (default is auto)                                                                                                                                                                                                                                                     |
| `-O<x>`      | -                         | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2` (default), `-O3`, `-Os`, `-Oz`                                                                                                                                                                                                                      |
| `-g`         | `--debug-info`            | Generate debug info to debug the executable in GDB, etc. <br> Valid values: `full` (default when the flag is given without a value), `line-only` (DWARF line tables only) and `none`. <br> A level can only be attached to the long form (e.g. `--debug-info=line-only`); `-g` is an alias for `--debug-info=full`. |
| `-b`         | `--build-var`             | Add build variable to parametrize the compiled program (e.g. -v key=value)                                                                                                                                                                                                                                          |
| -            | `--sanitize`              | Enable instrumentation for sanitizer. <br> Valid values: `none` (default), `address`, `thread`, `memory` and `type`.                                                                                                                                                                                                |
| -            | `--coverage`              | Instrument code for coverage analysis (gcov-compatible `.gcno`/`.gcda` output). Implies `--debug-info=line-only` if no debug info level was requested explicitly, rejects an explicit `--debug-info=none`, and is incompatible with `-lto`.                                                                         |
| -            | `--disable-verifier`      | Disable LLVM module and function verification (only recommended for debugging the compiler)                                                                                                                                                                                                                         |
| -            | `--ignore-cache`          | Compile always and ignore the compile cache                                                                                                                                                                                                                                                                         |
| -            | `--use-lifetime-markers`  | Generate lifetime markers to enhance optimizations                                                                                                                                                                                                                                                                  |
| -            | `--use-tbaa-metadata`     | Generate metadata for type-based alias analysis to enhance optimizations                                                                                                                                                                                                                                            |
| -            | `--keep-frame-pointers`   | Set up a frame pointer in every function, so profilers can walk the stack                                                                                                                                                                                                                                           |