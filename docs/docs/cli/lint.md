---
title: Lint subcommand
tags:
  - Command Line Interface
  - Lint
  - Linter
---

The `lint` subcommand checks your Spice program for style and best-practice issues, like names that do not follow the naming
conventions or functions that grew too complex. It runs the compiler up to and including the type checker, so it also reports
all compile errors, but it does not generate any code.

## Usage
=== "Long form"
    Use the `lint` subcommand by executing:
    ```sh
    $ spice lint [options] <main-source-file>
    ```
=== "Short form"
    Use the `lint` subcommand by executing:
    ```sh
    $ spice l [options] <main-source-file>
    ```

The subcommand lints the main source file and all source files of your project, that it imports. The source files of the
standard library and of [external packages](../language/modules.md) are left out.

## Options
You can apply following options to the `lint` subcommand:

| Option                    | Short  | Description                                                                                              |
|---------------------------|--------|----------------------------------------------------------------------------------------------------------|
| `--build-mode`            | `-m`   | Controls the build mode. <br> Valid values: `debug` (default), `release` and `test`.                     |
| `--build-var`             | `-b`   | Add build variable to parametrize the compiled program (e.g. -b key=value). Can be passed multiple times |
| `--freestanding`          | -      | Lint for an environment without hosted libc, crt0 and OS, e.g. a kernel or firmware                      |
| `--debug-output`          | `-d`   | Enable debug output                                                                                      |
| `--quiet`                 | `-q`   | Do not show the compile progress                                                                         |
| `--dump-ast`              | `-ast` | Dump AST as serialized string and SVG image                                                              |
| `--dump-symtab`           | -      | Dump serialized symbol tables                                                                            |
| `--dump-types`            | -      | Dump all used types                                                                                      |
| `--dump-cache-stats`      | -      | Dump stats for compiler-internal lookup caches                                                           |
| `--dump-dependency-graph` | -      | Dump compile unit dependency graph                                                                       |
| `--dump-stats`            | -      | Write compile statistics (stage times, peak memory usage, counters) as JSON to the given file            |

The options, that only affect the code generation, like the optimization levels or `--target`, are not available,
because linting stops before the code generation.

## Output
Every finding is printed on its own line with the location of the code, the id of the rule and a message:

```
[Lint] ./main.spice:12:1: naming-convention: Global variable name 'MaxRetries' should be SNAKE_CASE
```

The findings do not fail the subcommand: it exits with code 0, as long as the program compiles. Findings with the `error`
severity (see [Configuration](#configuration)) are the exception: they are printed as `[Lint error]` and make the subcommand
exit with code 1. If there is a compile error, it reports the error and exits with code 1. The regular compiler warnings,
e.g. for unused variables, are not printed by the `lint` subcommand. You get them with the [`build`](build.md) subcommand.

## Rules
The linter currently comes with the following rules:

| Rule id                 | Checks                                                                                                                                                                                                                             |
|-------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `naming-convention`     | Functions and procedures are named in `camelCase`, structs, interfaces and unions in `PascalCase` and global variables in `SNAKE_CASE` (uppercase letters, digits and underscores). Constructors and operator overloads are exempt |
| `too-many-parameters`   | Functions and procedures have at most 6 parameters. Consider grouping related parameters into a struct                                                                                                                             |
| `cyclomatic-complexity` | Functions and procedures have a cyclomatic complexity of at most 10. It starts at 1 and every `if`, loop and `case` branch adds 1, including the ones in lambdas within the function. Consider splitting the function up           |

The naming conventions follow the [coding style guide](https://github.com/spicelang/spice/blob/main/STYLE_GUIDE.md).

## Configuration
You can configure the rules in the `[lint.rules]` table of the manifest of your project, the `spice.toml` file, that also
describes your package for [Spice Shaker](https://github.com/spicelang/spice-shaker). The `lint` subcommand uses the manifest
in the directory of the main source file or the nearest one in its parent directories. Without a manifest, all rules apply
with their default configuration. The [format of the manifest](https://github.com/spicelang/packages/blob/main/packages/manifest/README.md#the-format) is specified by the `manifest` package.

```toml
[lint.rules]
naming-convention = { enabled = false }
too-many-parameters = { max = 8, severity = "error" }
cyclomatic-complexity = { max = 15, severity = "info" }
```

Each rule accepts the following keys. Rules and keys, that you leave out, keep their default:

| Key        | Default     | Description                                                                                                         |
|------------|-------------|---------------------------------------------------------------------------------------------------------------------|
| `enabled`  | `true`      | Whether the rule runs at all                                                                                        |
| `severity` | `"warning"` | Severity of the findings: `"info"`, `"warning"` or `"error"`. Findings with `"error"` fail the subcommand           |
| `max`      | see rules   | Threshold of `too-many-parameters` (default: 6) and `cyclomatic-complexity` (default: 10). An integer of at least 1 |

The compiler rejects unknown rules and keys, so that a typo does not go unnoticed. It only reads the `[lint]` and `[format]`
tables of the manifest and leaves the other ones to Spice Shaker.

## Suppressing findings
To suppress all findings for a source file, add the `core.compiler.warnings.ignore` attribute to it. It also suppresses the
compiler warnings for the file:

```spice
#![core.compiler.warnings.ignore]
```

## Examples
```sh
# Lint the program with main.spice as main source file
$ spice lint main.spice

# Lint the program for the release build mode with a build variable
$ spice lint -m release -b feature=on main.spice
```
