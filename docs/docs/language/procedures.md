---
title: Procedures
---

Spice distinguishes strictly between functions and procedures. Procedures are concise and enclosed blocks of code, which a
programmer can define within his source code. Each procedure holds a set of instructions to execute in the stated order.
Other than functions, procedures do not have a return value. The paradigm here is, that a function calculates and returns
something whereas a procedure executes some action.

## Usage

Procedures in Spice can be defined like this:
```spice
p procedureName() {
	... // Do something
}
```

Spice also allows you to pass arguments to a procedure. This arguments may also have default values, which can then be omitted
when calling the procedure. 

!!! note
    Optional arguments have to go after mandatory arguments in the procedure head.

## Example with default parameter value

Here, the second argument is one with a default value attached.
```spice
p demoProcedure(int arg1, bool arg2 = false) {
	... // Do something with the arguments
}
```

This procedure could get called like so:
```spice
demoProcedure(10);
demoProcedure(7, true);
```

!!! tip
    If you want to calculate something and need to return a value to the caller, please consider to use [functions](../functions)
    instead of procedures.