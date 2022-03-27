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
While running the analyzer, Spice will check which concrete manifestations generic functions or structs are being used elsewhere within the code base. It will generate IR for exactly those manifestations.