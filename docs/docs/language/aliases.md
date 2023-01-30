---
title: Type Aliases
---

Spice supports defining type aliases like this:

## Usage

```spice
type i32 alias int;
type i64 alias long;

type T int|long|short;
type TestStruct<T> struct {
    bool f1
    unsigned T f2
}
type TS alias TestStruct<short>;
```