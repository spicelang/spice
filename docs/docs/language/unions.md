---
title: Unions
---

Spice offers support for tagged (discriminated) unions to let a value be one of several field types while sharing the
same storage.

## Usage

To declare a `union`, list all possible field types with names, just like a [struct](structs.md):

```spice
type Setting union {
	int intValue
	double doubleValue
	bool boolValue
}
```

Unions can only be declared in the global scope, like [structs](structs.md).

Unlike a struct, a union has no instantiation syntax (there is no `Setting{...}`) and cannot have
[methods, constructors or destructors](constructors-destructors.md). You create one by declaring a variable and then
assigning one of its fields; the field you assign becomes the union's active field:

```spice
type Setting union {
	int intValue
	double doubleValue
	bool boolValue
}

f<int> main() {
	Setting s;
	s.intValue = 42;
	printf("value: %d\n", s.intValue);

	s.doubleValue = 3.5; // switches the active field to doubleValue
	printf("value: %f\n", s.doubleValue);
}
```

Every read of a union field checks, at runtime, that the field you're reading is the one that was last written.
See [Runtime safety](#runtime-safety) below for what happens otherwise.

Because a union only ever stores one field's value at a time, only trivially constructible, copyable and destructible
field types are allowed — plain value types such as numbers, `bool`, and structs made only of such types. Reference
fields and fields of non-trivial types (types with a user-defined constructor, destructor, or copy behavior) are
rejected at compile time.

## Default field values

At most one field may be given a default value. If present, that field is the union's active field immediately after
declaration, with no assignment required:

```spice
type Setting union {
	int intValue = 0
	double doubleValue
}

f<int> main() {
	Setting s;
	printf("value: %d\n", s.intValue); // 0, from the default
}
```

Giving a second field a default value is a compile-time error.

## Runtime safety

A union has no field of its own that says which field is currently active — that information is tracked internally by
a hidden tag. Every field access (read, compound assignment, or taking a reference) checks this tag against the field
you're accessing, in every build mode, including release builds. If the check fails, the program panics immediately
with a message identifying the mismatched field, the same way the [`panic()`](builtins.md#the-panic-builtin) builtin
does.

If a union has no field with a default value, it starts out in an **unset** state: no field is considered active until
the first assignment. Reading any field of such a union before assigning one first also triggers the panic:

```spice
type Setting union {
	int intValue
	double doubleValue
}

f<int> main() {
	Setting s;
	printf("value: %d\n", s.intValue); // panics: no field has been assigned yet
}
```

This makes it impossible to silently read stale or wrongly-typed data out of a union — a mismatch always surfaces
immediately, instead of being deferred to undefined behavior.

## Public and private fields

Union fields follow the same visibility rules as [struct fields](structs.md#public-and-private-fields): private by
default, and accessible from importing files only when marked `public`.

## What unions don't support

To keep the safety guarantees above simple and unconditional, unions deliberately leave out a few things that structs
support:

- No instantiation syntax — build one by declaring it and assigning a field.
- No methods, constructors, or destructors.
- No implementing [interfaces](interfaces.md).
