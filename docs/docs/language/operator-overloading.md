---
title: Operator Overloading
---

Spice allows overloading operators for [custom struct types](structs.md).
Currently, this works for the operators `+`, `-`, `*`, `/`, `==`, `!=`, `<<`, `>>`, `+=`, `-=`, `*=`, `/=`, `[]`, `=`,
`++` (postfix) and `--` (postfix).
In the future, more operators will be supported for overloading.

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