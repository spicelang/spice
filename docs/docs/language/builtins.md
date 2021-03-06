---
title: Builtin Functions
---

Spice offers two builtin functions out of the box. Those can be used anywhere without having to be imported manually and can be used to establish a minimal setup for testing or the like.

## The `printf` builtin
Printf works the same as the `printf` function in C and is designed for printing a string to the standard text output (cout).

### Signature
`void printf(string template, ...args)`

`template`: Template string, which can contain placeholders for values, passed as args to the `printf` builtin. <br>
`args`: Arbitrary number of arguments of any type. The particular type and the order of the types have to match the placeholders of the template string.

Within the template string you can use the `\n` control character to achieve a line break. This works on Linux and Windows the same way.

### Placeholders
| Placeholder | Type                        | Output                                        | Example      |
| ----------- | --------------------------- | --------------------------------------------- | ------------ |
| `%c`        | `char`                      | Character                                     | a            |
| `%d`/`%i`   | `int`, `short`, `long`      | Signed decimal integer                        | 392          |
| `%u`        | `int`, `short`, `long`      | Unsigned decimal integer                      | 7235         |
| `%o`        | `int`, `short`, `long`      | Unsigned octal                                | 610          |
| `%x`        | `int`, `short`, `long`      | Unsigned hexadecimal integer                  | 7fa          |
| `%X`        | `int`, `short`, `long`      | Unsigned hexadecimal integer (uppercase)      | 7FA          |
| `%lld`      | `int`, `short`, `long`      | Signed decimal long integer                   | 7235         |
| `%llu`      | `int`, `short`, `long`      | Unsigned decimal long integer                 | 392          |
| `%a`        | `double`                    | Hexadecimal floating point (lowercase)        | -0xc.90fep-2 |
| `%A`        | `double`                    | Hexadecimal floating point (uppercase)        | -0XC.90FEP-2 |
| `%f`        | `double`                    | Decimal floating point (lowercase)            | 392.65       |
| `%F`        | `double`                    | Decimal floating point (uppercase)            | 392.65       |
| `%e`        | `double`                    | Scientific notation (lowercase)               | 3.9265e+2    |
| `%E`        | `double`                    | Scientific notation (uppercase)               | 3.9265E+2    |
| `%g`        | `double`                    | Use the shortest representation: `%e` or `%f` | 392.65       |
| `%G`        | `double`                    | Use the shortest representation: `%e` or `%f` | 392.65       |
| `%s`        | `string`, `char*`, `char[]` | String of characters                          | sample       |
| `%p`        | `<any>*`                    | Pointer address                               | b8000000     |
| `%%`        | -                           | Escape character for printing a single `%`    | %            |

### Usage example
```spice
printf("Here is a string: %s.\nAnd here is a double: %f", "Demo", 1.123);
```

## The `sizeof` builtin
Sizeof returns the internal size of a variable or a constant in bits. To get the size in bytes, simply divide the result by 8.

### Signature
`int sizeof(any variable)`

`variable`: Variable or constant of any type.

If the variable is a pointer type, the size of the contained type is being returned.

### Usage example
```spice
sizeof(12); // 32

int[9] intArray = {};
sizeof(intArray); // 9 * 32 = 288

sizeof("Hello World!"); // 64 (Strings are Char pointers internally)
```

## The `len` builtin
Len returns the length of a Spice array in items.

### Signature
`int len(any[] variable)`

`variable`: Variable of any type.

### Usage example
```spice
len({1, 2, 3, 4}); // 4

string[5] stringArray = {"string1", "string2", "string3"};
len(stringArray); // 5
```