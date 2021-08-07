---
title: For Loops
---

Spice distinguishes strictly between functions and procedures. Procedures are concise and enclosed blocks of functionality which a programmer can define within his souce code. Other than functions, procedures do not have a return value. The paradigma here is, that a function calculates and returns something whereas a procedure executes some action.

Procedures in Spice can be defined like this:
```spice
p procedureName() {
	... // Do something
}
```

Spice also allows you to pass parameters to a procedure. This parameters may also have default values, which can then be omitted when calling the method. 

!!! note
    Parameters with default value / optional parameters have to go after those without default value in the procedure declaration.

Here, the second parameter is one with a default value attached.
```spice
p demoProcedure(int parameter1, bool parameter2 = false) {
	... // Do something with the parameters
}
```

Theis procedure could get called like so:
```spice
demoProcedure(10);
demoProcedure(7, true);
```

!!! tip
    If you want to calculate something and need to return a value to the caller, please consider to use [functions](../functions) instead of procedures.