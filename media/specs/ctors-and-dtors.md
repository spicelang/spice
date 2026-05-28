# Constructors and destructors design document

## Roadmap

- [x] Produce error when using `heap` qualifier on non-pointer types
- [x] Add mechanism to generate functions and methods, that are not part of the AST
- [x] Generate default dtor
- [x] Generate default ctor
- [x] Generate default copy ctor
- [x] (Generate default move ctor)
- [x] Make the compiler error out when reference fields do not get initialized by all ctors

## Overview

Spice has a feature to automatically generate default constructors and destructors for structs. This feature is inspired by C++.
This document describes the design of this feature.

## Constructors

### Implicit ctor actions

All constructors of a struct must initialize all fields of the struct. For that, the compiler automatically generates code in the
following order:

- Initialize all value-type-fields with their default values (specified default value, if set. Otherwise, default value for type)
- Execute the constructor body

For fields of reference type, the compiler will raise an error, if the field is not initialized in the constructor body.

If some fields are getting assigned multiple times, dead store elimination will remove the unnecessary assignments later on.

### Default ctor

The default constructor is generated for a struct if it has no user-defined constructors. The default constructor is generated
with the following signature:

```spice
p TestStruct.ctor() {
    // Initialize all value-type-fields with their default values
    // Code for ctor body
}
```

## Copy constructors

A constructor is a copy constructor, if it has exactly one parameter of a const reference to the type of the struct.
The default copy constructor is generated for a struct if it has no user-defined copy constructor. The default copy constructor
is generated with the following signature:

### Implicit copy ctor actions

The copy constructor of a struct must initialize all fields of the struct. For that, the compiler automatically generates code in
the following order:

- Copy all fields from the other struct by value
- If a field is of type struct that has a copy constructor, call the copy constructor on the field

```spice
p TestStruct.ctor(const TestStruct& other) {
    // Copy all fields from the other struct by value or call the copy ctor on the field if available
}
```

## Move constructors

A constructor is a move constructor, if it has exactly one parameter that is a non-const reference to the type of the struct.
The default move constructor is generated for a struct if it has no user-defined move constructor and at least one field needs
non-trivial moving (either a heap-owning pointer or a struct field that itself has a move ctor).

### Implicit move ctor actions

The move constructor transfers state from the source instance to the new instance. The compiler-generated body performs the
following steps:

- For each struct field that has a move ctor, call the move ctor on the field (passing the corresponding source field).
- For each struct field that does not have a move ctor but is non-trivially copyable, call the copy ctor on the field.
- For each owning heap pointer field, transfer ownership: copy the pointer to the destination and null out the source pointer
  so the source's dtor does not free the storage.
- For all other fields, perform a shallow copy from the source.

```spice
p TestStruct.ctor(TestStruct& other) {
    // Move-construct field-by-field. For heap-owning fields, ownership is transferred and the source is nulled.
}
```

### Overload resolution

When both a copy ctor (`ctor(const T&)`) and a move ctor (`ctor(T&)`) exist on the same struct, overload resolution picks the
move ctor for non-const lvalue arguments (the candidate that requires no constification of the argument) and picks the copy
ctor for const arguments (binding a const argument to a non-const reference parameter would require const-loss). Temporaries
are not bindable to non-const reference parameters, so a temporary argument always selects the copy ctor.

## Destructors

### Implicit dtor actions

All destructors of a struct must free all fields of the struct, that are marked with the `heap` qualifier and call the destructor
on all fields. <br>
For that, the compiler automatically generates code in the following order:

- Execute the destructor body
- Call the dtor of all fields, that are of type struct and have a dtor
- Free all fields, that are marked with the `heap` qualifier

If a field is getting freed in the dtor body, the compiler will not free it again.

### Default dtor

The default destructor is generated for a struct if it has no user-defined destructor. The default destructor is generated
with the following signature:

```spice
p TestStruct.dtor() {
    // Call the dtor of all fields, that are of type struct and have a dtor
    // Free all fields, that are marked with the `heap` qualifier
}
```