---
title: Leveraging Spice WebAssembly capabilities
---

Due to the fact, that Spice uses LLVM as compiler backbone, it is capable of cross-compiling to many different target
architectures which LLVM provides backends for. LLVM also supports compiling to WebAssembly and so does Spice.
This guide shows you how to use Spice as programming language underneath your web applications, using WebAssembly.

## Prerequisites

For WebAssembly compilation and linkage, a WebAssembly linker is required. Spice uses the `wasm-link` linker, shipped with Clang
per default. This means, you need Clang and `wasm-link` installed on your system. On Linux, you can simply install the Clang
package. On Windows you can download MinGW64 with Clang from e.g. [winlibs.com](https://winlibs.com).

## Writing the program

For the sake of example, we write a recursive fibonacci algorithm in Spice, which we can make accessible to JavaScript.

```spice
public f<int> fibo(int n) {
    if n <= 1 { return n; }
    return fibo(n - 1) + fibo(n - 2);
}

f<int> main() {
    printf("%d", fibo(45));
}
```

You can test, if the code works by compiling and running it, using the following command: <br>
`$ spice run main.spice`

Spice will not emit the `main` function when compiling to WebAssembly, so you may delete it now. The important part is to mark all
functions as public, which should be accessible from JavaScript later on. Please also note, that you cannot link any external
functions from the C standard library into your WebAssembly program, because it is required to be portable to any system. The
`wasm-link` linker will complain if you try to use any external function when trying to compile to `wasm32` or `wasm64`.

## Cross-compile to wasm32

Now execute the following command to compile the code to an output `.wasm` file: <br>
`$ spice build -O3 --no-entry --target-arch=wasm32 main.spice`

- `-O3` enables the highest possible optimization. You might also want to use `-Oz` instead to optimize for binary size.
- `--no-entry` makes Spice not generate code for the `main` function. This also lifts the requirement for `main`
- `--target-arch=wasm32` sets the cross-compilation target to WebAssembly. Another valid option is `wasm64`, which is currently not
   fully supported by browser engines as well as LLVM

If you have installed wabt (the WebAssembly binary toolkit), you can view a textual form of your WebAssembly binary like this: <br>
`$ wasm-objdump -x main.wasm`

## Setup WebAssembly project

Create an `index.html` file in the same directory, where the `main.wasm` file lives with the following contents:

```html
<!DOCTYPE html>
<html lang="en-US">
<head>
    <title>Spice WebAssembly Demo</title>
    <meta charset="UTF-8">
    <script src="index.js"></script>
</head>
<body></body>
</html>
```

Also create an `index.js` file:

```js
const fiboBase = 45;

WebAssembly.instantiateStreaming(fetch('main.wasm'))
  .then((results) => {
    // WebAssembly
    const startWasm = window.performance.now();
    let result = results.instance.exports._Z4fiboi(fiboBase); // Use the mangled name here
    const stopWasm = window.performance.now();
    console.log("Fibonacci Spice wasm: " + result);
    console.log("Duration (millis): " + (stopWasm - startWasm));
    // JS
    const startJS = window.performance.now();
    result = fibo(fiboBase);
    const stopJS = window.performance.now();
    console.log("Fibonacci JS: " + result);
    console.log("Duration (millis): " + (stopJS - startJS));
  });

function fibo(n) {
  if (n <= 1) { return n; }
  return fibo(n - 1) + fibo(n - 2);
}
```

As you can see, you can load the `main.wasm` file in JavaScript using the WebAssembly module. When successful, we can call all
exposed Spice functions using their mangled names. If you don't know the mangled name of your function, you can either use
`wasm-objdump` like described above or add the flag `-asm` to the compile command to dump the assembly. There you will find the
mangled names of all functions.

## Execute

Now you can use e.g. the node `http-server` to spin up the web project locally:
```shell
npm i -g http-server
http-server .
```

Navigate to `http://127.0.0.1:8080` and open the developer console. After waiting a bit, you should see something like this:

```
Fibonacci Spice wasm: 1134903170
Duration (millis): 5346.60000000149
Fibonacci JS: 1134903170
Duration (millis): 8946.19999999553
```

Congrats on your first WebAssembly project in Spice!