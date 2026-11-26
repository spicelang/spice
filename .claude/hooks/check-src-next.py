#!/usr/bin/env python3
# PostToolUse hook for the file edit tools. Keeps the re-implementation in src-next/ (see
# media/specs/compiler-reimplementation.md) formatted and within its layers:
# - Formats an edited src-next/**/*.spice file with the stage0 compiler, the same formatter the format CI job checks with.
#   If the file cannot be formatted, e.g. because it does not parse, the error is reported back to Claude.
# - Runs tools/check_layering.py on src-next/, once it exists (phase 0, WP 0.4), and reports violations back to Claude.
# Reports with exit code 2, which shows the message to Claude. Does nothing for files outside of src-next/.
import json
import os
import subprocess
import sys


def run(args, cwd, env=None):
    try:
        out = subprocess.run(args, cwd=cwd, capture_output=True, text=True, timeout=60, env=env)
        return out.returncode, (out.stdout + out.stderr).strip()
    except Exception as e:
        return 1, str(e)


def main():
    try:
        data = json.load(sys.stdin)
    except Exception:
        return 0
    root = os.environ.get("CLAUDE_PROJECT_DIR") or data.get("cwd") or os.getcwd()
    path = (data.get("tool_input", {}) or {}).get("file_path", "")
    if not path:
        return 0
    rel = os.path.relpath(os.path.abspath(os.path.join(root, path)), root)
    if rel.split(os.sep)[0] != "src-next" or not rel.endswith(".spice") or "spice_packages" in rel.split(os.sep):
        return 0

    problems = []
    stage0 = os.path.join(root, "build", "stage0", "spice")
    if os.path.isfile(stage0):
        env = dict(os.environ, SPICE_STD_DIR=os.path.join(root, "std"))
        code, output = run([stage0, "format", rel], root, env)
        if code != 0:
            problems.append(f"'{rel}' could not be formatted:\n{output}")

    layering_checker = os.path.join(root, "tools", "check_layering.py")
    if os.path.isfile(layering_checker):
        code, output = run([sys.executable, layering_checker, "src-next"], root)
        if code != 0:
            problems.append(f"Layering violations in src-next/ (see section 4.3 of the plan):\n{output}")

    if not problems:
        return 0
    print("\n\n".join(problems), file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
