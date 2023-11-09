---
title: Attributes
---

Spice offers the option to annotate single function or whole modules via attributes.

## Module attributes

```spice
// Attribute for the whole module
#![core.linker.flags = "-pthread"]

// ...
```

### Available attributes

- `core.linker.flag: string`: Append linker flag
- `core.compiler.alwaysKeepOnNameCollision: bool`: Always keep the symbols of this source files when merging the name registries of multiple source files

## Function attributes

```spice
#[core.compiler.mangleName = false]
f<int> test(long input) {
    return input == 0 ? 123 : 456;
}
```

### Available attributes

- `core.compiler.mangle: bool`: Disable name mangling for the annotated function
- `core.compiler.mangledName: string`: Set the mangled name for the annotated function
- `core.linker.dll: bool`: Enable linkage as dll (only relevant for Windows)
- `test: bool`: Mark the annotated function as test
- `test.name: string`: Set the name of the test (only procedures with the `test` attribute are considered as tests)
- `test.skip: bool`: Skip the annotated function (only procedures with the `test` attribute are considered as tests)

## Struct attributes

```spice
#[core.compiler.alwaysEmitVTable = true]
public type A struct {
    // ...
}
```

### Available attributes

- `core.compiler.alwaysEmitVTable: bool`: Always emit a vtable for the annotated struct
- `core.compiler.packed: bool`: Pack the annotated struct