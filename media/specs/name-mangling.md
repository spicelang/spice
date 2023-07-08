# Technical Specification for Name Mangling in Spice

With Spice having support for function overloading, default parameter values, generics, etc., it is required to identify
functions, structs and types uniquely. For this purpose, Spice offers a flexible name mangling scheme for names on ABI level.

## Function mangling

### Components

- Function type (function: `f`, procedure `p`, method function `mf` or method procedure `mp`)
- Function name length (number of characters in function name)
- Function name (will be encoded as is)
- This type (default: empty)
- Param types (default: empty)
- Primitive types (double: `d`, int: `i`, short: `s`, long: `l`, byte: `y`, char: `c`, string: `s`, bool: `b`)
- Struct types (struct name, prefixed with `struct_`)

### Mangled name

Here is the scheme, how Spice mangles function/procedure/method names:

`<function-type>_<function-name-length><function-name>[_<this-type>][_<param-types>]`

**Example:**

Mangled name: `mp_pushBack_i`

### Exception: main function

As the `main` function is exposed to the operating system, it is the only Spice function, where the mangling rules do not apply.
It's name stays the same on the ABI level.

### Implementation

To see the implementation for function name mangling, have a look here:
[Function::getMangledName](../../src/model/Function.cpp#:~:text=Function::getMangledName)


## Struct mangling

### Components

- Struct name
- Field types (separated by `_`, default: empty)
- Template types (separated by `_`, default: empty)

### Mangled name

Here is the scheme, how Spice mangles struct names:

`_s[__<template-types>]__<struct-name>[__field-types]`

**Example:**

`_s__int_string__Pair__int_string`

### Implementation

To see the implementation for struct name mangling, have a look here:
[Struct::getMangledName](../../src/model/Struct.cpp#:~:text=Struct::getMangledName)