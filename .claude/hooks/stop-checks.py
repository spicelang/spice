#!/usr/bin/env python3
# Stop hook. On migration branches of the compiler re-implementation ('<type>/next-<slug>'), it runs the fast checks of
# media/specs/compiler-reimplementation.md before Claude ends a turn:
# - 'tools/gen/generate.py --check': the generated AST, SIR, diagnostics and operator files are up to date (WP 0.5).
# - 'tools/check_layering.py src-next': no layering violations (WP 0.4).
# Each check runs only once its script exists. If a check fails, the hook keeps Claude working with the failure as the
# reason. It never blocks twice in a row (stop_hook_active), so a check that cannot be fixed does not loop.
import json
import os
import re
import subprocess
import sys

MIGRATION_BRANCH_REGEX = re.compile(r"^[a-z]+/next-[a-z0-9]+(-[a-z0-9]+)*$")


def run(args, cwd):
    try:
        out = subprocess.run(args, cwd=cwd, capture_output=True, text=True, timeout=120)
        return out.returncode, (out.stdout + out.stderr).strip()
    except Exception as e:
        return 1, str(e)


def main():
    try:
        data = json.load(sys.stdin)
    except Exception:
        return 0
    if data.get("stop_hook_active"):
        return 0
    root = os.environ.get("CLAUDE_PROJECT_DIR") or data.get("cwd") or os.getcwd()
    code, branch = run(["git", "rev-parse", "--abbrev-ref", "HEAD"], root)
    if code != 0 or not MIGRATION_BRANCH_REGEX.match(branch) or not os.path.isdir(os.path.join(root, "src-next")):
        return 0

    checks = [
        (os.path.join("tools", "gen", "generate.py"), ["--check"], "Generated files are out of date"),
        (os.path.join("tools", "check_layering.py"), ["src-next"], "Layering violations in src-next/"),
    ]
    failures = []
    for script, args, title in checks:
        if os.path.isfile(os.path.join(root, script)):
            code, output = run([sys.executable, script, *args], root)
            if code != 0:
                failures.append(f"{title} ('python {script} {' '.join(args)}'):\n{output}")

    if failures:
        print(json.dumps({"decision": "block", "reason": "Fix before finishing:\n\n" + "\n\n".join(failures)}))
    return 0


if __name__ == "__main__":
    sys.exit(main())
