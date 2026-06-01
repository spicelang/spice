---
name: spice-diagnostics
description: Locate or add a Spice compiler diagnostic — errors (semantic, parser, lexer, compiler-internal, linker, CLI) and warnings. Covers the exception classes in src/exception, the message tables, how to throw a diagnostic from a pass, the warning mechanism, and the matching test convention (exception.out / warning.out). Use when adding a new error/warning or tracking down where a message comes from.
---

# Spice compiler diagnostics

All diagnostics live in `src/exception/` (errors) and `src/util/CompilerWarning.*`
(warnings). `src/exception/ErrorManager.*` coordinates collection/reporting, and
`AntlrThrowingErrorListener.*` converts ANTLR parse errors into Spice exceptions.

## Error classes

Each is a `std::exception` subclass with a paired `*Type` enum and a
`getMessagePrefix(type)` switch that maps the enum to a human prefix:

| Class | Header / enum | Raised for |
|-------|---------------|-----------|
| `SemanticError` | `SemanticError.h` → `SemanticErrorType` (~100+ values) | Name resolution, types, generics, memory-safety rules |
| `ParserError` | `ParserError.h` → `ParserErrorType` | Syntax errors |
| `LexerError` | `LexerError.h` → `LexerErrorType` | Tokenization errors |
| `CompilerError` | `CompilerError.h` → `CompilerErrorType` | Internal/IO/target failures |
| `LinkerError` | `LinkerError.h` → `LinkerErrorType` | Linking failures |
| `CliError` | `CliError.h` → `CliErrorType` | Bad CLI usage |

Pattern (using `SemanticError` as the example):
- `SemanticError.h` declares `enum SemanticErrorType { ... }`.
- `SemanticError.cpp` `getMessagePrefix()` has a `case NEW_TYPE: return "Prefix";`.
- The constructor takes the offending `ASTNode*`, the type, and a detail message,
  and formats `[Error|Semantic] <codeLoc>: <prefix>: <msg>` plus a source snippet.

## Add a new error

1. Add a value to the relevant `*ErrorType` enum in the header.
2. Add the matching `case` returning a short prefix in the `.cpp`
   `getMessagePrefix`.
3. Throw it from the pass that detects the condition, e.g. in the type checker:
   ```cpp
   throw SemanticError(node, YOUR_NEW_TYPE, "Specific detail about what went wrong");
   ```
   Use the AST node at the error location so the code-location snippet is correct.

Keep prefixes terse and consistent with existing entries; put the specifics in
the per-call message string.

## Warnings — `src/util/CompilerWarning.*`

Non-fatal diagnostics use `CompilerWarning` with a `WarningType` enum (+ message
text in the `.cpp`). Warnings are collected per source file
(`SourceFile::collectAndPrintWarnings`) and surfaced via
`compilerOutput.warnings`. Add a `WarningType` value and emit it where detected,
mirroring existing usages.

## Test convention (see `spice-add-test` skill)

- An error is asserted by an **`exception.out`** file in the test case dir
  containing the expected message. The runner fails the case if no error is
  raised when `exception.out` exists.
- Warnings are asserted by a **`warning.out`** file (one warning message per
  line).
- Use `--update-refs` to capture the exact current message text, then review it.
  Add both a triggering `source.spice` and the expectation file.
