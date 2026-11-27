#!/usr/bin/env python3
"""Unit tests of tools/ir_normalize.py and tools/diff_compilers.py. Run: python -m unittest tools/test_diff_tools.py

The harness is tested with fake compilers (small Python scripts that mimic the CLI dump flags), so that no real compiler is
needed.
"""
import json
import stat
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(TOOLS_DIR))
import ir_normalize as irn  # noqa: E402

IR = """; ModuleID = 'source'
%struct.Vec = type { i32 }

@anon.string.0 = private constant [3 x i8] c"hi\\00"

define dso_local i32 @main() #0 !dbg !10 {
entry:
  %result = alloca i32, align 4
  br label %cond.true.L3C4
cond.true.L3C4:                                   ; preds = %entry
  %0 = load i32, ptr %result, align 4, !dbg !11
  ret i32 %0
}

define void @helper() #1 {
entry:
  ret void
}

declare void @puts(ptr)

attributes #0 = { noinline }
attributes #1 = { nounwind }
!10 = distinct !DISubprogram(name: "main")
!11 = !DILocation(line: 3, scope: !10)
"""


class NormalizeTests(unittest.TestCase):
    def test_identical(self):
        self.assertEqual(irn.classify(IR, IR), [irn.IDENTICAL])

    def test_order(self):
        helper_start = IR.index("define void @helper")
        helper_end = IR.index("declare void")
        swapped = IR[:IR.index("define dso_local")] + IR[helper_start:helper_end] + IR[IR.index("define dso_local"):helper_start] \
            + IR[helper_end:]
        self.assertNotEqual(swapped, IR)
        self.assertEqual(irn.classify(IR, swapped), [irn.ORDER])

    def test_names(self):
        renamed = IR.replace("%result", "%res").replace("cond.true.L3C4", "if.then")
        self.assertEqual(irn.classify(IR, renamed), [irn.NAMES])

    def test_unnamed_values_are_names(self):
        self.assertEqual(irn.classify(IR, IR.replace("%0", "%tmp")), [irn.NAMES])

    def test_metadata_numbering(self):
        renumbered = IR.replace("!10", "!20").replace("!11", "!5").replace("#0", "#2").replace("#1", "#3")
        self.assertEqual(irn.classify(IR, renumbered), [irn.METADATA])

    def test_combined(self):
        other = IR.replace("%result", "%res").replace("!10", "!20")
        self.assertEqual(irn.classify(IR, other), [irn.NAMES, irn.METADATA])

    def test_structural(self):
        self.assertEqual(irn.classify(IR, IR.replace("align 4", "align 8")), [irn.STRUCTURAL])
        self.assertEqual(irn.classify(IR, IR.replace("ret void", "unreachable")), [irn.STRUCTURAL])

    def test_type_names_are_not_renamed(self):
        a = IR + "define void @f() {\n  %p = alloca %struct.Vec\n  ret void\n}\n"
        b = a.replace("%struct.Vec\n", "%struct.Other\n")
        self.assertEqual(irn.classify(a, b), [irn.STRUCTURAL])

    def test_first_diff_lines_limit(self):
        lines = irn.first_diff_lines("a\nb\nc", "a\nx\ny", limit=1)
        self.assertLessEqual(len(lines), 3)


FAKE_COMPILER = """#!{python}
import sys
from pathlib import Path
args = sys.argv[1:]
out = Path(args[args.index("--output") + 1]).parent
source = Path(args[-1])
mode = Path(__file__).with_suffix(".mode").read_text().strip()
o = next((a for a in args if a.startswith("-O")), "-O0")[2:]
text = source.read_text()
if "error" in text:
    print("[Error|Semantic] boom")
    sys.exit(1)
if "--dump-ir" in args:
    ir = "define i32 @main() {{\\nentry:\\n  %x = alloca i32\\n  ret i32 0\\n}}\\n"
    if mode == "names":
        ir = ir.replace("%x", "%y")
    if mode == "structural" and o == "2":
        ir = ir.replace("ret i32 0", "ret i32 1")
    (out / ("source-ir-code-O" + o + ".ll")).write_text(ir)
    (out / ("dep-ir-code-O" + o + ".ll")).write_text("declare void @f()\\n")
if "--dump-assembly" in args:
    (out / "source-assembly-code.s").write_text("ret\\n")
if "--dump-symtab" in args:
    (out / "source-symbol-table.json").write_text('{{"mode": "' + mode + '"}}')
if "--dump-ast" in args:
    print("Serialized AST:\\n\\n" + ("digraph {{ " + mode + " }}"))
"""


def make_compiler(directory: Path, name: str, mode: str) -> Path:
    path = directory / name
    path.write_text(FAKE_COMPILER.format(python=sys.executable))
    path.chmod(path.stat().st_mode | stat.S_IEXEC)
    path.with_suffix(".mode").write_text(mode)
    return path


class HarnessTests(unittest.TestCase):
    def run_harness(self, legacy_mode: str, next_mode: str, extra: list[str] | None = None):
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            legacy = make_compiler(tmp, "legacy", legacy_mode)
            nxt = make_compiler(tmp, "next", next_mode)
            report = tmp / "report.json"
            proc = subprocess.run([sys.executable, str(TOOLS_DIR / "diff_compilers.py"), "--legacy", str(legacy), "--next",
                                   str(nxt), "--scope", "compiler,std", "--filter", "std.io_filepath:compiler.*",
                                   "--json", str(report), *(extra or [])],
                                  capture_output=True, text=True)
            return proc, json.loads(report.read_text()) if report.exists() else None

    def test_same_compiler_has_no_differences(self):
        proc, report = self.run_harness("same", "same")
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
        self.assertTrue(all(not c["differences"] and not c["error"] for c in report))
        self.assertIn("0 gating differences", proc.stdout)

    def test_name_difference_is_classified(self):
        proc, report = self.run_harness("same", "names", ["--stages", "ir-O0"])
        self.assertEqual(proc.returncode, 1)
        diffs = [d for c in report for d in c["differences"]]
        self.assertTrue(diffs)
        self.assertTrue(all(d["classes"] == ["names"] and d["stage"] == "ir-O0" for d in diffs), diffs)

    def test_structural_difference_only_at_O2(self):
        proc, report = self.run_harness("same", "structural", ["--stages", "ir-O0,ir-O2"])
        self.assertEqual(proc.returncode, 1)
        stages = {d["stage"] for c in report for d in c["differences"]}
        self.assertEqual(stages, {"ir-O2"})

    def test_whitebox_differences_do_not_gate(self):
        proc, report = self.run_harness("same", "names", ["--stages", "whitebox-console,whitebox-files"])
        self.assertEqual(proc.returncode, 0, proc.stdout)
        diffs = [d for c in report for d in c["differences"]]
        self.assertTrue(diffs and all(not d["gating"] for d in diffs))



class DocsSourceTest(unittest.TestCase):
    def test_docs_example_is_written_unchanged(self):
        import diff_compilers as dc
        code = "// see https://example.org//path\nf<int> main() {\r\n    return 0;\n}\n"
        with tempfile.TemporaryDirectory() as tmp:
            case = dc.Case("docs", "docs", "docs.x_1", Path(tmp) / "x.md", text=code)
            source = dc.prepare_case_source(case, Path(tmp))
            self.assertEqual(source.read_bytes().decode("utf-8"), code)


if __name__ == "__main__":
    unittest.main()
