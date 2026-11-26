#!/usr/bin/env python3
# PreToolUse hook for the file edit tools and the Bash tool.
# On migration branches of the compiler re-implementation ('<type>/next-<slug>', see the spice-next-migration skill), it
# enforces the rules of media/specs/compiler-reimplementation.md (section 10.2):
# - The legacy compiler in src/ is not modified. It is the oracle of the differential harness until cutover.
# - Existing base reference files under test/test-files are not changed. Intended differences of the new compiler go
#   into '-next' ref variants (sections 8.6 and 8.8).
# - 'run-tests.py --update-refs' is only used together with '--variant next', so it writes '-next' variants.
# On every branch, it also blocks force pushes ('--force', '-f', combined short flags with 'f', '+<refspec>'), which
# would overwrite remote history. '--force-with-lease' stays allowed.
# Blocks the call with exit code 2 and tells Claude why. Best-effort: writes through arbitrary shell commands are not
# detected.
import json
import os
import re
import shlex
import subprocess
import sys

MIGRATION_BRANCH_REGEX = re.compile(r"^[a-z]+/next-[a-z0-9]+(-[a-z0-9]+)*$")
EDIT_TOOLS = {"Edit", "Write", "MultiEdit", "NotebookEdit"}
# Extensions of the reference files, that test/run-tests.py compares against
REF_EXTENSIONS = {".out", ".ll", ".asm", ".dot", ".json"}
PLAN = "media/specs/compiler-reimplementation.md"


def git(args, cwd):
    try:
        out = subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True, timeout=5)
        return out.returncode, out.stdout.strip()
    except Exception:
        return 1, ""


def is_migration_branch(root):
    code, branch = git(["rev-parse", "--abbrev-ref", "HEAD"], root)
    return code == 0 and MIGRATION_BRANCH_REGEX.match(branch) is not None


def check_edited_path(path, root):
    """Return the reason, why editing the given path is not allowed, or None"""
    rel = os.path.relpath(os.path.abspath(os.path.join(root, path)), root)
    if rel.startswith(".."):
        return None
    parts = rel.split(os.sep)
    if parts[0] == "src":
        return (f"'{rel}' belongs to the legacy compiler, which stays unchanged on migration branches: it is the oracle "
                f"of the differential harness ({PLAN}, section 10.2). Port the behavior into src-next/ instead. A legacy "
                f"change, that the plan allows (e.g. a read-only dump flag), goes on a separate branch without the "
                f"'next-' prefix.")
    if len(parts) > 2 and parts[0] == "test" and parts[1] == "test-files":
        stem, ext = os.path.splitext(parts[-1])
        tracked = git(["ls-files", "--error-unmatch", rel], root)[0] == 0
        if ext in REF_EXTENSIONS and "-next" not in stem and tracked:
            return (f"'{rel}' is a base reference file, which stays unchanged until cutover. Write an intended "
                    f"difference of the new compiler into a '-next' variant instead (e.g. '{stem}-next{ext}'), see "
                    f"{PLAN}, sections 8.6 and 8.8.")
    return None


def split_command(command):
    """Split a shell command into the token lists of its segments. Shell comments are dropped, like the shell does"""
    for segment in re.split(r"&&|\|\||[;|\n]", command):
        try:
            yield shlex.split(segment, comments=True)
        except ValueError:
            continue


def is_update_refs_flag(token):
    # argparse accepts every unambiguous prefix of '--update-refs', e.g. '--update-ref'
    return len(token) >= len("--u") and "--update-refs".startswith(token)


def check_force_push(command):
    """Return the reason, why the given command is a force push, or None"""
    for tokens in split_command(command):
        if len(tokens) < 2 or tokens[0] != "git" or "push" not in tokens[1:]:
            continue
        args = tokens[tokens.index("push") + 1:]
        forced = any(arg == "--force" or arg.startswith("+") or (re.match(r"^-[a-zA-Z]+$", arg) and "f" in arg)
                     for arg in args)
        if forced:
            return ("Force pushes overwrite remote history and are not allowed. Push normally, or use "
                    "'--force-with-lease' on your own branch, if a rewrite is really needed.")
    return None


def check_bash_command(command):
    """Return the reason, why the given command is not allowed on a migration branch, or None"""
    for tokens in split_command(command):
        if not any(token.endswith("run-tests.py") for token in tokens) or not any(map(is_update_refs_flag, tokens)):
            continue
        uses_next_variant = "--variant=next" in tokens or any(
            token == "--variant" and i + 1 < len(tokens) and tokens[i + 1] == "next" for i, token in enumerate(tokens))
        if not uses_next_variant:
            return (f"'run-tests.py --update-refs' would overwrite base reference files, which stay unchanged until "
                    f"cutover. Add '--variant next' to write '-next' variants instead ({PLAN}, sections 8.6 and 8.8).")
    return None


def main():
    try:
        data = json.load(sys.stdin)
    except Exception:
        return 0
    root = os.environ.get("CLAUDE_PROJECT_DIR") or data.get("cwd") or os.getcwd()
    tool = data.get("tool_name", "")
    tool_input = data.get("tool_input", {}) or {}
    if tool == "Bash":
        reason = check_force_push(tool_input.get("command", ""))
        if reason is not None:
            print(f"Blocked: {reason}", file=sys.stderr)
            return 2
    if not is_migration_branch(root):
        return 0

    reason = None
    if tool in EDIT_TOOLS:
        path = tool_input.get("file_path") or tool_input.get("notebook_path") or ""
        reason = check_edited_path(path, root) if path else None
    elif tool == "Bash":
        reason = check_bash_command(tool_input.get("command", ""))

    if reason is None:
        return 0
    print(f"Blocked on a migration branch: {reason}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
