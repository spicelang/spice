# Technical Specification for Interfaces in Spice

## Implementation steps:

- [x] 1. Adjust grammar
- [x] 2. Update IntelliJ plugin
- [x] 3. Add semantic checks
- [x] 4. Add tests for semantic checks
- [ ] 5. Allow optional args for interface methods
- [ ] 6. Allow template types for interface methods
- [ ] 7. Match visibility modifiers

## Syntax

```spice
type AstNode interface {
    f<bool> accept(ASTVisitor*)
    p dump()
}

type MainFctDefNode struct : AstNode {
    SymbolTable* fctScope
    bool takesArgs
}

// Must exist. Otherwise the compiler throws an error
f<bool> MainFctDefNode.accept(ASTVisitor* v) {
    // Do something
}
```

## Functionality
Interfaces in Spice enable the programmer to define a bunch of methods that a struct must implement, when the interface is attached
to it. The interface needs to be attached to a struct explicitly to make it clearer for the reader, that the struct is part of the
collection of structs, implementing this particular interface.