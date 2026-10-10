---
name: spice-diagnostics
description: Locate or add a Spice compiler diagnostic — errors (semantic, parser, lexer, compiler-internal, linker, CLI) and warnings. Covers the error types in src/exception, the message tables, how to raise a diagnostic from a pass, the warning mechanism, and the matching test convention (exception.out / warning.out). Use when adding a new error/warning or tracking down where a message comes from.
---

# Spice compiler diagnostics

All diagnostics live in `src/exception/` (errors) and `src/util/compiler-warning.spice` (warnings).
`src/exception/error-manager.spice` collects the semantic errors of a compilation.

Spice has no exceptions. A compile error prints its message and exits the compiler right away (`abortWithError` in
`src/exception/fatal-error.spice`), without a stack trace, since the user input caused it. Internal compiler errors are
raised as panics (`panic(...)`, failed `assert`s), which print the location in the compiler sources and a stack trace.

## Error types

Each has a paired `*Type` enum and a `getMessagePrefix(type)` switch that maps the enum to a human prefix:

| Type | File → enum | Raised for |
|------|-------------|-----------|
| `SemanticError` | `semantic-error.spice` → `SemanticErrorType` (~100+ values) | Name resolution, types, generics, memory-safety rules |
| `ParserError` | `parser-error.spice` → `ParserErrorType` | Syntax errors |
| `LexerError` | `lexer-error.spice` → `LexerErrorType` | Tokenization errors |
| `CompilerError` | `compiler-error.spice` → `CompilerErrorType` | Internal/IO/target failures |
| `LinkerError` | `linker-error.spice` → `LinkerErrorType` | Linking failures |
| `CliError` | `cli-error.spice` → `CliErrorType` | Bad CLI usage |

Pattern (using `SemanticError` as the example):
- `semantic-error.spice` declares `public type SemanticErrorType enum { ... }`.
- `SemanticError.getMessagePrefix()` has a `case SemanticErrorType::NEW_TYPE: { return "Prefix"; }`.
- The constructor takes the offending `ASTNode*`, the type, and a detail message, and formats
  `[Error|Semantic] <codeLoc>:\n<prefix>: <msg>` plus a source snippet.

## Add a new error

1. Add a value to the relevant `*ErrorType` enum.
2. Add the matching `case` returning a short prefix in `getMessagePrefix`.
3. Raise it from the pass that detects the condition. In the type checker:
   ```spice
   // Soft error: recorded, the pass continues
   this.softError(node, SemanticErrorType::YOUR_NEW_TYPE, String("Specific detail about what went wrong"));
   // Hard error: recorded on the active error manager, the caller has to return right away
   throwSemanticError(node, SemanticErrorType::YOUR_NEW_TYPE, String("Specific detail about what went wrong"));
   return ...;
   ```
   The parser uses `this.throwParserError(token.codeLoc, ParserErrorType::..., message)`. Use the AST node / code location
   of the error so the code snippet is correct.

Keep prefixes terse and consistent with existing entries; put the specifics in the per-call message string.

## Warnings — `src/util/compiler-warning.spice`

Non-fatal diagnostics use `CompilerWarning` with a `CompilerWarningType` enum (+ message prefix). Passes collect them per
source file (e.g. `TypeChecker.addWarning`), and `SourceFile.collectAndPrintWarnings` prints them. Add a
`CompilerWarningType` value and emit it where detected, mirroring existing usages.

## Test convention (see `spice-add-test` skill)

- An error is asserted by an **`exception.out`** file in the test case dir containing the expected message. The runner
  fails the case if no error is raised when `exception.out` exists.
- Warnings are asserted by a **`warning.out`** file (one warning message per line).
- Use `python test/run-tests.py --update-refs` to capture the exact current message text, then review it. Add both a
  triggering `source.spice` and the expectation file.
