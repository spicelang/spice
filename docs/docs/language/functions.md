---
title: Functions
---

Spice distinguishes strictly between functions and procedures. Functions are used to return something, that was calculated from a given input. Other than procedures, functions must have a return value. The paradigm here is, that a function calculates and returns something whereas a procedure executes some action without a result.

Functions in Spice can be defined like this:
```spice
f<double> functionName() {
	... // Do something
	return 5.6; // Return some value anywhere in the function body
}
```

Spice also allows you to pass arguments to a function. This arguments may also have default values, which can then be omitted when calling the function. 

!!! note
    Optional arguments have to go after the mandatory arguments in the function head.

Here, the second argument is one with a default value attached.
```spice
f<int> demoFunction(string arg1, double arg2 = 5.89) {
	... // Do something with the arguments
	result = -4;
}
```

This function could get called like so:
```spice
int result1 = demoFunction("input");
int result1 = demoFunction("another input", 1.0);
```

!!! tip
    If you only want to execute some actions and don't need to return a value to the caller, please consider to use [procedures](../procedures) instead of functions.