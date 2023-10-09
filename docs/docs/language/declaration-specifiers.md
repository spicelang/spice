---
title: Declaration specifiers
---

## The `public` specifier

All structures in Spice, that are accessible from outside the module (like functions, procedures, structs, etc.), have private
visibility per default. That means that they can't be accessed from outside the module from a source file, that imported the file,
where the structures live. If you need to access e.g. a struct from outside the module, you need to attach the specifier `public`
to it.

### Applicable for

- Functions
- Procedures
- Global variables
- Structs
- Struct fields
- Enums
- Interfaces
- Interface signatures

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

By having private visibility per default, Spice encourages the programmer to adhere to the information hiding principle and expose
only the things, that need to be exposed to other modules.

## The `inline` specifier

The `inline` specifier can be used for functions and procedures to mark them as inlineable for the compiler explicitly. By marking
a function or procedure as inlineable, the compiler is forced to always inline it.

!!! note
    Marking private functions/procedures with the `inline` keyword is not necessary in most of the cases, because the compiler
    inlines smaller, private scoped functions automatically when compiling with an optimization level >= O1.

### Applicable for

- Functions
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

## The `const` specifier

This specifier can be used to make variables immutable after the first assignment.

### Applicable for

- Local variables
- Global variables
- Functions
- Procedures

### Example

```spice
const int THIS_IS_A_CONSTANT = 101;

f<int> main() {
    THIS_IS_A_CONSTANT++; // Will give a compiler error
}
```

## The `signed` specifier

Marks a numeric variable to use signed numbers explicitly.

### Applicable for

- Local variables
- Global variables
- Struct fields

### Example

```spice
f<int> main() {
    signed int signedInt = -0x567;
}
```

## The `unsigned` specifier

Marks a numeric variable to use unsigned numbers explicitly.

### Applicable for

- Local variables
- Global variables
- Struct fields

### Example

```spice
f<int> main() {
    unsigned int unsignedInt = -0x567; // Will wrap around
}
```

## The `heap` specifier

Marks a variable to be allocated on the heap. The compiler will generate code to free the memory of the variable, when it goes out
of scope. For structs, the compiler will generate code to free all fields, that are marked with the `heap` keyword, in the dtor.

*Note: The `heap` specifier only works for pointer types. This is enforced by the compiler.*

### Applicable for

- Local variables
- Struct fields

### Example

```spice
type TestStruct struct {
    heap int* ptr
}
```

## The `compose` specifier

Includes a struct info the current strut. This is useful for composing structs from other structs.

*Note: The `compose` specifiers only works for by-value struct types. This is enforced by the compiler.*

### Applicable for

- Struct fields

### Example

```spice
type A struct {
    int f1
}

type B struct {
    compose A a
    int f2
}
```
