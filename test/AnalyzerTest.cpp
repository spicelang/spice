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

std::vector<TestCase> detectAnalyzerTestCases(const std::string &suitePath) {
  std::vector<std::string> subDirs = TestUtil::getSubdirs(suitePath);

  std::vector<TestCase> testCases;
  testCases.reserve(subDirs.size());
  for (std::string &dirName : subDirs)
    testCases.push_back({dirName, suitePath + FileUtil::DIR_SEPARATOR + dirName});

  return testCases;
}

std::vector<TestSuite> detectAnalyzerTestSuites() {
  std::string dirSepString(1, FileUtil::DIR_SEPARATOR);
  std::string testFilesPath = "." + dirSepString + "test-files" + dirSepString + "analyzer";
  std::vector<std::string> subDirs = TestUtil::getSubdirs(testFilesPath);

  std::vector<TestSuite> testSuites;
  testSuites.reserve(subDirs.size());
  for (std::string &dirName : subDirs)
    testSuites.push_back(detectAnalyzerTestCases(testFilesPath + FileUtil::DIR_SEPARATOR + dirName));

  return testSuites;
}

void executeAnalyzerTest(const TestCase &testCase) {
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
    CliOptions options = {sourceFile, "", "", "", "", ".", ".", false, false, false, false, false, false, 0, false, false, true};
    CliInterface cli(options);
    cli.validate();
    cli.enrich();
    options = cli.getOptions();

    // Create main source file
    SourceFile mainSourceFile = SourceFile(options, nullptr, "root", sourceFile, false);

    // Check if the CST matches the expected output
    std::string cstFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "parse-tree.dot";
    if (FileUtil::fileExists(cstFileName)) {
      // Execute visualizer
      mainSourceFile.visualizeCST(nullptr);

      std::string actualCST = mainSourceFile.compilerOutput.cstString;
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(cstFileName, actualCST);
      } else {
        std::string expectedCST = TestUtil::getFileContent(cstFileName);
        EXPECT_EQ(expectedCST, mainSourceFile.compilerOutput.cstString);
      }
    }

    // Execute AST builder
    mainSourceFile.buildAST();

    // Check if the AST matches the expected output
    std::string astFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "syntax-tree.dot";
    if (FileUtil::fileExists(astFileName)) {
      // Execute visualizer
      mainSourceFile.visualizeAST(nullptr);

      std::string actualAST = mainSourceFile.compilerOutput.astString;
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(astFileName, actualAST);
      } else {
        std::string expectedAST = TestUtil::getFileContent(astFileName);
        EXPECT_EQ(expectedAST, mainSourceFile.compilerOutput.astString);
      }
    }

    // Execute pre-analyzer
    mainSourceFile.preAnalyze();

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
class AnalyzerArbitraryTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerArrayTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerAssertionTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerBuiltinTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerComingSoonTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerForLoopTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerForEachLoopTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerFunctionCallTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerFunctionTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerGenericsTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerIfStatementTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerImportTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerLexerTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerLoopCtlInstTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerMethodTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerOperatorTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerParserTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerPointersTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerProcedureTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerStructTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerTernaryTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerThreadTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerUnsafeTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerVariableTests : public ::testing::TestWithParam<TestCase> {};
class AnalyzerWhileLoopTests : public ::testing::TestWithParam<TestCase> {};

// Test macros
TEST_P(AnalyzerArbitraryTests, ArbitraryTests) { executeAnalyzerTest(GetParam()); }       // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerArrayTests, ArrayTests) { executeAnalyzerTest(GetParam()); }               // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerAssertionTests, AssertionTests) { executeAnalyzerTest(GetParam()); }       // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerBuiltinTests, BuiltinTests) { executeAnalyzerTest(GetParam()); }           // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerComingSoonTests, ComingSoonTests) { executeAnalyzerTest(GetParam()); }     // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerForLoopTests, ForLoopTests) { executeAnalyzerTest(GetParam()); }           // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerForEachLoopTests, ForEachLoopTests) { executeAnalyzerTest(GetParam()); }   // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerFunctionCallTests, FunctionCallTests) { executeAnalyzerTest(GetParam()); } // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerFunctionTests, FunctionTests) { executeAnalyzerTest(GetParam()); }         // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerGenericsTests, GenericsTests) { executeAnalyzerTest(GetParam()); }         // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerIfStatementTests, IfStatementTests) { executeAnalyzerTest(GetParam()); }   // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerImportTests, ImportTests) { executeAnalyzerTest(GetParam()); }             // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerLexerTests, LexerTests) { executeAnalyzerTest(GetParam()); }               // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerLoopCtlInstTests, LoopCtlInstTests) { executeAnalyzerTest(GetParam()); }   // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerMethodTests, MethodTests) { executeAnalyzerTest(GetParam()); }             // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerOperatorTests, OperatorTests) { executeAnalyzerTest(GetParam()); }         // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerParserTests, ParserTests) { executeAnalyzerTest(GetParam()); }             // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerPointersTests, PointersTests) { executeAnalyzerTest(GetParam()); }         // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerProcedureTests, ProcedureTests) { executeAnalyzerTest(GetParam()); }       // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerStructTests, StructTests) { executeAnalyzerTest(GetParam()); }             // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerTernaryTests, TernaryTests) { executeAnalyzerTest(GetParam()); }           // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerThreadTests, TheadTests) { executeAnalyzerTest(GetParam()); }              // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerUnsafeTests, UsafeTests) { executeAnalyzerTest(GetParam()); }              // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerVariableTests, VariableTests) { executeAnalyzerTest(GetParam()); }         // NOLINT(cert-err58-cpp)
TEST_P(AnalyzerWhileLoopTests, WhileLoopTests) { executeAnalyzerTest(GetParam()); }       // NOLINT(cert-err58-cpp)

// Name resolver
struct NameResolver {
  template <class AnalyzerTestCase> std::string operator()(const ::testing::TestParamInfo<AnalyzerTestCase> &info) const {
    auto testCase = static_cast<AnalyzerTestCase>(info.param);
    return TestUtil::toCamelCase(testCase.testName);
  }
};

// Instantiations
const std::vector<TestSuite> testSuites = detectAnalyzerTestSuites(); // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(, AnalyzerArbitraryTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[0]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerArrayTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[1]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerAssertionTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[2]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerBuiltinTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[3]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerComingSoonTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[4]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerForLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[5]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerForEachLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[6]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerFunctionCallTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[7]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerFunctionTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[8]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerGenericsTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[9]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerIfStatementTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[10]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerImportTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[11]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerLexerTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[12]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerLoopCtlInstTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[13]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerMethodTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[14]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerOperatorTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[15]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerParserTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[16]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerPointersTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[17]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerProcedureTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[18]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerStructTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[19]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerTernaryTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[20]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerThreadTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[21]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerUnsafeTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[22]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerVariableTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[23]), NameResolver());
INSTANTIATE_TEST_SUITE_P(, AnalyzerWhileLoopTests, // NOLINT(cert-err58-cpp)
                         ::testing::ValuesIn(testSuites[24]), NameResolver());

// GCOV_EXCL_STOP