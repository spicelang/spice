---
title: Pointers
---

Spice supports pointers by providing the address-of operator `&` and indirection operator `*`. <br>
For any primitive or custom data type you can also use the respective pointer type. For the `int` type, the pointer type is `int*`.

## Usage

See an example here:
```spice
int variable = 1234;
int* variablePtr = &variable;
printf("Value: %d, Pointer: %p", *variablePtr, variablePtr);
```

Here is a more complex example where pointers are used to pass a reference of a person to a procedure and modify a field.

```spice
type Person struct {
    string firstName
    string lastName
    int age
}

p birthday(Person* person) {
    person.age++;
}

f<int> main() {
    dyn mike = Person { "Mike", "Miller", 32 };
    printf("Age before birthday: %d\n", mike.age);
    birthday(&mike);
    printf("Age after birthday: %d", mike.age);
}
```

!!! tip "Multiple pointers"
    Spice also supports the usage of double, triple, ... pointers like `char***` or types like `int*[]*`.
    Please use such types sparely and only where they are necessary, due to readability.
    To get the pointer to the pointer to an integer, you can use `&&intVariable` and `**doubleIntPtr` in reverse.