# Technical Specification for Generics in Spice

## Implementation steps:

- [x] 1. Re-write the function choosing mechanism
- [x] 2. Implement Generics for functions/procedures/methods
- [ ] 3. Add tests for this feature
- [ ] 4. Implement Generics for structs
- [ ] 5. Add tests for this feature
- [ ] 6. Add documentation for generics

## Syntax

### Generics with functions / procedures / methods

#### Callee
```spice
type T dyn;

f<T> max<T>(T a, T b) {
	return b > a ? b : a;
}
```

#### Caller
```spice
...
int result = max<int>(1, 4);
...
```

### Generics with structs

#### Definition
```spice
type T dyn;

type Vector<T> struct {
	T* arrayPtr
	long size
}
```

#### Referencing
```spice
...
Vector<int> v = Vector<int>{};
// or
dyn v = Vector<string>{};
...
```

## Functionality
While running the analyzer, Spice will check which concrete manifestations generic functions or structs are being used elsewhere
within the code base. It will generate IR for exactly those manifestations.

### Detailed technical description
After a call to `substantiate()`, the optional arguments are substantiated, but the generic types are not. When `match()` is called,
it also checks if the call matches to a generic function. If yes, the generic function is duplicated in the function registry, the
duplicate gets the concrete types by the `match()` method and the new function gets returned to the caller of `match()`.  At the end
of the analyzer run, all functions that are not fully substantiated (optional types as well as generic types) get removed from the
symbol table.

### Change order of analyzer/generator runs
For generics, it is essential to have the whole symbol table (for all source files) present when the generator runs for the first
source file.

#### Example

```mermaid
  graph TD;
      A-->B;
      A-->C;
      B-->D;
      C-->D;
```

*Note: A is the main source file*

Up to now, the compiler executed the analyzer/generator runs in following order:

- Analyzer run for A
- Analyzer run for B
- Analyzer run for D
- 