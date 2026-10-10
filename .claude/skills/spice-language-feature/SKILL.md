---
name: spice-language-feature
description: End-to-end recipe for adding or changing a Spice language construct — extend the hand-written lexer and parser, add/extend an AST node and its visitor method, then thread the construct through SymbolTableBuilder, TypeChecker and IRGenerator, and add tests. Use when implementing new syntax/semantics in the compiler.
---

# Add a Spice language feature

A new construct touches the whole pipeline in `src/`. Work front-to-back; the compiler won't build until the AST/visitor
wiring is consistent. Remember that the compiler is built by the pinned stage0 compiler: the compiler sources themselves
can only use the new construct once a release with it is the stage0 compiler (see the `spice-build` skill).

## 1. Lexer — `src/lexer/`

Add new tokens to `TokenType` in `src/lexer/token.spice` and recognize them in `src/lexer/lexer.spice` (keywords are matched
by their identifier, e.g. `if identifier == "if" { return Token(TokenType::IF, "if", codeLoc); }`).

## 2. AST node — `src/ast/`

For a brand-new node:

1. **`src/ast/ast-nodes.spice`** — add an `ASTNodeKind` value and a `public type ASTXyzNode struct : IVisitable` that
   composes the matching base (e.g. `compose public ASTStmtNode node`), with a ctor setting `this.kind`, the `accept()`
   method calling `visitor.visitXyz(this)` and typed fields for its children, mirroring nearby nodes. Add the node kind to
   `destructASTNode` (`destructAs<ASTXyzNode>(node)`) and to the other `switch`es over node kinds where it matters.
2. **`src/ast/abstract-ast-visitor-intf.spice`** — add the `visitXyz` signature to `IAbstractAstVisitor` and its default
   method, which visits the children via `visitChildren`.
3. **`src/visualizer/ast-visualizer.spice`** — add `visitXyz` (`return this.buildNode(node);`), so `--dump-ast` shows it.

If you're only extending an existing construct, you may just add fields/children to the existing node instead.

## 3. Parser — `src/parser/parser.spice`

The hand-written recursive descent parser builds the AST directly. Add/extend the `parseXyz` method: create the node with
`this.createNode<ASTXyzNode>()` (it becomes a child of the node on top of the parent stack), consume tokens with
`this.expect(TokenType::...)` / `this.currentTokenIs(...)`, assign the parsed children to the node's fields, and finish
with `return this.concludeNode(node);`. Call it from the parse method of the enclosing construct. Raise syntax errors with
`this.throwParserError(...)`.

## 4. Thread through the semantic + back-end passes

Implement `visitXyz` (or update the touched node's handlers) in each pass that must understand the construct:

- **`src/symboltablebuilder/symbol-table-builder.spice`** — declare scopes/symbols it introduces.
- **`src/typechecker/type-checker.spice`** — type rules. Remember it runs twice (pre mode bottom-up for generics, post mode
  top-down). Split logic by mode as neighbors do.
- **`src/irgenerator/ir-generator.spice`** — emit LLVM IR (via the std LLVM bindings, `std/bindings/llvm`).
- Touch `src/importcollector/` only if the construct affects imports, and `src/linter/` if a lint rule should know it.

Emit diagnostics for invalid uses via the `spice-diagnostics` skill.

## 5. Build, inspect, test

```sh
python build.py --build-type Debug
# Eyeball each stage for a scratch file (see spice-dump skill)
build/spice build -ast --dump-symtab -ir --abort-after-dump scratch.spice
```

Then add reference tests (see `spice-add-test` skill): typically a `parser`/`typechecker`/`irgenerator` case with
`source.spice` plus generated refs via `--update-refs`, and an error case with `exception.out` for invalid
syntax/semantics. Update `docs/docs/language/` if the feature is user-facing.

## Checklist

- [ ] Tokens in `token.spice` / `lexer.spice`
- [ ] AST node in `ast-nodes.spice` (`ASTNodeKind`, `accept`, destruction)
- [ ] `visitXyz` in `IAbstractAstVisitor` + default method, AST visualizer
- [ ] `parseXyz` in `parser.spice`
- [ ] `SymbolTableBuilder` / `TypeChecker` (pre+post) / `IRGenerator`
- [ ] Diagnostics for misuse
- [ ] Tests (success + error) and docs
