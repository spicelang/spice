# Compile stages design document

In order to achieve "separation of concerns" the plan is, to re-organize the compiler structure. We want to separate
such things like semantic analysis and type checker.

## Overview

```mermaid
graph TD;
    Lexer-->
    Parser-->
    CSTVisualizer-->
    ASTBuilder-->
    ASTVisualizer-->
    ImportCollector-->
    SymbolTableBuilder-->
    TypeChecker-->
    PostTypeCheckingVerifier-->
    DepGraphVisualizer-->
    IRGenerator-->
    DefaultIROptimizer-->
    ObjectEmitter-->
    Linker;
    
    IRGenerator-->
    PreLinkOptimizer-->
    BitcodeLinker-->
    PostLinkOptimizer-->
    ObjectEmitter;
```

## Compile stages in detail

1.  **Lexer** <br>
    Input/Output: Text input -> Tokens <br>
    Note: Splits the source input into tokens (ANTLR)

2.  **Parser** <br>
    Input/Output: Tokens -> CST <br>
    Note: Parses tokens to a Concrete Syntax Tree (ANTLR)

3.  **CST Visualizer** <br>
    Input/Output: CST -> CST <br>
    Note: Prints the CST as Dot code.

4.  **AST Builder** <br>
    Input/Output: CST -> AST <br>
    Note: Transforms the Concrete Syntax Tree to an Abstract Syntax Tree, enriches it and executes concluding syntax checks.

5.  **AST Visualizer** <br>
    Input/Output: AST -> AST <br>
    Note: Prints the AST as Dot code.

6.  **Import Collector** <br>
    Input/Output: AST -> AST <br>
    Note: Checks which other source files are imported by the current one (recursively, allowing import cycles) and
    processes module attributes. External symbols are merged into each file's name registry in a separate pass once
    every reachable file has built its own symbol table - see [imports design](./better-imports.md).

7.  **Symbol Table Builder** <br>
    Input/Output: AST -> AST <br>
    Note: Creates the symbol table without types and lifetimes

8.  **Type Checker** <br>
    Input/Output: AST -> AST <br>
    Note: Checks if all types match, performs type inference, fill types in symbol table

9.  **Post-TypeChecker Annotation Verifier** *(debug builds only)* <br>
    Input/Output: AST -> AST <br>
    Note: Asserts that the TypeChecker has fully annotated the AST before codegen begins — entry pointers are non-null,
    per-manifestation vectors are correctly sized, and every non-function-pointer call has a resolved callee. Compiled
    out in release builds (`NDEBUG`).

10. **Dependency Graph Visualizer** <br>
    Input/Output: AST -> AST <br>
    Note: Prints the compile unit dependency graph as Dot code (the graph may contain cycles when source files import
    each other)

11. **IR Generator** <br>
    Input/Output: AST -> IR <br>
    Additional used resources: Symbol Table <br>
    Note: Uses several helper modules to generate IR from the information of AST and Symbol Table.

12. **IR Optimizer** <br>
    Input/Output: IR -> IR <br>
    Note: Uses the stated optimization level to call the LLVM optimizer. In case of -O0, the IR Optimizer is not invoked.

13. **Object Emitter** <br>
    Input/Output: IR -> Object file <br>
    Note: Calls LLVM to emit an object file from the generated IR.

These stages are organized in classes, inheriting from the `CompilerPass` class. This class is responsible for holding
general information like references to the current source file, or the global resource manager.

When the compilation of a source file is triggered, it calls the stages one after another.

## Order of execution for multiple source files

Source file A imports B and C.

1. Lexer for A
2. Parser for A
3. CST Visualizer for A
4. AST Builder for A
5. AST Visualizer for A
6. Import Collector for A
   1. Trigger compilation of B
      1. Lexer for B
      2. Parser for B
      3. CST Visualizer for B
      4. AST Builder for B
      5. AST Visualizer for B
      6. Import Collector for B
   2. Trigger compilation of C
       1. Lexer for C
       2. Parser for C
       3. CST Visualizer for C
       4. AST Builder for C
       5. AST Visualizer for C
       6. Import Collector for C
7. Symbol Table Builder for B
8. Symbol Table Builder for C
9. Symbol Table Builder for A
10. Type Checker for B (prepare)
11. Type Checker for C (prepare)
12. Type Checker for A (prepare)
13. Type Checker for A (check)
14. Type Checker for B (check)
15. Type Checker for C (check)
16. Post-TypeChecker Annotation Verifier for A *(debug only)*
17. Post-TypeChecker Annotation Verifier for B *(debug only)*
18. Post-TypeChecker Annotation Verifier for C *(debug only)*
19. IR Generator for B
20. IR Optimizer for B
21. Object Emitter for B
22. IR Generator for C
23. IR Optimizer for C 
24. Object Emitter for C 
25. IR Generator for A 
26. IR Optimizer for A 
27. Object Emitter for A

## Circular imports

The dependency graph may contain cycles, because two source files are allowed to import each other (directly or
transitively). To support this without infinite recursion, the per-file pipeline drivers guard against re-entering a
file that is already being processed in the same stage, and two stages are decoupled from the strict
bottom-up/top-down ordering that a DAG would otherwise allow:

- The external name registries are merged in a dedicated pass after the whole front-end has run, so a file in a cycle
  sees the fully-built registry of its cycle peers (a dependency's own symbols are guaranteed to be registered by then).
  A file's own symbols still take precedence over imported symbols of the same name.
- A struct or interface that is referenced across a cycle before it has been prepared is given its opaque type on
  demand (an implicit forward declaration); the full definition - body scope, fields and manifestations - is filled in
  when that type is prepared.
- The type checker treats a strongly connected component of the dependency graph as a unit: a file does not wait for
  cycle peers before being type-checked, and the component is iterated to a fixpoint via the existing re-visit
  mechanism instead of relying on a strict topological order.

## Note for parallelization:

The last three steps (IR Generator, IR Optimizer, Object Emitter) are executed for every source file individually and
can be parallelized.