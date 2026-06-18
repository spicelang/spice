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

## Breaking circular imports with forward declarations

If two source files need to import each other (e.g. struct `A` holds a pointer to `B`, and `B` holds a pointer to
`A`), a direct import cycle would cause a compiler error. The solution is to introduce a forward-declaration file that
sits at the bottom of the dependency graph with no imports of its own:

**`b-fwd.spice`** — declares `B` without a body, imports nothing:
```spice
public forward type B struct;
```

**`a.spice`** — imports only the forward declaration:
```spice
import "b-fwd" as bfwd;

public type A struct {
    bfwd::B* b
}
```

**`b.spice`** — imports the full definition of `A` and provides the full body of `B`. It also imports `b-fwd` so
that the full type `B` reuses the forward declaration's type id, ensuring `A`'s `bfwd::B*` field is type-identical
to a `B*` resolved through `b.spice`:
```spice
import "a" as a;
import "b-fwd"; // re-import the forward declaration so the type ids line up

public type B struct {
    a::A* a
}
```

**`main.spice`** — imports both full definitions without any cycle:
```spice
import "a" as a;
import "b" as b;

f<int> main() {
    a::A* aPtr = nil<a::A*>;
    b::B* bPtr = nil<b::B*>;
}
```

The import graph is a DAG: `main → a → b-fwd` and `main → b → a → b-fwd`. No cycle exists because `b-fwd.spice`
imports nothing.

A forward-declared type can only be used as a pointer or reference — not as a bare value — because its size is unknown
until the full definition is imported. See [structs — forward declarations](structs.md#forward-declarations) and
[interfaces — forward declarations](interfaces.md#forward-declarations) for more detail.

## Platform dependent imports
If you have written code, that only works on one particular platform (e.g. two function with the same functionality, one for Linux
and one for Windows), you can name source files with the suffix of the OS e.g. `example_linux.spice` / `example_windows.spice`.
Then you can import the files without the suffix: `import "example" as example;`. Depending on your target triple, the matching
source file will be chosen at compile time automatically.