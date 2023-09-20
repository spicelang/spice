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
| `-cst`       | `--dump-cst`         | Dump CST as serialized string and SVG image                                                                     |
| `-ast`       | `--dump-ast`         | Dump AST as serialized string and SVG image                                                                     |
| `-symtab`    | `--dump-symtab`      | Dump serialized symbol tables                                                                                   |
| `-ir`        | `--dump-ir`          | Dump LLVM-IR                                                                                                    |
| `-s`, `-asm` | `--dump-assembly`    | Dump Assembly code                                                                                              |
| `-b`, `-obj` | `--dump-object-file` | Dump object files                                                                                               |
| -            | `--dump-to-files`    | Redirect all dumps to files instead of printing them to the screen                                              |
| `-j <n>`     | `--jobs <n>`         | Set number of jobs to parallelize compilation (default is auto)                                                 |
| `-t`         | `--target`           | Target triple for the emitted executable (for cross-compiling). <br> Format: `<arch><sub>-<vendor>-<sys>-<abi>` |
| `-o`         | `--output`           | Set path for executable output.                                                                                 |
| `-O<n>`      | -                    | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`                            |
| `-m`         | `--build-mode`       | Controls the build mode. Valid values are `debug` and `release`                                                 |
| `-lto`       | -                    | Enable link-time-optimization                                                                                   |
| `-g`         | `--debug-info`       | Generate debug info to debug the executable in GDB, etc.                                                        |
| -            | `--static`           | Produce stand-alone executable by linking statically                                                            |
| -            | `--no-entry`         | Do not require or generate main function (useful for web assembly target)                                       |
| -            | `--disable-verifier` | Disable LLVM module and function verification (only recommended for debugging the compiler)                     |
| -            | `--ignore-cache`     | Compile always and ignore the compile cache                                                                     |
| -            | `--enable-ast-opt`   | Enable AST optimization (recommended for debugging the compiler)                                                |