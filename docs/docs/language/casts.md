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
## Pointer casts

Casting between two pointer types, and between a pointer and an integer, is only allowed inside an `unsafe`
block, because the compiler cannot check that the result is still a valid pointer.

On the integer side, only `long` (and `unsigned long`) is accepted. It is the only Spice integer type that is
guaranteed to be 64 bit wide on every target, so it is the only one that cannot silently truncate a pointer:

```spice
f<int> main() {
    int i = 42;
    int* iPtr = &i;

    unsafe {
        long address = cast<long>(iPtr);   // pointer -> integer
        int* backPtr = cast<int*>(address); // integer -> pointer
        assert backPtr == iPtr;
    }
}
```

Casting to a narrower integer type such as `int` or `short` is rejected:

```spice
unsafe {
    int truncated = cast<int>(iPtr); // error: cannot apply '(cast)' operator on types int and int*
}
```

Since Spice has no pointer-difference operator, going through `long` is also how you compute the numeric
distance between two pointers, in bytes:

```spice
f<int> main() {
    int[3] a = [1, 2, 3];
    int* first = a;
    int* third = a;
    unsafe {
        third += 2;
        long byteDistance = cast<long>(third) - cast<long>(first);
        assert byteDistance == 8l; // two int elements
    }
}
```
