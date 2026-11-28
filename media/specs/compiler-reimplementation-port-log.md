# Port log of the compiler re-implementation

The legacy compiler (`src/`) keeps changing while the new compiler (`src-next/`) is built, see section 8.7 of
[compiler-reimplementation.md](compiler-reimplementation.md). This log lists every legacy commit after the fork point that
changes compiler behavior, with the new component it affects. A work package is not done until its port-log entries are
ported (definition of done, section 10.3).

## Fork point

**`a270b2a`** (2026-11-26): *Bump the stage0 compiler to 0.30.2 and re-enable the container move ctors (#1887)*. It is
the last commit that changed the legacy compiler, the std or the test cases before the plan (`a49ded7`) was merged. The
plan, the work packages of phase 0 (#1889 to #1894) and everything else up to the next entry changed only tooling,
documentation and `src-next/`. The plan and the numbers in it (line numbers of the legacy sources, the 78 AST node
kinds, the test counts) refer to this commit.

## What belongs into the log

A commit after the fork point that changes one of these, because the new compiler has to behave the same:

- anything below `src/`, i.e. diagnostics, semantics, IR generation, the formatter, the linter, CLI options
- the std, if the compiler relies on it (builtin tests, `core.*` linker flags)
- test cases or reference files below `test/test-files`, because the ratchet then shows them as failing for `spice-next`

A commit that only touches docs, CI, benchmarks, the std without a compiler dependency or `src-next/` is left out.

## How to keep it up to date

List the candidates since the last entry (or since the fork point, if there is none):

```sh
git log --first-parent --format='%h %ad %s' --date=short a270b2a..main -- src std test/test-files
```

Add one row per commit (or per PR) at the end of the table. Every session that starts a work package checks for new
candidates first, and a legacy PR that changes behavior adds its own row. The affected component is the owner from
appendix A of the plan. If a change has no counterpart yet, because the component is not ported, the status is `open`.
The work package that ports the component ports the entry and sets the status to `ported` with its PR.

| Status   | Meaning                                                                   |
|----------|---------------------------------------------------------------------------|
| `open`   | The new compiler does not contain the change yet                         |
| `ported` | Ported in the given PR, the differential harness shows no difference     |
| `n/a`    | Affects only code that the re-implementation does not carry over (reason) |

## Entries

| Commit | PR | Summary | Behavior change | New component | Status | Ported in |
|--------|----|---------|-----------------|---------------|--------|-----------|
| (this change) | | Fix the legacy lexer bugs #1906, #1907 and #1908 | `..` without a third `.`, a string that ends in a `\` and a block comment that ends in a `*` at the end of the input are lexer errors instead of failed assertions. `/**/` is an empty block comment and `**/` ends a block comment. An unterminated block or doc comment is the lexer error ``token recognition error at: '/*'`` (``'/**'`` for a doc comment) at its start. New LexerTests cases | `syntax/lexer` | `ported` | the follow-up of WP 2.2 (same change set) |
