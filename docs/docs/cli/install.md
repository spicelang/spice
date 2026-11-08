---
title: Install subcommand
tags:
  - Command Line Interface
  - Install
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

| Option       | Long                      | Description                                                                                    |
|--------------|---------------------------|------------------------------------------------------------------------------------------------|
| `-m`         | `--build-mode`            | Controls the build mode. <br> Valid values: `debug` (default) and `release`                    |
| `-b`         | `--build-var`             | Add build variable to parametrize the compiled program (e.g. -v key=value)                     |
| -            | `--code-model`            | Code model: `default`, `tiny`, `small`, `kernel`, `medium`, `large`                            |
| -            | `--cpu-features`          | CPU features to enable (`+`) or disable (`-`), e.g. `+avx2,-sse4.1`                            |
| `-d`         | `--debug-output`          | Print compiler output for debugging.                                                           |
| `-s`, `-asm` | `--dump-assembly`         | Dump Assembly code                                                                             |
| `-ast`       | `--dump-ast`              | Dump AST as serialized string and SVG image                                                    |
| -            | `--dump-cache-stats`      | Dump stats for compiler-internal lookup caches                                                 |
| -            | `--dump-dependency-graph` | Dump compile unit dependency graph                                                             |
| `-ir`        | `--dump-ir`               | Dump LLVM-IR                                                                                   |
| -            | `--dump-object-file`      | Dump object files                                                                              |
| -            | `--dump-stats`            | Write compile statistics (stage times, peak memory usage, counters) as JSON to the given file.                                              |
| -            | `--dump-symtab`           | Dump serialized symbol tables                                                                  |
| -            | `--dump-types`            | Dump all used types                                                                            |
| -            | `--freestanding`          | Compile without hosted libc, crt0 and OS                                                       |
| `-j <n>`     | `--jobs <n>`              | Set number of jobs to parallelize compilation (Default is auto)                                |
| -            | `--keep-frame-pointers`   | Set up a frame pointer in every function, so profilers can walk the stack                      |
| -            | `--native-features`       | Tune the output for the CPU of the compiling machine (output not portable)                     |
| -            | `--no-red-zone`           | Do not use the red zone below the stack pointer (x86_64), e.g. for kernel code                 |
| `-O<n>`      | -                         | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2` (default), `-O3`, `-Os`, `-Oz` |
| `-o`         | `--output`                | Set path for executable output.                                                                |
| `-q`         | `--quiet`                 | Do not show the compile progress bar.                                                          |
| -            | `--relocation-model`      | Relocation model: `default`, `static`, `pic`, `pie`                                            |
| -            | `--use-lifetime-markers`  | Generate lifetime markers to enhance optimizations                                             |