---
name: spice-language-feature
description: End-to-end recipe for adding or changing a Spice language construct — edit the ANTLR grammar (Spice.g4), rebuild to regenerate the parser, add/extend an AST node and its visitor method, then thread the construct through ASTBuilder, SymbolTableBuilder, TypeChecker and IRGenerator, and add tests. Use when implementing new syntax/semantics in the compiler.
---

# Add a Spice language feature

A new construct touches the whole pipeline. Work front-to-back; the compiler
won't build until the AST/visitor wiring is consistent.

## 1. Grammar — `src-host/Spice.g4`

ANTLR 4.13.2 grammar. Add/modify the rule (e.g. under "Control structures" or
"Statements"). The parser is **generated at build time** via
`antlr_target(Spice Spice.g4 VISITOR PACKAGE spice::compiler)` in
`src-host/CMakeLists.txt` — there is no separate codegen command, just rebuild:

```sh
cmake --build cmake-build-debug --target spice
```

This regenerates `SpiceLexer`, `SpiceParser`, and `SpiceVisitor`. Keep tokens
(uppercase lexer rules) and existing rule names consistent with the file's style.

## 2. AST node — `src-host/ast/`

For a brand-new node:

1. **`src-host/ast/ASTNodes.h`** — add a `class XyzNode : public ASTNode`, give it the
   `accept()` override and the `GET_CHILDREN(...)` macro listing child nodes;
   add typed accessors mirroring nearby nodes.
2. **`src-host/ast/AbstractASTVisitor.h`** — add a forward declaration and a
   `virtual std::any visitXyz(XyzNode *node) = 0;` (and the const variant in
   `ParallelizableASTVisitor.h` if IR gen will visit it).
3. **`src-host/ast/ASTVisitor.{h,cpp}`** — add the default `visitXyz` that recurses
   into children (so passes that don't care still traverse).

If you're only extending an existing rule, you may just add fields/children to
the existing node instead of creating a new one.

## 3. CST → AST — `ASTBuilder` (`src-host/ast/ASTBuilder.*`)

`ASTBuilder` overrides the generated `SpiceVisitor` `visit*` methods and
constructs AST nodes from parser contexts. Add/extend the `visitXyz` that maps
your grammar rule's parse context onto the new/changed AST node.

## 4. Thread through the semantic + back-end passes

Implement `visitXyz` (or update the touched node's handlers) in each pass that
must understand the construct:

- **`src-host/symboltablebuilder/SymbolTableBuilder.*`** — declare scopes/symbols it introduces.
- **`src-host/typechecker/TypeChecker*.cpp`** — type rules. Remember it runs twice
  (`TC_MODE_PRE` bottom-up for generics, `TC_MODE_POST` top-down). Split logic by mode as neighbors do.
- **`src-host/irgenerator/IRGenerator*.cpp`** — emit LLVM IR (const/parallelizable visitor).
- Touch `ImportCollector` only if the construct affects imports.

Emit diagnostics for invalid uses via the `spice-diagnostics` skill.

## 5. Build, inspect, test

```sh
cmake --build cmake-build-debug --target spice
# Eyeball each stage for a scratch file (see spice-dump skill)
cmake-build-debug/src-host/spice build -cst -ast --dump-symtab -ir --abort-after-dump scratch.spice
```

Then add reference tests (see `spice-add-test` skill): typically a
`parser`/`typechecker`/`irgenerator` case with `source.spice` plus generated
refs via `--update-refs`, and an error case with `exception.out` for invalid
syntax/semantics. Update `docs/docs/language/` if the feature is user-facing.

## Checklist

- [ ] `Spice.g4` rule (+ tokens)
- [ ] AST node in `ASTNodes.h` (`accept` + `GET_CHILDREN`)
- [ ] `visitXyz` in `AbstractASTVisitor.h` / `ParallelizableASTVisitor.h` + default in `ASTVisitor`
- [ ] `ASTBuilder` mapping
- [ ] `SymbolTableBuilder` / `TypeChecker` (pre+post) / `IRGenerator`
- [ ] Diagnostics for misuse
- [ ] Tests (success + error) and docs
