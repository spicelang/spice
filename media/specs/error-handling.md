# Error Handling in Spice

## Status

Implemented and shipped, always enabled: discarding a `Result<T>` is a compile error, and error return traces are
recorded everywhere except in freestanding mode. The module attributes `core.compiler.explicitErrorHandling` and
`core.compiler.errorReturnTracing`, that gated both features while they were opt-in, are removed; using them is an
`UNKNOWN_ATTR` error.

Remaining work:

- [x] Migrate `std`'s internal `panic()`-on-recoverable-failure call sites to return `Result<T>` (#1797, #1801, #1803,
  #1804, #1805). The remaining `panic()` calls in `std` are programming errors (see `panic`'s role below).
- [x] Update docs and examples (`docs/docs/language/error-handling.md`).
- [x] Flip both attributes' defaults to `true`.
- [x] Remove the gates.

## Background

`Result<T>` (`std/runtime/result_rt.spice`) and `Error` (`std/runtime/error_rt.spice`) already existed as plain
library types before this feature: `Result<T>` wraps a `T data` and an `Error error`; `Error` is `int code` +
`string message`; `panic(Error)` is a builtin that prints to stderr and calls `exit(EXIT_FAILURE)`. Nothing
stopped a caller from dropping a `Result<T>` on the floor or ignoring `.isErr()`. This feature adds a one-token
way to propagate an error up the call stack, makes discarding a `Result<T>` a hard error, and adds a debug aid
(error return traces) — all without new syntax beyond a single reused token (postfix `!`).

## Postfix `!`: propagate or unwrap

One alternative on `postfixUnaryExpr` (`Spice.g4:78`): `postfixUnaryExpr NOT`. For an operand of type
`Result<T>`:

- The enclosing function/procedure must itself return `Result<U>`, or it's a compile error
  (`TypeChecker.visitPostfixUnaryExpr`, `src/typechecker/type-checker.spice`).
- At runtime: if `isErr()`, the enclosing function returns `err<U>(operand.getErr())` immediately (same
  early-return-with-scope-cleanup shape `panic`'s `exit` path uses, minus the `exit` call). Otherwise the
  expression evaluates to `unwrap()`'s result.

```spice
f<Result<Config>> loadConfig(string path) {
    string raw = readFile(path)!;      // propagates readFile's error as Result<Config>'s error
    return parseConfig(raw);
}
```

## `Result.unwrapOr` / `Result.unwrapOrElse`

Plain methods in `result_rt.spice`, next to `unwrap()`/`isOk()`/`isErr()`/`getErr()`:

```spice
public inline f<T&> Result.unwrapOr(const T& alternativeValue) {
    return this.error.code == 0 ? this.data : alternativeValue;
}

public inline f<T&> Result.unwrapOrElse(f<T&>(const Error&) callback) {
    return this.error.code == 0 ? this.data : callback(this.error);
}
```

```spice
Config cfg = loadConfig(path).unwrapOrElse((const Error& e) -> {
    log.warn("using defaults: " + e.message);
    return Config.default();
});

Config cfg2 = loadConfig(path).unwrapOr(Config.default());
```

## Mandatory handling

A bare, receiver-less call statement whose type is `Result<T>` is a hard error (`DISCARDED_RESULT_VALUE`,
`src/typechecker/type-checker.spice`) instead of the generic `UNUSED_RETURN_VALUE` warning, and is **not** suppressible
by `#[core.compiler.ignoreUnusedReturnValue]` (which keeps its weaker meaning for ordinary return values).

The escape hatch is Spice's existing intentionally-unused-binding convention: a leading underscore.

```spice
Result<LogEntry> _ = writeLog(msg);   // explicitly, visibly acknowledged as ignored
```

Tests: `test/test-files/typechecker/error-handling/`.

## Error return traces

Always enabled, except in freestanding mode. A debugging aid, not a correctness mechanism — in freestanding mode, no
instrumentation calls are emitted at all.

**Storage.** `ErrorTraceBuffer` (`std/runtime/error_trace_rt.spice`) holds a fixed-capacity array of
`ErrorTraceFrame { file, signature, line, column }` (`ERR_TRACE_CAPACITY = 16`) plus a frame count — a plain
buffer, not a ring buffer: once it's full, further hops are dropped rather than overwriting earlier ones (see
below). `signature` is the enclosing function/procedure's full signature (fully-qualified name plus parameter and
return types, e.g. `f<Result<double>> TestStruct.frame0(bool)`), produced via `Function.getSignature()` for the
manifestation that is actually being generated — not just its bare name. It is *not* part of
`Result<T>`'s LLVM layout — that layout is shared across every file that instantiates `Result<T>` for a given
`T`, so a hidden field, that only exists while tracing is enabled, would risk two files silently disagreeing on the type's
size (an ABI mismatch, not a compile error). Instead, a compiler builtin, `__err_trace_buffer()`, hands back a
reference to a thread-local instance (a genuine LLVM `thread_local` global, lazily created per module). It is
not attached to any specific `Result<T>` value.

**Population.** `sErrTraceReset()`/`sErrTracePush()` are compiler-injected calls: the former at every
`err<T>(...)` call site (origin, frame 0), the latter at every propagating postfix `!` (hop). Both keep normal
name mangling — `StdFunctionManager` reconstructs their mangled names the same way it does for
`sAllocUnsafe`/`sDealloc`, rather than declaring them unmangled. Once the buffer is at capacity, further hops
are silently dropped, keeping the origin and earliest hops rather than the most recent ones.

**Consumption.** `Error.dumpTrace()`/`Result.dumpTrace()` print every recorded frame to stderr via
`fprintf(__stderr(), ...)` — `__stderr()` is a niladic builtin exposing the real libc `stderr` `FILE*`, since
Spice has no syntax for declaring an external variable. `panic()` calls the same dump automatically right after
its own message. Both are safe no-ops when nothing was ever recorded. After it, `panic()` prints the stack trace too
(see `media/specs/stack-traces.md`).

**Known trade-offs:**

- The trace reflects the most recently active chain *on the current thread*, not a specific `Result<T>` value:
  calling `dumpTrace()` on an unrelated, successful `Result` can print a stale trace from an earlier failure on
  the same thread (test: `result-err-trace-global-not-per-value`). Different threads never see each other's
  traces (test: `result-err-trace-thread-isolated`).
- Only `err<T>(...)` calls are recognized as an origin; constructing a `Result<T>` directly via
  `Result<T>(someError)` records no origin at all.

## `panic`'s role

`panic(Error)` keeps its existing signature and behavior (now trace-augmented, except in freestanding mode). Its role narrows to
assertion-like, programmer-error situations — not the default reaction to a recoverable failure such as a
missing file or a parse error. `std`'s recoverable-failure call sites return `Result<T>`; where a panicking lookup is
still useful, a `Result<T>`-returning variant with the `Safe` suffix sits next to it (e.g. `getField()`/
`getFieldSafe()`).

## Open questions

- **OQ-1**: Should `main` be allowed to return `Result<int>`? `mainFunctionDef` still pins the return type to
  `TYPE_INT` (`Spice.g4:7`). Letting `main` propagate a top-level error (printed, then a non-zero exit) is a
  small, separate grammar change.
- **OQ-2**: Custom/domain error types. `Error` is a single concrete struct today. A plausible extension is an
  `Error` *interface* that user structs can attach to, with `Result<T>` holding an interface reference instead
  of the concrete struct.
- **OQ-3**: Does mandatory handling produce annoying boilerplate at call sites that already fully handle the
  error through some other means? Worth revisiting once `std`'s internals are migrated (real-world call sites
  to check against).
- **OQ-4**: Error return tracing is enabled in all build modes. Should release builds disable it (mirroring Zig's
  Debug/ReleaseSafe-on split)?
