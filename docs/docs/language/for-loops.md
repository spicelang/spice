---
title: For Loops
---

For loops are designed to execute a block of code n times. That means that you can specify a variable, set an initial value to it and mutate this variable after each loop run, until a certain condition is met. For loops can not only work with integer counters.

Here is an example of a simple `for` loop:
```spice
for int i = 1; i <= 10; i++ {
	printf("Round %d\n", i);
}
```

!!! tip "Usage of loop alternatives"
    For loops should only be used when it is foreseeable how often a block of code will run. If this is not the case, we recommend using the [while loop](../while-loops) instead.