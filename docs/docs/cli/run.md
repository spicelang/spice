---
title: Run command
---

The `run` command can be used to compile your Spice project to an executable file and run it immediately.

## Usage
=== "Long form"
    Use the `run` command by executing:
    ```sh
    $ spice run [main-source-file]
    ```
=== "Short form"
    Use the `run` command by executing:
    ```sh
    $ spice r [mail-source-file]
    ```

## Options
You can apply following options to the `run` command:

| Option           | Short | Description                                                     |
| ---------------- | ----- | --------------------------------------------------------------- |
| `--debug-output` | `-d`  | Print compiler output for debugging.                            |
| `--output`       | `-o`  | Set path for executable output.                                 |
| `--opt-0`        | `-O0` | Set optimization to level 0.                                    |
| `--opt-1`        | `-O1` | Set optimization to level 1.                                    |
| `--opt-2`        | `-O2` | Set optimization to level 2.                                    |
| `--opt-3`        | `-O3` | Set optimization to level 3.                                    |