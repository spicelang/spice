# Technical Specification for Name Mangling in Spice

With Spice having support for function overloading, default parameter values, generics, etc., it is required to identify
functions, structs and types uniquely. For this purpose, Spice offers a flexible name mangling scheme for names on ABI level.
This mangling scheme is oriented on the Itanium C++ ABI mangling scheme, but is not compatible with it completely.

## Function mangling
### Mangled name
Here is the scheme, how Spice mangles functions/procedures:

`_Z[N<this-type-length><this-type>][I<template-type-list>]<function-name-length><function-name>[]`

**Example:**

Mangled name: `_Z10isRawEqualPcPc`

This is the mangled name for the following function signature: `f<bool> isRawEqual(string lhs, string rhs)`

### Exception: main function
As the `main` function is exposed to the operating system, it is the only Spice function, where the mangling rules do not apply.
It's name stays the same on the ABI level.

### Implementation
To see the implementation for function name mangling, have a look here:
[Function::getMangledName](../../src/irgenerator/NameMangling.cpp#:~:text=NameMangling::mangleFunction)


## Struct mangling
### Mangled name
Here is the scheme, how Spice mangles structs:

`struct.<struct-name>`

**Example:**

`struct.Pair`

### Implementation
To see the implementation for struct name mangling, have a look here:
[Struct::getMangledName](../../src/irgenerator/NameMangling.cpp#:~:text=NameMangling::mangleStruct)

## Type mangling
### Mangled name
Here is the scheme, how Spice mangles types:

- Pointer (`*`) / Array with unknown size (`[]`): `P`
- Reference (`&`): `R`
- Array (`[5]`): `A<array-length>`
- Double: `d`
- Int: `i`
- Unsigned Int: `j`
- Short: `s`
- Unsigned Short: `t`
- Long: `l`
- Unsigned Long: `m`
- Byte: `a`
- Unsigned Byte / unsigned char: `h`
- Char: `c`
- String / char*: `Pc`
- Bool: `b`
- Struct / Interface / Enum: `<struct-name-length><struct-name>`
- Function: `PF[<this-type>|v][<param-types>]E`