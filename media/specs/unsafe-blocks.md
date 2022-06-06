# Technical Specification for Unsafe blocks in Spice

## Implementation steps:

- [x] 1. Support unsafe blocks in the code
- [ ] 2. Decide for each operator/type combination if unsafe

## Syntax

Spice aims to be memory-safe per default and only allow unsafe operations in
designated and specified areas of code. This can be achieved with the support
of `unsafe` blocks. The use of unsafe operations are illegal outside such a
block. When entering an `unsafe` block, unsafe operations are enabled and
when exiting those block, they get disabled again.

```spice
unsafe {
    // Do unsafe operation
}
```

## Unsafe operator/type combinations
In general, all pointer-modifying operations should be marked as unsafe and
should only be available within `unsafe` blocks.

Following operations should be (introduced and) marked unsafe:

- [ ] Operators `+=`, `-=`, `+` and `-` with pointer lhs
- [ ] Cast from any pointer type to any other pointer type

## External function calls unsafe (no decision yet)
There are external functions like `malloc` or `free` which the user should
not be using without knowing, that it is not recommended. An option would be
to implicitly mark all external functions as unsafe.