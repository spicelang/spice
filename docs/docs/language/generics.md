---
title: Generics
---

Spice offers basic support for generics. [Structs](./structs) as well as [functions](./functions) and [procedures](./procedures)
can be marked as generic by attaching a template list with one or more generic types to it.

Spice resolves the substantiations of generic types at compile time. This helps to keep the runtime performance up.
Generic function substantiations that are unused, are removed by the compiler automatically.

## Generic types
Before using generic types in function or struct templates, you have to declare the types first. This can be done like this:

```spice
type T dyn;
type U int|double|long;
```

In the above example `T` can substantiate to an arbitrary type, whilst `U` can only become an `int`, a `double` or a `long`.

## Generic functions or procedures
Here is an example function:

```spice
type T dyn;
type U int|double|long;

f<double> genericFunction<T, U>(T arg1, U arg2, int arg3 = 10) {
    return arg1 + arg2 + arg3;
}
```

To be able to use generic types in the argument list, the return type or in the function/procedure body, they have to be included
in the template list, that is attached at the function (in the above example `<T, U>`). If this is not the case, the compiler will
ask you to do so.

As mentioned above, the substantiations of generic types are collected at compile time. That means, that the compiler is able to
type-check to ensure that e.g. operators are compatible with the concrete types.

## Generic structs
Here is an example struct:

```spice
type T bool|short|long;

type Vector<T> struct {
    T* contents
    int cap
}

p Vector.print() {
    printf("Ptr: %p", this.contents);
}
```

The above example shows, that you can use the generic type as part of struct field types. Like normal structs, generic structs can
also have [methods](./methods). The concrete types for the generic types, used for the parent struct, are inherited by the method,
so you do not have to define the method above with following signature: `Vector<T>.print()`.