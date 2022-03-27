# Function choosing design document

## Usage examples

### Example 1

```spice
f<int> testFunc(int i = 1) {
	// ...
	return 0;
}
```

Result:

- `testFunc(): int`
- `testFunc(int): int`

### Example 2

```spice
p testFunc1(string arg1, double arg2 = 1.1, short arg3 = 1) {
	// ...
}
```

Result:

- `testFunc1(string)`
- `testFunc1(string, double)`
- `testFunc1(string, double, short)`

### Example 3

```spice
f<bool> TestStruct.existsFilter(int arg1 = 1, int arg2 = 2, double arg3 = 4.5)
```

Result:

- `TestStruct.existsFilter(): bool`
- `TestStruct.existsFilter(int): bool`
- `TestStruct.existsFilter(int, int): bool`
- `TestStruct.existsFilter(int, int, double): bool`