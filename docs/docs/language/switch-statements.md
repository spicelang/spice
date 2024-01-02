---
title: Switch Statements
---

Switch statements in Spice can be used to execute different code paths based on the value of a variable. This is similar to
`switch` statements in other languages, but with some differences. The expression in the `switch` statement can be of type `int`,
`long`, `short`, `byte`, `char` or `bool` and the individual match expressions need to be literals of the respective type.

## Usage

Here's a simple example for an `switch` statement:
```spice
switch number {
    case 0: { printf("Zero"); }
    case 1: { printf("One"); }
    case 2: { printf("Two"); }
    case 3: { printf("Three"); }
    case 4: { printf("Four"); }
    case 5: { printf("Five"); }
    case 6: { printf("Six"); }
    case 7: { printf("Seven"); }
    case 8: { printf("Eight"); }
    case 9: { printf("Nine"); }
    default: { printf("NaN"); }
}
```
Each `case` branch gets executed, if the value of the `switch` statement matches the value of the case branch. If no case branch
matches, the optional `default` branch gets executed. The default branch must be the last branch in the `switch` statement, the
order of case branches does not matter.

!!! note "Optional parentheses"
As with the for, foreach and while loops, the parentheses around the head of the `switch` statement are optional.

Let's take a look at a switch statement:
```spice
switch (input) {
    case 0, 2, 4, 6, 8: { printf("Even"); }
    case 1, 3, 5, 7, 9: { printf("Odd"); }
}
```
This piece of code is a bit pointless, but you can see, that you can also assign two constant values to result in the same branch
being executed.

## Fallthrough

In Spice, the `switch` statement does not fall through by default. This means, that if you don't have to use the `break` keyword
to prevent the next case from being executed, like in other languages. In Spice, you have to use the `fallthrough` keyword to
fall through to the next case. This originated from the assumption, that you more often have the situation, that you only want to
execute one branch, instead of having fallthrough behaviour.

The fallthrough keyword can be used like this:
```spice
int i = 1;
switch i {
    case 1, 2: {
        printf("1 or 2\n");
        fallthrough;
    }
    case 6, 9: {
        printf("6 or 9\n");
    }
    default: {
        printf("Default\n");
    }
}
```
The `fallthrough` keyword ensures that in the above example, the `case 6, 9` branch gets executed, even though the value of `i`
is `1` or `2` and the respective branch was executed beforehand.
