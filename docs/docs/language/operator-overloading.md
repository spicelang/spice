---
title: Operator Overloading
---

Spice allows overloading operators for [custom struct types](structs.md).
Currently, this works for the operators `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `<<`, `>>`, `&`, `|`, `^`,
`~` (prefix), `+=`, `-=`, `*=`, `/=`, `[]`, `=`, `++` (postfix) and `--` (postfix).

## Usage

Overloading operators works like this:
```spice
type Size alias long;

type Counter struct {
    Size value
}

p Counter.ctor(long initialValue = 0l) {
    this.value = initialValue;
}

f<Size> Counter.getValue() {
    return this.value;
}

/**
 * Here we define the plus operator for lhs of type Counter and rhs of type Counter
 *
 * @param c1 Counter 1
 * @param c2 Counter 2
 * @return New counter with the sum of c1 and c2
 */
f<Counter> operator+(const Counter& c1, const Counter& c2) {
    return Counter(c1.value + c2.value);
}

f<int> main() {
    Counter counter1 = Counter(2l);
    Counter counter2 = Counter(3l);
    printf("Counter1 value: %d\n", counter1.getValue());
    printf("Counter2 value: %d\n", counter2.getValue());
    Counter counter3 = counter1 + counter2; // Here we call the overloaded operator
    printf("Counter3 value: %d\n", counter3.getValue());
}
```

## Bitwise operators

The binary bitwise operators `&`, `|` and `^` as well as the unary prefix operator `~` can be overloaded as well.
The unary `~` operator takes a single operand:

```spice
type Flags struct {
    int bits
}

f<Flags> operator&(const Flags f1, const Flags f2) {
    return Flags{f1.bits & f2.bits};
}

f<Flags> operator|(const Flags f1, const Flags f2) {
    return Flags{f1.bits | f2.bits};
}

f<Flags> operator^(const Flags f1, const Flags f2) {
    return Flags{f1.bits ^ f2.bits};
}

f<Flags> operator~(const Flags f) {
    return Flags{~f.bits};
}

f<int> main() {
    Flags a = Flags{0b1100};
    Flags b = Flags{0b1010};
    Flags both = a & b;       // 0b1000
    Flags either = a | b;     // 0b1110
    Flags exclusive = a ^ b;  // 0b0110
    Flags inverted = ~a;      // bitwise complement of 0b1100
    printf("%d %d %d %d\n", both.bits, either.bits, exclusive.bits, inverted.bits);
}
```

## Relational operators

The relational operators `<`, `>`, `<=` and `>=` can be overloaded as well. Each of them is looked up on its own, so
define all of the ones you use. This makes struct types usable in containers like `PriorityQueue`, that order their items
with `<`:

```spice
type Version struct {
    int major
    int minor
}

f<bool> operator<(const Version& v1, const Version& v2) {
    return v1.major < v2.major || (v1.major == v2.major && v1.minor < v2.minor);
}

f<bool> operator>=(const Version& v1, const Version& v2) {
    return !(v1 < v2);
}

f<int> main() {
    Version v1 = Version{1, 2};
    Version v2 = Version{1, 10};
    printf("%d %d\n", v1 < v2, v1 >= v2); // 1 0
}
```

A generic less operator is written as `operator<<T>(...)`: the `<` operator, followed by the template type list. The shift
left operator is still recognized, if the parameter list or a template type list follows (`operator<<(...)` or
`operator<<<T>(...)`).
