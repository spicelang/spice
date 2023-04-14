// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include "util/TestUtil.h"

#include <SourceFile.h>
#include <cli/CLIInterface.h>
#include <exception/CompilerError.h>
#include <exception/IRError.h>
#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolTable.h>
#include <util/FileUtil.h>

using namespace spice::compiler;

void execTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase, skipNonGitHubTests))
    GTEST_SKIP();

  // Create fake cli options
  std::string sourceFilePath = testCase.testPath + spice::compiler::FileUtil::DIR_SEPARATOR + REF_NAME_SOURCE;
  llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
  CliOptions cliOptions = {/* mainSourceFile= */ sourceFilePath,
                           /* targetTriple= */ targetTriple.getTriple(),
                           /* targetArch= */ std::string(targetTriple.getArchName()),
                           /* targetVendor= */ std::string(targetTriple.getVendorName()),
                           /* targetOs= */ std::string(targetTriple.getOSName()),
                           /* execute= */ false, // If we set this to 'true', the compiler will not emit object files
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

    // Check symbol table output (check happens here, to include updates from type checker, borrow checker and escape analyzer)
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SYMBOL_TABLE,
                            [&]() { return mainSourceFile.globalScope->getSymbolTableJSON().dump(/*indent=*/2); });

    // Fail if an error was expected
    if (FileUtil::fileExists(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_ERROR_OUTPUT))
      FAIL() << "Expected error, but got no error";

    // Run backend for all dependencies
    for (auto &dependency : mainSourceFile.dependencies)
      dependency.second.first->runBackEnd();

    // Execute IR generator
    mainSourceFile.runIRGenerator();

    // Check unoptimized IR code
    TestUtil::checkRefMatch(
        testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_IR, [&]() { return mainSourceFile.compilerOutput.irString; },
        [&](std::string &expectedOutput, std::string &actualOutput) {
          // Cut of first n lines to be target independent
          TestUtil::eraseIRModuleHeader(expectedOutput);
          TestUtil::eraseIRModuleHeader(actualOutput);
        });

    // Check unoptimized IR code with debug info
    TestUtil::checkRefMatch(
        testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_IR_DEBUG_INFO,
        [&]() {
          cliOptions.generateDebugInfo = true;
          mainSourceFile.runIRGenerator();
          return mainSourceFile.compilerOutput.irString;
        },
        [&](std::string &expectedOutput, std::string &actualOutput) {
          // Cut of first n lines to be target independent
          TestUtil::eraseIRModuleHeader(expectedOutput);
          TestUtil::eraseIRModuleHeader(actualOutput);
          // Remove the lines, containing paths on the local file system
          TestUtil::eraseLinesBySubstring(expectedOutput, " = !DIFile(filename:");
          TestUtil::eraseLinesBySubstring(actualOutput, " = !DIFile(filename:");
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
            // Cut of first n lines to be target independent
            TestUtil::eraseIRModuleHeader(expectedOutput);
            TestUtil::eraseIRModuleHeader(actualOutput);
          });
    }

    // Check assembly code
    bool objectFilesEmitted = false;
    if (!skipNonGitHubTests) {
      TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_ASM, [&]() {
        mainSourceFile.runObjectEmitter();
        objectFilesEmitted = true;

        return mainSourceFile.compilerOutput.asmString;
      });
    }

    // Check warnings
    mainSourceFile.collectAndPrintWarnings();
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_WARNING_OUTPUT, [&]() {
      std::stringstream actualWarningString;
      for (const CompilerWarning &warning : mainSourceFile.compilerOutput.warnings)
        actualWarningString << warning.warningMessage << "\n";
      return actualWarningString.str();
    });

    // Check if the execution output matches the expected output
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_EXECUTION_OUTPUT, [&]() {
      // Prepare linker
      resourceManager.linker.outputPath = TestUtil::getDefaultExecutableName();

      // Parse linker flags
      const std::string linkerFlagsFile = testCase.testPath + FileUtil::DIR_SEPARATOR + CTL_NAME_LINKER_FLAGS;
      if (FileUtil::fileExists(linkerFlagsFile))
        for (const std::string &linkerFlag : TestUtil::getFileContentLinesVector(linkerFlagsFile))
          resourceManager.linker.addLinkerFlag(linkerFlag);

      // Emit main source file object if not done already
      if (!objectFilesEmitted)
        mainSourceFile.runObjectEmitter();

      // Conclude the compilation
      mainSourceFile.concludeCompilation();

      // Prepare and run linker
      resourceManager.linker.prepare();
      resourceManager.linker.link();

      // Execute binary
      const ExecResult result = FileUtil::exec(TestUtil::getDefaultExecutableName());

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
      // Check if the exit code matches the expected one
      const bool exitRefFileFound = TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_EXIT_CODE,
                                                            [&]() { return std::to_string(result.exitCode); });
      // If no exit code ref file exists, check against 0
      if (!exitRefFileFound)
        EXPECT_EQ(0, result.exitCode);
#endif

      return result.output;
    });
  } catch (LexerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (ParserError &error) {
    TestUtil::handleError(testCase, error);
  } catch (SemanticError &error) {
    TestUtil::handleError(testCase, error);
  } catch (IRError &error) {
    TestUtil::handleError(testCase, error);
  } catch (CompilerError &error) {
    TestUtil::handleError(testCase, error);
  }

  SUCCEED();
}

class CommonTests : public testing::TestWithParam<TestCase> {};
TEST_P(CommonTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, CommonTests, testing::ValuesIn(TestUtil::collectTestCases("common", false)), TestUtil::NameResolver());

class LexerTests : public testing::TestWithParam<TestCase> {};
TEST_P(LexerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, LexerTests, testing::ValuesIn(TestUtil::collectTestCases("lexer", false)), TestUtil::NameResolver());

class ParserTests : public testing::TestWithParam<TestCase> {};
TEST_P(ParserTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ParserTests, testing::ValuesIn(TestUtil::collectTestCases("parser", false)), TestUtil::NameResolver());

/*class SymbolTableBuilderTests : public testing::TestWithParam<TestCase> {};
TEST_P(SymbolTableBuilderTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, SymbolTableBuilderTests, testing::ValuesIn(TestUtil::collectTestCases("symboltablebuilder", true)),
                         TestUtil::NameResolver());*/

class TypeCheckerTests : public testing::TestWithParam<TestCase> {};
TEST_P(TypeCheckerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, TypeCheckerTests, testing::ValuesIn(TestUtil::collectTestCases("typechecker", true)),
                         TestUtil::NameResolver());

/*class BorrowCheckerTests : public testing::TestWithParam<TestCase> {};
TEST_P(BorrowCheckerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BorrowCheckerTests, testing::ValuesIn(TestUtil::collectTestCases("borrowchecker", true)),
                         TestUtil::NameResolver());*/

/*class EscapeAnalyzerTests : public testing::TestWithParam<TestCase> {};
TEST_P(EscapeAnalyzerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, EscapeAnalyzerTests, testing::ValuesIn(TestUtil::collectTestCases("borrowchecker", true)),
                         TestUtil::NameResolver());*/

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