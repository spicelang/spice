---
title: Type Casts
---

Sometimes it is useful to directly cast [a type](primitive-types.md) to another one. Spice offers type casting for some
type combinations. Additionally, the casting operator can always be applied when the source type matches the cast
destination type.

## Usage

Casting an int to the short data type:
```spice
int intVar = 12;
short shortVar = cast<short>(intVar);
```

Example for casting for a function call:
```spice
p testProc(char c, long l) {
    printf("Char was: %c, long was: %d", c, l);
}

f<int> main() {
    int i = 1234567;
    short s = 65s;
    testProc(cast<char>(s), cast<long>(i));
}
```