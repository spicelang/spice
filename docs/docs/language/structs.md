---
title: Structs
---

Spice offers support for C-like structs to aggregate variables in groups.

## Usage

To declare a `struct`, list all field types with names like this:

```spice
type Person struct {
	string firstName
	string lastName
	int age
}
```

Structs can only be declared in the global scope, like [functions](../functions.md) and [procedures](../procedures.md).

For creating an instance of the declared struct, you can pass values for either all or none of the fields in curly braces.
To access a field of the instance, you can address the field by its name:

```spice
type Person struct {
	string firstName
	string lastName
	int age
}

f<int> main() {
	Person john = Person { "John", "Doe", 46 };
	john.age = 47;
	printf("John's age: %d", john.age);
}
```