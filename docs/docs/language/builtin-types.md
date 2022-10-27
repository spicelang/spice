---
title: Builtin data types
---

In extension to the [primitive data types](../primitive-types), Spice offers builtin data types.

## The `String` data type
In opposite to the [string primitive type](../primitive-types#the-string-data-type), the `String` builtin type is
mutable and offers several ways to modify the contained value.

!!! tip "Tip"
    Use the `string` primitive type over the `String` builtin type as much as possible, due to its advantages in runtime
    performance. The usage of the `String` builtin type is only recommended, when you need to modify the value of the
    string.

### Constructors
The `String` builtin type offers the following constructors:

- `void String()`: Initialize empty
- `void String(string)`: Initialize with a raw `string` as start value
- `void String(string, string)`: Initialize with the concatenation of string1 and string2
- `void String(char)`: Initialize with a single char
- `void String(String)`: Initialize with another `String` (copy constructor) 

### Methods
The `String` builtin type offers the following methods:

- `void append(string)`: Appends a raw string
- `void append(char)`: Appends a single char
- `char* getRaw()`: Returns a char* to the heap allocated value
- `long getLength()`: Returns the length of the string in chars
- `bool isEmpty()`: Checks if the string has a length of 0
- `long getCapacity()`: Returns the allocated space in bytes
- `bool isFull()`: Checks if the length is equal with the capacity
- `void clear()`: Clear the value of the string
- `long find(string, int)`: Returns the index, where a substring was found, starting from a start index
- `long find(string, long)`: Returns the index, where a substring was found, starting from a start index
- `bool contains(string)`: Checks if the string contains a substring
- `void reverse()`: Reverses the value of the string
- `void reserve(int)`: Increase the capacity to the given number
- `void reserve(long)`: Increase the capacity to the given number

### Operators
The `String` builtin type overrides the following operators:

- `<String> + <String>`: Concatenates two `String` types and returns the result
- `<String> * <int|short|long>`: Concatenate itself, e.g.: `Hi * 2 -> HiHi`
- `<int|short|long> * <String>`: Concatenate itself, e.g.: `2 * Hi -> HiHi`
- `<String> == <String>`: Checks for equality
- `<String> != <String>`: Check for inequality