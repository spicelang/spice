---
title: C/C++ Interoperability
---

Spice is designed to interoperate seamlessly with C and C++ code. Because Spice compiles to native machine code via
LLVM, there is no runtime bridge or conversion layer—calling a C function from Spice (or vice versa) is
zero-cost and ABI-compatible.

This tutorial covers the four building blocks of Spice–C interop:

1. Declaring external C functions with `ext`
2. Controlling name mangling with attributes
3. Linking C libraries with linker-flag attributes
4. Exposing Spice functions back to C

## Declaring external C functions

Use the `ext` keyword to declare a C function without defining it. Spice resolves the symbol at link time.

```spice
ext f<int> getpid();

f<int> main() {
    int pid = getpid();
    printf("Process ID: %d\n", pid);
    return 0;
}
```

The declaration syntax mirrors normal Spice functions:

- `f<ReturnType>` for functions that return a value
- `p` for procedures (`void` functions in C)

To bind `malloc` and `free` from the C standard library:

```spice
ext f<heap byte*> malloc(unsigned long);
ext p free(heap byte*);

f<int> main() {
    heap byte* buf = malloc(64ul);
    // ... use buf ...
    free(buf);
    return 0;
}
```

!!! note "The `heap` qualifier"
    `heap` on a pointer return type tells the Spice compiler that the memory was heap-allocated by the callee.
    It enables the compiler to track the pointer's ownership and lifetime correctly.

Variadic C functions (those declared with `...` in C) are supported too:

```spice
ext f<int> snprintf(byte*, unsigned long, string, ...);
```

## Renaming an external symbol

`ext` declarations are never name-mangled—the compiler always uses the plain function name as the linker symbol,
so they are directly compatible with C and `extern "C"` C++ symbols out of the box. No attribute is needed for
a straightforward binding.

The `core.compiler.mangledName` attribute lets you give the declaration a different name on the Spice side while
still linking against the original C symbol:

```spice
#[core.compiler.mangledName = "pthread_self"]
ext f<byte*> pthreadSelf();
```

This declares `pthreadSelf()` in Spice but tells the linker to look for the symbol `pthread_self`, so the Spice
code can use the more idiomatic camel-case name without any runtime cost.

## Type aliases for opaque C types

Many C APIs use opaque handles—pointers to internal structs that the caller never dereferences. Model these as
`alias byte*` in Spice:

```spice
type FileHandle alias byte*;

ext f<FileHandle> fopen(string, string);
ext f<int>        fclose(FileHandle);
ext f<long>       fread(byte*, long, long, FileHandle);
ext f<long>       fwrite(byte*, long, long, FileHandle);

f<int> main() {
    FileHandle fh = fopen("data.bin", "rb");
    if fh == nil<FileHandle> {
        printf("Failed to open file\n");
        return 1;
    }
    fclose(fh);
    return 0;
}
```

The alias keeps the code readable and makes function signatures self-documenting while remaining fully compatible
with the C ABI.

## Linking an external C library

Use the module-level `core.linker.flag` attribute to pass flags to the linker. Place it at the top of any
`.spice` file in your project:

```spice
#![core.linker.flag = "-lm"]

ext f<double> sqrt(double);
ext f<double> pow(double, double);

f<int> main() {
    printf("sqrt(2) = %f\n", sqrt(2.0));
    printf("2^10 = %f\n", pow(2.0, 10.0));
    return 0;
}
```

For libraries discovered via `pkg-config`, you can embed the shell command directly in the flag value. Spice
evaluates backtick expressions at build time:

```spice
#![core.linux.linker.flag   = "`pkg-config --cflags --libs libcurl`"]
#![core.darwin.linker.flag  = "`pkg-config --cflags --libs libcurl`"]
#![core.windows.linker.flag = "-lcurl"]
```

!!! tip "Platform-specific flags"
    Use `core.linux.linker.flag`, `core.darwin.linker.flag`, and `core.windows.linker.flag` instead of the
    generic `core.linker.flag` when a library is linked differently across platforms (different flag syntax,
    different package names, DLL vs. static lib, etc.).

!!! note "Multiple flags"
    You can specify multiple linker flags in a single attribute block by separating them with commas:
    ```spice
    #![
        core.linker.flag = "-lz",
        core.linker.flag = "-lzstd",
        core.linker.flag = "-pthread"
    ]
    ```

## Exposing Spice functions to C

To call a Spice function from C, mark it `public` and disable name mangling so the C linker can find the plain
symbol name:

```spice
#[core.compiler.mangle = false]
public f<int> add(int a, int b) {
    return a + b;
}

f<int> main() {
    return 0;
}
```

Compile this to an object file and link it into your C project. On the C side, declare the function with a
matching signature:

```c
/* spice_math.h */
int add(int a, int b);
```

```c
/* main.c */
#include <stdio.h>
#include "spice_math.h"

int main(void) {
    printf("%d\n", add(3, 4));  /* prints 7 */
    return 0;
}
```

!!! note "Calling from C++"
    When the consumer is a C++ translation unit, wrap the declaration in `extern "C"` to prevent the C++
    compiler from mangling the name on its side:
    ```cpp
    extern "C" {
        int add(int a, int b);
    }
    ```

## Putting it all together

The following example ties together all four concepts to build a small libcurl wrapper that fetches a URL:

```spice
#![core.linux.linker.flag   = "`pkg-config --cflags --libs libcurl`"]
#![core.darwin.linker.flag  = "`pkg-config --cflags --libs libcurl`"]
#![core.windows.linker.flag = "-lcurl"]

type CurlHandle alias byte*;

ext f<CurlHandle> curl_easy_init();
ext f<int>        curl_easy_setopt(CurlHandle, int, ...);
ext f<int>        curl_easy_perform(CurlHandle);
ext p             curl_easy_cleanup(CurlHandle);

const int CURLOPT_URL = 10002;

f<int> main() {
    CurlHandle curl = curl_easy_init();
    if curl == nil<CurlHandle> {
        printf("curl_easy_init failed\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    int res = curl_easy_perform(curl);
    if res != 0 {
        printf("curl_easy_perform failed with code %d\n", res);
    }

    curl_easy_cleanup(curl);
    return 0;
}
```

Run it with:

```shell
spice run main.spice
```

Spice invokes `pkg-config` at build time to obtain the compiler and linker flags for libcurl, compiles the Spice
source, and links everything in a single step.

!!! note "Existing bindings"
    The Spice standard library ships ready-made bindings for several popular C libraries under `std/bindings/`,
    including `libcurl`, `GTK4`, and LLVM. Check those files for complete, production-ready examples of the
    patterns shown in this tutorial.
