---
title: Primitive data types
---

Spice supports eight different primitive data types out of the box: `double`, `int`, `short`, `long`, `byte`, `char`, `string` and
`bool`. In addition, there is a builtin type-inferred type, called `dyn`.
Let us take a look at each one individually!

## The `double` data type
Doubles are signed double-precision 64-bit IEEE 754 floating point numbers.

### Variables
In Spice, variables of type `double` can be defined like this:
```spice
double variable1 = 56.9042;

double variable2;
variable2 = -0.2349;
```

### Literals
Double literals require a decimal separator, may have a preceding sign and the scientific notation is supported.

They must match the following regular expression:
```
[-]?[0-9]*[.][0-9]+([eE][+-]?[0-9]+)?
```

Examples:
```spice
-0.5
611.456
2.51e-11
```

## The `int` data type
Integers are whole numbers of 32-bit.
Signed integers have a range from a min of -2,147,483,648 to a max of 2,147,483,647.
Unsigned integers have a range from a min of 0 to a max of 4,294,967,295.

### Variables
In Spice, variables of type `int` can be defined like this:
```spice
int variable1 = 903;

int variable2;
variable2 = -2;

unsigned int variable3 = 4294967295u;
```

### Literals
Integer literals may have a preceding sign. The literal can be noted in decimal, binary, hexadecimal or octal system,
each with its respective prefix and number/letter range. For unsigned int literals, the suffix `u` can be appended.
Scientific notation is currently not supported.

They must match the following regular expression:
```
INT_LIT := [-]?(DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?;
DEC_LIT := ([0][dD])?[0-9]+;
BIN_LIT := [0][bB][01]+;
HEX_LIT := [0][xXhH][0-9a-fA-F]+;
OCT_LIT := [0][oO][0-7]+;
```

Examples:
```spice
123
-5
0D6
0b101
0xf6a9
0o71037u
```

## The `short` data type
Shorts are whole numbers of 16-bit.
Signed shorts have a range from a min of -32,768 to a max of 32,767.
Unsigned shorts have a range from a min of 0 to a max of 65535.

### Variables
In Spice, variables of type `short` can be defined like this:
```spice
short variable1 = 15s;

short variable2;
variable2 = 0s;

unsigned short variable3 = 65535us;
```

### Literals
Short literals may have a preceding sign. The literal can be noted in decimal, binary, hexadecimal or octal system,
each with its respective prefix and number/letter range. For unsigned short literals, the suffix `u` can be appended.
The literal must end with the suffix `s`.
Scientific notation is currently not supported.

They must match the following regular expression:
```
SHORT_LIT := [-]?(DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?s;
DEC_LIT := ([0][dD])?[0-9]+;
BIN_LIT := [0][bB][01]+;
HEX_LIT := [0][xXhH][0-9a-fA-F]+;
OCT_LIT := [0][oO][0-7]+;
```

Examples:
```spice
0s
-87s
0d45us
0b0010s
0x76cbus
0o04324s
```

## The `long` data type
Longs are whole numbers of 64-bit.
Signed long have a range from a min of -9,223,372,036,854,775,808 to a max of 9,223,372,036,854,775,807.
Unsigned integers have a range from a min of 0 to a max of 18,446,744,073,709,551,615.

### Variables
In Spice, variables of type `long` can be defined like this:
```spice
long variable1 = 12492309573l;

long variable2;
variable2 = -34945968l;

unsigned long variable3 = 18446744073709551615ul;
```

### Literals
Long literals may have a preceding sign. The literal can be noted in decimal, binary, hexadecimal or octal system,
each with its respective prefix and number/letter range. For unsigned long literals, the suffix `u` can be appended.
The literal must end with the suffix `l`.
Scientific notation is currently not supported.

They must match the following regular expression:
```
LONG_LIT := [-]?(DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?l;
DEC_LIT := ([0][dD])?[0-9]+;
BIN_LIT := [0][bB][01]+;
HEX_LIT := [0][xXhH][0-9a-fA-F]+;
OCT_LIT := [0][oO][0-7]+;
```

Examples:
```spice
9283423923l
-435987l
0d7539ul
0b11001ul
0x7821l
0o6666ul
```

## The `byte` data type
Bytes are unsigned whole numbers of 8-bit, which have a range from a min of 0 to a max of 255.

### Variables
In Spice, variables of type `byte` can be defined like this:
```spice
byte variable1 = cast<byte>(11);

byte variable2;
variable2 = cast<byte>(12);
```

### Literals
There are no byte literals in Spice. Use type casting to cast an int, short or long literal to a byte.

```spice
byte b = cast<byte>(123u);
```

## The `char` data type
Chars are unsigned whole numbers of 8-bit, which have a range from a min of 0 to a max of 255. The value of a char represents the UTF-8 sign of the  8-bit integer value.

### Variables
In Spice, variables of type `char` can be defined like this:
```spice
char variable1 = 'A';

char variable2;
variable2 = '.';
```

### Literals
Char literals are noted either as a single character or a single escape sequence, enclosed in single quotes.

They must match the following regular expression:
```
'([^'\\\r\n]|\\.)'
```

Examples:
```spice
'c'
'\n'
```

## The `string` data type
Strings are immutable sequences of 8-bit integers (chars) and contain text-like information. The length of a string is unlimited.

!!! info "Note"
    This type of string is immutable. So if you want to mutate your string at runtime, you can use the
    [builtin String data type](builtin-types.md#the-string-data-type).

!!! tip "Tip"
    Use the `string` primitive type over the `String` builtin type as much as possible, due to its advantages in runtime
    performance. The usage of the `String` builtin type is only recommended, when you need to modify the value of the
    string at runtime.

### Variables
In Spice, variables of type `string` can be defined like this:
```spice
string variable1 = "test string";

string variable2;
variable2 = "Hello World!";
```

### Literals
String literals are noted as a sequence of character and/or escape sequences, enclosed in double quotes.

They must match the following regular expression:
```
"([^"\\\r\n]|\\.)*"
```

Examples:
```spice
"this is a test"
"\n\t\r\"' hello"
```

## The `bool` data type
Booleans are 1-bit integers and can be assigned with the exactly two values: `true` or `false`.

!!! info "Additional information"
    Many language components like [if statements](if-statements.md), [for loops](for-loops.md),
    [while loops](while-loops.md), etc. use the `bool` data type as evaluation unit for conditions.
    You can find more information about that in the respective sections.

### Variables
In Spice, variables of type `bool` can be defined like this:
```spice
bool variable1 = true;

bool variable2;
variable2 = false;
```

### Literals
Bool literals are either the keyword `true` or `false`.

## The `dyn` data type
The `dyn` data type (`dyn` stands for dynamic) is a more unconventional data type. The concrete type of a `dyn`
variable or field gets inferred at compile time so that the language stays type-safe. This also means, that once you
assign a value to a `dyn` variable, the type is pinned and can't be inferred anymore. Assigning a value of another type
to the same variable will result in a compile error.

!!! note "Usage of the dyn data type"
    The dyn data type can not be used everywhere. Function arguments can only be declared as `dyn`, when they have a default value
    attached to them, from which the type can be inferred. For more information about [functions](functions.md), please visit the respective
    documentation section.

### Variables
Dyn variables can be defined like this:

```spice
dyn variable1 = 1.24;
dyn variable2 = -67;
dyn variable3;
dyn variable4 = false;

variable3 = "demo string";
```
