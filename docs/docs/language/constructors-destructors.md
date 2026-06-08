---
title: Constructors and destructors
---

Sometimes you want to initialize the field of a struct or execute some action right after creating an instance of it. Spice
enables you doing that, by supporting constructors. To clean up a struct instance, destructors can be used.

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

The `ctor` method can also be called manually like calling [other methods](methods.md).

## Constructor overloading

A struct can have multiple constructors with different parameter lists, just like [function overloading](functions.md):

```spice
type Vec2 struct {
    double x
    double y
}

p Vec2.ctor() {
    this.x = 0.0;
    this.y = 0.0;
}

p Vec2.ctor(double x, double y) {
    this.x = x;
    this.y = y;
}

f<int> main() {
    Vec2 origin = Vec2();         // calls the no-arg constructor
    Vec2 point  = Vec2(3.0, 4.0); // calls the two-arg constructor
}
```

## Copy constructors

When a struct value is copied (passed by value, returned by value, or assigned), Spice calls the *copy constructor*
if one is defined. The copy constructor takes a `const` reference to an instance of the same type:

```spice
type Buffer struct {
    int copies
}

p Buffer.ctor() {
    this.copies = 0;
}

p Buffer.ctor(const Buffer& other) {
    this.copies = other.copies + 1;
    printf("copy #%d\n", this.copies);
}

f<int> main() {
    Buffer a = Buffer();
    Buffer b = a;   // prints "copy #1"
    Buffer c = b;   // prints "copy #2"
}
```

If no copy constructor is defined, Spice generates a default one that copies each field.

## Destructors

You have the option to create a destructor by providing a `dtor` method on a struct. It does not allow any arguments and has no
return type, since it is a procedure. Destructors can be especially useful for de-allocating objects in heap memory, that were
allocated via `malloc()`. Whenever a struct variable goes out of scope somewhere in the program, the compiler searches for a
destructor and calls it if available.

Here is an example for a custom destructor:

```spice
// Declarations of the generic type T as well as malloc() and free() ...

type ExampleStruct<T> struct {
	string message
	T* messageObject
}

p ExampleStruct.dtor() {
    free(this.messageObject);
}
```
