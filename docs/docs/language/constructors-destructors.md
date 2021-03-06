---
title: Constructors and destructors
---

Sometimes you want to initialize the field of a struct or execute some action right after creating an instance of it. Spice enables you doing that, by supporting constructors. To clean up a struct instance, destructors can be used.

## Constructors
Whenever you create a struct instance like this, Spice will automatically call the `ctor` method of this struct:

```spice
type ExampleStruct struct {
	string message
	bool showMessage
}

p ExampleStruct.ctor() {
	this.message = "Hello world!";
	showMessage = false;
}

f<int> main() {
	// Note the parentheses instead of the braces of a normal struct instantiation
	ExampleStruct es = ExampleStruct();
	// Do something with 'es' ...
}
```

Constructors can also have arguments to pass some information from the caller to the constructor of the struct:

```spice
// Struct definition ...

p ExampleStruct.ctor(string msg = "Hello World!") {
	this.message = msg;
	showMessage = false;
}

f<int> main() {
	ExampleStruct es = ExampleStruct("Hi fellow programmers!");
	// Do something with 'es' ...
}
```

The `ctor` method can also be called manually like calling [other methods](./methods).

## Destructors
*Feature to come ...*