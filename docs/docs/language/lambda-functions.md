---
title: Lambda functions
---

Spice allows you to create lambda functions. Lambda functions are anonymous functions that can be used as values.
They are useful for e.g. passing callback functions as parameters into other functions.

## Usage

Lambdas in Spice work like this:

### Procedure lambdas

```spice
// Definition
p(int, double) lambda = p(int a, double b) {
    printf("Lambda was called with %d and %f", a, b);
};
// Call
lambda(5, 3.14);
```

### Function lambdas

```spice
// Definition
f<int>(const String&, bool) lambda = f<int>(const String& str, bool b) {
    if (b) {
        return str.getLength();
    } else {
        return -1;
    }
};
// Call
int result = lambda(String("Hello"), true);
```

### Inline lambdas

```spice
sort((int x, int y) => x < y);
```

## Capturing lambdas and the `Lambda` wrapper

A lambda that captures variables stores those captures in the stack frame that created it. This is efficient and
perfectly safe as long as the lambda does not outlive that frame - for example when it is called immediately or
passed to a function that uses it synchronously.

It becomes unsafe when a **capturing** lambda is kept somewhere that lives longer than the frame that created it,
such as a struct field or the return value of a function. To prevent dangling captures, the compiler rejects this:

```spice
type Button struct {
    p() onClick // a capturing lambda assigned here would be rejected, because it would dangle
}
```

To store a capturing lambda, wrap it in the owning `Lambda` type from `std/type/lambda`. The wrapper relocates the
captures onto the heap and owns them, so the lambda stays valid for as long as the `Lambda` object lives. Retrieve
the underlying lambda with `get()` to call it:

```spice
import "std/type/lambda";

type Button struct {
    Lambda<p()> onClick
}

p Button.ctor(string label) {
    this.onClick = Lambda<p()>(p() {
        printf("Clicked: %s\n", label); // captures `label`
    });
}

p Button.click() {
    p() handler = this.onClick.get();
    handler();
}
```

`Lambda` values can be copied, assigned and passed around freely. Non-capturing lambdas (plain function pointers)
do not need the wrapper - they are safe to store directly and can be used as field and return types as before.
