---
title: Arrays
---

You can use arrays in Spice for any [primitive](../data-types) or custom [struct](../structs) data type.

To initialize an `array` e.g. of type string, you can write:
```spice
string[5] myStringArray = { "First item", "Second Item", "Third item" };
```

The `5` in the brackets is the size of the array. The initial values can be provided in curly braces after the assign operator and must be of the same data type. <br>
As you can see, there are fewer values (3) than the array size is (5). The rest of the values are set to the default data type value. The default data type values are:

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
bool[12] myBoolArray = { condition ? true : false, false, true };

// Access item 0
int i = 2;
bool itemValue = myBoolArray[i -= 2];

printf("Value: %u", itemValue);
```