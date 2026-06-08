---
title: Type Aliases
---

Spice supports defining type aliases like this:

## Usage

```spice
type I32 alias int;
type I64 alias long;

type T int|long|short;
type TestStruct<T> struct {
    bool f1
    unsigned T f2
}
type TS alias TestStruct<short>;
```

## True aliases vs. type constraints

The `type` keyword serves two different purposes depending on what follows it:

**`type Name alias OtherType`** — creates a true alias. `Name` and `OtherType` are completely interchangeable; the
compiler treats them as the same type:

```spice
type Meters alias double;
type Seconds alias double;

f<Meters> speed(Meters dist, Seconds time) {
    return dist / time;
}
```

**`type Name A|B|C`** — declares a *generic type constraint*, not an alias. It does not create a new type on its own;
it is only meaningful as a template parameter in a generic function, procedure, or struct. It restricts which concrete
types may be substituted for that parameter:

```spice
type NumericType int|long|double;

f<NumericType> sum<NumericType>(NumericType a, NumericType b) {
    return a + b;
}

f<int> main() {
    sum<int>(1, 2);      // ok
    sum<double>(1.0, 2.0); // ok
    // sum<string>("a", "b"); // compiler error — string not in the constraint
}
```

Using `type T dyn` as a constraint means the generic type parameter is unconstrained and may be substituted by any
type.
