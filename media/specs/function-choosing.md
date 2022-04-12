# Function choosing design document

## Usage examples

### Example 1

**Declaration:**

```spice
f<int> testFunc(int i = 1) {
	// ...
	return 0;
}
```

**Result:**

- `testFunc(): int`
- `testFunc(int): int`

### Example 2

**Declaration:**

```spice
p testFunc1(string arg1, double arg2 = 1.1, short arg3 = 1) {
	// ...
}
```

**Result:**

- `testFunc1(string)`
- `testFunc1(string,double)`
- `testFunc1(string,double,short)`

### Example 3

**Declaration:**

```spice
f<bool> TestStruct.existsFilter(int arg1 = 1, int arg2 = 2, double arg3 = 4.5)
```

**Result:**

- `TestStruct.existsFilter(): bool`
- `TestStruct.existsFilter(int): bool`
- `TestStruct.existsFilter(int,int): bool`
- `TestStruct.existsFilter(int,int,double): bool`

### Example 4 (Generic function)

**Rules for generic functions:**

1. Result is empty if the function is not used anywhere
2. Result contains exactly that manifestations that are being used
3. The optional args (if involved) are substantiated normally

**Declaration:**

```spice
type T dyn;

f<string> genericFunc<T>(T arg1, T arg2) {}
```

**Usages:**

- `genericFunc<int>(3, 4)`
- `genericFunc<bool>(false, true)`

**Result:**

- `genericFunc<int>(int,int): string`
- `genericFunc<bool>(bool,bool): string`

### Example 5 (Generic functions with optional arguments)

**Declaration:**

```spice
type T dyn;
type U dyn;

p TestStruct1.genericOptFunc<T, U>(T arg0, U arg1, int arg2 = 10) {}
```

**Usages:**

- `genericOptFunc<double, string>(3.56, "test")`
- `genericOptFunc<long, short>(123454l, 2s, 9)`

**Result:**

- `genericOptFunc<double,string>(double,string)`
- `genericOptFunc<long,short>(long,short,int)`