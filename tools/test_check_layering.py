#!/usr/bin/env python3
"""Unit tests of tools/check_layering.py. Run: python -m unittest tools/test_check_layering.py"""
import contextlib
import io
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(TOOLS_DIR))
import check_layering as cl  # noqa: E402


def run_check(files: dict[str, str], *args: str) -> tuple[int, str]:
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp) / "src-next"
        for rel, content in files.items():
            path = root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            code = cl.main([str(root), *args])
        return code, out.getvalue()


def imp(target: str) -> str:
    return f'import "{target}";\n'


class ImportRules(unittest.TestCase):
    def assert_ok(self, source: str, target: str):
        code, out = run_check({source: imp(target), target[2:] + ".spice": ""})
        self.assertEqual(code, 0, out)

    def assert_violation(self, source: str, target: str):
        code, out = run_check({source: imp(target), target[2:] + ".spice": ""})
        self.assertEqual(code, 1, out)
        self.assertIn("must not import", out)

    def test_lower_layers(self):
        self.assert_ok("syntax/lexer.spice", "@/base/arena")
        self.assert_ok("syntax/lexer.spice", "@/diag/engine")
        self.assert_ok("modules/loader.spice", "@/syntax/parser/parser")
        self.assert_ok("sema/check/expr.spice", "@/modules/graph")
        self.assert_ok("driver/pipeline.spice", "@/codegen/llvm/unit")

    def test_upper_layers_are_forbidden(self):
        self.assert_violation("base/arena.spice", "@/source/span")
        self.assert_violation("source/span.spice", "@/diag/engine")
        self.assert_violation("syntax/lexer.spice", "@/modules/graph")
        self.assert_violation("modules/graph.spice", "@/sema/types/store")
        self.assert_violation("sema/types/store.spice", "@/codegen/target/info")
        self.assert_violation("sema/check/expr.spice", "@/driver/context/ctx")

    def test_sema_order(self):
        self.assert_ok("sema/check/expr.spice", "@/sema/overload/match")
        self.assert_ok("sema/overload/match.spice", "@/sema/types/store")
        self.assert_ok("sema/overload/match.spice", "@/sema/instantiate/memo")
        self.assert_violation("sema/types/store.spice", "@/sema/model/decl")
        self.assert_violation("sema/overload/match.spice", "@/sema/check/expr")
        self.assert_violation("sema/lifetime/state.spice", "@/sema/lower/pass")

    def test_sir_is_used_from_check_onwards(self):
        self.assert_ok("sema/check/expr.spice", "@/sema/sir/node")
        self.assert_ok("sema/lower/pass.spice", "@/sema/sir/node")
        self.assert_violation("sema/names/scope.spice", "@/sema/sir/node")
        self.assert_violation("sema/sir/node.spice", "@/sema/check/expr")

    def test_dump_and_warnings_read_everything(self):
        self.assert_ok("sema/dump/symtab.spice", "@/sema/check/expr")
        self.assert_ok("sema/warnings/unused.spice", "@/sema/lower/pass")
        self.assert_violation("sema/check/expr.spice", "@/sema/warnings/unused")

    def test_codegen(self):
        self.assert_ok("codegen/llvm/unit.spice", "@/sema/sir/node")
        self.assert_ok("codegen/llvm/unit.spice", "@/sema/types/store")
        self.assert_ok("codegen/llvm/unit.spice", "@/codegen/abi/sig")
        self.assert_violation("codegen/llvm/unit.spice", "@/sema/check/expr")
        self.assert_violation("codegen/llvm/unit.spice", "@/sema/overload/match")
        self.assert_violation("codegen/llvm/unit.spice", "@/sema/instantiate/memo")
        self.assert_violation("codegen/abi/sig.spice", "@/driver/options/opts")

    def test_link_and_tools(self):
        self.assert_ok("link/gcc.spice", "@/codegen/target/info")
        self.assert_violation("link/gcc.spice", "@/codegen/llvm/unit")
        self.assert_ok("tools/format/printer.spice", "@/syntax/ast/node")
        self.assert_violation("tools/format/printer.spice", "@/modules/graph")
        self.assert_ok("tools/lint/rule.spice", "@/modules/graph")
        self.assert_ok("tools/viz/dot.spice", "@/sema/names/scope")
        self.assert_violation("tools/lint/rule.spice", "@/sema/check/expr")
        self.assert_violation("tools/lint/rule.spice", "@/driver/cli/args")

    def test_same_component_and_root_file(self):
        self.assert_ok("syntax/parser/expr.spice", "@/syntax/lexer/lexer")
        self.assert_ok("main.spice", "@/driver/cli/cli")

    def test_std_and_packages(self):
        code, out = run_check({"base/a.spice": imp("std/io/file"), "syntax/b.spice": imp("@/spice_packages/x/y")})
        self.assertEqual(code, 0, out)
        code, out = run_check({"base/a.spice": imp("@/spice_packages/x/y")})
        self.assertEqual(code, 1, out)

    def test_relative_import(self):
        code, out = run_check({"base/a.spice": imp("../syntax/b")})
        self.assertEqual(code, 1, out)
        code, out = run_check({"syntax/a.spice": imp("./b")})
        self.assertEqual(code, 0, out)

    def test_unknown_component(self):
        code, out = run_check({"foo/a.spice": ""})
        self.assertEqual(code, 1, out)
        self.assertIn("not part of a known component", out)
        code, out = run_check({"syntax/a.spice": imp("@/foo/b")})
        self.assertEqual(code, 1, out)

    def test_reports_line_number(self):
        code, out = run_check({"base/a.spice": "// header\n\n" + imp("@/syntax/b")})
        self.assertEqual(code, 1)
        self.assertRegex(out, r"a\.spice:3: ")

    def test_imports_in_comments_are_ignored(self):
        code, out = run_check({"base/a.spice": '// import "@/syntax/b";\n'})
        self.assertEqual(code, 0, out)


    def test_import_split_over_lines(self):
        code, out = run_check({"base/a.spice": 'import\n    "@/syntax/b";\n'})
        self.assertEqual(code, 1, out)
        self.assertRegex(out, r"a\.spice:1: ")

    def test_imports_in_block_comments_are_ignored(self):
        code, out = run_check({"base/a.spice": '/*\nimport "@/syntax/b";\n*/\nimport "@/syntax/c";\n'})
        self.assertEqual(code, 1, out)
        self.assertRegex(out, r"a\.spice:4: ")
        self.assertNotIn("syntax/b", out)


class Budgets(unittest.TestCase):
    def body(self, header: str, lines: int) -> str:
        return header + " {\n" + "    int x = 1;\n" * (lines - 2) + "}\n"

    def test_function_budget_is_a_warning(self):
        text = self.body("f<int> big()", 151) + self.body("p ok()", 150)
        code, out = run_check({"base/a.spice": text})
        self.assertEqual(code, 0, out)
        self.assertIn("warning:", out)
        self.assertIn("'f<int> big()' has 151 lines", out)
        self.assertNotIn("'p ok()'", out)

    def test_strict_budgets_fail(self):
        code, _ = run_check({"base/a.spice": self.body("f<int> big()", 151)}, "--strict-budgets")
        self.assertEqual(code, 1)

    def test_file_budget(self):
        code, out = run_check({"base/a.spice": "// c\n" * 1201})
        self.assertEqual(code, 0, out)
        self.assertIn("file has 1201 lines", out)
        code, out = run_check({"base/a.spice": "// c\n" * 1200})
        self.assertNotIn("warning:", out)

    def test_method_with_braces_in_strings_and_comments(self):
        text = 'public f<int> Foo.bar(int a, Foo f = Foo{}) {\n    // }\n    printf("}{");\n    char c = \'}\';\n    return 1;\n}\n'
        self.assertEqual(cl.function_lengths(text), [(1, 6, "public f<int> Foo.bar(int a, Foo f = Foo{})")])

    def test_nested_generics_and_declarations_without_body(self):
        text = "ext f<int> puts(string s);\nf<Result<heap byte*>> alloc() {\n    if x {\n        return 1;\n    }\n}\n"
        self.assertEqual(cl.function_lengths(text), [(2, 5, "f<Result<heap byte*>> alloc()")])

    def test_whitespace_between_function_tokens(self):
        self.assertEqual(cl.function_lengths("f <int> big() {\n    return 1;\n}\n"), [(1, 3, "f <int> big()")])

    def test_operator_functions_are_counted(self):
        text = self.body("public f<bool> operator==(const Foo& a, const Foo& b)", 151)
        self.assertEqual(len(cl.function_lengths(text)), 1)
        text = self.body("f<bool> Foo.operator==(const Foo& other)", 151)
        self.assertEqual(len(cl.function_lengths(text)), 1)
        code, _ = run_check({"base/a.spice": text}, "--strict-budgets")
        self.assertEqual(code, 1)

    def test_function_type_field_is_no_declaration(self):
        text = "type S struct {\n    f<int>() callback\n}\n\n" + self.body("p proc()", 150)
        self.assertEqual(cl.function_lengths(text), [(5, 150, "p proc()")])
        code, out = run_check({"base/a.spice": text}, "--strict-budgets")
        self.assertEqual(code, 0, out)

    def test_external_packages_are_skipped(self):
        code, out = run_check({"spice_packages/x/y.spice": "// c\n" * 2000})
        self.assertEqual(code, 0, out)
        self.assertNotIn("warning:", out)


class Cli(unittest.TestCase):
    def test_missing_root(self):
        with contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(cl.main(["/nonexistent/src-next"]), 2)

    def test_current_src_next_is_clean(self):
        root = TOOLS_DIR.parent / "src-next"
        if root.is_dir():
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(cl.main([str(root)]), 0)


if __name__ == "__main__":
    unittest.main()
