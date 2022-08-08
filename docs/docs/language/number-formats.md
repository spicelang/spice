---
title: Number formats
---

Spice supports four different number formats to cover as many use-cases as possible.

## Decimal format (base 10)

Decimal numbers can be written in two ways in Spice. The implicit way:

```spice
int i32 = 123;
short i16 = 123s;
long i64 = 123l;
```

or the explicit way:

```spice
int i32 = 0d123;
short i16 = 0d123s;
long i64 = 0d123l;
```

## Binary format (base 2)

```spice
int i32 = 0b1010;
short i16 = 0b1010s;
long i64 = 0b1010l;
```

## Hexadecimal format (base 16)

```spice
int i32 = 0xA76E;
short i16 = 0xA76Es;
long i64 = 0xA76El;
```

## Octal format (base 8)

```spice
int i32 = 0o1274;
short i16 = 0o1274s;
long i64 = 0o1274l;
```