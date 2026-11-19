---
title: Fmt subcommand
tags:
  - Command Line Interface
  - Fmt
  - Formatter
---

The `fmt` subcommand formats your Spice source files, so that all of them follow the same layout. It only changes whitespace,
the code itself stays untouched.

## Usage
=== "Long form"
    Use the `fmt` subcommand by executing:
    ```sh
    $ spice fmt [options] <path>
    ```
=== "Alias"
    Use the `fmt` subcommand by executing:
    ```sh
    $ spice format [options] <path>
    ```

The path can be:

- a source file, which is formatted in place.
- a directory, whose `.spice` files are formatted in place, including the ones in its subdirectories. Hidden directories and
  `spice_packages` directories are left out.
- `-`, which reads the source code from stdin and prints the formatted code to stdout. This is useful for editor integrations.

The subcommand prints the paths of the files, that it reformatted.

## Options
You can apply following options to the `fmt` subcommand:

| Option     | Description                                                                                                                      |
|------------|----------------------------------------------------------------------------------------------------------------------------------|
| `--check`  | Do not write the files, but print the paths of the files, that are not formatted, and exit with code 1, if there are any. For CI |
| `--stdout` | Print the formatted code instead of writing it back to the file. Only works with a single source file                           |

## Examples
```sh
# Format all source files in the current directory
$ spice fmt .

# Fail, if a source file in src is not formatted
$ spice fmt --check src

# Format code from stdin
$ echo 'f<int>main(){return 0;}' | spice fmt -
f<int> main() { return 0; }
```

## Formatting rules
The formatter brings the code into the layout of the
[coding style guide](https://github.com/spicelang/spice/blob/main/STYLE_GUIDE.md):

- **Indentation:** Four spaces per level. Continuation lines of a statement get one more level. The items of a list, e.g. the
  arguments of a call, whose first item is on the line of the opening bracket, stay aligned to the first item, if they are
  aligned already. Otherwise, they get one more level as well.
- **Braces:** The opening brace of a block goes on the line of its construct (K&R style). `else` and the `while` of a
  do-while loop go on the line of the preceding closing brace.
- **Spaces:** One space around binary operators, after commas, after keywords and before the opening brace of a block. No
  space between a function name and its arguments, within generic type lists like `Vector<int>`, after unary operators like
  `-x` or `&x`, before type modifiers like `int*` or `T&` and within struct instantiations like `Pair{1, 2}`.
- **Blank lines:** At most one blank line in a row and no blank lines at the start or end of a block. The file ends with a
  single line break.
- **Alignment:** Comments at the end of a line, the names of consecutive fields, the `=` of consecutive declarations and the
  items of tables keep their column, if they are aligned with extra spaces and the column can still be reached.

The formatter keeps the line breaks within statements as they are, so you decide where long lines are wrapped.

!!! note
    The formatter only formats syntactically valid code. If the code cannot be parsed, it reports the error and leaves the
    file as it is.

## Editor integration
The Spice extensions for [VS Code](https://github.com/spicelang/vscode-spice) and
[IntelliJ-based IDEs](https://github.com/spicelang/intellij-spice) use `spice fmt -` to format the open file, e.g. via the
"Format Document" / "Reformat Code" action or on save. Any other editor can do the same: pipe the content of the file into
`spice fmt -` and replace it with the output.
