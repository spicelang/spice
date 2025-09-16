# Technical Specification for the heap qualifier

## Implementation steps:

- [x] Add basic support
- [x] Move all std data structures to use the heap qualifier
- [x] Add tests for this feature
- [x] Generate default dtor for structs with heap fields where free is called on those fields
- [x] Make copy ctor mandatory for structs with heap fields
- [x] Allow assignment from heap to non-heap pointers to create non-owning pointers
- [x] Perform an ownership transfer when assigning from heap to heap pointers
- [x] Deallocate local heap variables with sDealloc when they go out of scope

## Syntax

```spice
// As struct field
type TestStruct struct {
    heap int* ptr
}

// As local variable
f<int> foo() {
    heap int* ptr = sNew<int>();
    *ptr = 5;
    return ptr;
}
```

## Functionality

### General introduction

The `heap` qualifier can only be applied to pointer variables/fields and is used to indicate that the memory for the
variable/field was or is going to be allocated on the heap. It acts similar to a `unique_ptr` would in C++, but builtin
into the language.

The heap qualifier can be used in the following ways:

- As a field type qualifier in a struct
- As a local variable type qualifier
- As a function return type qualifier
- As a parameter type qualifier

When the `heap` qualifier is used for a struct field, the compiler will automatically generate a destructor for that
struct, that will free the memory allocated for the field. The `heap` qualifier can also be used in local variables
and function return types. When a function returns a `heap` pointer, the caller is responsible for freeing the memory
allocated for the pointer. The `heap` qualifier can also be used in function parameters. When a function parameter is
declared as `heap`, the callee is responsible for freeing the memory allocated for the parameter.

### Assignment semantics

When assigning a `heap` pointer to a non-heap pointer, the non-heap pointer becomes a non-owning pointer. This means
that the non-heap pointer does not have to free its memory, as it does not own it. The memory is still owned by the
`heap` pointer, and the memory will be freed when the `heap` pointer goes out of scope.

When assigning a `heap` pointer to a reference to a `heap` pointer, it results in a non-owning reference.

When assigning a `heap` pointer to another `heap` pointer, the memory is not copied, but the ownership gets
transferred to the left hand side variable. This means that the original `heap` variable is no longer responsible for
freeing the memory, instead the new `heap` variable is. This is done to prevent double freeing of the underlying memory.

Assigning a non-heap pointer to a `heap` pointer is not allowed, as the non-heap pointer does not own the memory which
it points to.

### Struct implications

When a struct has a field with the `heap` qualifier, the compiler will automatically generate a destructor for the
struct. The destructor will free the memory allocated for the `heap` field. The compiler will also make the copy
constructor mandatory for the struct. This is done to prevent double freeing of the memory.
