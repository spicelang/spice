# Technical Specification for per-level const

## Implementation steps:

- [ ] Move the const of inner type levels into the type chain, keep the top-level const in the qualifiers
- [ ] Attach the prefix `const` of a declaration to the base level
- [ ] Apply `const T` to the top level of the substituted type, also for aliases
- [ ] Deduce generic types without the top-level const of the argument
- [ ] Replace the const checks of assignments and argument matching by the qualification conversion rule
- [ ] Carry the const of a struct over to all of its fields, including pointer fields
- [ ] Add the postfix `const` syntax (`int* const`)
- [ ] Mangle and emit debug info per level
- [ ] Print per-level const in type names
- [ ] Remove the workarounds listed in [Cleanups](#cleanups)
- [ ] Update the docs and add tests for this feature

## Motivation

Spice stores one set of qualifiers per type: a `QualType` is an interned, immutable `Type` (the type chain, e.g.
`int`, `*`, `&`) plus one `TypeQualifiers` struct. Its `isConst` flag has no level. Today it means:

- for a value type (`const int`, `const Box`): the value is const
- for a pointer or reference type (`const int*`, `const int&`): the **pointee or referent** is const, the pointer
  itself can be re-assigned

There is no way to express a const pointer, and two different types share one representation. A generic `const T&`
substituted with `T = int*` yields the same `QualType` as a user-written `const int*&`:

| Source                          | Meaning                               | Current representation |
|---------------------------------|---------------------------------------|------------------------|
| `const int*& r`                 | reference to a pointer to const int   | `const int*&`          |
| `const T& r` with `T = int*`    | const reference to a pointer to int   | `const int*&`          |

The compiler cannot tell them apart, so it either rejects valid code or accepts invalid code:

```spice
// Valid: copies the pointer out of a const reference (Result<heap byte*>, every container of pointers)
p Result.ctor(const T& data) {
    this.data = data;
}

// Invalid, but compiles: drops the const of the pointee
const int c = 1;
const int* cip = &c;
const int*& rcip = cip;
int* pa = rcip;
```

Rejecting the second one rejects the first one as well: an attempt to close the hole broke 143 tests, see #1660. The
same ambiguity forced these workarounds:

- Copying a value out of a reference strips the whole const (`toNonConst()` in `getAssignResultType`), which opens the
  hole above.
- Struct fields of a const struct only become const if they are no pointers or references (#1663), because a const
  pointer field cannot be expressed.
- Bare generic params (`T&`, `T*`) are exempt from the const argument check until `T` is known (#1660, #1665).
- Handles declared as `const LLVMTypeRef` meant a const handle, but were read as a pointer to const (#1663 removed the
  `const`).

## Goals

- A type carries const per level: `int* const` (const pointer) and `const int*` (pointer to const) are different.
- `const T` with a pointer `T` makes the pointer const, like a typedef in C/C++.
- Copying a value drops exactly its top-level const, nothing else.
- Existing source code keeps its meaning: a prefix `const` keeps applying to the base type.

## Non-goals

- Const methods (`this` as a const reference). They would build on this, but are a separate feature.
- Per-level `heap`. `heap` keeps describing the outermost pointer.
- `volatile` or other cv-qualifiers.

## Representation

### Option A (recommended): const in the type chain, like clang

Each `TypeChainElement` gets an `isConst` flag. `QualType.qualifiers.isConst` keeps existing, but means the const of
the **top level** only. For a chain `[base, l1, ..., ln]`:

- the top level `ln` is described by `QualType.qualifiers.isConst`
- every inner level (`base` up to `ln-1`) is described by the `isConst` flag of its chain element

Examples:

| Source             | Type chain (const marked with `!`) | `qualifiers.isConst` |
|--------------------|-------------------------------------|----------------------|
| `const int`        | `[int]`                             | true                 |
| `const int*`       | `[int!, *]`                         | false                |
| `int* const`       | `[int, *]`                          | true                 |
| `const int* const` | `[int!, *]`                         | true                 |
| `const int&`       | `[int!, &]`                         | false                |
| `int* const&`      | `[int, *!, &]`                      | false                |

This mirrors clang's model (fast top-level qualifiers on `QualType`, inner qualifiers in the canonical type):

- Inner const is part of the type identity: `int*` and `const int*` are different interned types.
- Top-level const is not part of the identity, which is what C and C++ do for function signatures and copies.

The type operations move the const between the two places:

- `toPtr()` / `toRef()` / `toArr()`: the current top-level const moves into the old top chain element, the new top
  level starts as non-const.
- `getContained()`: the const of the new top chain element moves into `qualifiers.isConst`, the element is stored
  without it.
- `getBase()`: `qualifiers.isConst` becomes the const of the base element.
- `isConst()` reports the top-level const for all types, also pointers (today it is limited to extended primitives).

Consequences:

- `Type` hashing and equality in the type registry include the per-element const. `const int*` and `int*` are interned
  separately.
- `Type.matches()` gets an option to compare inner const by the qualification conversion rule, see below.

### Option B: const bitmask in `TypeQualifiers`

`TypeQualifiers` gets a bitmask with one bit per chain level, the interned `Type` stays unchanged. This avoids touching
the type registry, but every operation on the chain has to shift the mask (wrapping, unwrapping, generic substitution
with chains of different depth), and the inner const would not be part of the type identity, so caches keyed by the
`Type` would mix up `int*` and `const int*`.

Option A puts the information where the identity of the type lives, and keeps `qualifiers.isConst` meaningful as "this
value is const", so I recommend it.

## Syntax

The prefix `const` keeps applying to the base type, so existing code keeps its meaning:

```spice
const int c = 1;          // const int
const int* ptr = &c;      // pointer to const int, the pointer can be re-assigned
const int& ref = c;       // reference to const int
```

A `const` after a `*` makes that pointer level const:

```spice
int* const ptr = &a;              // const pointer to int, *ptr can be written
const int* const ptr2 = &c;       // const pointer to const int
int* const* ptrToConstPtr = &ptr; // pointer to a const pointer to int
```

`const` after `&` is an error, because a reference can never be re-bound. `const` after `[...]` is an error as well:
the const of an array is the const of its elements, written with the prefix (`const int[3]`).

Parser change: in `Parser.parseDataType`, after `*` accept an optional `const` token. It becomes a flag of the
`TypeModifier` that the parser already pushes to `ASTDataTypeNode.tmQueue` for each pointer level, and
`TypeChecker.visitDataType` applies it when it wraps the type in that pointer.

## Semantics

### Declarations

A variable has exactly its declared constness (#1663), now per level. A declaration copies the value, so only the
top-level const of the rhs is dropped. The inner levels have to be compatible by the qualification conversion rule.

### Qualification conversion

Adding const at an inner level is only safe if all levels above it are const as well. Otherwise a pointer to a const
value could be written through a non-const pointer of the outer level. This is the rule of C++ ([conv.qual]):

```spice
int a = 1;
int* pa = &a;
int** ppa = &pa;
const int** bad = ppa;          // must be an error: *bad = &constValue would make pa point to a const value
const int* const* good = ppa;   // ok
```

For two types with the same chain, `lhs = rhs` (and binding an argument to a param) is allowed if:

1. the top level is ignored for a copy, and must satisfy `lhs const >= rhs const` for a reference binding
2. at every inner level, the lhs is at least as const as the rhs
3. at every inner level where lhs adds const, all levels between it and the top are const in the lhs

This replaces:

- the `allowConstify` flag in `TypeQualifiers.match`
- the `toNonConst()` of contained types in `OpRuleManager.getAssignResultType` and `getFieldAssignResultType`
- the pre-unwrap check for reference and pointer candidates in `matchRequestedToCandidateType` (#1660, #1665)

### References

A reference has no own const (it can never be re-bound). `const int&` is a reference to a const int: the const lives
in the `int` element. Binding a temporary to a reference requires the referent to be const, as today.

### Generic substitution

`const T` makes the top level of the substituted type const, like a typedef in C/C++:

| Generic        | `T`          | Result               |
|----------------|--------------|----------------------|
| `const T&`     | `int`        | `const int&`         |
| `const T&`     | `int*`       | `int* const&`        |
| `const T&`     | `const int*` | `const int* const&`  |
| `const T*`     | `int*`       | `int* const*`        |

`QualType.replaceBaseType` takes the substituted type's chain, puts the generic's const on its top level and merges the
remaining qualifiers as today.

This is what makes `Result.ctor(const T& data) { this.data = data; }` valid for `T = heap byte*`: `data` is a
`heap byte* const&`, the copy drops the top-level const and yields a `heap byte*`.

### Generic deduction

The deduced type does not take over the const that the candidate adds itself (as since #1663), now per level:

- `const T&` with a `const int*` argument: `T = const int*` (the pointee const belongs to `T`)
- `const T&` with an `int* const` argument: `T = int*`
- `T&` with a `const int` argument: `T = const int`, so the body is checked with a const param (as today)

The exemption of bare generic params in `matchRequestedToCandidateType` becomes unnecessary: the qualification
conversion rule applies to the substituted candidate.

### Aliases

An alias behaves like a typedef: `const Alias` makes the top level of the aliased type const. With
`type LLVMTypeRef alias byte*`, `const LLVMTypeRef` is a const handle (`byte* const`), which is what the declarations
removed in #1663 meant.

This changes the meaning of existing code: today `const LLVMTypeRef` is a pointer to const bytes. A search over `src/`
and `std/` for `const` with an alias of a pointer type finds the affected declarations.

### Member access

A field of a const struct gets top-level const, also pointer fields: a `int* ptr` field of a const struct is an
`int* const`. The pointer cannot be re-assigned, but its pointee can be written, as in C++. This removes the exclusion
of pointer fields from #1663. Reference fields stay as they are, a reference has no own const.

### Function signatures

Like in C++, the top-level const of a by-value param is not part of the signature:

- `p f(int* const ptr)` and `p f(int* ptr)` are the same function. Overload resolution ignores the difference.
- The mangled name does not contain it either.
- Inside the body the param is const.

## Name mangling

The Itanium ABI puts a `K` in front of the type it qualifies. With the const per level:

| Type                | Today  | New      |
|---------------------|--------|----------|
| `const int*`        | `PKi`  | `PKi`    |
| `const int&`        | `RKi`  | `RKi`    |
| `int* const&`       | n/a    | `RKPi`   |
| `const T&`, `T=int*`| `RPKi` | `RKPi`   |
| `int* const` (param)| n/a    | `Pi`     |

`mangleType` emits a `K` before every chain element with const, and before the base element if it is const. The
top-level const is only emitted below a pointer or reference, i.e. never for a by-value param. The `V` for `heap` stays
at its current position.

Mangled names only change for types where the old representation put the const on the wrong level: mainly generic
manifestations with `const T` and a pointer `T`, and declarations with `const` and an alias of a pointer type. Spice
compiles the whole program including the std from source, so there is no binary compatibility to keep. 58 IR
reference files of the test suite contain a `K` in a mangled name, none of them in the `RPK` form that changes.

## Debug info

`DebugInfoGenerator.getDITypeForQualType` wraps every const level in a `DW_TAG_const_type`, not only the top level of
an extended primitive.

## Type names

`QualType.getName` prints the const per level: `const int* const&`, `int* const`. Error messages and the test
references that contain such names change accordingly.

## Compiler changes by area

| Area                     | Files                                                     | Change                                                                 |
|--------------------------|-----------------------------------------------------------|------------------------------------------------------------------------|
| Representation           | `type-chain.spice`, `type.spice`, `type-registry.spice`   | `isConst` per element, included in hashing and equality                |
| Type operations          | `qual-type.spice`                                         | move const in `toPtr`/`toRef`/`toArr`/`getContained`/`getBase`/`replaceBaseType`, `isConst()` for all types |
| Data types               | `parser.spice`, `ast-nodes.spice`, `type-checker.spice`   | postfix `const` after `*`, prefix `const` to the base level            |
| Assignment rules         | `op-rule-manager.spice`                                   | qualification conversion instead of `toNonConst()`/`allowConstify`     |
| Function matching        | `type-matcher.spice`, `function-manager.spice`            | qualification conversion, ignore top-level const of by-value params, scoring per level |
| Generics                 | `type-matcher.spice`, `generic-type.spice`                | substitution puts `const T` on the top level, deduction strips per level |
| Member access            | `type-checker.spice`                                      | field const for all fields                                             |
| Mangling, debug info     | `name-mangling.spice`, `debug-info-generator.spice`       | per level                                                              |

About 80 call sites currently read or write the single const flag:

- 16 `qualifiers.isConst`
- 10 `isConst()`
- 7 `makeConst()`
- 20 `toNonConst()`
- 4 `isConstRef()`
- 12 `toConstRef()`
- 2 `qualifiers.merge()`
- 7 `allowConstify`

Each one has to be checked for which level it means. Most mean the top level, e.g. `ensureNoConstAssign`, the RVO
and copy decisions, and the const `this` type of copy ctors. The pointer cases mean the pointee, e.g. the matcher check
from #1660 and overload scoring from #1663.

## Cleanups

Once the representation is in place, these workarounds can go:

- `toNonConst()` of contained types in `OpRuleManager.getAssignResultType` / `getFieldAssignResultType`
- the pre-unwrap pointer/reference check and the bare generic exemption in `matchRequestedToCandidateType`
- the known-`T` check from #1665 in the same function (covered by the substituted candidate)
- the pointer/reference field exclusion of the member access const (#1663)
- the special case for pointers in the declaration const rule of `TypeChecker.visitDeclStmt` (#1663)

## Migration

Behavior changes for existing code:

- **`const` with an alias of a pointer type** now makes the pointer const instead of the pointee. Declarations that
  write through such a handle stay valid. Declarations that rely on the pointee being const (none known in `std/` and
  `src/`) need an explicit `const` on the pointee.
- **Generic code** with `const T&` and pointer types gets the right semantics: the pointer can be copied out, its
  pointee stays writable. Code that compiled before keeps compiling.
- **`const int*& r; int* p = r;`** becomes an error (dropping the const of the pointee).
- **Multi-level const conversions** like `const int** = int**` become errors.
- **Mangled names** change for the cases in [Name mangling](#name-mangling).

The compiler itself has to build with the stage0 compiler. The new syntax (`int* const`) can only be used in `src/`
once the stage0 version supports it. Everything else (stricter checks, new representation) applies to `src/` right
away and has to be validated with a self-compile.

## Testing

- Type checker tests per rule: declarations with postfix `const`, qualification conversion (including the multi-level
  error case), generic substitution and deduction with pointer `T`, aliases of pointer types, const pointer fields.
- The cases that motivated this note:
  - `Result<heap byte*>` must keep compiling
  - `const int*& r; int* p = r;` must be rejected
  - `p f<T>(const T& v)` with `T = int*` must allow `int* copy = v;`
- Name mangling tests for the table above, and that `p f(int* const)` / `p f(int*)` collide as a redefinition.
- Debug info test with a const pointer and a pointer to const.
- Self-compile, stage2 suite, and cross-target type checks for Windows and macOS, since the change touches every type.

## Open questions

- Should `const` after `[...]` be accepted for const arrays, or is the prefix enough?
- Should a const by-value param be visible in error messages and the signature shown to users, although it is not part
  of the mangled name?
- Should the multi-level qualification conversion error get its own error message, explaining why
  `const int** = int**` is unsafe?
