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

p Human.ctor(string firstName, string lastName, unsigned int age) {
    this.firstName = firstName;
    this.lastName = lastName;
    this.age = age;
}

p Human.makeSound() {
    printf("Sigh...\n");
}

p Human.sayHello(string name) {
    printf("Hi, %s!\n", name);
}

type Car struct : MakeSound {
    string brand
    string model
    unsigned int seats
}

p Car.ctor(string brand, string model, unsigned int seats) {
    this.brand = brand;
    this.model = model;
    this.seats = seats;
}

p Car.makeSound() {
    printf("Wroom, wroom!\n");
}

type Parrot struct : MakeSound, Speak {
    string name
    unsigned int age
}

p Parrot.ctor(string name, unsigned int age) {
    this.name = name;
    this.age = age;
}

p Parrot.makeSound() {
    printf("Sqawk!\n");
}

p Parrot.sayHello(string name) {
    printf("Hello %s, squawk!\n", name);
}

f<int> main() {
    Human human = Human("John", "Doe", 25);
    Car car = Car("Toyota", "Corolla", 5);
    Parrot parrot = Parrot("Polly", 3);

    human.makeSound();
    car.makeSound();
    parrot.makeSound();

    human.sayHello("Jane");
    parrot.sayHello("Jane");
    return 0;
}
```

All living beings, parrots and humans have an age. So you might want to extract the `unsigned int age`, that exists
in both structs to a separate struct called `LivingBeing`.

```spice
// ...

type LivingBeing struct {
    unsigned int age
}

type Human struct : MakeSound, Speak {
    compose LivingBeing livingBeing
    string firstName
    string lastName
}

type Parrot struct : MakeSound, Speak {
    compose LivingBeing livingBeing
    string name
}

// ...
```

The `compose` keyword is used to include the fields and methods of another struct into the current struct. The members of the
parent struct are accessible directly via name. e.g. `parrot.age` or through the compose member itself, e.g.
`parrot.livingBeing.age`.