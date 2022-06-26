---
title: Generics
---

Spice offers basic support for generics. [Structs](./structs) as well as [functions](./functions) and [procedures](./procedures)
can be marked as generic by attaching a template list with one or more generic types to it.

Spice resolves the substantiations of generic types at compile time. This helps to keep the runtime performance up.
Generic function substantiations that are unused, are removed by the compiler automatically.

## Generic functions
Here is an example function:

```spice
type T dyn;
type U int|double;

f<double> genericFunction<T, U>(T arg1, U arg2, int arg3 = 10) {
    return arg1 + arg2 + arg3;
}
```

As you can see, the types `T` and `U` have to be declared before you can use them. In the above example `T` can substantiate to an
arbitrary type, whilst `U` can only become `int` or `double`.

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