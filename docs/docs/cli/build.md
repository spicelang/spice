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

| Option           | Short | Description                                                     |
| ---------------- | ----- | --------------------------------------------------------------- |
| `--debug-output` | `-d`  | Print compiler output for debugging.                            |
| `--target`       | `-t`  | Target triple for the emitted executable (for cross-compiling). |
| `--output`       | `-o`  | Set path for executable output.                                 |
| `--opt-0`        | `-O0` | Set optimization to level 0.                                    |
| `--opt-1`        | `-O1` | Set optimization to level 1.                                    |
| `--opt-2`        | `-O2` | Set optimization to level 2.                                    |
| `--opt-3`        | `-O3` | Set optimization to level 3.                                    |