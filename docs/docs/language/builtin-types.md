---
title: Builtin data types
---

In extension to the [primitive data types](../primitive-types.md), Spice offers builtin data types.

## The `String` data type
In opposite to the [string primitive type](primitive-types.md#the-string-data-type), the `String` builtin type is
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
- `void String(const String&)`: Initialize by copying another `String` (copy constructor) 
- `void String(int)`: Initialize with an initial size
- `void String(long)`: Initialize with an initial size

### Methods
The `String` builtin type offers the following methods:

- `void append(string)`: Appends a raw string
- `void append(const String&)`: Appends a string
- `void append(char)`: Appends a single char
- `string getRaw()`: Returns a char* to the heap allocated value
- `unsigned long getLength()`: Returns the length of the string in chars
- `bool isEmpty()`: Checks if the string has a length of 0
- `unsigned long getCapacity()`: Returns the allocated space in bytes
- `bool isFull()`: Checks if the length is equal with the capacity
- `void clear()`: Clear the value of the string
- `long find(string, unsigned int)`: Returns the index, where a substring was found, starting from a start index
- `long find(string, unsigned long)`: Returns the index, where a substring was found, starting from a start index
- `long find(char, unsigned long)`: Returns the index, where a subchar was found, starting from a start index
- `long rfind(string, unsigned int)`: Returns the index, where a substring was found, starting from reversed at a start index
- `long rfind(string, unsigned long)`: Returns the index, where a substring was found, starting from reversed at a start index
- `long rfind(char, unsigned long)`: Returns the index, where a subchar was found, starting from reversed at a start index
- `bool contains(string)`: Checks if the string contains a substring
- `bool startsWith(string)`: Checks if the string starts with a substring
- `bool endsWith(string)`: Checks if the string ends with a substring
- `void reverse()`: Reverses the value of the string
- `void replace(string, string, unsigned long)`: Replaces a substring with another string, starting from a start index
- `void replaceAll(string, string)`: Replaces all occurrences of a substring with another string
- `void replaceAll(char, char)`: Replaces all occurrences of a subchar with another char
- `String getSubstring(unsigned int, long)`: Returns the substring from start index `x` and length `y`
- `String getSubstring(unsigned long, long)`: Returns the substring from start index `x` and length `y`
- `String getSubstring(unsigned short, long)`: Returns the substring from start index `x` and length `y`
- `void reserve(unsigned int)`: Increase the capacity to the given number
- `void reserve(unsigned long)`: Increase the capacity to the given number
- `void reserve(unsigned short)`: Increase the capacity to the given number

### Static functions
The `String` builtin type offers the following static functions:

- `getRawLength(string)`: Returns the length of a raw string
- `isRawEqual(string, string)`: Checks if two raw strings are equal in value

### Operators
The `String` builtin type overrides the following operators:

- `String operator+(const String&, const String&)`: Concatenates two strings and returns the result
- `String operator+(const String&, const string&)`: Concatenates a string and a raw string and returns the result
- `String operator+(const String&, const char&)`: Concatenates a string and a raw string and returns the result
- `String operator+(const string&, const String&)`: Concatenates a raw string and a string and returns the result
- `String operator+(const string&, const string&)`: Concatenates two raw strings and returns the result
- `String operator+(const string&, const char&)`: Concatenates two raw strings and returns the result
- `void operator+=(String&, String)`: Appends a string
- `void operator+=(String&, string)`: Appends a raw string
- `void operator+=(String&, char)`: Appends a single char
- `String operator*(const String&, int)`: Concatenates a string with itself n times
- `String operator*(const String&, long)`: Concatenates a string with itself n times
- `String operator*(const String&, short)`: Concatenates a string with itself n times
- `String operator*(int, const String&)`: Concatenates a string with itself n times
- `String operator*(long, const String&)`: Concatenates a string with itself n times
- `String operator*(short, const String&)`: Concatenates a string with itself n times
- `void operator*=(String&, int)`: Concatenates with itself n times
- `void operator*=(String&, long)`: Concatenates with itself n times
- `void operator*=(String&, short)`: Concatenates with itself n times
- `bool operator==(const String&, const String&)`: Checks if two strings are equal in value
- `bool operator==(const String&, string)`: Checks if two strings are equal in value
- `bool operator==(string, const String&)`: Checks if two strings are equal in value
- `bool operator!=(const String&, const String&)`: Checks if two strings are unequal in value
- `bool operator!=(const String&, string)`: Checks if two strings are unequal in value
- `bool operator!=(string, const String&)`: Checks if two strings are unequal in value

## The `Result` data type
The `Result<T>` builtin type is a generic type, which is used to return a value or an error. It is used to handle errors

### Constructors
The `Result<T>` builtin type offers the following constructors:

- `void Result(const T&)`: Initialize Result object with a value
- `void Result(const Error&)`: Initialize Result object with an error

### Methods
The `Result<T>` builtin type offers the following methods:

- `T unwrap()`: Returns the value of the Result object. If the Result object contains an error, the program will panic
- `Error getErr()`: Returns the error of the Result object. If no error is present, an error object with error code 0 is returned.
- `bool isOk()`: Checks if the Result object contains a value
- `bool isErr()`: Checks if the Result object contains an error

### Static functions
The `Result<T>` builtin type offers the following static functions:

- `Result<T> ok(const T&)`: Returns a Result object with a value
- `Result<T> err(const Error&)`: Returns a Result object with an error
- `Result<T> err(int, string)`: Returns a Result object with an error, constructed with an error code and an error message
- `Result<T> err(string)`: Returns a Result object with an error, constructed with an error message

## The `Error` data type
The `Error` builtin type is used to represent an error. It can be used e.g. in combination with the `Result<T>` type.

### Constructors
The `Error` builtin type offers the following constructors:

- `void Error()`: Initialize an empty error object. This object has an error code of 0 and the error message `Runtime error`
- `void Error(int, string)`: Initialize an error object with an error code and an error message
- `void Error(string)`: Initialize an error object with an error message

### Methods
The `Error` builtin type offers the following methods:

- `void print()`: Prints the error message to the standard error output
- `void toPanic()`: Triggers a panic with this error