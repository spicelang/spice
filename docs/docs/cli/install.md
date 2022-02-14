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

| Option           | Short | Description                                                     |
| ---------------- | ----- | --------------------------------------------------------------- |
| `--debug-output` | `-d`  | Print compiler output for debugging.                            |
| `--output`       | `-o`  | Set path for executable output.                                 |
| `--opt-0`        | `-O0` | Set optimization to level 0.                                    |
| `--opt-1`        | `-O1` | Set optimization to level 1.                                    |
| `--opt-2`        | `-O2` | Set optimization to level 2.                                    |
| `--opt-3`        | `-O3` | Set optimization to level 3.                                    |