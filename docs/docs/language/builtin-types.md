---
title: Builtin data types
---

In extension to the [primitive data types](../primitive-types.md), Spice offers builtin data types.

## The `String` data type
In opposite to the [string primitive type](../primitive-types.md#the-string-data-type), the `String` builtin type is
mutable and offers several ways to modify the contained value.

!!! tip "Tip"
    Use the `string` primitive type over the `String` builtin type as much as possible, due to its advantages in runtime
    performance. The usage of the `String` builtin type is only recommended, when you need to modify the value of the
    string at runtime. `string` variables are always immutable.

### Constructors
The `String` builtin type offers the following constructors:

- `void String()`: Initialize empty
- `void String(string)`: Initialize with a raw `string` as start value
- `void String(char)`: Initialize with a single char
- `void String(String)`: Initialize by copying another `String` (copy constructor) 

### Methods
The `String` builtin type offers the following methods:

- `void append(string)`: Appends a raw string
- `void append(String)`: Appends a string
- `void append(char)`: Appends a single char
- `char* getRaw()`: Returns a char* to the heap allocated value
- `long getLength()`: Returns the length of the string in chars
- `bool isEmpty()`: Checks if the string has a length of 0
- `long getCapacity()`: Returns the allocated space in bytes
- `bool isFull()`: Checks if the length is equal with the capacity
- `void clear()`: Clear the value of the string
- `long find(string, int)`: Returns the index, where a substring was found, starting from a start index
- `long find(string, long)`: Returns the index, where a substring was found, starting from a start index
- `long find(string, short)`: Returns the index, where a substring was found, starting from a start index
- `bool contains(string)`: Checks if the string contains a substring
- `void reverse()`: Reverses the value of the string
- `String substring(int, long)`: Returns the substring from start index `x` and length `y`
- `String substring(long, long)`: Returns the substring from start index `x` and length `y`
- `String substring(short, long)`: Returns the substring from start index `x` and length `y`
- `void reserve(int)`: Increase the capacity to the given number
- `void reserve(long)`: Increase the capacity to the given number
- `void reserve(short)`: Increase the capacity to the given number

### Operators
The `String` builtin type overrides the following operators:

- `String operator+(String, String)`: Concatenates two strings and returns the result
- `String operator+(String, string)`: Concatenates a string and a raw string and returns the result
- `String operator+(string, String)`: Concatenates a raw string and a string and returns the result
- `String operator+(string, string)`: Concatenates two raw strings and returns the result
- `void operator+=(String&, String)`: Appends a string
- `void operator+=(String&, string)`: Appends a raw string
- `void operator+=(String&, string)`: Appends a single char
- `String operator*(String, int)`: Concatenates a string with itself n times
- `String operator*(String, long)`: Concatenates a string with itself n times
- `String operator*(String, short)`: Concatenates a string with itself n times
- `String operator*(int, String)`: Concatenates a string with itself n times
- `String operator*(long, String)`: Concatenates a string with itself n times
- `String operator*(short, String)`: Concatenates a string with itself n times
- `void operator*=(String&, int)`: Concatenates with itself n times
- `void operator*=(String&, long)`: Concatenates with itself n times
- `void operator*=(String&, short)`: Concatenates with itself n times
- `bool operator==(String, String)`: Checks if two strings are equal in value
- `bool operator!=(String, String)`: Checks if two strings are unequal in value