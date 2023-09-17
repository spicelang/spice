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

| Option       | Long                  | Description                                                                          |
|--------------|-----------------------|--------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`      | Print compiler output for debugging.                                                 |
| `-cst`       | `--dump-cst`          | Dump CST as serialized string and SVG image                                          |
| `-ast`       | `--dump-ast`          | Dump AST as serialized string and SVG image                                          |
| `-symtab`    | `--dump-symtab`       | Dump serialized symbol tables                                                        |
| `-ir`        | `--dump-ir`           | Dump LLVM-IR                                                                         |
| `-s`, `-asm` | `--dump-assembly`     | Dump Assembly code                                                                   |
| `-b`, `-obj` | `--dump-object-file`  | Dump object files                                                                                               |
| -            | `--dump-to-files`     | Redirect all dumps to files instead of printing them to the screen                   |
| `-d`         | `--debug-output`      | Print compiler output for debugging.                                                 |
| `-j <n>`     | `--jobs <n>`          | Set number of jobs to parallelize compilation (Default is auto)                      |
| `-o`         | `--output`            | Set path for executable output.                                                      |
| `-O<n>`      | -                     | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz` |
| `-m`         | `--build-mode`        | Controls the build mode. Valid values are `debug` and `release`                      |
| -            | `--ignore-cache`      | Compile always and ignore the compile cache                                          |
| -            | `--disable-ast-opt`   | Disable AST optimization (recommended for debugging the compiler)                    |