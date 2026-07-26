---
title: Arrays
---

You can use arrays in Spice for any [primitive](primitive-types.md) or custom [struct](structs.md) data type.

## Usage

To initialize an `array` e.g. of type string, you can write:
```spice
string[3] myStringArray = [ "First item", "Second Item", "Third item" ];
```

The `3` in the brackets is the size of the array. The initial values can be provided in curly braces after the assign operator and
must be of the same data type. <br>
You can only initialize an array with the number of items equal the arrays size or no items.

| Type     | Default value                               |
|----------|---------------------------------------------|
| `int`    | `0`                                         |
| `double` | `0.0`                                       |
| `string` | `""`                                        |
| `bool`   | `false`                                     |
| `struct` | Instance with default values for all fields |

To access an array item, you can use the index. The indexes are >= 0 per definition:
```spice
string item3Value = myStringArray[3];
```

More complex expressions for initial array values and item indexing are possible. Here's an example:
```spice
// Initialize array
dyn condition = 1 != 2;
bool[3] myBoolArray = [ condition ? true : false, false, true ];

// Access item 0
int i = 2;
bool itemValue = myBoolArray[i -= 2];

printf("Value: %u", itemValue);
```

## Arrays as parameters

Like in C, an array parameter decays to a pointer to the callers array. No copy is made when calling the function, so
assigning to an item of the parameter modifies the array of the caller:

```spice
p setFirst(int[3] values) {
    values[0] = 42; // Modifies the array of the caller
}

f<int> main() {
    int[3] numbers = [ 1, 2, 3 ];
    setFirst(numbers);
    printf("First item: %d", numbers[0]); // Prints 42
}
```

If you want the callee to work on its own copy, pass a copy explicitly. To hand over an array of arbitrary size, use an
unsized array parameter (e.g. `int[]`) instead, which is a plain pointer without a known item count.