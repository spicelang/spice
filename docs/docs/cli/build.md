---
title: Build command
---

The `build` command can be used to compile your Spice project to an executable file and save it to the current directory.

## Usage
=== "Long form"
    Use the `build` command by executing:
    ```sh
    $ spice build [main-source-file]
    ```
=== "Short form"
    Use the `build` command by executing:
    ```sh
    $ spice b [mail-source-file]
    ```

## Options
You can apply following options to the `build` command:

| Option       | Long                 | Description                                                                                                     |
|--------------|----------------------|-----------------------------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`     | Print compiler output for debugging.                                                                            |
| `-ast`       | `--dump-ast`         | Dump AST as serialized string and SVG image                                                                     |
| `-symtab`    | `--dump-symtab`      | Dump serialized symbol tables                                                                                   |
| `-ir`        | `--dump-ir`          | Dump LLVM-IR                                                                                                    |
| `-s`, `-asm` | `--dump-assembly`    | Dump Assembly code                                                                                              |
| `-j <n>`     | `--jobs <n>`         | Set number of jobs to parallelize compilation (default is auto)                                                 |
| `-t`         | `--target`           | Target triple for the emitted executable (for cross-compiling). <br> Format: `<arch><sub>-<vendor>-<sys>-<abi>` |
| `-o`         | `--output`           | Set path for executable output.                                                                                 |
| `-O<n>`      | -                    | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`                            |
| `-lto`       | -                    | Enable link-time-optimization                                                                                   |
| `-g`         | `--debug-info`       | Generate debug info to debug the executable in GDB, etc.                                                        |
| -            | `--no-entry`         | Do not require or generate main function (useful for web assembly target)                                       |
| -            | `--disable-verifier` | Disable LLVM module and function verification (only recommended for debugging the compiler)                     |
| -            | `--ignore-cache`     | Compile always and ignore the compile cache                                                                     |
| -            | `--disable-ast-opt`  | Disable AST optimization (recommended for debugging the compiler)                                               |