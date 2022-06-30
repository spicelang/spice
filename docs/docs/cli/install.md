---
title: Install command
---

The `install` command can be used to compile your Spice project to an executable file and copy it to a binary directory in the PATH variable.

## Usage
=== "Long form"
    Use the `install` command by executing:
    ```sh
    $ spice install [main-source-file]
    ```
=== "Short form"
    Use the `install` command by executing:
    ```sh
    $ spice i [mail-source-file]
    ```

## Options
You can apply following options to the `install` command:

| Option       | Long              | Description                                                                          |
|--------------|-------------------|--------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`  | Print compiler output for debugging.                                                 |
| `-ast`       | `--dump-ast`      | Dump AST as serialized string and SVG image                                          |
| `-symtab`    | `--dump-symtab`   | Dump serialized symbol tables                                                        |
| `-ir`        | `--dump-ir`       | Dump LLVM-IR                                                                         |
| `-s`, `-asm` | `--dump-assembly` | Dump Assembly code                                                                   |
| `-d`         | `--debug-output`  | Print compiler output for debugging.                                                 |
| `-o`         | `--output`        | Set path for executable output.                                                      |
| `-O<x>`      | -                 | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz` |