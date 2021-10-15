---
title: Primitive data types
---

Spice supports four different primitive data types out of the box: `double`, `int`, `string` and `bool`. In addition, there is a builtin type-inferred type, called `dyn`.
Let us take a look at each one individually!

## The `double` data type
Doubles are signed, double-precision 64-bit IEEE 754 floating point numbers.

In Spice, variables of type `double` can be defined like this:
```spice
double variable1 = 56.9042;

double variable2;
variable2 = -0.2349;
```

## The `int` data type
Integers are signed, whole numbers of 32-bit, which have a range from a min of -2,147,483,648 to a max of 2,147,483,647.

In Spice, variables of type `int` can be defined like this:
```spice
int variable1 = 903;

int variable2;
variable2 = -2;
```

## The `string` data type
Strings are arrays of 8-bit integers (chars) and contain text-like information. The length of strings is unlimited.

In Spice, variables of type `string` can be defined like this:
```spice
string variable1 = "test string";

string variable2;
variable2 = "Hello World!";
```

## The `bool` data type
Booleans are 1-bit integers and can be assigned with the exactly two values: `true` or `false`.

In Spice, variables of type `bool` can be defined like this:
```spice
bool variable1 = true;

bool variable2;
variable2 = false;
```

!!! info "Additional information"
    Many language components like [if statements](../if-statements), [for loops](../for-loops), [while loops](../while-loops), etc. use the `bool` data type as evaluation unit for conditions.
	You can find more information about that in the respective sections.

## The `dyn` data type
The `dyn` data type is a more unconventional data type. Dyn stands for dynamic and means that the `dyn` data type can hold any value of one of the four types `double`, `int`, `string` or `bool`. The concrete type of a `dyn` variable gets inferred at compile time so that the language stays type-safe. This also means, that as soon as you assign a value to a `dyn` variable, the type gets set fixed and is not mutable anymore.

Dyn variables can defined like this:

```spice
dyn variable1 = 1.24;
dyn variable2 = -67;
dyn variable3;
dyn variable4 = false;

variable3 = "demo string";
```

!!! note "Usage of the dyn data type"
    The dyn data type can not be used everywhere. Function arguments can only be declared as `dyn`, when they have a default value attached to them. For more information about [functions](../functions), visit the respective documentation section.