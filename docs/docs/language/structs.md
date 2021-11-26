---
title: Structs
---

Spice offers support for C-like structs to group variables.

To declare a `struct`, list all field names with types like this:

```spice
type Person {
	string firstName
	string lastName
	int age
}
```

this structs can only be declared in the global scope, in parallel to [functions](../functions) and [procedures](../procedures).

For creating an instance of the declared struct, you can use the new operator and pass values for all fields in curly braces. To access fields of the instance, you can address the fields by their names:

```spice
type Person {
	string firstName
	string lastName
	int age
}

f<int> main() {
	Person john = new Person { "John", "Doe", 46 };
	john.age = 47;
	printf("John's age: %d", john.age);
}
```