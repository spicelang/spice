# Technical Specification for Inline Assembly in Spice

## Implementation steps:

- [x] Phase 1: `asm` builtin
    - [x] Register the builtin
    - [x] Type checking incl. structural constraint string and template verification
    - [x] IR generation
    - [ ] Extend the `getInlineAsm` LLVM binding (alignStack, dialect, canThrow) - deferred to phase 2, nothing needs it yet
    - [x] Turn LLVM inline asm diagnostics into Spice compiler errors
    - [x] Tests and docs
- [ ] Phase 2: extensions (Intel dialect, pure asm, multiple direct outputs, module-level asm, syscall on top of `asm`)
- [ ] Phase 3 (optional): dedicated syntax with named operands

## Syntax

Inline assembly is a builtin call expression, that may only be used within `unsafe` blocks. It passes the LLVM inline
asm template string and constraint string through to LLVM verbatim:

```spice
unsafe {
    // One direct output ($0), two inputs; "0" ties the first input to the output register
    const int sum = asm<int>("addl $2, $0", "=r,0,r", a, b);

    // No output, memory clobber (compiler barrier)
    asm("mfence", "~{memory}");

    // Indirect output through a pointer argument
    long value;
    asm("movq $$42, $0", "=*m", &value);
}
```

- The optional template type is the result type. Without it, the call has no result (like `__volatile_store`).
- The template string uses the LLVM syntax: operands `$0`, `${0:w}` (with modifier), `$$` for a literal `$`.
- The constraint string uses the LLVM syntax: outputs `=r`, explicit registers `{rax}`, clobbers `~{memory}`, indirect
  operands `=*m`, tied operands `0`…`9`.

Using the LLVM syntax as-is avoids a translation layer, is documented by LLVM, and is already what the `syscall` builtin
emits internally. A builtin needs no lexer, parser, AST or visitor changes, since builtins go through
`lookupBuiltinFunction` and the `visitBuiltin…Call` methods of the type checker and the IR generator. A friendlier syntax
can be layered on top later and lower to the same form (see phase 3).

## Phase 1: `asm` builtin

### Builtin registration

`src/typechecker/builtin-functions.spice`: add a name constant and `BuiltinFunctionInfo(0u, 1u, 2u, unlimited)`.

### Type checking

`TypeChecker.visitBuiltinAsmCall`:

- Only allowed in unsafe scopes (`Scope.doesAllowUnsafeOperations()`), otherwise `UNSAFE_OPERATION_IN_SAFE_CONTEXT`.
- The template string and the constraint string must be compile-time string values (`EXPECTED_COMPILE_TIME_VALUE`).
  They are read via `GlobalResourceManager.getCompileTimeStringValue`, like `__get_build_var` does.
- The result type and the operand types must be primitive, pointer (incl. `string`) or enum types. Structs, arrays and
  interfaces are rejected with `BUILTIN_ARG_TYPE_MISMATCH`. References are unwrapped.
- Structural verification of the constraint string, mirroring `llvm::InlineAsm::verify`. `LLVMGetInlineAsm` does not
  verify anything, so a malformed constraint string would otherwise hit an assertion or crash later inside LLVM:
    - Split the constraint string into outputs (`=`), inputs and clobbers (`~`).
    - Number of direct outputs == 1 if a result type is given, 0 otherwise (only one direct output in phase 1).
    - Number of arguments == number of indirect outputs + number of inputs.
    - Every `*` (indirect) operand has a pointer argument.
    - Tied operands (`0`…`9`) refer to an existing output.
    - Clobbers come last.

  Whether a register class is valid for the target is left to LLVM (see "Assembler errors").
- With the TPDE backend, report `UNSUPPORTED_BY_BACKEND` (TPDE does not support inline assembly, same as for `syscall`).

### IR generation

`IRGenerator.visitBuiltinAsmCall`:

- Build the function type: result type (or `void`) and the operand types, with `ptr` for indirect operands.
- `llvm::getInlineAsm(fctType, asmString, constraints, hasSideEffects = true)`, then `createCall`.
- Add the `elementtype` parameter attribute for every indirect operand (`addCallParamTypeAttr`, as `syscall` does).
- Always marking the asm as having side effects (like GCC's `asm volatile`) is the conservative default.

### LLVM bindings

`std/bindings/llvm/llvm.spice`: extend `getInlineAsm` with optional `alignStack`, `dialect` and `canThrow` parameters, so
that existing callers (`syscall`) stay unchanged.

### Assembler errors

An invalid template (e.g. `"movq %rax"`) is only detected by the integrated assembler at object emission (or at link
time with LTO). LLVM's default diagnostic handler prints the error and calls `exit(1)`, bypassing Spice's error
handling. Therefore:

- Bind `LLVMContextSetDiagnosticHandler`, `LLVMGetDiagInfoDescription` and `LLVMGetDiagInfoSeverity`.
- Install a handler, that turns errors into a Spice compiler error.
- The callback can likely be passed with the same fat pointer trick as `pthread_create` in `std/os/thread.spice`
  (`{fn, ctx}` matches the C `(handler, context)` pair). Verify first, that the captures pointer is passed where C
  expects the context argument.
- Limitation: the C API does not expose the `!srcloc` cookie of `DiagnosticInfoInlineAsm`, so the error contains the
  asm line, but not the Spice source location. Add the enclosing function name as context instead.

### Tests

- Type checker error tests: outside of `unsafe`, non-compile-time strings, direct output count vs. result type mismatch,
  too few/too many arguments, `*` operand without a pointer argument, tied operand out of range, struct operand, TPDE.
- IR generator tests with `--target` for x86_64 and aarch64: direct output, indirect output, clobber-only, tied operand.
- One executable test (x86_64 only).
- One test for an invalid template, that goes through the diagnostic handler.

### Docs

- New `asm` section in `docs/docs/language/builtins.md`.
- Extend the TPDE limitations in `docs/docs/how-to/experimental-backends.md` (currently mentions `syscall` only).

### Implementation notes

- The constraint parser and the template verification live in `src/typechecker/inline-asm.spice` and are shared by the
  type checker and the IR generator.
- The template verification is required, not optional: LLVM aborts the compilation with `report_fatal_error` on an
  invalid operand reference like `$42` (instead of `$$42`) in the template.
- Stricter than LLVM: label constraints (`!`), multiple alternatives (`|`) and inputs tied to indirect outputs are
  rejected. A tied input must have the result type.
- New error types: `INVALID_ASM_CONSTRAINTS`, `INVALID_ASM_TEMPLATE` (semantic), `BACKEND_ERROR` (compiler) and the
  `BACKEND_WARNING` warning, for the diagnostics of the backend.
- On x86, `~{dirflag},~{fpsr},~{flags}` are appended to the constraints, like clang does, since GCC semantics assume
  that inline asm always clobbers the flags.
- `bool` operands and results are passed as `i8` like clang does, since the backends cannot allocate registers for `i1`.
- The diagnostic handler is installed by `LLVMObjectEmitter.emit` only for the duration of the emission
  (`llvm::DiagnosticCollector` in the LLVM bindings).

## Phase 2: Extensions

- Intel dialect: `asm` uses AT&T on x86 by default. Offer Intel via a separate builtin (`__asm_intel`) or a flag
  argument, using the new `dialect` parameter of the binding.
- Pure asm without the side effect flag, so that the optimizer may move or eliminate it.
- Multiple direct outputs: LLVM returns them as a struct, which needs a way to destructure them in Spice. Until then,
  indirect outputs cover this case.
- Module-level asm via `LLVMAppendModuleInlineAsm` at file scope (e.g. for `.section` directives or naked trampolines).
- Rebuild `syscall` on top of `asm`: either on a shared helper in the IR generator, or as a std function, that selects
  the asm per target via compile-time ifs on build vars. This proves the feature on real code and removes the
  hand-written per-target asm string generation from `IRGenerator.visitBuiltinSyscallCall`.

## Phase 3 (optional): Dedicated syntax

A Rust-like `asm("mov {dst}, {src}", dst = out(reg) x, src = in(reg) y)` would follow the `spice-language-feature`
recipe (lexer → parser → AST node → visitors), but lower to the same LLVM template and constraint strings. So it only
replaces the front end of phase 1.

## Open decisions

1. Name: `asm` (recommended) is a documented builtin, but takes the name away from user functions called `asm`. `__asm`
   is safer, but reads like an internal builtin, which does not fit a user-facing feature.
2. Constraint syntax: keep the LLVM syntax for good (recommended), or translate from the GCC syntax. Both are nearly
   identical, but GCC writes `"a"` where LLVM writes `{rax}`.
3. The compiler sources in `src/` can only use `asm` after a release and a stage0 bump.

## Further notes

- MemorySanitizer does not see writes performed in asm, so outputs produce false positives. Either document this or
  unpoison the outputs after the call.
- Debug info needs no work, the asm gets the location of the call.
