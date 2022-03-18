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

| Option  | Long             | Description                                                                                                     |
| ------- | ---------------- | --------------------------------------------------------------------------------------------------------------- |
| `-d`    | `--debug-output` | Print compiler output for debugging.                                                                            |
| `-t`    | `--target`       | Target triple for the emitted executable (for cross-compiling). <br> Format: `<arch><sub>-<vendor>-<sys>-<abi>` |
| `-o`    | `--output`       | Set path for executable output.                                                                                 |
| `-O<x>` | -                | Set optimization level. Valid options are `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`                              |