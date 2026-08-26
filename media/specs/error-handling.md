# Technical Specification for Explicit, Mandatory Error Handling in Spice

## Status

Design proposal. Not yet implemented. This revision deliberately minimizes new syntax: rather than importing
Zig's sigils/keywords (`!T`, `try`, `catch |err|`) wholesale, every piece below is either an existing Spice
construct used as-is, or the smallest possible extension of one that already exists in the grammar.

## Implementation steps

- [ ] Add postfix `!` (propagation) to the `postfixUnaryExpr` grammar rule
- [ ] Update IntelliJ plugin
- [ ] Type-check postfix `!`: operand must be `Result<T>`-shaped, enclosing function must return `Result<U>`
- [ ] IR-gen postfix `!`: branch on `isErr()`, early-return-with-error or unwrap
- [ ] Add `Result.unwrapOr(const T&)` and `Result.unwrapOrElse(f<T>(Error))` to `result_rt.spice`
- [ ] Add tests for postfix `!` and the new `Result` methods
- [ ] Upgrade the discarded-`Result` case of `UNUSED_RETURN_VALUE` from warning to hard error, gated by a module attribute
- [ ] Add tests for the new diagnostic (on/off)
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

This proposal leans on four things that already exist, unchanged:

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

## Design

### 1. Fallible functions: still just `Result<T>`

No new type syntax. A function that can fail returns `Result<T>`, exactly as today:

```spice
f<Result<Config>> loadConfig(string path) { ... }
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

### 5. `panic` stays, with a narrowed role

`panic(Error)` keeps its current signature and runtime behavior. Its role narrows to assertion-like,
programmer-error, "this should be provably impossible" situations (mirroring the existing `assertStmt`) — not
the default reaction to a recoverable failure such as a missing file or a parse error. `std` migrates its own
internal uses of `panic` on recoverable paths to `Result<T>` returns (Migration Phase 3 below).

## Compatibility strategy

Sections 1-3 above are pure additions: nothing that compiles today stops compiling, and `Result<T>` itself is
untouched. Section 4 (mandatory handling) is the one behavior change that can break code that silently drops a
`Result`. To keep every phase shippable and non-breaking until explicitly intended:

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

**Phase 3 — mandatory handling, opt-in.**
Add the module attribute gate. Add the `Result<T>`-aware branch next to `UNUSED_RETURN_VALUE`
(`TypeCheckerValues.cpp:279-291`) that hard-errors instead of warning when the attribute is set, pointing users
at postfix `!`, `.unwrapOr(...)`/`.unwrapOrElse(...)`, or binding to an `_`-prefixed variable. Tests: one file
per attribute state (on/off) confirming the same source either errors or is silently accepted.

**Phase 4 — migrate `std`.**
Audit `std/` for internal `panic()` calls on recoverable conditions (I/O, parsing — excluding true invariant
violations) and convert their public signatures to `Result<T>`, updating internal call sites to use postfix
`!`. Land function-by-function with tests, not as one sweep, since this is where real design gaps (e.g. "does
this failure even have a sensible `Error` to construct?") are most likely to surface.

**Phase 5 — docs & examples.**
Update `docs/`, `STYLE_GUIDE.md`, and example `.spice` files to recommend `Result<T>` + postfix `!` +
`.unwrapOr(...)`/`.unwrapOrElse(...)` as the idiom, with direct `.isOk()`/`.isErr()`/`.unwrap()`/`.getErr()`
documented as the still-valid lower-level mechanism everything above sugars over.

**Phase 6 — flip default, remove gate (major version).**
Change the module attribute's default to `true`; after a deprecation window, remove the attribute and the old
silently-discardable code path, making mandatory handling unconditional. Update CHANGELOG with migration notes
(mechanical fix in each case: add `!`, `.unwrapOr(...)`/`.unwrapOrElse(...)`, or bind to `_...`).

## Testing plan

Following the existing reference-test convention (`spice-add-test` skill / `test/test-files/`): a grouped suite
`error-handling/` with subdirectories `postfix-propagation/`, `unwrap-or/`, `unwrap-or-else/`,
`mandatory-handling/` (positive cases plus `exception.out`-matched negative cases for the new diagnostic).
Unit tests in `test/unittest/` for the postfix-`!` type checks and the new `Result` methods' codegen.

## Open questions

- **OQ-1**: Should `main` be allowed to return `Result<int>`? `mainFunctionDef` currently pins the return type
  to `TYPE_INT` (`F LESS TYPE_INT GREATER MAIN`), not general `dataType`. Letting `main` propagate a top-level
  error (printed, then a non-zero exit) is a small, separate grammar change worth doing alongside Phase 1 but
  not required for it.
- **OQ-2**: Custom/domain error types. This proposal keeps `Error` a single concrete struct (code + message).
  A plausible future extension is an `Error` *interface* (structural, like the existing `interfaces.md`
  mechanism) that user structs can attach to, with `Result<T>` holding an `Error` interface reference instead
  of the concrete struct. Deferred out of this proposal to keep scope contained.
- **OQ-3**: Does forcing a bind-or-underscore for every discarded `Result<T>` (Phase 3) ever produce annoying
  boilerplate at call sites that already fully handle the error through some other means (e.g. a `switch` over
  `.getErr().code` performed as part of a larger expression)? Worth revisiting once Phase 4 (migrating `std`)
  produces real-world call sites to check against.
