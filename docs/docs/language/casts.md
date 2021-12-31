---
title: Type Casts
---

Sometimes it is useful to directly cast [a type](../data-types) to another one. Spice offers type casting for some type combinations. Additionally the casting operator can always be applied when the source type matches the cast destination type.

Casting an int to the short data type:
```spice
short = (short) 12;
```

Example for casting for a function fall:
```spice
f<int> testFunc(char c, long l) {
    printf("Char was: %c, long was: %d", c, l);
}

f<int> main() {
    int i = 1234567;
    short s = 65;
    testFunc((char) s, (long) i);
}
```