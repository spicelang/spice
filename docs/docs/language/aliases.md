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