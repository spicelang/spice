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

## Function attributes

```spice
#[core.compiler.mangleName = false]
f<int> test(long input) {
    return input == 0 ? 123 : 456;
}
```

### Available attributes

- `core.compiler.mangleName: bool`: Disable name mangling for the annotated function
- `core.linker.dll: bool`: Enable linkage as dll (only relevant for Windows)