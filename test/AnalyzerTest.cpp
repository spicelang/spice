// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <stdexcept>

#include <gtest/gtest.h>

#include <antlr4-runtime.h>

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <analyzer/AnalyzerVisitor.h>
#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/LexerParserError.h>
#include <exception/SemanticError.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>
#include <util/ThreadFactory.h>

#include "TestUtil.h"

struct AnalyzerTestCase {
  const std::string testName;
  const std::string testPath;
};

typedef std::vector<AnalyzerTestCase> AnalyzerTestSuite;

std::vector<AnalyzerTestCase> detectAnalyzerTestCases(const std::string &suitePath) {
  std::vector<std::string> subDirs = TestUtil::getSubdirs(suitePath);

  std::vector<AnalyzerTestCase> testCases;
  testCases.reserve(subDirs.size());
  for (std::string &dirName : subDirs) {
    // Save test suite
    testCases.push_back({dirName, suitePath + FileUtil::DIR_SEPARATOR + dirName});
  }

  return testCases;
}

std::vector<AnalyzerTestSuite> detectAnalyzerTestSuites(const std::string &testFilesPath) {
  std::vector<std::string> subDirs = TestUtil::getSubdirs(testFilesPath);

  std::vector<AnalyzerTestSuite> testSuites;
  testSuites.reserve(subDirs.size());
  for (std::string &dirName : subDirs)
    testSuites.push_back(detectAnalyzerTestCases(testFilesPath + FileUtil::DIR_SEPARATOR + dirName));

  return testSuites;
}

void executeTest(const AnalyzerTestCase &testCase) {
  // Check if disabled
  std::string disabledFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "disabled";
  if (FileUtil::fileExists(disabledFile))
    GTEST_SKIP();
#ifdef SPICE_IS_GH_ACTIONS
  std::string disabledGHFile = testCase.testPath + "/disabled-gh-actions";
  if (FileUtil::fileExists(disabledGHFile))
    GTEST_SKIP();
#endif

  // Read source file
  std::string sourceFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "source.spice";
  std::ifstream sourceStream;
  sourceStream.open(sourceFile);
  if (!sourceStream)
    throw std::runtime_error("Test file '" + sourceFile + "' does not exist");
  antlr4::ANTLRInputStream input(sourceStream);

  // Create error handlers for lexer and parser
  AntlrThrowingErrorListener lexerErrorHandler = AntlrThrowingErrorListener(LEXER);
  AntlrThrowingErrorListener parserErrorHandler = AntlrThrowingErrorListener(PARSER);

  try {
    // Tokenize input
    SpiceLexer lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrorHandler);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource *)&lexer);

    // Parse input to AST
    SpiceParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrorHandler);

    // Prepare global LLVM assets
    std::shared_ptr<llvm::LLVMContext> context = std::make_shared<llvm::LLVMContext>();
    std::shared_ptr<llvm::IRBuilder<>> builder = std::make_shared<llvm::IRBuilder<>>(*context);

    // Prepare instance of thread factory, which has to exist exactly once per executable
    ThreadFactory threadFactory = ThreadFactory();

    // Create instance of cli options
    CliOptions options = {sourceFile, "", "", "", "", ".", ".", false, false, false, false, 0, false, true};
    CliInterface cli(options);
    cli.validate();
    cli.enrich();
    options = cli.getOptions();

    // Create main source file
    SourceFile mainSourceFile = SourceFile(options, nullptr, "root", sourceFile, false);

    // Execute pre-analyzer
    mainSourceFile.preAnalyze(options);

    // Check if the AST matches the expected output
    std::string astFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "syntax-tree.dot";
    if (FileUtil::fileExists(astFileName)) {
      // Execute visualizer
      mainSourceFile.visualizeAST(options, nullptr);

      std::string actualAST = mainSourceFile.compilerOutput.astString;
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(astFileName, actualAST);
      } else {
        std::string expectedAST = TestUtil::getFileContent(astFileName);
        EXPECT_EQ(expectedAST, mainSourceFile.compilerOutput.astString);
      }
    }

    // Execute semantic analysis
    mainSourceFile.analyze(context, builder, threadFactory);
    mainSourceFile.reAnalyze(context, builder, threadFactory);

    // Fail if an error was expected
    if (FileUtil::fileExists(testCase.testPath + FileUtil::DIR_SEPARATOR + "exception.out"))
      FAIL() << "Expected error, but got no error";

    // Check if the symbol table matches the expected output
    std::string symbolTableFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "symbol-table.json";
    if (FileUtil::fileExists(symbolTableFileName)) {
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(symbolTableFileName, mainSourceFile.compilerOutput.symbolTableString);
      } else {
        std::string expectedSymbolTable = TestUtil::getFileContent(symbolTableFileName);
        EXPECT_EQ(expectedSymbolTable, mainSourceFile.symbolTable->toJSON().dump(2));
      }
    }

    SUCCEED();
  } catch (LexerParserError &error) {
    // Check if the exception message matches the expected output
    std::string errorWhat = error.what();
    CommonUtil::replaceAll(errorWhat, "\\", "/");
    std::string exceptionFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "exception.out";
    if (FileUtil::fileExists(exceptionFile)) {
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(exceptionFile, errorWhat);
      } else {
        std::string expectedException = TestUtil::getFileContent(exceptionFile);
        EXPECT_EQ(std::string(errorWhat), expectedException);
      }
    } else {
      FAIL() << "Expected no error, but got '" << errorWhat << "'";
    }
  } catch (SemanticError &error) {
    // Check if the exception message matches the expected output
    std::string errorWhat = error.what();
    CommonUtil::replaceAll(errorWhat, "\\", "/");
    std::string exceptionFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "exception.out";
    if (FileUtil::fileExists(exceptionFile)) {
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(exceptionFile, errorWhat);
      } else {
        std::string expectedException = TestUtil::getFileContent(exceptionFile);
        EXPECT_EQ(std::string(errorWhat), expectedException);
      }
    } else {
      FAIL() << "Expected no error, but got '" << errorWhat << "'";
    }
  }
}

// Test classes

class AnalyzerArbitraryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerArrayTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerAssertionTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerBuiltinTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerComingSoonTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForEachLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionCallTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerGenericsTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerIfStatementTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerImportTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerLexerTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerLoopCtlInstTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerMethodTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerOperatorTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerParserTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerProcedureTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerStructTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerTernaryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerThreadTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerUnsafeTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerVariableTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerWhileLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};

// Test macros

TEST_P(AnalyzerArbitraryTests, ArbitraryTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerArrayTests, ArrayTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerAssertionTests, AssertionTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerBuiltinTests, BuiltinTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerComingSoonTests, ComingSoonTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerForLoopTests, ForLoopTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerForEachLoopTests, ForEachLoopTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerFunctionCallTests, FunctionCallTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerFunctionTests, FunctionTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerGenericsTests, GenericsTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerIfStatementTests, IfStatementTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerImportTests, ImportTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerLexerTests, LexerTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerLoopCtlInstTests, LoopCtlInstTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerMethodTests, MethodTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerOperatorTests, OperatorTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerParserTests, ParserTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerProcedureTests, ProcedureTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerStructTests, StructTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerTernaryTests, TernaryTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerThreadTests, TheadTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerUnsafeTests, TheadTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerVariableTests, VariableTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(AnalyzerWhileLoopTests, WhileLoopTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

// Name resolver

struct NameResolver {
  template <class AnalyzerTestCase> std::string operator()(const ::testing::TestParamInfo<AnalyzerTestCase> &info) const {
    auto testCase = static_cast<AnalyzerTestCase>(info.param);
    return TestUtil::toCamelCase(testCase.testName);
  }
};

// Instantiations

const std::string dirSepString = std::string(1, FileUtil::DIR_SEPARATOR);                           // NOLINT(cert-err58-cpp)
const std::string testRefsBasePath = "." + dirSepString + "test-files" + dirSepString + "analyzer"; // NOLINT(cert-err58-cpp)
const std::vector<AnalyzerTestSuite> testSuites = detectAnalyzerTestSuites(testRefsBasePath);       // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(AnalyzerArbitraryTests, AnalyzerArbitraryTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[0]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerArrayTests, AnalyzerArrayTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[1]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerAssertionTests, AnalyzerAssertionTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[2]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerBuiltinTests, AnalyzerBuiltinTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[3]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerComingSoonTests, AnalyzerComingSoonTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[4]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerForLoopTests, AnalyzerForLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[5]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerForEachLoopTests, AnalyzerForEachLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[6]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerFunctionCallTests, AnalyzerFunctionCallTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[7]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerFunctionTests, AnalyzerFunctionTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[8]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerGenericsTests, AnalyzerGenericsTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[9]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerIfStatementTests, AnalyzerIfStatementTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[10]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerImportTests, AnalyzerImportTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[11]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerLexerTests, AnalyzerLexerTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[12]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerLoopCtlInstTests, AnalyzerLoopCtlInstTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[13]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerMethodTests, AnalyzerMethodTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[14]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerOperatorTests, AnalyzerOperatorTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[15]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerParserTests, AnalyzerParserTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[16]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerProcedureTests, AnalyzerProcedureTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[17]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerStructTests, AnalyzerStructTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[18]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerTernaryTests, AnalyzerTernaryTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[19]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerTheadTests, AnalyzerThreadTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[20]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerUnsafeTests, AnalyzerUnsafeTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[21]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerVariableTests, AnalyzerVariableTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[22]), NameResolver());

INSTANTIATE_TEST_SUITE_P(AnalyzerWhileLoopTests, AnalyzerWhileLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[23]), NameResolver());

// GCOV_EXCL_STOP