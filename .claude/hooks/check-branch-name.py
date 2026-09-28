#!/usr/bin/env python3
# PreToolUse hook for the Bash tool.
# Enforces the branch naming convention from AGENTS.md (<type>/<kebab-case-slug>)
# for git commands that create, rename or push a branch. Blocks the command with
# exit code 2 and tells Claude how to fix the name. Best-effort: commands it
# cannot parse are let through.
import json
import re
import shlex
import subprocess
import sys

PREFIXES = ["feature", "fix", "bug", "chore", "ci", "std", "bootstrap", "test", "docs", "security"]
BRANCH_NAME_REGEX = re.compile(r"^(" + "|".join(PREFIXES) + r")/[a-z0-9]+(-[a-z0-9]+)*$")
EXEMPT_BRANCHES = {"main", "HEAD"}
GIT_GLOBAL_OPTS_WITH_VALUE = {"-C", "-c", "--git-dir", "--work-tree", "--namespace"}
PUSH_OPTS_WITH_VALUE = {"-o", "--push-option", "--repo", "--receive-pack", "--exec"}
PUSH_SKIP_FLAGS = {"--tags", "--all", "--mirror", "-d", "--delete", "--prune"}
BRANCH_NON_CREATE_FLAGS = {"-d", "-D", "--delete", "-a", "--all", "-r", "--remotes", "-l", "--list", "-v", "-vv",
                           "--verbose", "--show-current", "-u", "--set-upstream-to", "--unset-upstream", "--contains",
                           "--no-contains", "--merged", "--no-merged", "--edit-description", "-c", "-C", "--copy",
                           "--points-at", "--sort", "--format", "--column", "--no-column"}


def current_branch(git_dir_args):
    try:
        out = subprocess.run(["git", *git_dir_args, "rev-parse", "--abbrev-ref", "HEAD"], capture_output=True,
                             text=True, timeout=5)
        return out.stdout.strip() if out.returncode == 0 else None
    except Exception:
        return None


def value_after(args, flags):
    for i, arg in enumerate(args):
        if arg in flags and i + 1 < len(args):
            return args[i + 1]
    return None


def positionals(args, opts_with_value=()):
    result = []
    skip_next = False
    for arg in args:
        if skip_next:
            skip_next = False
        elif arg in opts_with_value:
            skip_next = True
        elif not arg.startswith("-"):
            result.append(arg)
    return result


def branches_from_push(args, git_dir_args):
    if any(arg in PUSH_SKIP_FLAGS for arg in args):
        return []
    pos = positionals(args, PUSH_OPTS_WITH_VALUE)
    if len(pos) < 2:  # No refspec given -> pushes the current branch
        return [current_branch(git_dir_args)]
    names = []
    for refspec in pos[1:]:
        refspec = refspec.lstrip("+")
        src, _, dst = refspec.partition(":")
        if refspec.startswith(":"):  # Deletion
            continue
        name = dst or src
        if name == "HEAD":
            name = current_branch(git_dir_args)
        if name and name.startswith("refs/"):
            if not name.startswith("refs/heads/"):
                continue  # Tags, notes, etc.
            name = name[len("refs/heads/"):]
        names.append(name)
    return names


def branches_from_git_cmd(tokens):
    # Skip global git options to find the subcommand
    i = 1
    git_dir_args = []
    while i < len(tokens) and tokens[i].startswith("-"):
        if tokens[i] in GIT_GLOBAL_OPTS_WITH_VALUE and i + 1 < len(tokens):
            if tokens[i] == "-C":
                git_dir_args = ["-C", tokens[i + 1]]
            i += 1
        i += 1
    if i >= len(tokens):
        return []
    sub, args = tokens[i], tokens[i + 1:]

    if sub == "checkout":
        return [value_after(args, {"-b", "-B"})]
    if sub == "switch":
        return [value_after(args, {"-c", "-C", "--create", "--force-create"})]
    if sub == "worktree" and args[:1] == ["add"]:
        return [value_after(args, {"-b", "-B"})]
    if sub == "branch":
        pos = positionals(args)
        if any(arg in {"-m", "-M", "--move"} for arg in args):
            return [pos[-1]] if pos else []
        if any(arg in BRANCH_NON_CREATE_FLAGS for arg in args):
            return []
        return [pos[0]] if pos else []
    if sub == "push":
        return branches_from_push(args, git_dir_args)
    return []


def strip_redirections(tokens):
    # Drop shell redirections like '2>&1', '>out.log' or '> out.log'
    result = []
    skip_next = False
    for token in tokens:
        if skip_next:
            skip_next = False
        elif re.match(r"^(\d*|&)[<>]", token):
            skip_next = re.match(r"^(\d*|&)[<>]+&?$", token) is not None
        else:
            result.append(token)
    return result


def main():
    try:
        command = json.load(sys.stdin).get("tool_input", {}).get("command", "")
    except Exception:
        return 0

    invalid = []
    for segment in re.split(r"&&|\|\||[;|\n]", command):
        try:
            tokens = shlex.split(segment)
        except ValueError:
            continue
        # Drop leading env var assignments
        while tokens and re.match(r"^[A-Za-z_][A-Za-z0-9_]*=", tokens[0]):
            tokens.pop(0)
        tokens = strip_redirections(tokens)
        if not tokens or tokens[0] != "git":
            continue
        for name in branches_from_git_cmd(tokens):
            if name and name not in EXEMPT_BRANCHES and not BRANCH_NAME_REGEX.match(name):
                invalid.append(name)

    if not invalid:
        return 0
    print(f"Blocked: branch name(s) {', '.join(repr(n) for n in invalid)} violate the naming convention in AGENTS.md. "
          f"Use '<type>/<kebab-case-slug>' with type one of: {', '.join(PREFIXES)} "
          f"(e.g. 'fix/1409-condition-temporaries'). If you are on a pre-created branch, rename it first with "
          f"'git branch -m <type>/<slug>', then retry.", file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
