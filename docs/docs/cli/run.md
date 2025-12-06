---
title: Run subcommand
---

The `run` subcommand can be used to compile your Spice project to an executable file and run it immediately.

## Usage
=== "Long form"
    Use the `run` subcommand by executing:
    ```sh
    $ spice run [options] <main-source-file>
    ```
=== "Short form"
    Use the `run` subcommand by executing:
    ```sh
    $ spice r [options] <main-source-file>
    ```

## Options
You can apply following options to the `run` subcommand:

| Option       | Long                      | Description                                                                                              |
|--------------|---------------------------|----------------------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                                     |
| `-cst`       | `--dump-cst`              | Dump CST as serialized string and SVG image                                                              |
| `-ast`       | `--dump-ast`              | Dump AST as serialized string and SVG image                                                              |
| -            | `--dump-symtab`           | Dump serialized symbol tables                                                                            |
| -            | `--dump-types`            | Dump all used types                                                                                      |
| -            | `--dump-cache-stats`      | Dump stats for compiler-internal lookup caches                                                           |
| `-ir`        | `--dump-ir`               | Dump LLVM-IR                                                                                             |
| `-s`, `-asm` | `--dump-assembly`         | Dump Assembly code                                                                                       |
| -            | `--dump-object-file`      | Dump object files                                                                                        |
| -            | `--dump-dependency-graph` | Dump compile unit dependency graph                                                                       |
| `-j <n>`     | `--jobs <n>`              | Set number of jobs to parallelize compilation (default is auto)                                          |
| `-o`         | `--output`                | Set path for executable output.                                                                          |
| `-O<x>`      | -                         | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`                     |
| `-m`         | `--build-mode`            | Controls the build mode. Valid values are `debug` and `release`                                          |
| `-g`         | `--debug-info`            | Generate debug info to debug the executable in GDB, etc.                                                 |
| -            | `--sanitize`              | Enable instrumentation for sanitizer. Valid values are `none`, `address`, `thread`, `memory` and `type`. |
| -            | `--disable-verifier`      | Disable LLVM module and function verification (only recommended for debugging the compiler)              |
| -            | `--ignore-cache`          | Compile always and ignore the compile cache                                                              |
| -            | `--use-lifetime-markers`  | Generate lifetime markers to enhance optimizations                                                       |
| -            | `--use-tbaa-metadata`     | Generate metadata for type-based alias analysis to enhance optimizations                                 |