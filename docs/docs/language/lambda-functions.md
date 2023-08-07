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