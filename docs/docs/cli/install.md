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

| Option                    | Short        | Description                                                                                    |
|---------------------------|--------------|------------------------------------------------------------------------------------------------|
| `--build-mode`            | `-m`         | Controls the build mode. <br> Valid values: `debug` (default) and `release`                    |
| `--build-var`             | `-b`         | Add build variable to parametrize the compiled program (e.g. -v key=value)                     |
| `--code-model`            | -            | Code model: `default`, `tiny`, `small`, `kernel`, `medium`, `large`                            |
| `--cpu-features`          | -            | CPU features to enable (`+`) or disable (`-`), e.g. `+avx2,-sse4.1`                            |
| `--debug-output`          | `-d`         | Print compiler output for debugging.                                                           |
| `--dump-assembly`         | `-s`, `-asm` | Dump Assembly code                                                                             |
| `--dump-ast`              | `-ast`       | Dump AST as serialized string and SVG image                                                    |
| `--dump-cache-stats`      | -            | Dump stats for compiler-internal lookup caches                                                 |
| `--dump-dependency-graph` | -            | Dump compile unit dependency graph                                                             |
| `--dump-ir`               | `-ir`        | Dump LLVM-IR                                                                                   |
| `--dump-object-file`      | -            | Dump object files                                                                              |
| `--dump-stats`            | -            | Write compile statistics (stage times, peak memory usage, counters) as JSON to the given file. |
| `--dump-symtab`           | -            | Dump serialized symbol tables                                                                  |
| `--dump-types`            | -            | Dump all used types                                                                            |
| `--freestanding`          | -            | Compile without hosted libc, crt0 and OS                                                       |
| `--jobs <n>`              | `-j <n>`     | Set number of jobs to parallelize compilation (Default is auto)                                |
| `--keep-frame-pointers`   | -            | Set up a frame pointer in every function, so profilers can walk the stack                      |
| `--native-features`       | -            | Tune the output for the CPU of the compiling machine (output not portable)                     |
| `--no-red-zone`           | -            | Do not use the red zone below the stack pointer (x86_64), e.g. for kernel code                 |
| -                         | `-O<n>`      | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2` (default), `-O3`, `-Os`, `-Oz` |
| `--output`                | `-o`         | Set path for executable output.                                                                |
| `--quiet`                 | `-q`         | Do not show the compile progress bar.                                                          |
| `--relocation-model`      | -            | Relocation model: `default`, `static`, `pic`, `pie`                                            |
| `--use-lifetime-markers`  | -            | Generate lifetime markers to enhance optimizations                                             |