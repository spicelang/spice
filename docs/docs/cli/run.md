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
| `--opt-level`    | `-o`  | Set optimization level.                                         |