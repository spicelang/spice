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

| Option  | Long             | Description                                                                          |
| ------- | ---------------- | ------------------------------------------------------------------------------------ |
| `-d`    | `--debug-output` | Print compiler output for debugging.                                                 |
| `-o`    | `--output`       | Set path for executable output.                                                      |
| `-O<x>` | -                | Set optimization level. <br> Valid options: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz` |