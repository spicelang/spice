---
title: Functions
---

Spice distinguishes strictly between functions and procedures. Functions are used to calculate something from a given input. Other than procedures, functions must have a return value. The paradigma here is, that a function calculates and returns something whereas a procedure executes some action.

Functions in Spice can be defined like this:
```spice
f<double> functionName() {
	... // Do something
	return 5.6; // Return some value anywhere in the function body
}
```

Spice also allows you to pass parameters to a function. This parameters may also have default values, which can then be omitted when calling the function. 

!!! note
    Parameters with default value / optional parameters have to go after those without default value in the function declaration.

Here, the second parameter is one with a default value attached.
```spice
f<int> demoFunction(string parameter1, double parameter2 = 5.89) {
	... // Do something with the parameters
	result = -4;
}
```

Theis procedure could get called like so:
```spice
demoFunction("input");
demoFunction("another input", 1.0);
```

!!! tip
    If you only want to execute some actions and don't need to return a value to the caller, please consider to use [procedures](../procedures) instead of functions.