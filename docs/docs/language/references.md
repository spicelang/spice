---
title: References
---

In addition to [pointers](../pointers), Spice supports references. A reference, in Spice, creates an alias to a variable. Under
the hood, this works with non-null pointers.

## Usage

References can only be declared with an immediate value on the right side:

```spice
int variable = 123;
int& variableReference = variable;

variableReference++;

printf("%d", variable); // Prints 124
```

Whenever an assign operator is applied to the reference, not the reference is mapped to a new variable. Instead, the referenced
value gets assigned. Therefore, the reference itself can only be assigned once, namely at declaration.