# Spice Programming Language

## Grammar
The Spice grammar can be found [here](./compiler/src/grammar/Spice.g4) as a ANTLR grammar configuration file.

## Semantic analysis checks
- [x] Assignments have the same type
- [x] If condition is bool
- [x] While condition is bool
- [x] For condition is bool
- [x] Condition of ternery is bool
- [x] True type and false type of ternary are matching
- [x] Variables are declared before referenced
- [x] Variables are declared exactly once
- [x] Functions/Procedures are defined before called
- [x] Can only call functions/procedures and no variables
- [x] Only use parameters, that exist in the declaration
- [x] Parameter types are matching
- [x] Return statement only inside of function
- [x] Return data type matches function def
- [x] Logical operators are only applied to booleans
- [x] Bitwise operators are only applied to booleans or integers
- [x] Equality operators are only applied to some type combinations
- [x] Relational operators are only applied to doubles or integers
- [x] Additive operators are only applied to some type combinations
- [x] Multiplicative operators are only applied to some type combinations
- [x] Prefix/postfix unary operators are only applied to integers
- [x] Program contains main function
- [x] `++` and `--` are only applied on identifiers
- [ ] Variables with `const` attached are not modified later

## Special language features
- Something like `"Test" * 3` is valid and will evaluate to `"TestTestTest"`
- Alternatively to the return statement in a function, you can also assign variable `result` with a value, which was automatically declared by the function head
- Binary minus has to be applied with a space between (e.g.: `n - 5`) and unary minus has to be applied without a space between (e.g.: `-3.4`)
- Default values of function/procedure parameters are possible e.g.: `f<int> test(int param = 2) {...}`
- Function/procedure parameters with a default value have to be last in the param list

## CMake instructions for building
`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -G "CodeBlocks - MinGW Makefiles" ../llvm` <br>
`cmake --build .`