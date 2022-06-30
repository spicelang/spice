---
title: Run command
---

The `run` command can be used to compile your Spice project to an executable file and run it immediately.

## Usage
=== "Long form"
    Use the `run` command by executing:
    ```sh
    $ spice run [main-source-file]
    ```
=== "Short form"
    Use the `run` command by executing:
    ```sh
    $ spice r [mail-source-file]
    ```

## Options
You can apply following options to the `run` command:

| Option       | Long                 | Description                                                                                 |
|--------------|----------------------|---------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`     | Print compiler output for debugging.                                                        |
| `-ast`       | `--dump-ast`         | Dump AST as serialized string and SVG image                                                 |
| `-symtab`    | `--dump-symtab`      | Dump serialized symbol tables                                                               |
| `-ir`        | `--dump-ir`          | Dump LLVM-IR                                                                                |
| `-s`, `-asm` | `--dump-assembly`    | Dump Assembly code                                                                          |
| `-o`         | `--output`           | Set path for executable output.                                                             |
| `-O<x>`      | -                    | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`        |
| `-g`         | `--debug-info`       | Generate debug info to debug the executable in GDB, etc.                                    |
| -            | `--disable-verifier` | Disable LLVM module and function verification (only recommended for debugging the compiler) |