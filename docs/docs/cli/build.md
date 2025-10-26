---
title: Build subcommand
---

The `build` subcommand can be used to compile your Spice project to an executable file and save it to the current directory.

## Usage
=== "Long form"
    Use the `build` subcommand by executing:
    ```sh
    $ spice build [options] <main-source-file>
    ```
=== "Short form"
    Use the `build` command by executing:
    ```sh
    $ spice b [options] <mail-source-file>
    ```

## Options
You can apply following options to the `build` subcommand:

| Option       | Long                      | Description                                                                                                     |
|--------------|---------------------------|-----------------------------------------------------------------------------------------------------------------|
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                                            |
| `-cst`       | `--dump-cst`              | Dump CST as serialized string and SVG image                                                                     |
| `-ast`       | `--dump-ast`              | Dump AST as serialized string and SVG image                                                                     |
| -            | `--dump-symtab`           | Dump serialized symbol tables                                                                                   |
| -            | `--dump-types`            | Dump all used types                                                                                             |
| -            | `--dump-cache-stats`      | Dump stats for compiler-internal lookup caches                                                                  |
| `-ir`        | `--dump-ir`               | Dump LLVM-IR                                                                                                    |
| `-s`, `-asm` | `--dump-assembly`         | Dump Assembly code                                                                                              |
| -            | `--dump-object-file`      | Dump object files                                                                                               |
| -            | `--dump-dependency-graph` | Dump compile unit dependency graph                                                                              |
| -            | `--dump-to-files`         | Redirect all dumps to files instead of printing them to the screen                                              |
| -            | `--abort-after-dump`      | Abort the compilation process after dumping the first requested resource                                        |
| `-j <n>`     | `--jobs <n>`              | Set number of jobs to parallelize compilation (default is auto)                                                 |
| `-t`         | `--target`                | Target triple for the emitted executable (for cross-compiling). <br> Format: `<arch><sub>-<vendor>-<sys>-<abi>` |
| `-o`         | `--output`                | Set path for executable output.                                                                                 |
| `-O<n>`      | -                         | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`                            |
| `-m`         | `--build-mode`            | Controls the build mode. Valid values are `debug`, `release` and `test`.                                        |
| `-lto`       | -                         | Enable link-time-optimization                                                                                   |
| `-g`         | `--debug-info`            | Generate debug info to debug the executable in GDB, etc.                                                        |
| -            | `--sanitize`              | Enable instrumentation for sanitizer. Valid values are `address` and `thread`.                                  |
| -            | `--static`                | Produce stand-alone executable by linking statically                                                            |
| -            | `--no-entry`              | Do not require or generate main function (useful for web assembly target)                                       |
| -            | `--disable-verifier`      | Disable LLVM module and function verification (only recommended for debugging the compiler)                     |
| -            | `--ignore-cache`          | Compile always and ignore the compile cache                                                                     |
| -            | `--use-lifetime-markers`  | Generate lifetime markers to enhance optimizations                                                              |