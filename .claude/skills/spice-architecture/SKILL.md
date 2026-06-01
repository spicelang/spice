---
name: spice-architecture
description: Orientation map of the Spice compiler codebase — the compilation pipeline (lexer → parser → CST → AST → import collector → symbol table → type checker → IR generator → optimizer → object emitter → linker), the key class/file for each stage, and the SourceFile orchestration sequence. Use when navigating the compiler, deciding where a change belongs, or understanding how a stage feeds the next.
---

# Spice compiler architecture

The compiler lives in `src/` (C++23). A self-hosted mirror in Spice lives in
`src-bootstrap/` (work in progress, parallels `src/` file-for-file).

## Entry & orchestration

- `src/main.cpp` → `compileProject()` builds the project and invokes the linker.
- `src/SourceFile.cpp` / `.h` orchestrates the per-file pipeline in three phases.
  Each stage is a `run*` method; this is the call sequence to read first:

  **Front end** (`runFrontEnd`): `runLexer` → `runParser` → (`runCSTVisualizer`) →
  `runASTBuilder` → (`runASTVisualizer`) → `runImportCollector` → `runSymbolTableBuilder`

  **Middle end** (`runMiddleEnd`): `runTypeCheckerPre` → `runTypeCheckerPost` →
  (`runDependencyGraphVisualizer`)

  **Back end** (`runBackEnd`): `runIRGenerator` → IR optimizer
  (`runDefaultIROptimizer`, or `runPreLinkIROptimizer`/`runBitcodeLinker`/`runPostLinkIROptimizer` for LTO)
  → `runObjectEmitter` → `concludeCompilation`

- `src/CompilerPass.h` — base class for passes (scope tracking, source-file handle).
- `src/global/GlobalResourceManager.*` — central owner of source files, caches,
  the type registry, and the linker. `CacheManager`, `RuntimeModuleManager`,
  `TypeRegistry` also live in `src/global/`.

## Stages → where the code is

| Stage | Dir / key class | Purpose |
|-------|-----------------|---------|
| Lex + parse | ANTLR-generated `SpiceLexer`/`SpiceParser` (from `src/Spice.g4`) | Source → token stream → CST. See `spice-language-feature` skill. |
| CST → AST | `src/ast/` — `ASTBuilder` | Builds the AST from the parser's CST |
| AST nodes / visitors | `src/ast/ASTNodes.h`, `AbstractASTVisitor.h`, `ParallelizableASTVisitor.h`, `ASTVisitor.h` | Node defs + visitor interfaces |
| Imports | `src/importcollector/` — `ImportCollector` | Resolve & recursively load dependencies |
| Symbols | `src/symboltablebuilder/` — `SymbolTableBuilder`, `Scope`, `SymbolTable` | Build scopes & symbol tables |
| Types | `src/typechecker/` — `TypeChecker` (`TC_MODE_PRE` then `TC_MODE_POST`), `QualType`, `Type` | Type inference/checking, generic instantiation, overload resolution |
| IR gen | `src/irgenerator/` — `IRGenerator` (a `ParallelizableASTVisitor`) | AST → LLVM IR |
| IR opt | `src/iroptimizer/` — `IROptimizer` | LLVM pass pipeline (incl. LTO) |
| Emit | `src/objectemitter/` — `ObjectEmitter` | LLVM module → object/asm |
| Link | `src/linker/` — `ExternalLinkerInterface` | Object files → executable/library |
| CLI | `src/driver/Driver.*` — `Driver`, `CliOptions` | Subcommands & flags (see `spice-run`/`spice-dump`) |
| Errors | `src/exception/`, `src/util/CompilerWarning.*` | Diagnostics (see `spice-diagnostics` skill) |
| Visualizers | `src/visualizer/` | CST/AST/dependency-graph dumps (the `--dump-*` flags) |
| Models | `src/model/` | Cross-stage data (functions, structs, generics, …) |

## Notes

- The type checker runs **twice** (pre = bottom-up to resolve generic
  instantiations; post = top-down, may re-visit). Many "why isn't my type
  resolved" issues are about which pass populates what.
- IR generation uses the *parallelizable* (const) visitor; other passes use the
  mutating `ASTVisitor`.
- `src/Spice.g4` is the source of truth for syntax; the parser is **generated at
  build time**, not checked in.
- `media/specs/` holds design notes for language features — useful background
  before changing semantics.
- To see any stage's output for a file, use the `spice-dump` skill
  (`-cst`, `-ast`, `--dump-symtab`, `--dump-types`, `-ir`, `-s`).
