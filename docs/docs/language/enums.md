---
title: Enumerations
---

Spice supports simple enumerations. These constructs are syntactic sugar over `int` constants.

## Usage

Enumerations in Spice work like this:

```spice
type Fruit enum {
	APPLE,
	BANANA,
	MANGO,
	ORANGE
}

f<int> main() {
    printf("Test: %d", Fruit::MANGO);
}
```

They can be defined only in the global scope. Spice will assign the unsigned integer values
0 to 3 to the items automatically.

Enum items can have values assigned to them. Here is an example:

```spice
type Vegetable enum {
	CUCUMBER = 5,
	TOMATO,
	CARROT = 2,
	POTATO
}
```

For items without an explicit value, Spice assigns the lowest non-negative integer not already taken by any other
item in the enum. Here `CUCUMBER = 5` and `CARROT = 2` are already claimed, so the compiler assigns `TOMATO = 0`
and `POTATO = 1`.