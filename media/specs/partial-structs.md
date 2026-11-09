# Technical Specification for Partial Structs (Part Files) in Spice

**Status:** Draft / design sketch, nothing implemented yet.

## Implementation steps:

- [ ] Parse `part "<path>";` and `part of "<path>";` (contextual keyword, no new reserved word)
- [ ] Update IntelliJ plugin and docs syntax highlighting
- [ ] Import collector: load part files, validate the `part` / `part of` handshake
- [ ] Symbol table builder: build part files into the root scope of their owner
- [ ] Restrict the allowed top-level constructs in part files
- [ ] Type checker: check part files as part of the owner (pre + post)
- [ ] IR generator: emit part files into the LLVM module of the owner
- [ ] Diagnostics: show code snippets from the right file
- [ ] Driver: reject a part file as the main source file or as an import target
- [ ] Tests (positive + one per new error) and user docs (`docs/docs/language/modules.md`, `methods.md`)

## Motivation

All methods of a struct must currently be defined in the source file that defines the struct. The symbol table builder
looks up the struct of a method definition only among the children of the current root scope
(`SymbolTableBuilder.visitFctOrProcDef`). A struct imported from another file is not found there
(`Referenced undefined struct: Struct 'Counter' could not be found`). The method name syntax also cannot name an imported
struct (`lib::Counter.get()` fails to parse).

This makes large structs hard to organize. The compiler is a good example: every `TypeChecker` method is in
`src/typechecker/type-checker.spice`, and every `IRGenerator` method is in `src/irgenerator/ir-generator.spice`, which is
more than 7,000 lines long. We want a way to split these files by topic (expressions, statements, calls, ...) without
changing the struct, its visibility or its users.

## Goals and non-goals

Goals:

- Define the methods of one struct in more than one source file.
- Keep the method set of a struct closed: every caller sees the same methods, whatever it imports.
- Keep the visibility model: private fields and methods stay private to the code of the struct's own "unit".
- Keep the implementation small. Reuse the existing scope, type checking and code generation logic as much as possible.

Non-goals (for now):

- Extension methods, i.e. adding methods to a struct from arbitrary other files (e.g. to add methods to `String`).
- Splitting the *fields* of a struct over several files (C# `partial class`).
- Compiling the parts of a unit in parallel (see [Limitations](#limitations)).

## Options considered

### A) Open extension methods (Swift / Kotlin / C#-style)

Any file that imports a struct may define new methods on it, e.g. `public f<int> lib::Counter.get()`.

- The method set of a struct depends on the import set of the calling file. Two files can see different methods for the
  same type, and one can see an overload that hides another.
- Extension methods cannot see private fields. Making them see private fields would break the visibility model.
- Methods added by other files cannot implement interfaces of the struct. The vtable is emitted by the defining file,
  which does not know about them.
- Two files may add the same public method. That is already a link error (`Multiple exported symbols with the same linker
  name`), but it is confusing for the user.

This is a separate feature with its own trade-offs. It does not solve the "split up a large struct" problem well.

### B) `partial` struct definitions (C#-style)

The struct is marked `partial` and may be defined several times, with the fields and methods merged across files.

- The memory layout of the struct (field order) depends on the order in which files are compiled.
- Every file that uses the struct needs every partial definition to know its size.
- The interfaces, template types and qualifiers of all definitions must be merged and kept consistent.

This adds a lot of complexity for little gain over option C.

### C) Part files (Dart-style, **recommended**)

A source file (the *owner*) declares a list of *part files*. A part file holds more top-level functions, procedures and
methods of the owner. The owner and its parts form one *unit*:

- The unit has **one** root scope, so a method in a part finds the struct exactly like a method in the owner does.
- Privacy is per unit, not per file. A part may use private fields, methods, functions and globals of the owner.
- Other files import only the owner, and see all methods of the unit.
- The unit is still one module for the back end, so name mangling, linkage and vtables do not change.

The rest of this document specifies option C.

## Syntax

```spice
// type-checker.spice (owner)
import "@/ast/ast-nodes";
import "@/symboltablebuilder/scope";

part "type-checker-expr";
part "type-checker-stmt";

public type TypeChecker struct : IAbstractAstVisitor {
    compose public CompilerPass compilerPass
    OpRuleManager opRuleManager
    // ...
}

public f<Any> TypeChecker.visitEntry(ASTEntryNode* node) { /* ... */ }
```

```spice
// type-checker-expr.spice (part)
part of "type-checker";

import "@/typechecker/op-rule-manager";

public f<Any> TypeChecker.visitAssignExpr(ASTAssignExprNode* node) {
    this.opRuleManager.getAssignResultType(/* ... */); // private field of TypeChecker: allowed
    // ...
}

f<bool> isAssignable(const QualType& lhs, const QualType& rhs) { /* private helper of the unit */ }
```

Grammar additions:

```
entry: (... | importDef | partDef | partOfDef | ...)* EOF;
partDef: 'part' STRING_LIT ';';
partOfDef: 'part' 'of' STRING_LIT ';';
```

`part` is a contextual keyword. At the top level, an identifier `part` followed by a string literal or by `of` cannot start
any other construct, so the parser can tell these apart with one or two tokens of lookahead. Code that uses `part` as an
identifier keeps working. Paths are resolved like import paths (relative to the declaring file, `@/` prefix, std paths,
`.spice` extension optional).

## Semantics

1. **Handshake:** A part file must start with exactly one `part of` declaration, before any other top-level construct. It
   must name the file that declares it with `part`. Both sides are required, so a part cannot be claimed by two owners.
   Tooling (IDE, linter) can also find the owner of a part file that is opened on its own.
2. **One unit, one root scope:** The top-level definitions of all parts are added to the root scope of the owner, as if
   they had been written at the end of the owner, in the order of the `part` declarations. Every rule that applies to
   symbols in one file now applies to the whole unit. Two parts defining the same function signature, for example,
   cause the usual redefinition error.
3. **Allowed constructs in a part:** functions, procedures, methods (of structs and interfaces of the unit), `ext`
   declarations, `#[test]` functions and `import` statements. Not allowed: type definitions (`struct`, `interface`,
   `enum`, `union`, `alias`, generic types), global variables, module attributes (`#![...]`), the `main` function and
   further `part` declarations (no nesting). These stay in the owner, so the struct and its layout are defined in one place
   and "defined before usage" stays a check within one file.
4. **Methods only for structs of the unit:** A method in a part can only be defined for a struct or interface defined in
   the owner. A method for an imported struct is still an error, with the same message as today. Rule 2 makes this happen
   without extra logic.
5. **Visibility per unit:** Private symbols of the unit are visible to every file of the unit, and to nothing else.
   `public` keeps its meaning. It makes a symbol visible to files that import the owner.
6. **Imports:** The imports of a part are merged into the imports of the owner, as if the owner had written them. An
   import name that is used for two different paths in the unit is an error. A part importing its own owner, or another
   part of the same unit, is an error.
7. **No direct use of a part:** A part file cannot be imported, and cannot be the main source file of `spice build`,
   `spice run` or `spice test`. Files that need the code of a part import the owner.
8. **Generics:** Methods of generic structs defined in parts behave exactly like methods in the owner. The struct
   manager copies the struct scope for each manifestation, and that scope already contains the methods of all parts.
9. **Interfaces and vtables:** Methods in parts count when the compiler checks that a struct implements its interfaces,
   and they get vtable slots like any other method. The owner emits the vtable as today.

### Examples of errors

```spice
// helper.spice
part of "counter";

type Extra struct { int x }     // Error: Type definitions are not allowed in part files
p Other.doIt() {}               // Error: Referenced undefined struct: Struct 'Other' could not be found
```

```spice
// main.spice
import "helper";                // Error: Cannot import part file 'helper', import its owner 'counter' instead
```

```spice
// stray.spice
part of "counter";              // Error: 'counter' does not declare 'stray' as a part
```

## Implementation

The main idea: a part is a real `SourceFile` for reading, lexing and parsing (so code locations and error snippets point to
the right file). From the symbol table builder on, it is handled as an extension of its owner.

- **Lexer / parser:** Add `ASTPartDefNode` and `ASTPartOfDefNode` (path + code location) to `ASTEntryNode`, next to
  `importDefs`. Handle them in `Parser.parseTopLevelDef` before the discriminating-token logic. In a part, check that
  `part of` comes first.
- **Import collector:** For each `part` declaration of a file, resolve the path and create (or look up) the `SourceFile`
  of the part through the global resource manager. Mark it as a part (`SourceFile.ownerFile`). Parse it and check the
  handshake (rule 1). Collect the part's imports for the owner (rule 6): the owner's dependency list gets the part's
  dependencies, so the existing dependency graph, circular import handling and the middle end fixpoint stay as they are.
  An `import` of a file whose `ownerFile` is set fails with a new error (rule 7).
- **Symbol table builder:** After visiting the owner's AST, visit the top-level definitions of each part with
  `currentScope` set to the owner's root scope. This one change makes `visitFctOrProcDef` find the struct scope, and makes
  `Scope.isImportedBy` treat part and owner as the same unit, so the visibility checks in the type checker need no
  change. Report the forbidden constructs of rule 3 here.
- **Type checker:** The owner's `runTypeCheckerPre` / `runTypeCheckerPost` also visit the ASTs of its parts, with the
  same root scope, and the re-visit loop covers them. Part files do not run the middle end on their own. The "Structs
  must be defined before usage" check compares `CodeLoc.sourceFilePath`, so it ignores uses in parts. That is correct,
  because rule 3 keeps all types in the owner.
- **IR generator:** The owner's `IRGenerator` also visits the parts' top-level definitions and emits them into the owner's
  `llvm::Module`. Part files produce no module and no object file of their own. Name mangling does not change: the mangled
  name of a method has the struct name, not the file. Private symbols keep internal linkage, because they stay in one
  module.
- **Debug info:** `DebugInfoGenerator` needs a `DIFile` per part, chosen by the `sourceFilePath` of each node's code
  location, so that debuggers show the right file and line.
- **Diagnostics:** Errors already carry `CodeLoc.sourceFilePath`. The snippet printer must read the source of the file in
  the code location (the part), not of the `SourceFile` that runs the pass (the owner).
- **Driver:** Reject a part file as the main source file of `build`, `run`, `test` and `install`, with a hint naming
  the owner.
- **Dumps:** `--dump-ast` and `--dump-symtab` of the owner include the parts, so the dumps match what is compiled.
  The dependency graph visualizer shows parts as children of their owner, with a different edge style than imports.
- **Linter:** Lints a part file as part of its owner, found through the `part of` declaration.

### New diagnostics

| Kind     | Message (sketch)                                                                 |
|----------|----------------------------------------------------------------------------------|
| Semantic | `Part file '<path>' does not exist`                                              |
| Semantic | `'<owner>' does not declare '<part>' as a part`                                  |
| Semantic | `Part file '<part>' must start with 'part of "<owner>";'`                        |
| Semantic | `Part file '<part>' is claimed by '<owner1>' and '<owner2>'`                     |
| Semantic | `<construct> is not allowed in part files`                                       |
| Semantic | `Cannot import part file '<part>', import its owner '<owner>' instead`           |
| Semantic | `Import name '<name>' is used for different files within the unit '<owner>'`     |
| CLI      | `'<part>' is a part file and cannot be compiled on its own, use '<owner>'`       |

### Tests

Following the `spice-add-test` conventions in `test/test-files/`:

- Positive: struct with methods split over two parts, using private fields, calling private helpers across parts,
  generic struct with methods in a part, interface methods implemented in a part (virtual call), `#[test]` in a part.
- Debug info: line info of a method in a part points to the part file.
- One error test per row of the table above, plus a method in a part for an imported struct.

## Limitations

- **No parallel compilation of parts:** One unit is one LLVM module, so the back end compiles its parts one after the
  other. Splitting a file into parts organizes the source but does not speed up compilation. A later version could emit
  each part into its own module, which needs cross-module declarations for private symbols (and so non-internal linkage
  with unit-unique mangled names).
- **Types stay in the owner:** Private helper structs used by one part only must still be defined in the owner.
- **No extension methods:** Adding methods to structs of other units (option A) is still not possible.

## Open questions

1. Should `part of` be optional, as long as exactly one owner declares the part? It saves a line, but tooling would
   then need to search for the owner of a part opened on its own.
2. Should parts be allowed to define private types and globals (relaxing rule 3)? This needs a defined order for
   "defined before usage" across the files of a unit.
3. Should the `part` declarations be written as a module attribute instead
   (`#![core.compiler.part = "type-checker-expr"]`)? That needs no grammar change, but makes the feature less visible.
4. Should a struct be allowed to have methods spread over parts of different units (e.g. owner in `lib`, part in an
   application)? This is option A again and is left out on purpose.
