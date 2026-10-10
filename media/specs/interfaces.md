# Technical Specification for Interfaces in Spice

## Implementation steps:

- [x] Adjust grammar
- [x] Update IntelliJ plugin
- [x] Add semantic checks
- [x] Add tests for semantic checks
- [x] Allow template types for interface methods
- [ ] Allow optional args for interface methods
- [x] Default methods for interfaces
- [ ] Default methods for generic interfaces

## Syntax

```spice
type AstNode interface {
    f<bool> accept(ASTVisitor*);
    p dump();
}

type MainFctDefNode struct : AstNode {
    SymbolTable* extFunctionScope
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

## Default methods

### Motivation
Interfaces with many methods force every implementing struct to implement all of them, even if most implementations are
identical. The visitor interface of the compiler is a good example: every compiler pass implements ~80 visit methods,
most of them only visiting the children of the node or doing nothing at all.

### Syntax
A default method is defined like any other method, but on the interface instead of a struct. The interface body stays a pure
list of signatures:

```spice
public type IVisitor interface {
    public f<int> visitNode(Node*);
    public f<int> visitChildren(Node*);
}

// Default method
public f<int> IVisitor.visitNode(Node* node) {
    return this.visitChildren(node);
}

type Counter struct : IVisitor {
    int count
}

// Counter only implements visitChildren and inherits visitNode from IVisitor
public f<int> Counter.visitChildren(Node* node) {
    this.count++;
    return this.count;
}
```

### Semantics
Default methods keep the purpose of an interface intact, because they follow these rules:

1. **No state:** Interfaces have no fields. Within a default method, `this` is of the interface pointer type, so the body can
   only call other methods of the interface or pass `this` to functions, that accept the interface. Default methods are
   therefore pure behavior, derived from the contract of the interface.
2. **The struct always wins:** If a struct implements a method itself, its implementation is used. No keyword is required to
   override a default method.
3. **No silent conflicts:** If a struct implements multiple interfaces, that declare the same method, and one of them provides
   a default method for it, the struct has to implement the method itself. Otherwise, the compiler reports an error.
4. **Exact signature:** A default method has to implement a method signature of its interface with exactly the same parameter
   and return types. Default methods cannot be constructors or destructors and cannot have optional parameters.
5. **No accidental fallback:** If a struct declares a method with the name of a default method, but with a signature that
   does not match the interface method, the struct does not inherit the default method. Instead, the compiler reports that
   the interface method is not implemented.

Calls from within a default method to other methods of the interface are virtual calls. They always end up in the
implementation of the concrete struct, regardless of whether it is the implementation of the struct or another default method.

### Implementation
- **Symbol table builder:** The method definition `IVisitor.visitNode` resolves to the scope of the interface, if there is no
  struct with that name. The body scope of the default method is a child of the interface scope.
- **Type checker (prepare):** The default method gets `this` type `IVisitor`. It is inserted into the interface scope, but is
  marked as default method (`Function.isInterfaceDefaultMethod`), so that function matching ignores it. Calls on the interface
  still resolve to the method signatures.
- **Type checker (check):** The body of the default method is type-checked once against the interface. Additionally, the
  compiler verifies that the default method implements a method signature of the interface.
  When checking that a struct implements all methods of its interfaces, a missing method is looked up among the default
  methods of the respective interface. If found, an inherited method is inserted into the struct scope. It has no body of its
  own, but refers to the default method (`Function.defaultMethod`). This makes direct calls like `counter.visitNode(node)` work
  and gives the inherited method a vtable slot.
- **IR generator:** The default method is emitted once, by the source file that defines the interface. If the interface is
  public, the default method is public as well, because structs in other source files can inherit it. Inherited methods have
  the mangled name of the default method. The vtable slot of an inherited method points to the default method and direct calls
  pass a pointer to the interface part of the struct as `this`.

### Limitations
- Default methods of generic interfaces and generic default methods are not supported yet.
- Virtual calls through an interface, that is not the first interface of a struct, do not work correctly yet. This is
  independent of default methods, but also affects default methods of such interfaces.

### Usage in the compiler
Default methods are implemented in `symbol-table-builder.spice`, `type-checker.spice`, `function-manager.spice` and
`ir-generator.spice`. The compiler sources use them as well: The visitor interface `IAbstractAstVisitor` provides default
visit methods, which visit all children of the node, so that each compiler pass only implements the visit methods it needs.
