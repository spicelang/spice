---
title: Run subcommand
tags:
  - Command Line Interface
  - Run
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

## Exit code
`spice run` exits with the exit code of the program, so scripts can check it like for the program itself. If the program
exits with a non-zero exit code, the compiler reports it, e.g.:
```text
[Error|CLI] Non-zero exit code: Your Spice executable exited with non-zero exit code 3
```

If the program is terminated by a signal (Linux and macOS), the compiler reports the signal by number and name and, like a
shell, exits with 128 + signal number, e.g. 139 for a segmentation fault:
```text
[Error|CLI] Terminated by signal: Your Spice executable was terminated by signal 11 (SIGSEGV)
```

If compiling the program fails, `spice run` exits with 1.

## Options
You can apply following options to the `run` subcommand:

| Option                    | Short        | Description                                                                                                                                                                                                                                                                                                         |
|---------------------------|--------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `--build-mode`            | `-m`         | Controls the build mode. <br> Valid values: `debug` (default) and `release`                                                                                                                                                                                                                                         |
| `--build-var`             | `-b`         | Add build variable to parametrize the compiled program (e.g. -v key=value)                                                                                                                                                                                                                                          |
| `--code-model`            | -            | Code model. <br> Valid values: `default` (`small`), `tiny` (AArch64 ELF), `small`, `kernel` (x86_64), `medium` (x86_64), `large`                                                                                                                                                                                    |
| `--coverage`              | -            | Instrument code for coverage analysis (gcov-compatible `.gcno`/`.gcda` output). Implies `--debug-info=line-only` if no debug info level was requested explicitly, rejects an explicit `--debug-info=none`, and is incompatible with `-lto`.                                                                         |
| `--cpu-features`          | -            | Comma-separated list of CPU features to enable (`+`) or disable (`-`) for the target, e.g. `+avx2,-sse4.1`                                                                                                                                                                                                          |
| `--debug-info`            | `-g`         | Generate debug info to debug the executable in GDB, etc. <br> Valid values: `full` (default when the flag is given without a value), `line-only` (DWARF line tables only) and `none`. <br> A level can only be attached to the long form (e.g. `--debug-info=line-only`); `-g` is an alias for `--debug-info=full`. |
| `--debug-output`          | `-d`         | Print compiler output for debugging.                                                                                                                                                                                                                                                                                |
| `--disable-verifier`      | -            | Disable LLVM module and function verification (only recommended for debugging the compiler)                                                                                                                                                                                                                         |
| `--dump-assembly`         | `-s`, `-asm` | Dump Assembly code                                                                                                                                                                                                                                                                                                  |
| `--dump-ast`              | `-ast`       | Dump AST as serialized string and SVG image                                                                                                                                                                                                                                                                         |
| `--dump-cache-stats`      | -            | Dump stats for compiler-internal lookup caches                                                                                                                                                                                                                                                                      |
| `--dump-dependency-graph` | -            | Dump compile unit dependency graph                                                                                                                                                                                                                                                                                  |
| `--dump-ir`               | `-ir`        | Dump LLVM-IR                                                                                                                                                                                                                                                                                                        |
| `--dump-object-file`      | -            | Dump object files                                                                                                                                                                                                                                                                                                   |
| `--dump-stats`            | -            | Write compile statistics (stage times, peak memory usage, counters) as JSON to the given file.                                                                                                                                                                                                                      |
| `--dump-symtab`           | -            | Dump serialized symbol tables                                                                                                                                                                                                                                                                                       |
| `--dump-types`            | -            | Dump all used types                                                                                                                                                                                                                                                                                                 |
| `--freestanding`          | -            | Compile for an environment without hosted libc, crt0 and OS. See the [freestanding how-to](../how-to/freestanding.md)                                                                                                                                                                                               |
| `--jobs <n>`              | `-j <n>`     | Set number of jobs to parallelize compilation (default is auto)                                                                                                                                                                                                                                                     |
| `--keep-frame-pointers`   | -            | Set up a frame pointer in every function, so profilers can walk the stack                                                                                                                                                                                                                                           |
| `--native-features`       | -            | Tune the output for the CPU of the compiling machine. The output might not run on other machines                                                                                                                                                                                                                    |
| `--no-red-zone`           | -            | Do not use the red zone below the stack pointer (x86_64), e.g. for kernel code                                                                                                                                                                                                                                      |
| -                         | `-O<x>`      | Set optimization level. <br> Valid options: `-O0` (default in `debug` mode), `-O1`, `-O2` (default in `release` mode), `-O3`, `-Os`, `-Oz`                                                                                                                                                                          |
| `--output`                | `-o`         | Set path for executable output.                                                                                                                                                                                                                                                                                     |
| `--quiet`                 | `-q`         | Do not show the compile progress bar.                                                                                                                                                                                                                                                                               |
| `--relocation-model`      | -            | Relocation model. <br> Valid values: `default` (`pie`, `pic` for shared libraries, `static` in freestanding mode, except for TPDE, Windows and Darwin), `static`, `pic`, `pie`                                                                                                                                      |
| `--sanitizer`             | -            | Enable instrumentation for sanitizer, see [Sanitizers](../how-to/sanitizers.md). <br> Valid values: `none` (default), `address`, `thread`, `memory` and `type`.                                                                                                                                                     |
| `--use-lifetime-markers`  | -            | Generate lifetime markers to enhance optimizations                                                                                                                                                                                                                                                                  |
| `--use-tbaa-metadata`     | -            | Generate metadata for type-based alias analysis to enhance optimizations                                                                                                                                                                                                                                            |