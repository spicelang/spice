# Deviations log of the compiler re-implementation

Until cutover, the new compiler (`src-next/`) reproduces every observable output of the legacy compiler byte for byte,
including legacy bugs (decision D1). The only exceptions are the white-box dumps (D2, section 8.6) and the intended
deviations in this log: GitHub issues that are fixed in the new compiler instead of the legacy one (D7, section 8.8). See
[compiler-reimplementation.md](compiler-reimplementation.md).

The differential harness (`tools/diff_compilers.py`) groups its differences by these entries. **A difference without an
entry here is a porting bug.**

## Rules

1. **Parity first.** A fix lands in its own work package, after the owning component has passed its parity gate.
2. **One entry per fix.** It names the issue, the changed behavior, the affected test cases and corpus modules and the work
   package. The affected cases are filled in when the fix lands, with the exact `Suite.Case` names of the ratchet.
3. **New tests** for the issue carry a `next-only` control file. Existing refs that the fix changes get `-next` variants
   (for example `ir-code-next.ll`, `cout-next.out`). The legacy compiler keeps the original refs.
4. **Cutover.** The cutover PR renames the `-next` variants to the base refs, removes the `next-only` markers and closes
   the issues (`Fixes #…`).
5. **New bugs.** A legacy bug found during the port is reproduced first, filed as an issue, and then added here.

| Status    | Meaning                                                              |
|-----------|----------------------------------------------------------------------|
| `planned` | In the plan (section 8.8), the work package has not started          |
| `landed`  | Fixed in the new compiler, tests and `-next` variants are in `main`  |
| `shipped` | The cutover renamed the variants and closed the issue                |

## Entries

| Issue | Changed behavior | Affected cases | Work package | Status |
|-------|------------------|----------------|--------------|--------|
| none (design of 5.2, found in WP 1.3) | The snippet of a diagnostic is cut from the buffer of the `SourceManager`, so a source that was read from memory has a snippet. Legacy re-read the file from disk, which fails for the `<stdin>` source of `spice format`, so it printed none. The scope is empty today: only `spice format` reads stdin, and it reports lexer and parser errors, which never carry a snippet. The difference shows only if a later phase attaches a snippet to a diagnostic of an in-memory source | None. Unit tests of `getSnippet` use in-memory sources | 1.3 | `landed` |
| [#1835](https://github.com/spicelang/spice/issues/1835) | A keyword used as a variable name is reported with the reserved-keyword error instead of "unexpected token" | New parser tests. An existing `exception.out` changes only if a case uses a keyword as a name | 2.7, after 2.3 | `planned` |
| [#1827](https://github.com/spicelang/spice/issues/1827) | `const` is enforced for method calls. A warning while the std or the compiler sources still violate it, then an error | New diagnostics tests | 5j, after 5b | `planned` |
| [#1825](https://github.com/spicelang/spice/issues/1825) | Assigning `result` twice no longer leaks the first value (the old value is destructed before the copy) | IR and output of cases that assign `result` twice | 5j, after 5c | `planned` |
| [#1812](https://github.com/spicelang/spice/issues/1812) | `Result` and `Optional` no longer construct, copy or destruct an uninitialized payload (fields with manual lifetime, D8). `Result` and `Optional` adopt the mechanism after cutover, once stage0 supports it | IR of the implicit members of structs with such fields | 5j, after 5c | `planned` |
| [#1906](https://github.com/spicelang/spice/issues/1906) | `..` without a third `.`, a string that ends in a `\`, and a block comment that ends in a `*` at the end of the input are lexer errors instead of failed assertions | New LexerTests cases, no `-next` variant: the legacy lexer got the same fix | Legacy PR and the follow-up of 2.2 | `landed` |
| [#1907](https://github.com/spicelang/spice/issues/1907) | `/**/` is an empty block comment and `**/` ends a block comment | New LexerTests cases, no `-next` variant: the legacy lexer got the same fix | Legacy PR and the follow-up of 2.2 | `landed` |
| [#1908](https://github.com/spicelang/spice/issues/1908) | An unterminated block comment is a lexer error | New LexerTests cases, no `-next` variant: the legacy lexer got the same fix. No file of the corpus is affected | Legacy PR and the follow-up of 2.2 | `landed` |
| [#1868](https://github.com/spicelang/spice/issues/1868) | A returned local in a generic function or method is handed over instead of copied | IR of generic functions that return a local, output of cases that count copies | 5j, after 5d | `planned` |
| [#1832](https://github.com/spicelang/spice/issues/1832) | The linker toolchain is found on `PATH` and can be overridden | None. New unit tests for the lookup | 7.7, after 7.1 | `planned` |
| [#1833](https://github.com/spicelang/spice/issues/1833) | The arguments after `--` are passed to the program in `spice run` | None. New tests | 7.7, after 7.3 | `planned` |
| [#1830](https://github.com/spicelang/spice/issues/1830) | Signed integer overflow traps in debug builds. Phase 9, the first change after cutover, so no deviation entry is needed before it | The IR of every debug-build case with signed arithmetic (66 of the O0 IR refs today) | Phase 9 | `planned` |
