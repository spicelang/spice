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

A local variable or a by-value parameter only lives until the function returns, so a reference to it would dangle in
the caller. Therefore, returning a reference to a local variable, a by-value parameter or a field or array item, that is
stored in one of them, is a compile error. This also applies to a local reference variable, that is bound to one of them:

<!-- doc-test: expect-error Return of reference to local variable -->
```spice
f<int&> getValue() {
    int value = 42;
    return value; // Error: The returned reference would dangle
}

f<int> main() {
    printf("%d", getValue());
}
```

Array parameters are no copies, but refer to the array of the caller (see [arrays](arrays.md#arrays-as-parameters)), so
returning a reference to one of their items is allowed.

## RValue references

Values are either *LValues* or *RValues*. An LValue is a value with a name, that can still be used afterwards, e.g. a
variable, a field or the result of a function, that returns a reference. An RValue is a value, that nothing can refer to
afterwards: a temporary like a literal, a struct instantiation or the result of a function, that returns a value, or a
value, that is moved explicitly with the [`move` builtin](builtins.md#the-move-builtin).

A parameter of the type `T&&` is an *RValue reference*. It only binds RValues, while a non-const reference `T&` only
binds LValues. A const reference `const T&` binds both. If both a `const T&` and a `T&&` overload exist, RValues pick the
`T&&` overload. This is how [move constructors](constructors-destructors.md#move-constructors) and move assign operators
are told apart from copy constructors and copy assign operators:

```spice
type Buffer struct {
    int size
}

p printKind(const Buffer& _) {
    printf("LValue\n");
}

p printKind(Buffer&& _) {
    printf("RValue\n");
}

f<int> main() {
    Buffer buffer;
    printKind(buffer);       // Prints "LValue"
    printKind(Buffer{42});   // Prints "RValue"
    printKind(move(buffer)); // Prints "RValue"
}
```

Within the function, an RValue reference parameter has a name, so it is an LValue there. Passing it on to another
function, that takes a `T&&`, requires another `move()`.

RValue references are only allowed as parameter types. Local variables, fields and return types can not be RValue
references:

<!-- doc-test: expect-error RValue references are only allowed as parameter types -->
```spice
type Buffer struct {
    int size
}

f<int> main() {
    Buffer buffer;
    Buffer&& reference = move(buffer); // Error: Use a const reference or a value instead
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
