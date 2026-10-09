---
title: Interfaces
---

Interfaces let you define a set of method signatures that a [struct](structs.md) must implement. Any struct that declares
itself as implementing an interface is checked by the compiler — if a required method is missing or has the wrong
signature, compilation fails.

Spice uses interfaces for virtual dispatch: calling a method through an interface pointer resolves to the correct
implementation at runtime via a vtable, without carrying any type information overhead beyond the pointer itself.

## Defining an interface

```spice
type Driveable interface {
    p drive(int speed);
    f<bool> isDriving();
}
```

Method signatures inside an interface body use the same `f`/`p` syntax as regular functions and procedures, but have
no body.

## Implementing an interface

Attach the interface to a struct with `: InterfaceName` in the struct declaration. The compiler then requires that
every method declared in the interface is implemented as a method on that struct.

```spice
type Car struct : Driveable {
    bool driving
}

p Car.ctor() {
    this.driving = false;
}

p Car.drive(int speed) {
    this.driving = true;
}

f<bool> Car.isDriving() {
    return this.driving;
}
```

The method definitions live outside the struct body, exactly like any other [method](methods.md).

## Using an interface pointer

To call methods through an interface, take a pointer to the concrete value and assign it to a pointer of the
interface type. Calling a method on the interface pointer dispatches to the correct implementation at runtime.

```spice
f<int> main() {
    Car car = Car();
    Driveable* driveable = &car;
    driveable.drive(60);
    printf("Is driving: %d\n", driveable.isDriving()); // 1
}
```

You can also take an interface reference and pass it to a function that only knows about the interface:

```spice
p startDriving(Driveable& vehicle) {
    vehicle.drive(30);
}

f<int> main() {
    Car car = Car();
    Driveable& d = car;
    startDriving(d);
}
```

!!! note
    Spice does not allow assigning a struct value directly to an interface variable — you must use a pointer or
    reference. The compiler will report an error if you try `Driveable car = Car()`.

## Multiple interfaces

A struct can implement more than one interface by listing them separated by commas:

```spice
type Printable interface {
    p print();
}

type Serializable interface {
    f<string> serialize();
}

type Document struct : Printable, Serializable {
    string content
}

p Document.print() {
    printf("%s\n", this.content);
}

f<string> Document.serialize() {
    return this.content;
}
```

## Default methods

An interface can provide a default implementation for any of its methods. Default methods are defined outside the
interface body, like any other [method](methods.md), but on the interface instead of a struct. A struct, that implements
the interface, inherits the default method unless it implements the method itself.

```spice
type Greeter interface {
    f<string> getName();
    p greet();
}

// Default implementation of Greeter.greet
p Greeter.greet() {
    printf("Hello, %s!\n", this.getName());
}

type Person struct : Greeter {
    string name
}

p Person.ctor(string name) {
    this.name = name;
}

f<string> Person.getName() {
    return this.name;
}

type Robot struct : Greeter {}

f<string> Robot.getName() {
    return "R2D2";
}

// Robot replaces the default implementation
p Robot.greet() {
    printf("Beep boop, I am %s\n", this.getName());
}

f<int> main() {
    Person person = Person("Alice");
    Robot robot;
    person.greet(); // Hello, Alice!
    Greeter* greeter = &robot;
    greeter.greet(); // Beep boop, I am R2D2
}
```

Within a default method, `this` is a pointer to the interface. Default methods can therefore only use the other methods
of the interface and have no access to the fields of the struct. Calls to other interface methods always dispatch to the
implementation of the concrete struct.

A few rules apply to default methods:

- A default method must implement a method signature of its interface, with exactly the same parameter and return types.
- Default methods cannot be constructors or destructors and cannot have optional parameters.
- If a struct declares a method with the same name as a default method, but with another signature, it does not inherit
  the default method and has to implement the interface method itself.
- If a struct implements multiple interfaces, that declare the same method, and one of them provides a default method for
  it, the struct has to implement the method itself.
- Default methods of generic interfaces are not supported yet.

## Generic interfaces

Interfaces can have generic type parameters, just like [generic structs](generics.md):

```spice
type T dyn;

type Comparable<T> interface {
    f<int> compareTo<T>(const T& other);
}

type Temperature struct : Comparable<Temperature> {
    double celsius
}

p Temperature.ctor(double celsius) {
    this.celsius = celsius;
}

f<int> Temperature.compareTo(const Temperature& other) {
    if this.celsius < other.celsius { return -1; }
    if this.celsius > other.celsius { return 1; }
    return 0;
}

f<int> main() {
    Temperature cold = Temperature(0.0);
    Temperature warm = Temperature(25.0);
    printf("%d\n", cold.compareTo(warm)); // -1
}
```

## Interfaces across source files

Interfaces can be defined in one file and used in another. Mark the interface and its method signatures `public` so
they are visible when imported:

**`shapes.spice`:**
```spice
public type Shape interface {
    public f<double> area();
    public p print();
}
```

**`main.spice`:**
```spice
import "shapes";

type Circle struct : Shape {
    double radius
}

p Circle.ctor(double radius) {
    this.radius = radius;
}

f<double> Circle.area() {
    return 3.14159 * this.radius * this.radius;
}

p Circle.print() {
    printf("Circle(r=%.2f)\n", this.radius);
}

f<int> main() {
    Circle c = Circle(5.0);
    Shape* s = &c;
    s.print();
    printf("Area: %.2f\n", s.area());
}
```

## Standard library interfaces

The standard library ships two interfaces you will encounter frequently:

| Interface | Module | Purpose |
|---|---|---|
| `IIterable<T>` | `std/iterator/iterable` | Marks a type as iterable in a `foreach` loop |
| `IIterator<T>` | `std/iterator/iterator` | Consumed by `foreach` to step through elements |

Implementing `IIterable<T>` on your own collection type makes it work with `foreach` automatically. See
[foreach loops](foreach-loops.md) for details.
