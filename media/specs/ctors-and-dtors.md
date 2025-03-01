# Constructors and destructors design document

## Roadmap

- [x] Produce error when using `heap` qualifier on non-pointer types
- [x] Add mechanism to generate functions and methods, that are not part of the AST
- [x] Generate default dtor
- [x] Generate default ctor
- [x] Generate default copy ctor
- [ ] (Generate default move ctor)
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

ToDo

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