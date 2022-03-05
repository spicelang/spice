---
title: Declaration modifiers
---

## The `public` modifier

All structures in Spice, that are accessible from outside the module (like functions, procedures, structs, etc.), have private visibility per default. That means that they can't be accessed from outside the module from a source file, that imported the file, where the structures live. If you need to access e.g. a struct from outside the module, you need to attach the modifier `public` to it.

### Applicable for

- Functions
- Procedures
- Global variables
- Structs
- Struct fields

### Example

Source file a (`a.spice`):

```spice
public p visibleProcedure() {
    printf("Hello World!");
}
```

Source file b (`b.spice`):

```spice
import "a" as a;

f<int> main() {
    a.visibleProcedure(); // This only works if the procedure 'visibleProcedure' is public
}
```

By having private visibility per default, Spice encourages the programmer to adhere to the information hiding principle and expose only the things, that need to be exposed to other modules.

## The `inline` modifier

The `inline` modifier can be used for functions and procedures to mark them as inlinable for the compiler explicitly. By marking a function or procedure as inlinable, the compiler is forced to always inline it.

!!! note
    Marking private functions/procedures with the `inline` keyword is not necessary in most of the cases, because the compiler inlines smaller, private scoped functions automatically when compiling with an optimization level >= O1.

### Applicable for

- Function
- Procedures

### Example

```spice
inline p printHelloWorld() {
    printf("Hello World!");
}

f<int> main() {
    printHelloWorld();
}
```

corresponds to:

```spice
f<int> main() {
    printf("Hello World!");
}
```

## The `const` modifier

This modifier can be used to make variables immutable after the first assignment.

### Applicable for

- Local variables
- Global variables

### Example

```spice
const int THIS_IS_A_CONSTANT = 101;

f<int> main() {
    THIS_IS_A_CONSTANT++; // Will give a compiler error
}
```