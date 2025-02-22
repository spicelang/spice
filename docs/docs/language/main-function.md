---
title: Main Function
---

The `main` function is the entry point to any Spice program. When the operating system runs your compiled executable, the `main` function is
the first function, which is called. Therefore, it is mandatory and must have the signature `int main()` or
`int main(int argc, string[] argv)` for retrieving command line arguments. The main function has a return value of type `int`,
which represents the return / exit / status code of the executable.

## Usage

The `main` function can be declared like this:
```spice
f<int> main() {
	// Do something ...
}
```

Like any other [function](functions.md), the `main` function automatically declares the variable `result` of the same type as the
function return type, which is `int` in this case. You then have the option to use either the return statement
(e.g.: `return 1;`) or assign a value to the `result` variable. If you choose the second option, the function continues
to run and return the assigned value at the end of the function body.
Unlike normal functions, the `main` function has `0` as the initial value assigned to the `result` variable for reasons of
convenience and to not always have to write `return 0;` or `result = 0;` to exit the program with a positive exit code.

!!! info "Hello World program"
    Now, as you know how to start a program in Spice, you may like to write your first Spice program. Visit the guide for the
    [Hello World example](hello-world.md) to get started!

## Command Line Arguments
Spice programs can accept command line arguments similar you would write it in C:

```spice
f<int> main(int argc, string[] argv) {
    printf("Argc: %d\n", argc);
    printf("Argv no. 0: %s\n", argv[0]);
    if (argc > 1) {
        printf("Argv no. 1: %s\n", argv[1]);
    }
}
```

The parameter `argc` holds the number of passed arguments and the `argv` array contains the values of those arguments.

!!! info "--no-entry"
    If the `--no-entry` flag is passed to the Spice compiler, the main function will not be included in the final output
    binary. Also, if provided, this option makes the main function non-mandatory.
