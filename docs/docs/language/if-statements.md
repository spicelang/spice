---
title: If Statements
---

If statements can be used to execute parts of the program only if a condition is `true`. The specialist term for this is "branching". <br>
The condition of `if` statements can consist of complex expressions, but always have to evaluate to a [bool data type](../data-types#the-bool-data-type).

Here's a simple example for an `if` statement:
```spice
int counter = 1;
if counter <= 10 {
	counter += 3;
}
```
The block between the curly braces only gets executed when the variable `counter` is less or equal 10, otherwise it is going to be ignored.

Let's take a loop at a more complex condition:
```spice
dyn counter = 1;
bool otherCondition = false;
if (otherCondition ? counter : 5) != 1 {
	counter--;
}
```
This peace of code is a bit pointless, but you can see, that you can also have more complex conditions in there.