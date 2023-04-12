---
title: Foreach Loops
---

Foreach loops can be used to iterate over containers, that offer iterator implementations like `Vector`, `ArrayList`,
etc. as well as [arrays](../arrays) of arbitrary elements and number ranges.

## Usage

You can write `foreach` loops in two different ways:

### Ordinary
If you only need the item to work with, the recommended way is to use the ordinary `foreach` loop:

```spice
int[7] myIntArray = { 1, 5, 4, 0, 12, 12345, 9 };
foreach (const int item : myIntArray) {
    printf("Item: %d", item);
}
```

!!! note "Optional parentheses"
    As with the if statement, for loop and while loop, the parentheses around the head of the `foreach` loop are optional.

### Indexed
As soon as you need to access the index of the current item as well, you can use the indexed `foreach` loop:

```spice
string[3] welcomeMessage = { "Hello", "Spice", "programmers!" };
foreach int i, string word : welcomeMessage {
    printf("Word no. %d: %s\n", i, word);
}
```

!!! tip "Usage of loop alternatives"
    Foreach loops should only be used when you have a container data structure and want to iterate over its items.
    If this is not the case, we recommend using the [for loop](../for-loops), [while loop](../while-loops) or
	[do-while loop](../do-while-loops) instead.