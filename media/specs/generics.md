# Technical Specification for Generics in Spice

## Implementation steps:

- [x] 1. Re-write the function choosing mechanism
- [x] 2. Implement Generics for functions/procedures/methods
- [x] 3. Support basic type conditions for generic types
- [x] 3. Add tests for this feature
- [x] 4. Implement Generics for structs
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
int result = max<int>(1, 4);
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
Vector<int> v = Vector<int>{};
// or
dyn v = Vector<string>{};
```

### Generics with conditions
```spice
type T int | double | string { // T can only be either int, double or string
    toString(T);
}; 
```

## Functionality
While running the analyzer, Spice will check which concrete manifestations generic functions or structs are being used elsewhere
within the code base. It will generate IR for exactly those manifestations.

### Detailed technical description
After a call to `substantiateOptionalArgs()`, the optional arguments are substantiated, but the generic types are not. When `match()` is called,
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

A is the main source file.

Up to now, the compiler executed the analyzer/generator runs in following order:

1. ┌ Run analyzer for A
2. │┌ Run analyzer for B
3. ││┌ Run analyzer for D
4. ││└ Run generator for D 
5. │└ Run generator for B
6. │┌ Run analyzer for C
7. │└ Run generator for C
8. └ Generator run for A

The new execution order has to be:

1. ┌─── Run analyzer for A
2. │┌── Run analyzer for B
3. ││┌─ Run analyzer for D
4. │││┌ Run analyzer for C
5. └┼┼┼ Run generator or A
6.  └┼┼ Run generator for B
7.   └┼ Run generator for D
8.    └ Run generator for C