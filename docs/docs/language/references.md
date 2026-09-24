---
title: References
---
In addition to [pointers](pointers.md), Spice supports references. Under the hood, this works with non-nullable pointers.

## Usage - local variables

References can not be declared without initializing. Only declaring with immediate initialization is valid:

```spice
int variable = 123;
int& variableReference = variable;
variableReference++;
printf("%d", variable); // Prints 124
```

Whenever an assign operator is applied to the reference, not the reference is mapped to a new variable. Instead, the
referenced value gets assigned. Therefore, the reference itself can only be assigned once, namely at declaration.

## Usage - function parameters / return types

References can be used as function parameters and return types. This is useful for passing large objects without
copying them:

```spice
type LargeStruct struct { /* many large fields */ }

f<LargeStruct&> returnSelf(LargeStruct& s) {
    return s;
}

f<int> main() {
    LargeStruct s;
    LargeStruct& s2 = returnSelf(s);    
}
```

## Usage - struct fields

Struct fields can also be of type reference:

```spice
type T dyn;
type A<T> struct {
    T& data
}
f<int> main() {
    double d = 23.456;
    dyn a = A<double>{d};
}
```

## Usage - ternary expressions

If both operands of a ternary expression are lvalues (variables, fields, ...), the result is a reference to the selected
operand. It can be bound to a non-const reference and modified through it, without copying anything:

```spice
int a = 1;
int b = 2;
bool condition = false;
int& selected = condition ? a : b;
selected = 42;
printf("%d %d", a, b); // Prints 1 42
```

If at least one operand is a temporary value (e.g. a literal or the result of a function call), the ternary expression
yields a copy of the selected operand instead. Such a value can only be bound to a const reference. If only one of two
lvalues is const, the result is a const reference, so it can not be bound to a non-const reference either.
