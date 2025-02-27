---
title: Install subcommand
---

The `install` subcommand can be used to compile your Spice project to an executable file and copy it to a binary directory in the PATH variable.

## Usage
=== "Long form"
    Use the `install` subcommand by executing:
    ```sh
    $ spice install [options] <main-source-file>
    ```
=== "Short form"
    Use the `install` subcommand by executing:
    ```sh
    $ spice i [options] <mail-source-file>
    ```

## Options
You can apply following options to the `install` subcommand:

| Option       | Long                      | Description                                                                          |
|--------------|---------------------------|--------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                 |
| `-cst`       | `--dump-cst`              | Dump CST as serialized string and SVG image                                          |
| `-ast`       | `--dump-ast`              | Dump AST as serialized string and SVG image                                          |
| -            | `--dump-symtab`           | Dump serialized symbol tables                                                        |
| -            | `--dump-types`            | Dump all used types                                                                  |
| -            | `--dump-cache-stats`      | Dump stats for compiler-internal lookup caches                                       |
| `-ir`        | `--dump-ir`               | Dump LLVM-IR                                                                         |
| `-s`, `-asm` | `--dump-assembly`         | Dump Assembly code                                                                   |
| -            | `--dump-object-file`      | Dump object files                                                                    |
| -            | `--dump-dependency-graph` | Dump compile unit dependency graph                                                   |
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                 |
| `-j <n>`     | `--jobs <n>`              | Set number of jobs to parallelize compilation (Default is auto)                      |
| `-o`         | `--output`                | Set path for executable output.                                                      |
| `-O<n>`      | -                         | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz` |
| `-m`         | `--build-mode`            | Controls the build mode. Valid values are `debug` and `release`                      |
| -            | `--ignore-cache`          | Compile always and ignore the compile cache                                          |
| -            | `--use-lifetime-markers`  | Generate lifetime markers to enhance optimizations                                   |