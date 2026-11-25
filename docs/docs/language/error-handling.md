---
title: Error handling
---

Spice has no exceptions. A function, that can fail for reasons outside of the program's control, e.g. a missing file, a
network error or invalid input, returns a [`Result<T>`](builtin-types.md#the-result-data-type). It holds either a value
of type `T` or an [`Error`](builtin-types.md#the-error-data-type). Programming errors, like an index out of bounds,
end the program with a [`panic`](builtins.md#the-panic-builtin) instead.

## Returning errors
A function, that can fail, declares `Result<T>` as its return type. It returns its value with `ok()` and an error with
`err<T>()`:

```spice
f<Result<double>> safeDivide(double dividend, double divisor) {
    if divisor == 0.0 {
        return err<double>("Division by zero");
    }
    return ok(dividend / divisor);
}
```

`err<T>()` accepts an error message, an error code and a message, or an `Error` object.

## Handling errors
The caller checks the result with `isOk()` or `isErr()` and retrieves the value with `unwrap()` or the error with
`getErr()`. `unwrap()` panics, if the result holds an error:

<!-- doc-test: skip uses safeDivide of the code block above -->
```spice
f<int> main() {
    Result<double> quotient = safeDivide(10.0, 0.0);
    if quotient.isErr() {
        printf("Error: %s\n", quotient.getErr().getMessage());
        return 1;
    }
    printf("Quotient: %f\n", quotient.unwrap());
}
```

If a fallback value is fine, `unwrapOr()` returns the value or the given alternative, and `unwrapOrElse()` calls the
given function with the error to compute the alternative:

<!-- doc-test: skip uses safeDivide of the code block above -->
```spice
f<int> main() {
    Result<double> quotient = safeDivide(10.0, 0.0);
    double value = quotient.unwrapOr(0.0);
    printf("Quotient: %f\n", value); // Quotient: 0.000000
}
```

## Propagating errors
Often, a function can only pass an error on to its caller. The postfix `!` operator does this in one step: applied to a
`Result<T>`, it returns the error from the enclosing function right away, if there is one. Otherwise, it evaluates to
the value of the result:

```spice
f<Result<double>> average(double sum, double count) {
    double avg = safeDivide(sum, count)!; // Returns the error of safeDivide, if it fails
    return ok(avg);
}
```

The `!` operator can only be used in a function, that returns a `Result<U>` itself. The error is passed on as is, even
if `U` differs from `T`. The `main` function returns `int`, so it has to handle errors explicitly.

## Mandatory error handling
Discarding a `Result<T>`, that a call returned, is a compile error:

<!-- doc-test: expect-error Discarded Result value -->
```spice
import "std/io/file";

f<int> main() {
    writeFile("./log.txt", "Started"); // Error: A Result<T> returned from a call must be handled
}
```

To fix this, bind the result to a variable, propagate it with `!` or handle it with `unwrapOr()`/`unwrapOrElse()`. A
result, that is ignored on purpose, can be bound to a variable, whose name starts with an underscore:

```spice
import "std/io/file";

f<int> main() {
    Result<bool> _ = writeFile("./log.txt", "Started"); // Ignored on purpose
}
```

Unlike the warning about unused return values, this error cannot be suppressed with the `ignoreUnusedReturnValue`
function attribute.

## Error return traces
To find out, where an error came from and how it got to the place, where it is handled, Spice records an error return
trace: every `err<T>()` or `Result<T>(error)` call records the origin of an error and every `!` records a hop on the way
up. `dumpTrace()` on an `Error` or `Result<T>` prints the error and the recorded trace to stderr:

```spice
f<Result<int>> readPort(bool valid) {
    if !valid {
        return err<int>("Invalid port");
    }
    return ok(8080);
}

f<Result<int>> loadConfig(bool valid) {
    int port = readPort(valid)!;
    return ok(port);
}

f<int> main() {
    Result<int> config = loadConfig(false);
    if config.isErr() {
        config.dumpTrace();
        return 1;
    }
}
```

Output:

```text
Error code 1: Invalid port
Recorded error trace:
  #0  f<Result<int>> readPort(bool) at ./trace.spice:3:16
  #1  f<Result<int>> loadConfig(bool) at ./trace.spice:9:16
```

A `panic` prints the recorded trace as well, e.g. when `unwrap()` is called on a failed result. Some limits apply:

- The trace belongs to the current thread, not to a specific result. It shows the most recent chain of errors on the
  thread, so `dumpTrace()` on an unrelated result can print the trace of an earlier error.
- A trace holds up to 16 frames. Further hops are dropped, keeping the origin and the earliest hops.
- Error return tracing is disabled in release builds (`--build-mode=release`) and in
  [freestanding mode](../how-to/freestanding.md).

## Result or panic
The standard library returns a `Result<T>` for failures, that depend on the input or the environment, and panics for
programming errors:

- **Result:** File and directory access, network and DNS, processes, parsers (JSON, XML, YAML, TOML, regex, base64),
  `Thread.run()`/`Thread.join()`, `ThreadPool.start()` and `MessageQueue.tryPush()`.
- **Panic:** Accessing an index out of bounds, popping from an empty container, using an invalid iterator, calling
  `unwrap()` on a failed result and other violated preconditions.

Where both are useful, a lookup comes in two variants: the plain one panics, if the key does not exist, while the one
with the `Safe` suffix returns a `Result<T>`, e.g. `HashTable.get()`/`getSafe()`, `JsonValue.getField()`/`getFieldSafe()`
and `XmlNode.getAttribute()`/`getAttributeSafe()`.

The conversions from strings to numbers in `std/type/type-conversion` follow the same convention: `toInt()`, `toShort()`,
`toLong()`, `toByte()` and `toDouble()` parse the longest numeric prefix of the input like `strtoll`/`strtod` and give 0
for input, that is no number, while `toIntSafe()`, `toShortSafe()`, `toLongSafe()`, `toByteSafe()` and `toDoubleSafe()`
return an error for empty input, leading whitespace, trailing characters and values out of the range of the target type:

```spice
import "std/type/type-conversion";

f<int> main() {
    printf("%d\n", toInt("3000000000")); // -1294967296, wrapped around
    Result<int> port = toIntSafe("3000000000");
    if port.isErr() {
        printf("%s\n", port.getErr().getMessage()); // Value out of range
    }
    printf("%d\n", toIntSafe("ff", 16).unwrap()); // 255
}
```

When a result holds a heap payload, see [ownership](builtin-types.md#ownership) for how to take it over.
