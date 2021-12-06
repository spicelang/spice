---
title: Foreach Loops
---

Foreach loops can be used to iterate over an [array](../arrays) of arbitrary elements.

You can write `foreach` loops in two different ways:

### Normal
If you only need the array item to work with, the recommended way is to use the normal `foreach` loop:

```spice
int[7] myIntArray = { 1, 5, 4, 0, 12, 12345, 9 };
foreach (const int item : myIntArray) {
    printf("Item: %d", item);
}
```

!!! note "Optional parentheses"
    As with the if statement and while loop, the parantheses around the head of the `foreach` loop are optional.

### Indexed
As soon as you need to access the index of the array item as well, you can use the indexed 

```spice
string[5] welcomeMessage = { "Hello", "Spice", "programmers!" };
foreach int i, int word : welcomeMessage {
    printf("Word no. %d: %s", i, word);
}
```

??? tip "Skipping array items"
    Indexed `foreach` loops allow you to set the start index and to modify the index within the loop. Here's an example:
	```spice
	int[10] intArray = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	foreach int i = 3, int item : intArray {
	    printf("Array item no. %d: %d, ", i, item);
		i += 2;
	}
	```
	The code snippet above will print `Array item no. 3: 4, Array item no. 6: 7, Array item no. 9: 10,`. In the loop head, we set the start index to `3` and skip 3 items (2+1) each round.


!!! tip "Usage of loop alternatives"
    Foreach loops should only be used when you have an array data structure and want to access its items. If this is not the case, we recommend using the [for loop](../for-loops) or [while loop](../while-loops) instead.