---
name: spice-architecture
description: Orientation map of the Spice compiler codebase — the compilation pipeline (lexer → parser → AST → import collector → symbol table → type checker → IR generator → optimizer → object emitter → linker), the key file for each stage, and the SourceFile orchestration sequence. Use when navigating the compiler, deciding where a change belongs, or understanding how a stage feeds the next.
---

# Spice compiler architecture

The compiler is self-hosted: it is written in Spice and lives in `src/`. It is built by a released Spice compiler (the
stage0 compiler, see the `spice-build` skill). Imports of compiler files use the `bootstrap/` prefix (e.g.
`import "bootstrap/ast/ast-nodes";`), which resolves via `SPICE_BOOTSTRAP_DIR` to `src/`. The compiler emits LLVM IR through
the LLVM C API bindings of the std (`std/bindings/llvm`).

## Entry & orchestration

- `src/main.spice` → builds the project and invokes the linker.
- `src/source-file.spice` orchestrates the per-file pipeline in three phases. Each stage is a `run*` method; this is the
  call sequence to read first:

  **Front end** (`runFrontEnd`): `runParser` (lexer + parser) → (`runASTVisualizer`) → `runImportCollector` →
  `runSymbolTableBuilder`

  **Middle end** (`runMiddleEnd`): `runTypeCheckerPre` → `runTypeCheckerPost` → (`runDependencyGraphVisualizer`)

  **Back end** (`runBackEnd`): `runIRGenerator` → IR optimizer (`runDefaultIROptimizer`, or
  `runPreLinkIROptimizer`/`runBitcodeLinker`/`runPostLinkIROptimizer` for LTO) → `runObjectEmitter`

- `src/compiler-pass.spice` — base of the passes (scope tracking, source-file handle).
- `src/global/global-resource-manager.spice` — central owner of source files, caches, the type registry, and the linker.
  `cache-manager.spice`, `runtime-module-manager.spice` and `type-registry.spice` also live in `src/global/`.

## Stages → where the code is

| Stage | Dir / key file | Purpose |
|-------|----------------|---------|
| Read | `src/reader/` — `reader.spice`, `code-loc.spice` | Source text → characters with code locations |
| Lex | `src/lexer/` — `lexer.spice`, `token.spice` | Hand-written lexer: characters → tokens |
| Parse | `src/parser/parser.spice` | Hand-written recursive descent parser: tokens → AST. See `spice-language-feature` skill. |
| AST nodes / visitors | `src/ast/ast-nodes.spice`, `abstract-ast-visitor-intf.spice` | Node defs + visitor interface with default methods |
| Imports | `src/importcollector/import-collector.spice` | Resolve & recursively load dependencies |
| Symbols | `src/symboltablebuilder/` — `symbol-table-builder.spice`, `scope.spice`, `symbol-table.spice` | Build scopes & symbol tables |
| Types | `src/typechecker/` — `type-checker.spice` (pre then post mode), `function-manager.spice`, `struct-manager.spice`; `src/symboltablebuilder/qual-type.spice`, `type.spice` | Type inference/checking, generic instantiation, overload resolution |
| IR gen | `src/irgenerator/` — `ir-generator.spice`, `debug-info-generator.spice`, `name-mangling.spice` | AST → LLVM IR |
| IR opt | `src/iroptimizer/ir-optimizer.spice` | LLVM pass pipeline (incl. LTO) |
| Emit | `src/objectemitter/` — `llvm-object-emitter.spice`, `tpde-object-emitter.spice` | LLVM module → object/asm |
| Link | `src/linker/` — `external-linker-interface.spice`, `bitcode-linker.spice` | Object files → executable/library |
| CLI | `src/driver.spice` — `Driver`, `CliOptions` | Subcommands & flags (see `spice-run`/`spice-dump`) |
| Errors | `src/exception/`, `src/util/compiler-warning.spice` | Diagnostics (see `spice-diagnostics` skill) |
| Visualizers | `src/visualizer/` | AST/dependency-graph dumps (the `--dump-*` flags) |
| Linter | `src/linter/` | The `lint` subcommand |
| Models | `src/model/` | Cross-stage data (functions, structs, generics, …) |

## Notes

- The type checker runs **twice** (pre = bottom-up to resolve generic instantiations; post = top-down, may re-visit). Many
  "why isn't my type resolved" issues are about which pass populates what.
- Spice has no exceptions: compile errors print their message and exit (`src/exception/fatal-error.spice`), internal errors
  are panics.
- Unit tests are `#[test]` functions next to the code (e.g. in `src/driver.spice`), run by `python test/run-tests.py` as
  `BootstrapTests.BuiltinTests`.
- `media/specs/` holds design notes for language features — useful background before changing semantics. Some of them
  still describe the former host compiler (C++, removed in v0.29.0), which the compiler in `src/` mirrors.
- To see any stage's output for a file, use the `spice-dump` skill (`-ast`, `--dump-symtab`, `--dump-types`, `-ir`, `-s`).
