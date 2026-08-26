# Technical Specification for Explicit, Mandatory Error Handling in Spice

## Status

Design proposal. Not yet implemented. This document describes the target design and a phased,
independently-shippable migration path away from the current all-library, opt-in `Result`/`Error`/`panic`
mechanism, towards a compiler-enforced, Zig-style "errors as values" model.

## Implementation steps

- [ ] Add `_` as a discard target for declarations/assignments (prerequisite, self-contained)
- [ ] Adjust grammar for the `!T` error-union type and the `try` / `catch` operators
- [ ] Update IntelliJ plugin
- [ ] Desugar `!T` to `Result<T>` in the type checker (no behavior change yet)
- [ ] Implement `try` as sugar for propagate-or-unwrap
- [ ] Implement `catch` as sugar for handle-or-produce-fallback
- [ ] Add semantic checks for mandatory handling of error-union-typed expressions
- [ ] Add tests for semantic checks
- [ ] Gate mandatory handling behind an opt-in module attribute / CLI flag
- [ ] Migrate `std` to return `!T` instead of panicking on recoverable failures
- [ ] Update docs, examples, changelog
- [ ] Flip mandatory handling on by default (breaking change, major version)
- [ ] Remove the opt-in gate

## Motivation

Today, error handling in Spice is **entirely a library convention with no compiler backing**:

- `Result<T>` (`std/runtime/result_rt.spice`) is a plain generic struct holding `T data` and `Error error`.
  `unwrap()` panics if `error.code != 0`; `isOk()`/`isErr()`/`getErr()` are just field checks.
- `Error` (`std/runtime/error_rt.spice`) is a plain struct: `int code` + `string message`.
- `panic(Error)` is a builtin (`TypeChecker::visitBuiltinPanicCall`, `IRGenerator::visitBuiltinPanicCall`) that
  prints to stderr and calls `exit(EXIT_FAILURE)`. It is recognized by the compiler only insofar as its argument
  must satisfy `QualType::isErrorObj()`.

Nothing stops a caller from:
- Calling a `Result<T>`-returning function and dropping the result entirely as a bare expression statement.
- Calling `.unwrap()` without ever checking `.isErr()` first.
- Ignoring `Error` values that were never wrapped in a `Result` at all.

The type checker *does* already warn (not error) on an unused function-call return value in general
(`UNUSED_RETURN_VALUE`, `TypeCheckerValues.cpp:290`), suppressible per-function via
`#[core.compiler.ignoreUnusedReturnValue]` (`Attributes.h:29`). That is the closest thing to enforcement that
exists, and it is generic, non-error-aware, and only a warning.

Goal: move to a model where a function that can fail says so in its signature, the value it returns cannot be
silently discarded, and propagating or handling the error is part of the language grammar rather than a
struct method you may or may not remember to call — closer to Zig's `!T` / `try` / `catch`, adapted to Spice's
existing struct/interface/generics model rather than Zig's error-set model.

## Goals

1. A function that can fail declares it in its return type, and the type is different from a function that can't.
2. A caller cannot use the success value without acknowledging the possibility of failure, and cannot discard
   the failure possibility silently by accident (an explicit opt-out is fine; silence is not).
3. Propagating an error up the call stack is a one-token operation, not manual `if isErr() { return err }`
   boilerplate.
4. `panic` remains available, but becomes the tool for programmer errors / truly unrecoverable state
   (Zig's `unreachable`/`@panic`), not the primary path for recoverable failures like "file not found".
5. Migration is incremental: every phase compiles and existing `Result`/`Error`/`panic` user code keeps working
   until the final, explicitly-versioned breaking phase.

## Non-goals (for this proposal)

- Zig-style compile-time-checked **error sets** (per-function enumerated error types). Spice's `Error` stays a
  single concrete struct (code + message) for v1; see "Future extensions" below for the interface-based path.
- Exceptions / stack unwinding / `try`-`catch`-in-the-C++-sense. There is no unwinding — `try`/`catch` here are
  both sugar over ordinary value returns, matching how `Result<T>` already works today.
- Changing `panic`'s runtime behavior (print + `exit`). Out of scope here.

## Design

### 1. `!T`: error-union type, sugar over `Result<T>`

`!T` is new type syntax, valid wherever `dataType` is valid, that the type checker desugars directly to
`Result<T>`. No new struct layout, no ABI change — `Result<T>` remains the concrete representation, so this is
purely a front-end (grammar + type-checker) change:

```spice
f<!File> openFile(string path) { ... }   // same runtime type as f<Result<File>> openFile(...)
```

Using `!T` instead of spelling out `Result<T>` is what makes the value subject to the mandatory-handling check
below (see "Marking", not just the raw `Result<T>` spelling) — see open question OQ-1 for whether the check
should key off the spelling or off the type itself.

### 2. `_` as a discard target

Prerequisite for everything else. Add `_` as a valid assignment/declaration target that evaluates and discards
its right-hand side without binding a variable or triggering "unused" diagnostics:

```spice
_ = mightFail();   // explicitly, visibly acknowledges "I am ignoring this"
```

This is small and self-contained (grammar + `SymbolTableBuilder` + `TypeChecker` + `IRGenerator`), useful
independent of error handling, and is the escape hatch mandatory handling needs (see Goal 2: opt-out must be
explicit, not silent).

### 3. `try`: propagate-or-unwrap

`try <expr>` where `<expr>` has type `!T` (i.e. `Result<T>`):
- If the callee scope's own return type is not an error-union type, this is a compile error — `try` can only
  appear inside a function/procedure that itself returns `!U` for some `U`, so the error has somewhere to go.
- At runtime: if `<expr>.isErr()`, immediately `return err<U>(<expr>.getErr())` from the enclosing function
  (an early-return of the enclosing function's own error-union return type, analogous to how `terminateBlock`
  already emits scope cleanup before `panic`'s `exit` call in `GenBuiltinFunctions.cpp:143`).
- Otherwise, evaluates to `<expr>.unwrap()` (the `T`).

```spice
f<!Config> loadConfig(string path) {
    string raw = try readFile(path);      // propagates readFile's error as-is
    return parseConfig(raw);
}
```

### 4. `catch`: handle-or-produce-fallback

`<expr> catch |err| <block-or-expr>` where `<expr>` has type `!T`:
- If `<expr>.isOk()`, evaluates to the unwrapped `T`.
- Otherwise, binds `err` (type `Error`) in scope and evaluates `<block-or-expr>`, which must either produce a
  value of type `T` (fallback) or diverge (`return`, `panic`, `break`, `continue` — mirroring how Zig requires
  the `catch` block to diverge or produce `T`).

```spice
Config cfg = loadConfig(path) catch |e| {
    log.warn("using defaults: " + e.message);
    return Config.default();
};
```

`.isOk()`/`.isErr()`/`.unwrap()`/`.getErr()` on `Result<T>` keep working exactly as today — `try`/`catch` are
convenience sugar over them, not a replacement mechanism, which is what keeps `Result<T>` itself unchanged and
the migration additive.

### 5. Mandatory handling diagnostic

Currently `UNUSED_RETURN_VALUE` (`TypeCheckerValues.cpp:290`) is a warning for *any* discarded return value and
is suppressible per-callee via `#[core.compiler.ignoreUnusedReturnValue]`. This proposal adds a second,
narrower check specific to error-union-typed (`!T`) expressions:

- A bare expression statement of type `!T` that is not the direct operand of `try`, `catch`, or an explicit
  `_ = ...;` discard is a **hard compile error**, not a warning, and is *not* suppressible by
  `ignoreUnusedReturnValue` (that attribute keeps its existing, weaker meaning for ordinary return values).
- The general, non-error-aware `UNUSED_RETURN_VALUE` warning is untouched for everything else.

This is the piece that actually makes handling *mandatory* rather than merely convenient, and it is a
relatively small addition on top of existing infrastructure (`TypeCheckerValues.cpp:279-291` already has the
"is this a discarded call result" check; it just needs an error-union-aware branch).

### 6. `panic` stays, with a narrowed role

`panic(Error)` keeps its current signature and runtime behavior. Its documented role shifts to: assertion-like,
programmer-error, "this should be provably impossible" situations (mirrors `assertStmt`, which already exists
in the grammar) — not the default way to react to a recoverable failure such as I/O errors, parse errors, or
missing files. `std` migrates its own internal uses of `panic` on recoverable paths to `!T` returns (see
Migration Phase 5).

## Compatibility strategy

Steps 1-4 above (`_`, `!T`, `try`, `catch`) are pure additions: nothing that compiles today stops compiling.
Step 5 (mandatory handling) is the one behavior change that can break existing code that silently drops a
`Result`/error-union value. To keep every phase shippable and non-breaking until explicitly intended:

- Ship mandatory handling behind a module attribute, e.g. `#![core.compiler.explicitErrorHandling = true]`
  (same mechanism already used by `alwaysKeepOnNameCollision` in `result_rt.spice:1`), defaulting to `false`.
- Once `std` and the test suite are migrated and the feature has soaked, flip the default to `true` in a
  documented major/minor version bump, then eventually remove the gate.

This mirrors how `interfaces.md` and `generics.md` shipped incrementally with checklists rather than as one
atomic change.

## Step-by-step migration plan

Each phase is independently buildable/testable and leaves the compiler in a working state.

**Phase 1 — `_` discard target.**
Grammar (`declStmt`/assignment target), `ASTBuilder`, `SymbolTableBuilder`, `TypeChecker`, `IRGenerator`. Add
tests under `test/test-files/`. No interaction with error handling yet — a standalone, low-risk feature.

**Phase 2 — `!T` type syntax as pure sugar.**
Grammar: extend `dataType` (or a new rule referenced from it) with a `NOT dataType` alternative. Type checker:
resolve `!T` to `Result<T>` at the point `dataType` is turned into a `QualType` (desugar early, so every later
pass only ever sees `Result<T>`, avoiding a second special-cased type everywhere). No new diagnostics yet;
`!T` and `Result<T>` are fully interchangeable. Update the IntelliJ plugin grammar/highlighting.

**Phase 3 — `try`.**
Grammar: new prefix operator in the expression grammar, restricted to appear only where `assignExpr` is
allowed. Type checker: verify enclosing function/procedure return type is an error-union type; verify operand
type is an error-union type; unify the propagated error's `T` conversion. IR generator: emit the `isErr()`
check + early-return-with-error, patterned after the existing early-`return`/`terminateBlock` handling in
`GenStatements.cpp` and `GenBuiltinFunctions.cpp:141-149`. Tests: propagation across nested calls, propagation
through generic functions, propagation combined with existing `Result<T>` code that doesn't use `try`.

**Phase 4 — `catch`.**
Grammar: binary-operator-like construct `assignExpr CATCH (PIPE IDENTIFIER PIPE)? (stmtLst | assignExpr)`.
Type checker: bind the error identifier as `Error` in the block's scope; verify the block either produces `T`
on every path or diverges (reuse whatever exhaustiveness/return-path analysis already backs `ifStmt`/`switchStmt`
return checking). IR generator: conditional branch, PHI-merge the fallback value with the unwrapped value.
Tests mirroring Phase 3's, plus divergent vs. value-producing `catch` bodies.

**Phase 5 — mandatory handling diagnostic, opt-in.**
Add the module attribute gate. Add the error-union-aware branch next to `UNUSED_RETURN_VALUE`
(`TypeCheckerValues.cpp:279-291`): when the discarded value's type is an error union and the module attribute
is set, hard error instead of warn, pointing users at `try`/`catch`/`_ = `. Tests: one test file per attribute
state (on/off) confirming the same code errors or is silently accepted respectively.

**Phase 6 — migrate `std`.**
Audit `std/` for internal `panic()` calls on recoverable conditions (I/O, parsing, allocation-adjacent paths —
excluding true invariant violations) and convert their public signatures to `!T`, updating call sites within
`std` to use `try`/`catch`. This is the phase most likely to surface real design gaps (e.g. does every `std`
error path have a sensible `Error` to construct?) and should land function-by-function, each with its own
tests, rather than as one sweep.

**Phase 7 — docs & examples.**
Update `docs/`, `STYLE_GUIDE.md` error-handling guidance, and any example `.spice` files to use `!T`/`try`/
`catch` as the recommended idiom, with `Result<T>`/`.unwrap()` documented as the lower-level mechanism `!T`
sugars over (not deprecated — still valid, still needed for interop with code that predates this feature).

**Phase 8 — flip default, remove gate (major version).**
Change the module attribute's default to `true`. After a deprecation window, remove the attribute and the old
"discard silently allowed" code path entirely, making mandatory handling unconditional. Update CHANGELOG with
migration notes for external users (mechanical fix in each case: add `try`, `catch`, or `_ = `).

## Testing plan

Follow the existing reference-test convention (`spice-add-test` skill / `test/test-files/`): one grouped suite
`error-handling/` with subdirectories per construct (`discard/`, `error-union-type/`, `try/`, `catch/`,
`mandatory-handling/`), each with positive cases (compiles, expected stdout) and negative cases
(`exception.out` matching the new diagnostics). Unit tests in `test/unittest/` for the `QualType` desugaring of
`!T` and for the divergence analysis added in Phase 4.

## Open questions

- **OQ-1**: Should the mandatory-handling check key off the *spelling* `!T` in source, or off the *type*
  `Result<T>` regardless of spelling (i.e. also flag a hand-declared `Result<T>` return value)? This spec
  assumes the latter (type-based, not spelling-based) so users can't dodge the check by writing `Result<T>`
  instead of `!T`; needs confirming before Phase 5.
- **OQ-2**: Custom/domain error types. Zig's error sets don't map cleanly onto Spice's struct/interface model.
  A plausible future extension: an `Error` *interface* (structural, like the existing `interfaces.md` mechanism)
  that user structs can attach to, with the concrete `Error` struct as just one implementation — `Result<T>`
  would then hold an `Error` interface reference instead of the concrete struct. Deferred out of this proposal
  to keep v1 scoped; flagged here so Phase 6 doesn't accidentally foreclose it.
- **OQ-3**: Should `try` be allowed at top level in `main`/`f<int> main()`? Zig allows `main` to return an
  error union directly. Spice's `mainFunctionDef` is currently fixed to `TYPE_INT`; extending it to accept
  `!int` is a small, separate grammar change worth doing alongside Phase 3 but not required for it.
