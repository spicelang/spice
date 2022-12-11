// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include "symboltablebuilder/SymbolTable.h"
#include "util/TestUtil.h"
#include <SourceFile.h>
#include <cli/CLIInterface.h>
#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <util/FileUtil.h>

void execTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase, skipNonGitHubTests))
    GTEST_SKIP();

  // Create fake cli options
  std::string sourceFilePath = testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SOURCE;
  llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
  CliOptions cliOptions = {/* mainSourceFile= */ sourceFilePath,
                           /* targetTriple= */ targetTriple.getTriple(),
                           /* targetArch= */ std::string(targetTriple.getArchName()),
                           /* targetVendor= */ std::string(targetTriple.getVendorName()),
                           /* targetOs= */ std::string(targetTriple.getOSName()),
                           /* isNativeTarget= */ true,
                           /* cacheDir= */ "./cache",
                           /* outputDir= */ ".",
                           /* outputPath= */ ".",
                           /* compileJobCount= */ 0,
                           /* ignoreCache */ true,
                           /* printDebugOutput= */ false,
                           /* dumpCST= */ false,
                           /* dumpAST= */ false,
                           /* dumpIR= */ false,
                           /* dumpAssembly= */ false,
                           /* dumpSymbolTables= */ false,
                           /* disableAstOpt= */ false,
                           /* optLevel= */ 0,
                           /* generateDebugInfo= */ false,
                           /* disableVerifier= */ false,
                           /* testMode= */ true};

  // Instantiate GlobalResourceManager
  GlobalResourceManager resourceManager(cliOptions);

  try {
    // Create source file instance for main source file
    SourceFile mainSourceFile = SourceFile(resourceManager, nullptr, "root", cliOptions.mainSourceFile, false);

    // Run Lexer and Parser
    mainSourceFile.runLexer();
    mainSourceFile.runParser();

    // Check CST
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_PARSE_TREE, [&]() {
      mainSourceFile.runCSTVisualizer();
      return mainSourceFile.compilerOutput.cstString;
    });

    // Build and optimize AST
    mainSourceFile.runASTBuilder();
    mainSourceFile.runASTOptimizer();

    // Check AST
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SYNTAX_TREE, [&]() {
      mainSourceFile.runASTVisualizer();
      return mainSourceFile.compilerOutput.astString;
    });

    // Execute import collector and semantic analysis stages
    mainSourceFile.runImportCollector();
    mainSourceFile.runSymbolTableBuilder();
    mainSourceFile.runTypeChecker();
    mainSourceFile.runBorrowChecker();
    mainSourceFile.runEscapeAnalyzer();

    // Fail if an error was expected
    if (FileUtil::fileExists(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_ERROR_OUTPUT))
      FAIL() << "Expected error, but got no error";

    // Check SymbolTable
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SYMBOL_TABLE,
                            [&]() { return mainSourceFile.globalScope->symbolTable.toJSON().dump(2); });

    // Execute generator
    mainSourceFile.runIRGenerator();

    // Check unoptimized IR code
    TestUtil::checkRefMatch(
        testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_IR, [&]() { return mainSourceFile.compilerOutput.irString; },
        [&](std::string &expectedOutput, std::string &actualOutput) {
          // Cut of first n lines to have a target independent
          for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
            expectedOutput.erase(0, expectedOutput.find('\n') + 1);
            actualOutput.erase(0, actualOutput.find('\n') + 1);
          }
        });

    // Check optimized IR code
    for (short i = 1; i <= 5; i++) {
      TestUtil::checkRefMatch(
          testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_OPT_IR[i - 1],
          [&]() {
            cliOptions.optLevel = i;
            mainSourceFile.runIROptimizer();
            return mainSourceFile.compilerOutput.irOptString;
          },
          [&](std::string &expectedOutput, std::string &actualOutput) {
            // Cut of first n lines to have a target independent
            for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
              expectedOutput.erase(0, expectedOutput.find('\n') + 1);
              actualOutput.erase(0, actualOutput.find('\n') + 1);
            }
          });
    }

    // Check warnings
    mainSourceFile.collectAndPrintWarnings();
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_WARNING_OUTPUT, [&]() {
      std::string actualWarningString;
      for (const CompilerWarning &warning : mainSourceFile.compilerOutput.warnings)
        actualWarningString += warning.warningMessage + "\n";
      return actualWarningString;
    });

    // Check if the execution output matches the expected output
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_EXECUTION_OUTPUT, [&]() {
      // Prepare linker
      resourceManager.linker.outputPath = TestUtil::getDefaultExecutableName();

      // Parse linker flags
      std::string linkerFlagsFile = testCase.testPath + FileUtil::DIR_SEPARATOR + CTL_NAME_LINKER_FLAGS;
      if (FileUtil::fileExists(linkerFlagsFile)) {
        for (const std::string &linkerFlag : TestUtil::getFileContentLinesVector(linkerFlagsFile))
          resourceManager.linker.addLinkerFlag(linkerFlag);
      }

      // Emit object file
      mainSourceFile.runObjectEmitter();

      // Run linker
      resourceManager.linker.link();

      // Execute binary
      ExecResult result = FileUtil::exec(TestUtil::getDefaultExecutableName());

      EXPECT_EQ(0, result.exitCode);
      return result.output;
    });
  } catch (LexerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (ParserError &error) {
    TestUtil::handleError(testCase, error);
  } catch (SemanticError &error) {
    TestUtil::handleError(testCase, error);
  }

  SUCCEED();
}

class LexerTests : public testing::TestWithParam<TestCase> {};
TEST_P(LexerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, LexerTests, testing::ValuesIn(TestUtil::collectTestCases("lexer", false)), TestUtil::NameResolver());

class ParserTests : public testing::TestWithParam<TestCase> {};
TEST_P(ParserTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ParserTests, testing::ValuesIn(TestUtil::collectTestCases("parser", false)), TestUtil::NameResolver());

class SymbolTableBuilderTests : public testing::TestWithParam<TestCase> {};
TEST_P(SymbolTableBuilderTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, SymbolTableBuilderTests, testing::ValuesIn(TestUtil::collectTestCases("symboltablebuilder", true)),
                         TestUtil::NameResolver());

class TypeCheckerTests : public testing::TestWithParam<TestCase> {};
TEST_P(TypeCheckerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, TypeCheckerTests, testing::ValuesIn(TestUtil::collectTestCases("typechecker", true)),
                         TestUtil::NameResolver());

class BorrowCheckerTests : public testing::TestWithParam<TestCase> {};
TEST_P(BorrowCheckerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BorrowCheckerTests, testing::ValuesIn(TestUtil::collectTestCases("borrowchecker", true)),
                         TestUtil::NameResolver());

class EscapeAnalyzerTests : public testing::TestWithParam<TestCase> {};
TEST_P(EscapeAnalyzerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, EscapeAnalyzerTests, testing::ValuesIn(TestUtil::collectTestCases("borrowchecker", true)),
                         TestUtil::NameResolver());

class IRGeneratorTests : public testing::TestWithParam<TestCase> {};
TEST_P(IRGeneratorTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, IRGeneratorTests, testing::ValuesIn(TestUtil::collectTestCases("irgenerator", true)),
                         TestUtil::NameResolver());

class StdTests : public testing::TestWithParam<TestCase> {};
TEST_P(StdTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, StdTests, testing::ValuesIn(TestUtil::collectTestCases("std", true)), TestUtil::NameResolver());

class BenchmarkTests : public testing::TestWithParam<TestCase> {};
TEST_P(BenchmarkTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BenchmarkTests, testing::ValuesIn(TestUtil::collectTestCases("benchmark", false)),
                         TestUtil::NameResolver());

// GCOV_EXCL_STOP