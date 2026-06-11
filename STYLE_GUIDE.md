# Coding Style Guide

This document defines the coding conventions for the [Spice programming language](https://github.com/spicelang/spice)
repository. It is the single source of truth on style for **both human contributors and AI coding agents**.

The rules here are descriptive of the existing codebase, not aspirational — when in doubt, match the surrounding code.
If a file you are editing already follows a clearly established local pattern, prefer that pattern over a rule here and,
if the divergence is widespread, consider updating this guide.

> **For AI agents:** Read this file before writing or modifying C++ (`src/`, `test/`) or Spice (`std/`, `test/test-files/`)
> code. It is referenced from `AGENTS.md`. Keep changes minimal and idiomatic; do not reformat unrelated code.

## Table of contents

- [Golden rules](#golden-rules)
- [Formatting (C++)](#formatting-c)
- [File layout (C++)](#file-layout-c)
- [Naming (C++)](#naming-c)
- [Types, classes & structs (C++)](#types-classes--structs-c)
- [The visitor pattern & AST nodes](#the-visitor-pattern--ast-nodes)
- [Error handling & diagnostics](#error-handling--diagnostics)
- [Memory management](#memory-management)
- [Comments & documentation](#comments--documentation)
- [Spice language style](#spice-language-style)
- [Commits & pull requests](#commits--pull-requests)

## Golden rules

1. **Match the surrounding code.** Consistency within a file beats personal preference.
2. **Let `.clang-format` do the formatting.** Never hand-format C++; run the formatter (see below).
3. **Keep changes minimal and scoped.** No drive-by refactors or reformatting of untouched lines.
4. **Every source file starts with the copyright header** (see [File layout](#file-layout-c)).
5. **Names describe intent**, are verb-first for functions, and follow the casing tables below.

## Formatting (C++)

Formatting is governed entirely by the root [`.clang-format`](.clang-format):

```yaml
Language: Cpp
BasedOnStyle: LLVM
ColumnLimit: 130
```

This implies, among other things:
- **2-space indentation**, no tabs.
- **Opening braces on the same line** as the function/class/namespace/control statement (K&R / LLVM style).
- **130-column line limit** — wider than default LLVM (80); use the room before wrapping.

Always format before committing:

```bash
# Format a single file in place
clang-format -i src/path/To/File.cpp

# Format everything you changed (run from repo root)
git diff --name-only --diff-filter=d origin/main | grep -E '\.(cpp|h)$' | xargs -r clang-format -i
```

There is currently no CI gate enforcing formatting, so this discipline is on the author (human or agent).

## File layout (C++)

`.cpp` and `.h` files come in matching **PascalCase** pairs (`TypeChecker.h` / `TypeChecker.cpp`). Each file is structured
top-to-bottom as follows.

```cpp
// Copyright (c) 2021-2026 ChilliBits. All rights reserved.   // 1. Copyright header (every file)

#pragma once                                                  // 2. Header guard: #pragma once ONLY (.h files)

#include <stack>                                              // 3. Standard library headers
#include <string>

#include <symboltablebuilder/Lifecycle.h>                     // 4. Project headers (angle brackets, path-sorted)
#include <symboltablebuilder/QualType.h>

#include <nlohmann/json.hpp>                                  // 5. Third-party headers

// Forward declarations                                       // 6. Forward declarations, grouped by namespace
namespace llvm {
class Value;
} // namespace llvm

namespace spice::compiler {                                   // 7. Everything inside the project namespace
// ...
} // namespace spice::compiler                                //    closing brace carries a trailing comment
```

Include rules:
- **`.cpp` files include their own header first**, in `"quotes"`, followed by a blank line, then the grouped includes above.
- Group includes as: standard library → project (`<...>`, path-sorted) → third-party. Separate groups with a blank line.
- Use **`#pragma once`** for header guards — never `#ifndef` guards.

Within a class or large file, organize members under banner comments. Common banners, in order:
`// Constructors`, `// Public methods`, `// Public members`, then `private:` with `// Members`. Large multi-type headers
(e.g. `src/ast/ASTNodes.h`) use ASCII-art separators like `// =========== AstNode ===========`.

Namespaces:
- Compiler code lives in `namespace spice::compiler`.
- Test code lives in `namespace spice::testing` (often with `using namespace spice::compiler;` inside).

## Naming (C++)

| Kind | Convention | Example |
| --- | --- | --- |
| Class / struct | `PascalCase` | `class SymbolTableEntry`, `struct CodeLoc` |
| Function / method | `camelCase`, verb-first | `getQualType()`, `updateState()`, `isInitialized()` |
| Member variable | `camelCase` — **no `m_` prefix, no trailing `_`** | `scope`, `declNode`, `isVolatile` |
| Local variable / parameter | `camelCase` | `newType`, `overwriteExistingType` |
| Constant (`constexpr` / `const` global) | `SCREAMING_SNAKE_CASE` | `OP_FCT_PREFIX`, `TEST_FILE_NAME` |
| Enum type | `PascalCase` | `SemanticErrorType`, `LifecycleState` |
| Enum value | `SCREAMING_SNAKE_CASE` | `REFERENCED_UNDEFINED_FUNCTION` |
| Visitor method | `visit` + node `PascalCase` | `visitMainFctDef`, `visitFctDef` |
| File | `PascalCase`, `.h`/`.cpp` pair | `SymbolTableEntry.h` |
| Unit test file | `Unit` + `PascalCase` | `UnitFileUtil.cpp`, `UnitDriver.cpp` |

Reference example — note camelCase members with **no** prefix or suffix, and `const` correctness:

```cpp
class SymbolTableEntry final {
public:
  // Public methods
  [[nodiscard]] const QualType &getQualType() const;
  void updateState(const LifecycleState &newState, const ASTNode *node);

  // Public members
  const std::string name;
  Scope *scope;
  bool isVolatile = false;

private:
  // Members
  QualType qualType;
};
```

## Types, classes & structs (C++)

- Use **`struct`** for small, mostly-public data holders / POD types (`struct CodeLoc`, `struct Param`).
- Use **`class`** for types with meaningful behavior and encapsulation (`class Function`, `class TypeChecker`).
- Mark leaf classes that are not meant to be inherited from as **`final`** (`class SymbolTableEntry final`).
- Mark accessors and pure query methods that return a value with **`[[nodiscard]]`**.
- Prefer **explicit fixed-width underlying types** on enums: `enum SemanticErrorType : uint8_t { ... }`. Plain `enum`
  (with an explicit underlying type) is the norm here rather than `enum class`.
- Use **`std::move`** to transfer ownership of by-value constructor parameters into members.
- Guard the size of hot, frequently-allocated types with `static_assert`, e.g. `static_assert(sizeof(ASTNode) == 48);`.
  If you change such a type and the assert fires, that is intentional friction — review whether the growth is justified
  and update the expected size deliberately.

## The visitor pattern & AST nodes

The compiler passes (symbol table builder, type checker, IR generator, …) traverse the AST via classic double-dispatch:

- Every AST node implements `accept(AbstractASTVisitor *)` and a `const` `accept(ParallelizableASTVisitor *)`.
- A pass derives from `ASTVisitor` (sequential) or `ParallelizableASTVisitor` and overrides one
  `visitXxx(XxxNode *node)` method per node type, returning `std::any`.
- AST node children are declared with the `GET_CHILDREN(...)` macro, which expands to a `getChildren()` override built
  on `collectChildren(...)`.

When adding a new AST node, follow the existing node definitions in `src/ast/ASTNodes.h` and add the corresponding
`visit*` method to the visitor interfaces and every concrete pass. See the `spice-language-feature` skill for the
end-to-end recipe.

## Error handling & diagnostics

User-facing diagnostics are raised as **C++ exceptions** from a dedicated hierarchy under `src/exception/`:
`SemanticError`, `ParserError`, `LexerError`, `LinkerError`, `CompilerError`, `CliError`. They are caught at pass
boundaries.

Each diagnostic class pairs:
- an `enum XxxErrorType : uint8_t { ... }` of message codes,
- a `getMessagePrefix()` switch mapping each code to a human-readable prefix,
- a `what()` override producing a formatted message like `[Error|Semantic] <loc>:\n<prefix>: <message>`.

To add a new error or warning, add the enum value and its prefix, then throw from the relevant pass. Follow the
`spice-diagnostics` skill, and add the matching `exception.out` / `warning.out` reference test.

## Memory management

- The codebase favors **raw pointers owned by long-lived containers** (`Scope *`, `ASTNode *`, `llvm::Value *`) over
  per-object `std::unique_ptr`.
- **AST nodes are arena/block-allocated** (see `UnitBlockAllocator`), not individually `new`/`delete`-d. Do not
  introduce `unique_ptr<ASTNode>` ownership.
- Use smart pointers / RAII where a clear single owner exists and it matches nearby code, but do not retrofit the
  pointer style of existing subsystems.
- Validate memory-sensitive changes with sanitizers and valgrind (see `AGENTS.md` → *Debugging* and the
  `spice-debugging` skill).

## Comments & documentation

- Document public types and functions with **Doxygen-style block comments**:

  ```cpp
  /**
   * Entry of a symbol table, representing an individual symbol with all its properties
   */
  ```

  Use `@param` / `@return` tags where they add clarity.
- Use `//` line comments for inline explanation of non-obvious logic, and short `// Banner` comments to group sections.
- Comment the **why**, not the obvious **what**. Keep comments in sync with the code they describe.
- Update user-facing docs under `docs/docs/` whenever behavior, CLI, or output changes.

## Spice language style

Conventions for `.spice` sources in `std/` and `test/test-files/`:

| Kind | Convention | Example |
| --- | --- | --- |
| Type / struct | `PascalCase` | `String`, `LinkedList<T>`, `Node<T>` |
| Type alias | `PascalCase` | `type IntLong int\|long;` |
| Function / method | `camelCase` | `pushBack`, `isEmpty`, `createNode` |
| Field / variable | `camelCase` | `contents`, `capacity`, `length` |
| Constant | `SCREAMING_SNAKE_CASE` | `INITIAL_ALLOC_COUNT`, `RESIZE_FACTOR` |

- **Indentation:** 4 spaces inside type and function bodies.
- **Braces:** K&R style, opening brace on the same line; **no parentheses** around `if` / loop conditions
  (`if this.isEmpty() {`).
- Functions use the `f<ReturnType>` form; procedures use `p`. Mark exported items `public`, external C symbols `ext`.
- **Comments:** Doxygen-style `/** ... */` blocks above public functions and types; trailing `//` comments for
  clarifying inline logic; short `// Section` headers (e.g. `// Constants`, `// Std imports`).
- When adding test inputs under `test/test-files/`, follow the `spice-add-test` skill for the directory layout and
  reference-file conventions.

## Commits & pull requests

See `AGENTS.md` and `CONTRIBUTING.md` for the full workflow. In short:

- Keep commits incremental and reviewable; write clear, descriptive messages.
- Before opening a PR, run at least a focused build and the tests relevant to your change.
- In the PR description, state **what** changed, **why**, **how it was validated** (exact commands + outcomes), and any
  follow-ups or known limitations.
- If environment limits prevent running checks, say so explicitly.
