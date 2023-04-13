# Technical Specification for Iterators in Spice

## Implementation steps:
- [x] 1. Implement simple NumberIterator in std
- [x] 2. Adjust foreach loop to work with iterators
- [x] 3. Implement VectorIterator in std
- [x] 4. Offer iterator convenience helper

## Usage

Foreach loops in Spice are syntactic sugar to combine while loops with iterators to iterate over a container type.
The compiler checks if the input iterator type implements the `Iterable` interface and thus also implements the four
required methods. Before each iteration, the foreach loops checks that the `isValid` method still returns true. If
this is the case, the body is entered and the foreach loop will load the item (+ index), using the `get` or the `getIdx`
method respectively. After executing the body, the `next` method is called to advance the iterator.

```spice
foreach long idx, int item : range(3, 8) {
    printf("Idx: %d, item: %d\n", idx, item);
}
```

corresponds to

```spice
dyn iterator = range(3, 8);
while iterator.isValid() {
    Pair<unsigned long, int&> pair = iterator.getIdx();
    long idx = pair.getFirst();
    int item = pair.getSecond();
    printf("Idx: %d, item: %d\n", idx, item);
    iterator.next();
}
```

## Design

The Spice standard library offers an interface, called `Iterable`. This interface contains
four methods, that all structs that use it need to implement:

- `f<T&> get()`
- `f<Pair<unsigned long, T&> getIdx()`
- `f<bool> isValid()`
- `p next()`

The `get` method is used to retrieve the current item by reference. Per reference, because there could
be very large objects that are very costly to copy. Furthermore, objects returned by the `get` method can
be modified directly on the reference.

The `getIdx` method returns a pair, that contains the current index at its first and a reference to the
current item as its second field. This is useful for indexed foreach loops, to not have to call two methods
internally.

The `isValid` method is used to check if we have looped through all items or if there are any remaining ones.

The `next` method advances the iterator by one item, so that the next call to `get` returns the next item.

## Extending iterators with functionality

Some iterators implement additional methods for user-friendliness and offer overloaded operators
e.g. for `++`, `--`, `+=`, `-=`, `*`, etc.
