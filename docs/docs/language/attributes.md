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

- `core.linker.flag: string`

## Function attributes

```spice
#[core.compiler.mangleName = false]
f<int> test(long input) {
    return input == 0 ? 123 : 456;
}
```

### Available attributes

- `core.compiler.mangleName: bool`
- `core.linker.dll: bool`