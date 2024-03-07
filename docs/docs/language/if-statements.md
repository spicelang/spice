---
title: If Statements
---

If statements can be used to execute parts of the program only if a condition is `true`. The specialist term for this is "branching". <br>
The condition of `if` statements can consist of more complex expressions, but always have to evaluate to a
[bool data type](../primitive-types.md#the-bool-data-type).

## Usage

Here's a simple example for an `if` statement:
```spice
int counter = 1;
if counter <= 10 {
	counter += 3;
}
```
The block between the curly braces only gets executed when the variable `counter` is less or equal 10, otherwise it is going to be ignored.

!!! note "Optional parentheses"
    As with the for, foreach and while loops, the parentheses around the head of the `if` statement are optional.

Let's take a look at a more complex condition:
```spice
dyn counter = 1;
bool otherCondition = false;
if (otherCondition ? counter : 5) != 1 {
	counter--;
}
```

This piece of code is a bit pointless, but you can see, that you can also have more complex conditions in there.