## What changed
<!-- 1-3 bullets describing the change -->

-

## Why
<!-- Motivation, linked issue, or design rationale -->

## How it was validated
<!-- Exact commands run + outcome, e.g.:
- `python test/run-tests.py --compiler build/spice --filter='IRGeneratorTests.*'`: all pass
- Ran `build/spice run scratch.spice`: output correct
If the environment could not run some checks, say so here. -->

-

## Follow-up / known limitations
<!-- Anything deferred, or limitations the reviewer should know. Write "None" if there are none. -->

## Checklist
- [ ] Branch is named `<type>/<slug>` (`feature/`, `fix/`, `chore/`, `ci/`, `std/`, `test/`, `docs/`, `security/`)
- [ ] Build passes (`python build.py`)
- [ ] Relevant tests pass (`python test/run-tests.py --compiler build/spice`)
- [ ] `src/` only uses features the pinned stage0 compiler supports
- [ ] Reference files updated and reviewed (`--update-refs` + `git diff`)
- [ ] Docs updated if behavior, CLI or error messages changed

<!-- Link the issue, e.g. "Fixes #1234" (auto-closes it on merge) or "Refs #1234" -->
Fixes #
