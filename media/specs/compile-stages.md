# Compile stages design document

In order to achieve "separation of concerns" the plan is, to re-organize the compiler structure. We want to separate
such things like semantic analysis and type checker.

## Compile stages

1.  **Lexer** <br>
    Input/Output: Text input -> Tokens <br>
    Note: Splits the source input into tokens (ANTLR)

2.  **Parser** <br>
    Input/Output: Tokens -> CST <br>
    Note: Parses tokens to a Concrete Syntax Tree (ANTLR)

3.  **CST Visualizer** <br>
    Input/Output: CST -> CST <br>
    Note: Prints the CST as JSON or Dot code.

4.  **AST Builder** <br>
    Input/Output: CST -> AST <br>
    Note: Transforms the Concrete Syntax Tree to an Abstract Syntax Tree, enriches it and executes concluding syntax checks.

5.  **AST Optimizer** <br>
    Input/Output: AST -> AST <br>
    Note: Removes obviously unnecessary nodes like empty if-statements

6.  **AST Visualizer** <br>
    Input/Output: AST -> AST <br>
    Note: Prints the AST as JSON or Dot code.

7.  **Import Collector** <br>
    Input/Output: AST -> AST <br>
    Note: Checks, which other source file are imported by the current one. Registers external symbols as described
    [here](./better-imports.md).

8.  **Symbol Table Builder** <br>
    Input/Output: AST -> AST <br>
    Note: Creates the symbol table without types and lifetimes

9.  **Type Checker** <br>
    Input/Output: AST -> AST <br>
    Note: Checks if all types match, performs type inference, fill types in symbol table

10. **Borrow Checker** (new component, maybe added in the future) <br>
    Input/Output: AST -> AST <br>
    Additional used resources: Symbol Table <br>
    Note: Checks if all variable accesses are valid, escape analysis, fill lifetimes in symbol table

11. **Escape Analyzer** <br>
    Input/Output: AST -> AST <br>
    Additional used resources: Symbol Table <br>
    Note: Checks, which variables escape their scopes and check if these variables are marked as heap-allocated by the programmer.

12. **IR Generator** <br>
    Input/Output: AST -> IR <br>
    Additional used resources: Symbol Table <br>
    Note: Uses several helper modules to generate IR from the information of AST and Symbol Table.

13. **IR Optimizer** <br>
    Input/Output: IR -> IR <br>
    Note: Uses the stated optimization level to call the LLVM optimizer. In case of -O0, the IR Optimizer is not called.

14. **Object Emitter** <br>
    Input/Output: IR -> Object file <br>
    Note: Calls LLVM to emit an object file from the generated IR.

These stages are organized in classes, inheriting from the `CompilerPass` class. This class is responsible for holding
general information like the current source file, or the global resource manager.

When the compilation of a source file is triggered, it calls the stages one after another.

## Order of execution for multiple source files

Source file A imports B and C.

1. Lexer for A
2. Parser for A
3. CST Visualizer for A
4. AST Builder for A
5. AST Visualizer for A
6. Import Collector for A
   1. Trigger preparation for B
      1. Lexer for B
      2. Parser for B
      3. CST Visualizer for B
      4. AST Builder for B
      5. AST Visualizer for B
      6. Import Collector for B
   2. Trigger preparation for C
       1. Lexer for C
       2. Parser for C
       3. CST Visualizer for C
       4. AST Builder for C
       5. AST Visualizer for C
       6. Import Collector for C
7. Type Checker for B
8. Type Checker for C
9. Type Checker for A
10. Type Checker for A (again)
11. Type Checker for B (again)
12. Type Checker for C (again)
13. Borrow Checker for B
14. Borrow Checker for C
15. Borrow Checker for A
16. Escape Analyzer for B
17. Escape Analyzer for C
18. Escape Analyzer for A
19. IR Generator for B
20. IR Optimizer for B
21. Object Emitter for B
22. IR Generator for C
23. IR Optimizer for C
24. Object Emitter for C
25. IR Generator for A
26. IR Optimizer for A
27. Object Emitter for A

Note for parallelization: <br>
The last three steps (IR Generator, IR Optimizer, Object Emitter) are executed for every source file individually and
could be potentially parallelized.