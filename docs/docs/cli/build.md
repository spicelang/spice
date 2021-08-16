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
| `--opt-level`    | `-o`  | Set optimization level.                                         |
| `--output`       | -     | Path to the location where the output executable should go.     |