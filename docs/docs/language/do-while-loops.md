---
title: Do-While Loops
---

Do-While loops are designed to do something until a certain condition evaluates to `false`, but at least execute the action once.
This means, that a do-while loop could also run infinitely when the condition never gets fulfilled. <br>
Like the [if statement](if-statements.md), the condition of the `do-while` loop must evaluate to a
[boolean value](primitive-types.md#the-bool-data-type), but can hold complex, nested expressions.

## Usage

Here is an example of a simple `do-while` loop:
```spice
int counter = 0;
do {
	printf("Loop run %d", counter);
	counter++;
} while counter < 20;
```

!!! note "Optional parentheses"
    As with the if statement, for loop and foreach loop, the parentheses around the head of the `do-while` loop are optional.

If you wanted to intentionally achieve an endless loop, you would write something like this:
```spice
do {
	printf("This is gonna run forever ...");
} while true;
```

!!! tip "Usage of loop alternatives"
    If you somehow know how many times you want to execute something, we recommend you to use a [for loop](for-loops.md)
    instead. If you want to iterate over a container of items, consider using the [foreach loop](foreach-loops.md).