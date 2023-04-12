---
title: While Loops
---

While loops are designed to do something until a certain condition evaluates to `false`.
This means, that a while loop could also run infinitely when the condition never gets fulfilled. <br>
Like the [if statement](../if-statements), the condition of the `while` loop must evaluate to a
[boolean value](../primitive-types#the-bool-data-type), but can hold complex, nested expressions.

## Usage

Here is an example of a simple `while` loop:
```spice
int counter = 0;
while counter < 20 {
	printf("Loop run %d", counter);
	counter++;
}
```

!!! note "Optional parentheses"
    As with the if statement, for loop and foreach loop, the parentheses around the head of the `while` loop are optional.

If you wanted to intentionally achieve an endless loop, you would write something like this:
```spice
while true {
	printf("This is gonna run forever ...");
}
```

!!! tip "Usage of loop alternatives"
    If you somehow know how many times you want to execute something, we recommend you to use a [for loop](../for-loops)
    instead. If you want to iterate over a container of items, consider using the [foreach loop](../foreach-loops).
    