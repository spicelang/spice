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

Structs can only be declared in the global scope, like [functions](functions.md) and [procedures](procedures.md).

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

## Default field values

Fields can be given default values. A field with a default value does not need to be supplied when instantiating the
struct without a constructor:

```spice
type Config struct {
    int retries = 3
    bool verbose = false
    string host = "localhost"
}

f<int> main() {
    Config cfg;              // all fields take their defaults
    cfg.retries = 5;
    printf("retries: %d\n", cfg.retries);
}
```

## Public and private fields

All fields are private by default — they are only accessible within the same source file. Mark individual fields
`public` to expose them to code that imports the struct:

```spice
public type Socket struct {
    public int sock
    short _errorCode       // private — not accessible from importers
    public string host
}
```

This follows the same [visibility rules](declaration-qualifiers.md) as functions and structs themselves.

## Struct composition

The [`compose` qualifier](declaration-qualifiers.md#the-compose-qualifier) lets you embed one struct inside another
and access its fields directly on the outer struct, without going through an intermediate field name:

```spice
type Point struct {
    int x
    int y
}

type ColoredPoint struct {
    compose Point base
    int color
}

f<int> main() {
    ColoredPoint cp;
    cp.x = 10;      // accessed directly, no cp.base.x needed
    cp.y = 20;
    cp.color = 0xFF0000;
}
```

Multiple structs can be composed; if two composed structs expose the same field name, the compiler will require an
explicit qualifier to disambiguate.

## Adding behaviour

Structs can be extended with [methods](methods.md), [constructors and destructors](constructors-destructors.md), and
can implement [interfaces](interfaces.md).
