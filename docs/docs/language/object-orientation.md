---
title: Object-oriented programming
---

Spice supports object-oriented programming in the form of composition. That means that types can be composed via
structs, interfaces and methods.

## Example scenario

You need to model three different kinds of objects: humans, cars and parrots. Therefore, you create three structs with
the respective names:

```spice
type Human struct {
    string firstName
    string lastName
    unsigned int age
}

type Car struct {
    string brand
    string model
    unsigned int seats
}

type Parrot struct {
    string name
    unsigned int age
}
```

Now, as you have a speaking parrot, you want to give the parrot and the human the ability to speak. In addition to that,
all three objects can make sounds. The recommended way of doing this, is to use interfaces like this:

```spice
type Speak interface {
    p sayHello(string);
}

type MakeSound interface {
    p makeSound();
}

type Human struct : MakeSound, Speak {
    string firstName
    string lastName
    unsigned int age
}

p Human.makeSound() {
    // Sigh ...
}

p Human.sayHello() {
    // Hi!
}

type Car struct : MakeSound {
    string brand
    string model
    unsigned int seats
}

p Car.makeSound() {
    // Wroom, wroom
}

type Parrot struct : MakeSound, Speak {
    string name
    unsigned int age
}

p Parrot.makeSound() {
    // Sqawk!
}

p Parrot.sayHello() {
    // Hello, squawk!
}
```

As all living beings, parrots and humans have an age. So you might want to extract the `unsigned int age`, that exists
in both structs to a separate struct called `LivingBeing`.

```spice
// ...

type LivingBeing struct {
    unsigned int age
}

type Human struct : MakeSound, Speak {
    LivingBeing livingBeing
    string firstName
    string lastName
}

type Parrot struct : MakeSound, Speak {
    LivingBeing livingBeing
    string name
}

// ...
```