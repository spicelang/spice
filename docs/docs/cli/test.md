---
title: Test subcommand
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

| Option       | Long                 | Description                                                                                 |
|--------------|----------------------|---------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`     | Print compiler output for debugging.                                                        |
| `-cst`       | `--dump-cst`         | Dump CST as serialized string and SVG image                                                 |
| `-ast`       | `--dump-ast`         | Dump AST as serialized string and SVG image                                                 |
| `-symtab`    | `--dump-symtab`      | Dump serialized symbol tables                                                               |
| `-types`     | `--dump-types`       | Dump all used types                                                                         |
| `-ir`        | `--dump-ir`          | Dump LLVM-IR                                                                                |
| `-s`, `-asm` | `--dump-assembly`    | Dump Assembly code                                                                          |
| `-j <n>`     | `--jobs <n>`         | Set number of jobs to parallelize compilation (default is auto)                             |
| `-O<x>`      | -                    | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`        |
| `-g`         | `--debug-info`       | Generate debug info to debug the executable in GDB, etc.                                    |
| -            | `--disable-verifier` | Disable LLVM module and function verification (only recommended for debugging the compiler) |
| -            | `--ignore-cache`     | Compile always and ignore the compile cache                                                 |