# Coding Style Guide

This document defines the coding conventions for the [Spice programming language](https://github.com/spicelang/spice)
repository. It is the single source of truth on style for **both human contributors and AI coding agents**.

The rules here are descriptive of the existing codebase, not aspirational — when in doubt, match the surrounding code.
If a file you are editing already follows a clearly established local pattern, prefer that pattern over a rule here and,
if the divergence is widespread, consider updating this guide.

> **For AI agents:** Read this file before writing or modifying Spice (`src/`, `std/`, `test/test-files/`) or Python code.
> It is referenced from `AGENTS.md`. Keep changes minimal and idiomatic; do not reformat unrelated code.

## Table of contents

- [Golden rules](#golden-rules)
- [Spice language style](#spice-language-style)
- [The visitor pattern & AST nodes](#the-visitor-pattern--ast-nodes)
- [Error handling & diagnostics](#error-handling--diagnostics)
- [Memory management](#memory-management)
- [Comments & documentation](#comments--documentation)
- [Commits & pull requests](#commits--pull-requests)

## Golden rules

1. **Match the surrounding code.** Consistency within a file beats personal preference.
2. **Keep changes minimal and scoped.** No drive-by refactors or reformatting of untouched lines.
3. **Every compiler source file starts with the copyright header**
   (`// Copyright (c) 2021-2026 ChilliBits. All rights reserved.`).
4. **Names describe intent**, are verb-first for functions, and follow the casing table below.

## Spice language style

Conventions for `.spice` sources in `src/`, `std/` and `test/test-files/`:

| Kind              | Convention             | Example                                |
|-------------------|------------------------|----------------------------------------|
| Type / struct     | `PascalCase`           | `String`, `LinkedList<T>`, `Node<T>`   |
| Type alias        | `PascalCase`           | `type IntLong int\|long;`              |
| Function / method | `camelCase`            | `pushBack`, `isEmpty`, `createNode`    |
| Field / variable  | `camelCase`            | `contents`, `capacity`, `length`       |
| Constant          | `SCREAMING_SNAKE_CASE` | `INITIAL_ALLOC_COUNT`, `RESIZE_FACTOR` |

- **Indentation:** 4 spaces inside type and function bodies.
- **Braces:** K&R style, opening brace on the same line; **no parentheses** around `if` / loop conditions
  (`if this.isEmpty() {`).
- Functions use the `f<ReturnType>` form; procedures use `p`. Mark exported items `public`, external C symbols `ext`.
- **Comments:** Doxygen-style `/** ... */` blocks above public functions and types; trailing `//` comments for
  clarifying inline logic; short `// Section` headers (e.g. `// Constants`, `// Std imports`).
- When adding test inputs under `test/test-files/`, follow the `spice-add-test` skill for the directory layout and
  reference-file conventions.

## The visitor pattern & AST nodes

The compiler passes (symbol table builder, type checker, IR generator, …) traverse the AST via double-dispatch: every AST
node (`src/ast/ast-nodes.spice`) implements `accept(IAbstractAstVisitor*)`, which calls the matching `visitXxx` method of
the pass, returning `Any`.

The visitor interface `IAbstractAstVisitor` (`src/ast/abstract-ast-visitor-intf.spice`) provides a
[default method](docs/docs/language/interfaces.md#default-methods) for every visit method, which visits all children of the
node via `visitChildren`. A pass only implements the visit methods it needs and can override `visitChildren` to customize
the traversal. When adding a new AST node, add its signature and default method to the interface. See the
`spice-language-feature` skill for the end-to-end recipe.

## Error handling & diagnostics

User-facing diagnostics live in `src/exception/`: `SemanticError`, `ParserError`, `LexerError`, `LinkerError`,
`CompilerError`, `CliError`. Spice has no exceptions, so a compile error prints its message and exits the compiler
(`abortWithError`). Semantic errors are collected by the `ErrorManager` as soft errors (the pass continues) or a hard
error (the caller returns right away). Internal compiler errors are raised as panics.

Each diagnostic pairs:
- an `XxxErrorType` enum of message codes,
- a `getMessagePrefix()` switch mapping each code to a human-readable prefix,
- a formatted message like `[Error|Semantic] <loc>:\n<prefix>: <message>`.

To add a new error or warning, add the enum value and its prefix, then raise it from the relevant pass. Follow the
`spice-diagnostics` skill, and add the matching `exception.out` / `warning.out` reference test.

## Memory management

- The compiler favors **raw pointers owned by long-lived containers** (`Scope*`, `ASTNode*`, LLVM values) over
  per-object ownership.
- **AST nodes are block-allocated** (see `BlockAllocator`), not individually allocated and freed.
- Use owning types with a destructor where a clear single owner exists and it matches nearby code, but do not retrofit
  the pointer style of existing subsystems.
- Validate memory-sensitive changes with sanitizers and valgrind (see `AGENTS.md` → *Debugging* and the
  `spice-debugging` skill).

## Comments & documentation

- Document public types and functions with **Doxygen-style block comments**:

  ```spice
  /**
   * Entry of a symbol table, representing an individual symbol with all its properties
   */
  ```

  Use `@param` / `@return` tags where they add clarity.
- Use `//` line comments for inline explanation of non-obvious logic, and short `// Banner` comments to group sections.
- Comment the **why**, not the obvious **what**. Keep comments in sync with the code they describe.
- Update user-facing docs under `docs/docs/` whenever behavior, CLI, or output changes.

## Commits & pull requests

See `AGENTS.md` and `CONTRIBUTING.md` for the full workflow. In short:

- Keep commits incremental and reviewable; write clear, descriptive messages.
- Before opening a PR, run at least a focused build and the tests relevant to your change.
- In the PR description, state **what** changed, **why**, **how it was validated** (exact commands + outcomes), and any
  follow-ups or known limitations.
- If environment limits prevent running checks, say so explicitly.
