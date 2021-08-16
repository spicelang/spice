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
| `--opt-level`    | `-o`  | Set optimization level.                                         |