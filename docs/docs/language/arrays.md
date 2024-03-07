---
title: Arrays
---

You can use arrays in Spice for any [primitive](../primitive-types.md) or custom [struct](../structs.md) data type.

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