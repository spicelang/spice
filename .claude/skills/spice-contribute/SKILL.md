---
name: spice-contribute
description: Guidelines for contributing changes to the Spice GitHub repository — branch naming conventions, commit message format, PR workflow, no-push-to-main rule, and quality gates. Use when preparing a branch for review, writing a commit, opening a PR, or checking that a contribution is ready to merge.
---

# Contribute to Spice

This skill covers the full contribution workflow: naming branches, writing
commits, opening a PR, and satisfying the quality gates before merge.

## Branch naming

All work happens on a dedicated branch, never directly on `main`.

| Prefix | Use for |
|--------|---------|
| `feature/<slug>` | New language features, compiler capabilities, or stdlib additions |
| `fix/<slug>` | Bug fixes in the compiler, runtime, or stdlib |
| `chore/<slug>` | Build system, dependency updates, repo maintenance |
| `ci/<slug>` | GitHub Actions workflows, CI scripts, test infrastructure |
| `std/<slug>` | Standard library changes that aren't strictly a new feature or fix |
| `bootstrap/<slug>` | Changes to the self-hosted bootstrap compiler (`src-bootstrap/`) |
| `test/<slug>` | New or updated test cases, test utilities, reference files |
| `docs/<slug>` | Documentation-only changes under `docs/` |
| `security/<slug>` | Security-sensitive fixes (prefer non-public disclosure first — see `CONTRIBUTING.md`) |

Rules:
- Use lowercase `kebab-case` slugs (`fix/lambda-capture-crash`, not `Fix/LambdaCaptureCrash`).
- Keep slugs short but descriptive; the squash-commit headline is taken from the PR title.
- Tie the slug to the issue number when one exists: `fix/1234-lambda-capture-crash`.

```sh
git checkout main
git pull origin main
git checkout -b feature/my-new-thing
```

## Commit message format

Use a short, descriptive headline — no type prefix or scope annotation:

```
<Short descriptive headline>

[optional body — explain WHY, not what]

[optional footer — Fixes #<issue>, Closes #<issue>, Co-authored-by: …]
```

### Examples

```
Add support for lambda owning captures

Resolve incorrect type widening for unsigned ops

Fixes #1201
```

```
Bump minimum CMake version to 3.22
```

```
Add valgrind leak-check step to PR workflow
```

Rules:
- Headline ≤ 72 characters, imperative mood ("Add", "Fix", not "Added", "Fixes").
- No trailing period on the headline.
- Blank line between headline and body.
- Reference issues in the footer with `Fixes #N` (auto-closes) or `Refs #N`.

## No-push-to-main rule

`main` is a **protected branch**. Direct pushes are rejected. All changes must
arrive via a pull request that passes CI.

```sh
# Never do this:
git push origin main

# Always do this instead:
git push -u origin feature/my-new-thing
# … then open a PR via GitHub
```

If you accidentally commit to `main` locally, move the work to a branch before
pushing:

```sh
git checkout -b feature/my-new-thing   # create branch at current HEAD
git checkout main
git reset --hard origin/main           # restore main to remote state
git checkout feature/my-new-thing      # continue on the branch
```

## Pre-PR quality gate

Run these checks before opening (or requesting review on) a PR:

```sh
# 1. Build what you changed
cmake --build cmake-build-debug --target spice spicetest -j

# 2. Run affected tests (add --gtest_filter to narrow scope)
cmake-build-debug/test/spicetest

# 3. If you touched reference files, regenerate and review them
cmake-build-debug/test/spicetest --gtest_filter='<Suite>*<Case>*' --update-refs
git diff test/test-files   # review generated output before committing

# 4. If you changed stdlib or bootstrap, include relevant suite(s)
cmake-build-debug/test/spicetest --gtest_filter='StdTests*'
cmake-build-debug/test/spicetest --gtest_filter='BootstrapCompilerTests*'
```

If the environment cannot run the full suite (e.g. LLVM not installed), say so
explicitly in the PR description. Do not silently skip checks.

## Opening the pull request

Target branch: **`main`**.

PR title uses the same style as commit headlines: short, descriptive, imperative mood, no type prefix.

PR description template:

```markdown
## What changed
<!-- 1-3 bullets describing the change -->

## Why
<!-- Motivation, linked issue, or design rationale -->

## How it was validated
<!-- Exact commands run + outcome, e.g.: -->
- `cmake-build-debug/test/spicetest --gtest_filter='IRGeneratorTests*'` — all pass
- Manually ran `spice run scratch.spice` — output correct

## Follow-up / known limitations
<!-- Anything deferred, or limitations the reviewer should know -->

Fixes #<issue>
```

Additional rules:
- Keep PRs focused — one logical change per PR.
- Include updated reference files (`test/test-files/`) in the same PR as the
  code change that requires them; do not batch unrelated ref updates.
- Update `docs/docs/` if the PR changes user-visible CLI flags, language
  semantics, or error messages.
- Do not force-push a branch that already has a PR open (use a new commit
  instead) unless a reviewer explicitly asks for a rebase.

## Merge strategy

All PRs are merged by a maintainer using **squash and merge** — every PR lands
as a single commit on `main`. Merge commits are never used. Write clean
incremental commits on your branch so the reviewer can follow the progression;
they will be squashed at merge time.

## Checklist (use in PR description or locally before pushing)

- [ ] Branch named with the correct prefix and a descriptive slug
- [ ] All commits have a short descriptive headline (no type prefix)
- [ ] Build passes (`cmake --build cmake-build-debug --target spice spicetest`)
- [ ] Relevant tests pass (full suite or focused filter)
- [ ] Reference files updated and reviewed (`--update-refs` + `git diff`)
- [ ] Docs updated if behavior/CLI/error messages changed
- [ ] Issue linked in footer (`Fixes #N` or `Refs #N`)
- [ ] PR targets `main`, not another feature branch
