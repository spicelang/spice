---
title: Builtin Functions
---

Spice offers nine builtin functions out of the box. Those can be used anywhere without having to be imported manually and
can be used to establish a minimal setup for testing or the like.

## The `printf` builtin
Printf works the same as the `printf` function in C and is designed for printing a string to the standard text output (cout).

### Signature
`int printf(string template, ...args)`

`template`: Template string, which can contain placeholders for values, passed as args to the `printf` builtin. <br>
`args`: Arbitrary number of arguments of any type. The particular type and the order of the types have to match the placeholders of the template string.

Within the template string you can use control characters like the `\n` control character to achieve line breaks. This works on Linux and Windows the same way.

### Placeholders
| Placeholder | Type                        | Output                                        | Example      |
|-------------|-----------------------------|-----------------------------------------------|--------------|
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
Sizeof returns the internal size of a variable, constant or type in bytes. To get the size in bits, simply multiply the result by 8.

### Signature
`unsigned long sizeof(<any variable or constant>)` or `unsigned long sizeof<any type>()`

`any variable`: Variable or constant of any type, `any type`: Any data type

### Usage example
```spice
sizeof(12); // 4
sizeof<int>() // 4

int[9] intArray = [];
sizeof(intArray); // 9 * 4 = 36

sizeof("Hello World!"); // 8 (Strings are Char pointers internally)
```

## The `alignof` builtin
Alignof returns the alignment of a variable, constant or type in bytes. To get the alignment in bits, simply multiply the result by 8.

### Signature
`unsigned long alignof(<any variable or constant>)` or `unsigned long alignof<any type>()`

`any variable`: Variable or constant of any type, `any type`: Any data type

### Usage example
```spice
alignof(12); // 4
alignof<int>() // 4

DemoStruct s = DemoStruct{123, 56l, 12s, "String"};
alignof(s); // 8 (the long value is the widest type in the struct)

alignof("Hello World!"); // 8 (Strings are Char pointers internally)
```

## The `typeid` builtin
Typeid returns the type id of a variable, constant or type as unsigned long.
The type id is not guaranteed to be the same on each program run, but it is guaranteed to be the same for the same type
within the same program run.

### Signature
`unsigned long typeid(<any variable or constant>)` or `unsigned long typeid<any type>()`

`any variable`: Variable or constant of any type, `any type`: Any data type

### Usage example
```spice
typeid(12); // 1015829715
typeid<int>() // 1015829715

DemoStruct s = DemoStruct{123, 56l, 12s, "String"};
typeid(s); // 1502876624
typeid<DemoStruct>(); // 1502876624
```

## The `typename` builtin
Typename returns the name of the type of a variable, constant or type as string.

### Signature
`string typename(<any variable or constant>)` or `string typename<any type>()`

`any variable`: Variable or constant of any type, `any type`: Any data type

### Usage example
```spice
typename(12); // "int"
typename<int>() // "int"

DemoStruct s = DemoStruct{123, 56l, 12s, "String"};
typename(s); // "DemoStruct"
typename<DemoStruct>(); // "DemoStruct"
```

## The `len` builtin
Len returns the length of a Spice array in items.

### Signature
`int len(dyn[] variable)`

`variable`: Variable of any  array type.

### Usage example
```spice
len([1, 2, 3, 4]); // 4

string[5] stringArray = ["string1", "string2", "string3"];
len(stringArray); // 5
```

## The `panic` builtin
Panic is used to terminate the program with an error message. The error message will be printed to standard error (stderr),
followed by the [stack trace](../how-to/stack-traces.md#panics-and-failed-assertions) of the panic, and the program will
terminate with exit code `1`.

### Signature
`void panic(const Error& error)`

`error`: Error object containing the error code and message.

### Usage example
```spice
panic(Error("This is an error message")));
```

## The `syscall` builtin
Syscall is used to send a specific syscall to the underlying operating system.
Up to six arguments in addition to the syscall number are allowed.

### Signature
`long syscall(unsigned short syscallNumber, ...args)`

`syscallNumber`: Number of the syscall. See [here](https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/) for all Linux syscalls.

### Usage example
```spice
// Use write syscall to print "Hello World!"
const string str = "Hello World!";
syscall(/* syscall no = write */ 1s, /*fd = stdout*/ 1, /* buffer */ str, len(str));
```

!!! warning
    It is not recommended to use the syscall builtin directly. There is a std package for interacting with the OS via
    system calls. Please use `import "std/os/syscall";` instead.

!!! note
    The syscall builtin is lowered to inline assembly, which the experimental TPDE backend does not support. Using it
    in combination with `--backend=tpde` results in a compile error.

## The `asm` builtin
Asm embeds inline assembly into the code. Since the compiler cannot verify what the assembly does, it may only be used
within `unsafe` blocks. The template and the constraints use the syntax of
[LLVM inline assembly](https://llvm.org/docs/LangRef.html#inline-assembler-expressions) and are passed to LLVM verbatim.

### Signature
`ResultType asm<ResultType>(string template, string constraints, ...operands)` <br>
`asm(string template, string constraints, ...operands)`

`ResultType`: Type of the direct output. Required if the constraints contain a direct output, otherwise omitted. <br>
`template`: Assembly code. Operands are referenced with `$0`, `$1`, ... in the order of the constraints, modifiers can be
applied like `${0:w}`. A literal `$` is written as `$$`. <br>
`constraints`: Comma-separated constraint list: outputs (`=r`, `=*m`), then inputs (`r`, `*m`, `0` to tie the input to
output 0), then clobbers (`~{memory}`). <br>
`operands`: The operands of the indirect outputs and inputs, in the order of their constraints. Indirect operands (`*`)
are pointers to the memory, the assembly reads or writes.

Operands and the result type can be primitive, pointer or enum types. Both strings must be known at compile time, so they
have to be string literals or references to `const string` global variables. The latter allows to share a constraint list
between several `asm` calls or to keep long clobber lists readable.

### Usage example
```spice
int a = 40;
int b = 2;
unsafe {
    // Direct output, the first input is tied to it
    const int sum = asm<int>("addl $2, $0", "=r,0,r", a, b);

    // Indirect output
    long value = 0l;
    asm("movq $$42, $0", "=*m", &value);

    // No operands, memory clobber (compiler barrier)
    asm("mfence", "~{memory}");
}
```

```spice
const string ADD_CONSTRAINTS = "=r,0,r";

f<int> add(int a, int b) {
    unsafe {
        return asm<int>("addl $2, $0", ADD_CONSTRAINTS, a, b);
    }
}
```

The compiler treats every inline assembly as having side effects, so it is never removed or moved by the optimizer. On
x86, the flags are always considered clobbered, like with GCC and clang. Errors in the assembly code itself, like an unknown
instruction, are reported by the assembler of the backend without the location in the Spice source.

Spice supports a single direct output. Further results can be passed out via indirect outputs (`=*m`).

!!! note
    The experimental TPDE backend does not support inline assembly. Using `asm` in combination with `--backend=tpde`
    results in a compile error.
