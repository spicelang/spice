---
title: Functions
---

Spice distinguishes strictly between functions and procedures. Functions are used to return something, that was calculated from a given input. Other than procedures, functions must have a return value. The paradigm here is, that a function calculates and returns something whereas a procedure executes some action without a result.

## Usage

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

## Example with default parameter value

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

## The implicit `result` variable

Inside any function, Spice provides an implicit variable called `result` that holds the return value. You can assign
to it at any point and return early with a bare `return`, or simply fall off the end of the function:

```spice
f<int> getAge(bool adult) {
    if adult {
        result = 18;
        return; // returns 18
    }
    result = 10; // returned when adult is false
}
```

This is equivalent to using an explicit `return` expression, but can be more readable when the return value is built
up across multiple branches.

## Function overloading

Multiple functions may share the same name as long as their parameter lists differ. The compiler picks the best match
at the call site:

```spice
f<int> describe(int n) {
    printf("integer: %d\n", n);
    return n;
}

f<int> describe(string s) {
    printf("string: %s\n", s);
    return 0;
}

f<int> main() {
    describe(42);        // calls the int overload
    describe("hello");   // calls the string overload
}
```

Overloads can also differ in the number of parameters or in whether parameters have default values.

!!! tip
    If you only want to execute some actions and don't need to return a value to the caller, please consider to use
    [procedures](procedures.md) instead of functions.
