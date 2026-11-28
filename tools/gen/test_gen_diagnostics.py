#!/usr/bin/env python3
"""Unit tests of tools/gen/gen_diagnostics.py and extract_diagnostics_def.py. Run: python -m unittest tools/gen/test_gen_diagnostics.py"""
import sys
import tempfile
import unittest
from pathlib import Path

GEN_DIR = Path(__file__).resolve().parent
REPO_ROOT = GEN_DIR.parent.parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(GEN_DIR))
import extract_diagnostics_def as extract  # noqa: E402
import gen_diagnostics as gen  # noqa: E402

GOOD = """# comment
[Lexer]
enum = LexerErrorType
TOKENIZING_FAILED = Tokenizing failed

[Cli]
enum = CliErrorType
INCOMPATIBLE_OPTIONS = Incompatible options
INVALID_SANITIZER = Invalid sanitizer
"""


def generate(definition: str) -> str:
    with tempfile.TemporaryDirectory() as tmp:
        path = Path(tmp) / gen.DEF_PATH
        path.parent.mkdir(parents=True)
        path.write_text(definition, encoding="utf-8")
        return gen.generate(Path(tmp))[gen.OUT_PATH]


class ParseTest(unittest.TestCase):
    def test_parses_families_in_order(self):
        families = gen.parse(GOOD)
        self.assertEqual([f[0] for f in families], ["Lexer", "Cli"])
        self.assertEqual(families[1][2], [("INCOMPATIBLE_OPTIONS", "Incompatible options"), ("INVALID_SANITIZER", "Invalid sanitizer")])

    def test_rejects_malformed_definitions(self):
        bad = {
            "entry before family": "enum = X\n",
            "member before enum": "[A]\nFOO = x\n",
            "duplicate member": "[A]\nenum = AType\nFOO = x\nFOO = y\n",
            "duplicate family": "[A]\nenum = AType\nFOO = x\n[A]\nenum = BType\nBAR = y\n",
            "lower case member": "[A]\nenum = AType\nfoo = x\n",
            "quote in prefix": '[A]\nenum = AType\nFOO = a "b"\n',
            "family without members": "[A]\nenum = AType\n",
            "missing separator": "[A]\nenum = AType\nFOO\n",
        }
        for name, text in bad.items():
            with self.subTest(name), self.assertRaises(SystemExit):
                gen.parse(text)


class GenerateTest(unittest.TestCase):
    def test_output_has_header_enums_prefixes_and_tests(self):
        out = generate(GOOD)
        self.assertTrue(out.split("\n", 1)[0].endswith("do not edit"))
        self.assertIn("public type CliErrorType enum {\n    INCOMPATIBLE_OPTIONS,\n    INVALID_SANITIZER\n}", out)
        self.assertIn('case CliErrorType::INVALID_SANITIZER: { return "Invalid sanitizer"; }', out)
        self.assertIn("public f<unsigned int> getCliErrorTypeCount() {\n    return 2u;", out)
        self.assertIn("f<bool> testLexerErrorTypeCodes()", out)

    def test_generation_is_deterministic(self):
        self.assertEqual(generate(GOOD), generate(GOOD))


class CommittedDefinitionTest(unittest.TestCase):
    def test_counts_of_the_committed_definition(self):
        families = {f[0]: len(f[2]) for f in gen.parse((REPO_ROOT / gen.DEF_PATH).read_text(encoding="utf-8"))}
        self.assertEqual(families, {"Lexer": 1, "Parser": 9, "Semantic": 128, "Compiler": 13, "Linker": 5, "Cli": 13, "Warning": 22})

    def test_definition_matches_the_legacy_sources(self):
        # The legacy compiler is frozen until the cutover. A legacy PR that adds a code has to extend the definition as well
        self.assertEqual(extract.main(["--check"]), 0)


if __name__ == "__main__":
    unittest.main()
