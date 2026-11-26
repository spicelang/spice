---
name: spice-next-migration
description: Workflow for one work package (WP) of the compiler re-implementation in src-next/ (media/specs/compiler-reimplementation.md) - branch naming, reading the legacy code, porting behavior faithfully, the acceptance commands, the ratchet, -next ref variants, the deviations and port logs, model and effort choice, and the handover between sessions or accounts. Use it whenever you work on src-next/, the differential harness or the migration tooling.
---

# spice-next migration workflow

The compiler is re-implemented in `src-next/`, next to the legacy compiler in `src/`. The plan is
`media/specs/compiler-reimplementation.md`. Read the sections a WP refers to before you start; the plan is the source of
truth, this skill is the routine.

## 1. Start a work package

1. Find the WP in section 9 of the plan and its GitHub issue. The issue holds the progress and the handover notes of
   earlier sessions. Read it first.
2. Create the branch from the latest `main`: `<type>/next-<slug>`, e.g. `feature/next-wp-5b-calls`. The `next-` prefix
   turns on the migration guard (`.claude/hooks/guard-migration.py`): on such a branch, the legacy compiler in `src/`
   and the existing base reference files are read-only, and `run-tests.py --update-refs` needs `--variant next`.
   A legacy change, that the plan allows (e.g. a read-only dump flag), goes on a separate branch without the prefix.
3. Plan first (plan mode). Let the `legacy-explorer` agent extract the legacy behavior of the construct, instead of
   reading the large legacy files in the main session.

## 2. Rules (section 10.2 of the plan)

- Port behavior, re-home structure: the order of checks, bail-out points, diagnostics and the IR recipe stay exactly
  as in legacy. Mark each ported function with a reference comment like `// legacy: TC:5010-5359`.
- Layers and imports follow section 4.3. No global state, no `Any` and no string keys on hot paths, ids instead of
  cross-layer pointers. Files up to about 1,200 lines, functions up to about 150 lines.
- Never update base reference files. Intended differences go into `-next` variants (white-box dumps, section 8.6;
  issue fixes, section 8.8), with a reviewer's approval.
- A legacy bug is reproduced first. Then it is filed as an issue and fixed as a recorded deviation (section 8.8).
- A design question the plan does not answer: stop and ask. Record the decision in the plan afterwards.

## 3. Acceptance (once the phase 0 tooling exists)

```bash
python build.py --sources src-next --output-name spice-next
python test/run-tests.py --compiler build/spice-next --variant next --filter '<filter>'
python tools/diff_compilers.py --legacy build/spice --next build/spice-next --scope <scope>
python tools/check_layering.py src-next
python tools/gen/generate.py --check
```

The `diff-triager` agent runs the harness and groups the differences. Before the PR, let the `wp-reviewer` agent
review the diff. Then the definition of done of section 10.3 applies: the ratchet (`test/next-ratchet.txt`) is
updated, no ratcheted case regresses, unit tests exist, the port log and the deviations log are up to date.

## 4. Models and effort

| Work                                                                | Model and effort                                  |
|---------------------------------------------------------------------|---------------------------------------------------|
| Porting a well-specified construct, tooling, tests                  | Sonnet, `medium`                                  |
| Type checking, lifetimes, overloads, byte-identical IR, diff triage | Sonnet, `high`                                    |
| Designing a WP, SIR definitions, hard debugging                     | `opusplan`, or `ultrathink` for a single turn     |
| Agents                                                              | Sonnet (set in `.claude/agents/*.md`)             |

Avoid `max` effort, fast mode and agent teams. They use up the plan limits quickly.

## 5. Sessions, limits and handover

- One WP per session. Run `/clear` before the next WP. Long sessions resend the whole context with every request.
- All state lives in the repository and on GitHub, not in the conversation: the plan, the WP issue, the PR, the ratchet,
  the port log and the deviations log. That way any session, on any account, can continue.
- Before a session ends, or when a usage limit is close, write a handover comment on the WP issue: what is done, what
  is next, the exact commands, and open questions. Commit and push the work in progress first.
- A new session starts with: "Continue WP <n> of `media/specs/compiler-reimplementation.md`. Read the handover on
  issue #<issue> first."
