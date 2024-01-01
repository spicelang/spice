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

| Option       | Long                 | Description                                                                                 |
|--------------|----------------------|---------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`     | Print compiler output for debugging.                                                        |
| `-cst`       | `--dump-cst`         | Dump CST as serialized string and SVG image                                                 |
| `-ast`       | `--dump-ast`         | Dump AST as serialized string and SVG image                                                 |
| `-symtab`    | `--dump-symtab`      | Dump serialized symbol tables                                                               |
| `-ir`        | `--dump-ir`          | Dump LLVM-IR                                                                                |
| `-s`, `-asm` | `--dump-assembly`    | Dump Assembly code                                                                          |
| `-b`, `-obj` | `--dump-object-file` | Dump object files                                                                           |
| -            | `--dump-to-files`    | Redirect all dumps to files instead of printing them to the screen                          |
| `-j <n>`     | `--jobs <n>`         | Set number of jobs to parallelize compilation (default is auto)                             |
| `-o`         | `--output`           | Set path for executable output.                                                             |
| `-O<x>`      | -                    | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`        |
| `-m`         | `--build-mode`       | Controls the build mode. Valid values are `debug` and `release`                             |
| `-g`         | `--debug-info`       | Generate debug info to debug the executable in GDB, etc.                                    |
| -            | `--disable-verifier` | Disable LLVM module and function verification (only recommended for debugging the compiler) |
| -            | `--ignore-cache`     | Compile always and ignore the compile cache                                                 |