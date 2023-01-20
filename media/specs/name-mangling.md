# Technical Specification for Name Mangling in Spice

With Spice having support for function overloading, default parameter values, generics, etc., it is required to identify
functions, structs and types uniquely. For this purpose, Spice offers a flexible name mangling scheme for names on ABI level.

## Function mangling

### Components

- Function name
- Function type (function: `f`, procedure `p`, method function `mf` or method procedure `mp`)
- This type (struct type for methods, default `void`)
- Return type (`void` for procedures)
- Template types (separated by `_`, default: empty)
- Param types (separated by `_`, default: empty)

### Mangled name

Here is the scheme, how Spice mangles function/procedure/method names:

`_<function-type>__<this-type>__<return-type>[__<template-types>]__<function-name>[__<param-types>]`

**Example:**

Mangled name: `_mp__Vector__void__int__pushBack__int`

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
[Struct::gerMangledName](../../src/model/Struct.cpp#:~:text=Struct::getMangledName)