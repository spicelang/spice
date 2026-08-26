# Technical Specification for Explicit, Mandatory Error Handling in Spice

## Status

Design proposal. Not yet implemented. This revision deliberately minimizes new syntax: rather than importing
Zig's sigils/keywords (`!T`, `try`, `catch |err|`) wholesale, every piece below is either an existing Spice
construct used as-is, or the smallest possible extension of one that already exists in the grammar.

## Implementation steps

- [x] Add postfix `!` (propagation) to the `postfixUnaryExpr` grammar rule
- [x] Update IntelliJ plugin
- [x] Type-check postfix `!`: operand must be `Result<T>`-shaped, enclosing function must return `Result<U>`
- [x] IR-gen postfix `!`: branch on `isErr()`, early-return-with-error or unwrap
- [x] Add `Result.unwrapOr(const T&)` and `Result.unwrapOrElse(f<T>(Error))` to `result_rt.spice`
- [x] Add tests for postfix `!` and the new `Result` methods
- [ ] Upgrade the discarded-`Result` case of `UNUSED_RETURN_VALUE` from warning to hard error, gated by a module attribute
- [ ] Add tests for the new diagnostic (on/off)
- [ ] Add a synthesized, conditionally-present trace field to `Result<T>`'s LLVM layout, gated by a module attribute
- [ ] Emit trace-frame-recording code at `err<T>(...)`/error-`Result.ctor` (origin) and at each propagating postfix `!` (hop)
- [ ] Add `Error`/`Result` trace-dumping support and wire it into `panic`'s existing stderr path
- [ ] Add tests for error return traces (depth truncation, on/off, trace surviving `unwrapOr`/`unwrapOrElse` correctly not doing so)
- [ ] Migrate `std`'s internal `panic()`-on-recoverable-failure call sites to return `Result<T>`
- [ ] Update docs, examples, changelog
- [ ] Flip the module attribute's default to on (breaking change, major version)
- [ ] Remove the gate

## Motivation

Today, error handling in Spice is **entirely a library convention with no compiler backing**:

- `Result<T>` (`std/runtime/result_rt.spice`) is a plain generic struct holding `T data` and `Error error`.
  `unwrap()` panics if `error.code != 0`; `isOk()`/`isErr()`/`getErr()` are just field checks.
- `Error` (`std/runtime/error_rt.spice`) is a plain struct: `int code` + `string message`.
- `panic(Error)` is a builtin (`TypeChecker::visitBuiltinPanicCall`, `IRGenerator::visitBuiltinPanicCall`) that
  prints to stderr and calls `exit(EXIT_FAILURE)`.

Nothing stops a caller from calling a `Result<T>`-returning function and dropping the result as a bare
expression statement, or from ignoring `.isErr()` before ever touching `.unwrap()`'s payload. The type checker
already has a related, but generic and non-mandatory, mechanism: `UNUSED_RETURN_VALUE` is a *warning* emitted
for any discarded call return value (`TypeCheckerValues.cpp:279-291`), suppressible per callee-function via
`#[core.compiler.ignoreUnusedReturnValue]` (`Attributes.h:29`). It doesn't know or care whether the discarded
value carries an error.

Goal: make it impossible to drop a fallible call's result on the floor by accident, and make propagating an
error up the call stack a one-token operation — without introducing constructs foreign to Spice's existing
grammar and standard-library shape.

## Survey of what Spice already has to build on

This proposal leans on five things that already exist, unchanged:

1. **`Result<T>` itself.** Generic instantiation with `<T>` is already the idiom everywhere (`Vector<T>`,
   `max<T>`, `Result<T>`). There is no need for a new `!T` type sigil — a fallible function simply returns
   `Result<T>`, exactly as it does today.
2. **Postfix sigils on `postfixUnaryExpr`.** The grammar rule already reads
   `postfixUnaryExpr (LBRACKET assignExpr RBRACKET | DOT IDENTIFIER | PLUS_PLUS | MINUS_MINUS)`
   (`Spice.g4:78`) — i.e. it already hosts multiple single-token postfix operators on an expression. Adding one
   more alternative is a one-line grammar change, not a new category of syntax.
3. **Token reuse by grammar position.** Spice already gives `MUL` and `BITWISE_AND` different meanings as an
   infix operator (`*`, `&`), a prefix unary operator (dereference, address-of, `Spice.g4:77`), and a postfix
   modifier on `dataType` (pointer/reference type, `Spice.g4:92`). Reusing `NOT` (`!`) — today only a prefix
   boolean-negation operator — in a new postfix-on-expression position follows the same, already-established
   pattern rather than introducing a new token.
4. **First-class lambdas and the underscore-discard convention.** Spice already has lambda literals
   (`lambdaExpr`, `lambdaFunc`, `Spice.g4:87-89`, e.g. `(int x) -> x + 1`) and already treats any identifier
   starting with `_` as an intentionally-unused binding that is exempt from the `UNUSED_VARIABLE` warning
   (`UNUSED_VARIABLE_NAME = '_'` in `SymbolTableBuilder.h:21`, checked in `Scope.cpp:153`). Both are reused
   below instead of inventing pipe-bound closure syntax (`catch |err|`) or a new discard token (`_ = expr;`).
5. **Compile-time source-location builtins, and conditionally-synthesized hidden struct fields.**
   `__source_file()`/`__source_line()`/`__source_column()` (`TypeCheckerBuiltinFunctions.cpp:581-605`) already
   resolve to their own call site's location as compile-time constants — no runtime cost, no debug-info lookup,
   folded away at type-checking time exactly like C's `__FILE__`/`__LINE__`. Separately, struct layouts are
   already conditionally extended by the compiler itself: a struct that implements an interface gets a hidden
   vtable-pointer field prepended to its LLVM layout that never appears in its `.spice` source, gated by
   `Struct::hasSynthesizedVTablePtr()` (`Struct.cpp:59`) and accounted for wherever field offsets are computed
   (`DebugInfoGenerator.cpp:354`, `Type.cpp:554`). Both are reused in the trace design below, instead of
   inventing return-address capture plus debug-info symbolization the way Zig's mechanism needs to.

## Design

### 1. Fallible functions: still just `Result<T>`

No new type syntax. A function that can fail returns `Result<T>`, exactly as today:

```spice
f<Result<Config>> loadConfig(string path) { /*...*/ }
```

### 2. Postfix `!`: propagate-or-unwrap

Add one alternative to `postfixUnaryExpr` (`Spice.g4:78`): `postfixUnaryExpr NOT`. Semantics, for an operand of
type `Result<T>`:

- The enclosing function/procedure must itself return `Result<U>` for some `U` — otherwise a compile error
  (there is nowhere for the propagated error to go). This mirrors how `PLUS_PLUS`/`MINUS_MINUS` already require
  their operand to be an lvalue of a numeric type — a postfix operator with a precondition on its context is
  already how this grammar rule works.
- At runtime: if the operand's `isErr()` is true, immediately return that `Error` wrapped as `Result<U>` from
  the enclosing function (same early-return-with-scope-cleanup shape already used by `panic`'s `exit` path,
  `GenBuiltinFunctions.cpp:141-149`, minus the `exit` call). Otherwise, the expression evaluates to `unwrap()`'s
  result (`T`).

```spice
f<Result<Config>> loadConfig(string path) {
    string raw = readFile(path)!;      // propagates readFile's error as Result<Config>'s error
    return parseConfig(raw);
}
```

This is the one genuinely new piece of grammar in this proposal — there is no existing Spice construct for
"early-return-with-transformation" — but it is expressed as a single reused token in an existing rule, not a
new keyword or sigil family.

### 3. Inline handling: plain `Result<T>` methods, using existing lambda syntax

No new binary operator, no new binding syntax. Add two methods to `result_rt.spice`, next to the existing
`unwrap()`/`isOk()`/`isErr()`/`getErr()`:

```spice
public inline f<T> Result.unwrapOr(const T& fallback) {
    return this.isOk() ? this.data : fallback;
}

public f<T> Result.unwrapOrElse(f<T>(Error) handler) {
    return this.isOk() ? this.data : handler(this.error);
}
```

(`unwrapOr` is expressible with the existing ternary, `Spice.g4:65`, today. `unwrapOrElse` takes a function
value with Spice's existing function-typed-parameter and lambda syntax — no grammar change at all.)

```spice
Config cfg = loadConfig(path).unwrapOrElse((Error e) -> {
    log.warn("using defaults: " + e.message);
    return Config.default();
});

Config cfg2 = loadConfig(path).unwrapOr(Config.default());
```

This covers the same use case as Zig's `catch |err| { ... }`, but as ordinary generic-struct methods taking an
ordinary lambda — both already-existing constructs — rather than a new operator with new binding syntax.

### 4. Mandatory handling

Extend the existing discarded-return-value check (`TypeCheckerValues.cpp:279-291`) with a branch specific to
`Result<T>`-shaped return types: a bare, receiver-less call statement whose type is `Result<T>` becomes a
**hard error**, not the generic `UNUSED_RETURN_VALUE` warning, and is not suppressible by
`ignoreUnusedReturnValue` (which keeps its current, weaker meaning for ordinary return values). The caller is
forced to bind the result to a variable.

The escape hatch is the convention Spice already has for "I am intentionally not using this binding": name it
with a leading underscore, exactly as any other intentionally-unused local does today
(`Scope.cpp:153`) — no new token:

```spice
Result<LogEntry> _ = writeLog(msg);   // explicitly, visibly acknowledged as ignored
```

### 5. Optional error return trace instrumentation

Modeled on Zig's error return traces (see prior discussion), but built entirely out of the two existing
mechanisms in survey point 5 above, rather than Zig's return-address-plus-debug-info approach — Spice can skip
runtime symbolization altogether, because every instrumentation point here is a statically-known source
location at compile time, not a raw address resolved later.

**Gate.** A new module attribute, `#![core.compiler.errorReturnTracing = true]` (same mechanism as
`alwaysKeepOnNameCollision`, `result_rt.spice:1`), defaulting to `false`. A second, optional attribute,
`#![core.compiler.errorReturnTraceDepth = N]`, controls the fixed capacity (default e.g. `8`).

**Storage.** When the attribute is enabled for a compilation, `Result<T>`'s LLVM struct layout gains one
hidden, compiler-synthesized trailing field — `{ i8 count; [N x ptr] frames }` — exactly the same kind of
compiler-only, invisible-in-`.spice`-source field already used for the vtable pointer (survey point 5). When
the attribute is disabled, the field does not exist at all: `Result<T>`'s layout is byte-for-byte what it is
today, and none of the instrumentation below is emitted — genuinely zero cost, not just "cost skipped at
runtime," matching how Zig strips this in ReleaseFast/ReleaseSmall builds.

Each `frames[i]` is a pointer to a global constant string baked at compile time, using the same
`CreateGlobalString` mechanism `panic`'s message template already uses (`GenBuiltinFunctions.cpp:121-123`) and
the same `codeLoc`-derived formatting `__source_file()` already produces (`TypeCheckerBuiltinFunctions.cpp:585`)
— e.g. `"config.spice:42:18"`. No raw addresses, no unwinding, no reliance on the binary carrying debug info at
all.

**Population.**
- *Origin*: wherever a `Result<T>` is first put into the error state — the error-`Result.ctor`
  (`result_rt.spice:30`) and the `err<T>(...)` free functions (`result_rt.spice:77-92`). Since these are
  ordinary `std`-authored functions, not compiler builtins, they need the *caller's* location, not their own —
  the same caller-location need `__source_file`/`__source_line`/`__source_column` already satisfy for their own
  call site. This proposal adds one more builtin of that family, `__source_loc()`, returning an opaque,
  compile-time-baked location handle, for the runtime implementation of `err<T>(...)` to record as frame 0.
- *Propagation*: every postfix `!` site, at the point it observes `isErr() == true` and before performing its
  early-return, appends its own `__source_loc()`-equivalent frame into the `Result` value it is about to
  return (in place, before the value is copied/moved into the caller's return slot). Once `count` reaches the
  configured depth, further frames are dropped and `count` is left at the capacity (truncate at the oldest end
  of the chain, i.e. keep the origin and the most recent hops — mirrors Zig's fixed-capacity behavior).

**Consumption.** `Error`/`Result` gain a `dumpTrace()` method that, when the hidden field exists, prints each
recorded frame to stderr; when it doesn't exist (attribute disabled), the method compiles to a no-op rather
than a compile error — mirroring how Zig's `@errorReturnTrace()` simply returns `null` when tracing is off,
rather than making calling code conditional on a build flag. `Error.toPanic()` (`error_rt.spice:50`) and the
`panic` builtin's existing stderr dump (`GenBuiltinFunctions.cpp:100-151`) are the natural place to also print
the trace when one is present and non-empty, immediately after the error message.

```spice
f<Result<Config>> loadConfig(string path) {
    string raw = readFile(path)!;   // on error, records this line as a hop and returns
    return parseConfig(raw);
}

// with #![core.compiler.errorReturnTracing = true] and readFile failing:
//   Program panicked at main.spice:10: file not found
//   error return trace:
//     io.spice:88:12       <- origin (inside readFile, where the error was constructed)
//     config.spice:42:18   <- propagated through loadConfig's `!`
//     main.spice:10:15     <- propagated through main's `!`
```

### 6. `panic` stays, with a narrowed role

`panic(Error)` keeps its current signature and runtime behavior (now optionally trace-augmented, per above).
Its role narrows to assertion-like, programmer-error, "this should be provably impossible" situations
(mirroring the existing `assertStmt`) — not the default reaction to a recoverable failure such as a missing
file or a parse error. `std` migrates its own internal uses of `panic` on recoverable paths to `Result<T>`
returns (Migration Phase 5 below).

## Compatibility strategy

Sections 1-3 above are pure additions: nothing that compiles today stops compiling, and `Result<T>` itself is
untouched. Section 5 (error return traces) is also purely additive and ships opt-in behind its own gate, with
no intention of ever becoming unconditional the way mandatory handling does (see OQ-4) — it is a debug aid, not
a language rule. Section 4 (mandatory handling) is the one change with a real compatibility story: it can break
code that silently drops a `Result`. To keep every phase shippable and non-breaking until explicitly intended:

- Ship the hard-error upgrade behind a module attribute, e.g.
  `#![core.compiler.explicitErrorHandling = true]` (the same mechanism `result_rt.spice:1` already uses for
  `alwaysKeepOnNameCollision`), defaulting to `false`.
- Once `std` and the test suite are migrated and the feature has soaked, flip the default to `true` in a
  documented major/minor version bump, then eventually remove the gate.

## Step-by-step migration plan

Each phase is independently buildable/testable.

**Phase 1 — postfix `!`.**
Grammar: add the `postfixUnaryExpr NOT` alternative (`Spice.g4:78`). Type checker: validate operand is
`Result<T>`-shaped and enclosing function returns `Result<U>`; unify `T`→ propagated-error-as-`U` conversion.
IR generator: `isErr()` branch + early-return-with-error, patterned after the existing early-return/cleanup
handling already used for `panic` (`GenBuiltinFunctions.cpp:141-149`) minus the `exit` call. Update the
IntelliJ plugin grammar/highlighting. Tests: propagation across nested calls, through generic functions, mixed
with existing hand-written `Result<T>` code that never uses `!`.

**Phase 2 — `Result.unwrapOr` / `Result.unwrapOrElse`.**
Pure `std` addition to `result_rt.spice`; no compiler changes. Tests for both, including a diverging
`unwrapOrElse` lambda body (`return`/`panic` inside the handler).

**Phase 3 — error return trace instrumentation, opt-in.**
Add the `__source_loc()` builtin alongside the existing `__source_file`/`__source_line`/`__source_column`
family. Add the module attribute gate(s). Extend `Struct`'s synthesized-field mechanism (the one already
backing `hasSynthesizedVTablePtr()`, `Struct.cpp:59`) to conditionally append the trace field to `Result<T>`'s
LLVM layout, updating the same offset-computation sites that already account for the vtable pointer
(`DebugInfoGenerator.cpp:354`, `Type.cpp:554`). Emit frame-recording at the `err<T>(...)`/error-`Result.ctor`
origin and at each postfix-`!` hop. Add `dumpTrace()` and wire it into `panic`'s stderr path. This phase is
independent of mandatory handling and can ship before or after Phase 4. Tests: origin-only trace, multi-hop
trace, depth truncation, attribute off (field absent, `dumpTrace()` a no-op).

**Phase 4 — mandatory handling, opt-in.**
Add the module attribute gate. Add the `Result<T>`-aware branch next to `UNUSED_RETURN_VALUE`
(`TypeCheckerValues.cpp:279-291`) that hard-errors instead of warning when the attribute is set, pointing users
at postfix `!`, `.unwrapOr(...)`/`.unwrapOrElse(...)`, or binding to an `_`-prefixed variable. Tests: one file
per attribute state (on/off) confirming the same source either errors or is silently accepted.

**Phase 5 — migrate `std`.**
Audit `std/` for internal `panic()` calls on recoverable conditions (I/O, parsing — excluding true invariant
violations) and convert their public signatures to `Result<T>`, updating internal call sites to use postfix
`!`. Land function-by-function with tests, not as one sweep, since this is where real design gaps (e.g. "does
this failure even have a sensible `Error` to construct?") are most likely to surface.

**Phase 6 — docs & examples.**
Update `docs/`, `STYLE_GUIDE.md`, and example `.spice` files to recommend `Result<T>` + postfix `!` +
`.unwrapOr(...)`/`.unwrapOrElse(...)` as the idiom, with direct `.isOk()`/`.isErr()`/`.unwrap()`/`.getErr()`
documented as the still-valid lower-level mechanism everything above sugars over.

**Phase 7 — flip the mandatory-handling default, remove that gate (major version).**
Change the mandatory-handling module attribute's default to `true`; after a deprecation window, remove the
attribute and the old silently-discardable code path, making mandatory handling unconditional. Update
CHANGELOG with migration notes (mechanical fix in each case: add `!`, `.unwrapOr(...)`/`.unwrapOrElse(...)`, or
bind to `_...`). The error-return-tracing attribute from Phase 3 is a separate, orthogonal knob (a debug aid,
not a correctness rule) and is not implied by this phase — see OQ-4.

## Testing plan

Following the existing reference-test convention (`spice-add-test` skill / `test/test-files/`): a grouped suite
`error-handling/` with subdirectories `postfix-propagation/`, `unwrap-or/`, `unwrap-or-else/`, `error-trace/`,
`mandatory-handling/` (positive cases plus `exception.out`-matched negative cases for the new diagnostic).
Unit tests in `test/unittest/` for the postfix-`!` type checks, the new `Result` methods' codegen, and the
synthesized trace field's offset computation.

## Open questions

- **OQ-1**: Should `main` be allowed to return `Result<int>`? `mainFunctionDef` currently pins the return type
  to `TYPE_INT` (`F LESS TYPE_INT GREATER MAIN`), not general `dataType`. Letting `main` propagate a top-level
  error (printed, then a non-zero exit) is a small, separate grammar change worth doing alongside Phase 1 but
  not required for it.
- **OQ-2**: Custom/domain error types. This proposal keeps `Error` a single concrete struct (code + message).
  A plausible future extension is an `Error` *interface* (structural, like the existing `interfaces.md`
  mechanism) that user structs can attach to, with `Result<T>` holding an `Error` interface reference instead
  of the concrete struct. Deferred out of this proposal to keep scope contained.
- **OQ-3**: Does forcing a bind-or-underscore for every discarded `Result<T>` (Phase 4) ever produce annoying
  boilerplate at call sites that already fully handle the error through some other means (e.g. a `switch` over
  `.getErr().code` performed as part of a larger expression)? Worth revisiting once Phase 5 (migrating `std`)
  produces real-world call sites to check against.
- **OQ-4**: Should `errorReturnTracing` default to `true` in debug builds specifically (mirroring Zig's
  Debug/ReleaseSafe-on, ReleaseFast/ReleaseSmall-off split) rather than always defaulting to `false` until a
  user opts in? That would tie the attribute's default to the CLI's existing build-mode flag rather than to the
  module attribute system alone, which is a slightly different mechanism than anything else in this proposal
  and needs its own small design pass.
- **OQ-5**: What should happen to an already-recorded trace when a `Result<T>` in the error state is consumed
  by `.unwrapOr(...)`/`.unwrapOrElse(...)` and a *fresh* error is constructed inside the handler (as opposed to
  the fallback value)? The new error should get its own fresh origin frame, not inherit the old trace — worth
  an explicit test in Phase 3 rather than leaving it to fall out of the implementation accidentally.
