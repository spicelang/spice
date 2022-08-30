---
title: Enumerations
---

Enumerations in spice work like this:

```spice
type Fruit enum {
	APPLE,
	BANANA,
	MANGO,
	ORANGE
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

Spice will then assign 0 for `TOMATO` and 1 for `POTATO`.