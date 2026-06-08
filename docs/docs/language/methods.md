---
title: Methods
---

Methods in Spice basically are [functions](functions.md) / [procedures](procedures.md), assigned to a struct. Within methods,
fields of the parent struct can be accessed like this: `this.fieldName`.

## Usage

A basic setup with one struct field, as well es a getter and a setter could look like this:
```spice
type Letter struct {
    string content
}

f<string> Letter.getContent() {
    return this.content;
}

p Letter.setContent(string text) {
    this.content = text;
}

f<int> main() {
    dyn letter = Letter { "No content" };
    letter.setContent("Hello World!");
    printf("Content: %s", letter.getContent());
}
```

Output:
```
Content: Hello World!
```

## Method visibility

Like functions and struct fields, methods are private by default. Mark a method `public` to allow code in other
source files to call it:

```spice
public type Counter struct {
    int value
}

public p Counter.increment() {
    this.value++;
}

public f<int> Counter.get() {
    return this.value;
}
```

A method that is not marked `public` can only be called from within the same source file.

## Methods on generic structs

Methods on a generic struct automatically inherit the struct's type parameters — you do not repeat the parameter list
on the method:

```spice
type T dyn;

type Stack<T> struct {
    T* data
    int size
}

p Stack.push(T item) {
    // ...
}

f<T> Stack.pop() {
    // ...
}
```

When `Stack<int>` is instantiated, `push` accepts an `int` and `pop` returns an `int`.

!!! tip
    You can initialize or destroy structs by using [constructors and destructors](constructors-destructors.md).
    Read more about those in the respective documentation section.
