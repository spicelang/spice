---
title: Modules
---

Spice supports the linkage of several source files to one executable. To do so, you can use the `import` statement for tying in another source file to the current one.

## Importing own source files
Here is an example for importing an own source file into the main source file. Imagine you have the `utils.spice` file, containing some util functions, which you want to use in your main source file `main.spice`.

**Util functions file `utils.spice`:**
```spice
// Simple recursive fibonacci algorithm
public f<int> fib(int n) {
    if n <= 2 { return 1; }
    return fib(n - 1) + fib(n - 2);
}
```

**Main source file `main.spice`:**
```spice
import "utils" as fibonacci;

f<int> main() {
	dyn fibResult = fibonacci::fib(30);
	printf("Fibonacci result: %d", fibResult);
}
```

In the example above, the imported file lives in the same directory as the main source file. If you have a nested folder structure
you can state the import like this: `import "sub-folder/sub-sub-folder/utils" as fibonacci`.

You can also write the source file like this, using anonymous imports:

```spice
import "utils";

f<int> main() {
	dyn fibResult = fib(30);
	printf("Fibonacci result: %d", fibResult);
}
```

In the case that two imported source files expose symbols with the same name, Spice does not offer the anonymous usage of both
symbols to prevent the usage of the wrong symbol. Then, you need to specify explicitly, which of the two symbols is meant.

!!! tip "Linking against source files with another main function"
    Spice supports linking against source files which include a main function. Although the main function in the imported source
    file will be ignored and only the main function in the main source file will be taken into account.

## Importing functionality from the standard library
Every import starting with `std/`, will be automatically assigned to the standard library.

```spice
import "std/text/print";

f<int> main() {
	println("This is a single line");
}
```

## Circular imports

Source files may import each other, directly or transitively. Spice resolves such circular imports automatically — you
simply write the imports you need, no special handling required.

A common case is two structs that reference each other: struct `A` (in `a.spice`) holds a pointer to `B`, and struct `B`
(in `b.spice`) holds a pointer back to `A`.

**`a.spice`:**
```spice
import "b" as b;

public type A struct {
    B* bPtr
}
```

**`b.spice`:**
```spice
import "a" as a;

public type B struct {
    A* aPtr
}
```

**`main.spice`:**
```spice
import "a" as a;
import "b" as b;

f<int> main() {
    A aInst;
    B bInst;
    aInst.bPtr = &bInst;
    bInst.aPtr = &aInst;
}
```

Here `a.spice` and `b.spice` import each other, forming a cycle in the dependency graph. The compiler handles the cycle
internally, so the `B*` field in `A` is type-identical to a `B` resolved through `b.spice`, and vice versa. Mutually
recursive functions spread across files work the same way.

## Platform dependent imports
If you have written code, that only works on one particular platform (e.g. two function with the same functionality, one for Linux
and one for Windows), you can name source files with the suffix of the OS e.g. `example_linux.spice` / `example_windows.spice`.
Then you can import the files without the suffix: `import "example" as example;`. Depending on your target triple, the matching
source file will be chosen at compile time automatically.