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
#include <exception/ErrorFactory.h>
#include <exception/LexerParserError.h>
#include <linker/LinkerInterface.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>
#include <util/ThreadFactory.h>

#include "TestUtil.h"

const unsigned int IR_FILE_SKIP_LINES = 4;

struct StdTestCase {
  const std::string testName;
  const std::string testPath;
};

typedef std::vector<StdTestCase> StdTestSuite;

std::vector<StdTestCase> detectStdTestCases(const std::string &suitePath) {
  std::vector<std::string> subDirs = TestUtil::getSubdirs(suitePath);

  std::vector<StdTestCase> testCases;
  testCases.reserve(subDirs.size());
  for (std::string &dirName : subDirs) {
    // Save test suite
    testCases.push_back({dirName, suitePath + FileUtil::DIR_SEPARATOR + dirName});
  }

  return testCases;
}

std::vector<StdTestSuite> detectStdTestSuites(const std::string &testFilesPath) {
  std::vector<std::string> subDirs = TestUtil::getSubdirs(testFilesPath);

  std::vector<StdTestSuite> testSuites;
  testSuites.reserve(subDirs.size());
  for (std::string &dirName : subDirs)
    testSuites.push_back(detectStdTestCases(testFilesPath + FileUtil::DIR_SEPARATOR + dirName));

  return testSuites;
}

void executeTest(const StdTestCase &testCase) {
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

    // Create instance of error factory
    ErrorFactory err{};

    // Prepare instance of thread factory, which has to exist exactly once per executable
    ThreadFactory threadFactory = ThreadFactory();

    // Create instance of cli options
    CliOptions options = {sourceFile, "", "", "", "", ".", ".", false, false, false, false, false, 0, false, false, true};
    CliInterface cli(options);
    cli.validate();
    cli.enrich();
    options = cli.getOptions();

    // Create linker interface
    LinkerInterface linker = LinkerInterface(err, threadFactory, options);

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
      std::string actualSymbolTable = mainSourceFile.symbolTable->toJSON().dump(2);
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(symbolTableFileName, actualSymbolTable);
      } else {
        std::string expectedSymbolTable = TestUtil::getFileContent(symbolTableFileName);
        EXPECT_EQ(expectedSymbolTable, actualSymbolTable);
      }
    }

    // Determine the expected opt level
    std::string irCodeO1FileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code-O1.ll";
    std::string irCodeO2FileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code-O2.ll";
    std::string irCodeO3FileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code-O3.ll";
    std::string irCodeOsFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code-Os.ll";
    std::string irCodeOzFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code-Oz.ll";
    std::string irCodeOptFileName;
    std::string expectedOptIR;
    if (FileUtil::fileExists(irCodeO1FileName)) {
      options.optLevel = 1;
      irCodeOptFileName = irCodeO1FileName;
      expectedOptIR = TestUtil::getFileContent(irCodeO1FileName);
    } else if (FileUtil::fileExists(irCodeO2FileName)) {
      options.optLevel = 2;
      irCodeOptFileName = irCodeO2FileName;
      expectedOptIR = TestUtil::getFileContent(irCodeO2FileName);
    } else if (FileUtil::fileExists(irCodeO3FileName)) {
      options.optLevel = 3;
      irCodeOptFileName = irCodeO3FileName;
      expectedOptIR = TestUtil::getFileContent(irCodeO3FileName);
    } else if (FileUtil::fileExists(irCodeOsFileName)) {
      options.optLevel = 4;
      irCodeOptFileName = irCodeOsFileName;
      expectedOptIR = TestUtil::getFileContent(irCodeOsFileName);
    } else if (FileUtil::fileExists(irCodeOzFileName)) {
      options.optLevel = 5;
      irCodeOptFileName = irCodeOzFileName;
      expectedOptIR = TestUtil::getFileContent(irCodeOzFileName);
    }

    // Execute generator
    mainSourceFile.generate(context, builder, threadFactory, linker);

    // Check if the ir code matches the expected output
    std::string irCodeFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "ir-code.ll";
    if (FileUtil::fileExists(irCodeFileName)) {
      std::string actualIR = mainSourceFile.compilerOutput.irString;
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(irCodeFileName, actualIR);
      } else {
        std::string expectedIR = TestUtil::getFileContent(irCodeFileName);
        // Cut of first n lines to have a target independent
        for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
          expectedIR.erase(0, expectedIR.find('\n') + 1);
          actualIR.erase(0, actualIR.find('\n') + 1);
        }
        EXPECT_EQ(expectedIR, actualIR);
      }
    }

    // Check if the optimized ir code matches the expected output
    if (options.optLevel > 0) {
      std::string actualOptimizedIR = mainSourceFile.compilerOutput.irOptString;
      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(irCodeOptFileName, actualOptimizedIR);
      } else {
        // Cut of first n lines to have a target independent
        for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
          expectedOptIR.erase(0, expectedOptIR.find('\n') + 1);
          actualOptimizedIR.erase(0, actualOptimizedIR.find('\n') + 1);
        }
        EXPECT_EQ(expectedOptIR, actualOptimizedIR);
      }
    }

    // Check if the execution output matches the expected output
    std::string outputFileName = testCase.testPath + FileUtil::DIR_SEPARATOR + "cout.out";
    if (FileUtil::fileExists(outputFileName)) {
      // Prepare linker
      linker.setOutputPath(TestUtil::getDefaultExecutableName()); // Add output path

      std::string linkerFlagsFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "linker-flags.txt";
      std::string linkerFlags;
      if (FileUtil::fileExists(linkerFlagsFile)) {
        for (const auto &linkerFlag : TestUtil::getFileContentLinesVector(linkerFlagsFile))
          linker.addLinkerFlag(linkerFlag);
      }

      // Run linker
      linker.link();

      // Execute the program and get the output
      std::string actualOutput = FileUtil::exec(TestUtil::getDefaultExecutableName());

      if (TestUtil::isUpdateRefsEnabled()) {
        // Update ref
        FileUtil::writeToFile(outputFileName, actualOutput);
      } else {
        std::string expectedOutput = TestUtil::getFileContent(outputFileName);

        // Check if the outputs are matching
        EXPECT_EQ(expectedOutput, actualOutput);
      }
    }

    SUCCEED();
  } catch (LexerParserError &error) {
    std::string errorWhat = error.what();
    CommonUtil::replaceAll(errorWhat, "\\", "/");
    FAIL() << "Hit lexer/parser error: " << errorWhat;
  } catch (SemanticError &error) {
    // Check if the exception message matches the expected output
    std::string errorWhat = error.what();
    CommonUtil::replaceAll(errorWhat, "\\", "/");
    std::string exceptionFile = testCase.testPath + FileUtil::DIR_SEPARATOR + "exception.out";
    if (FileUtil::fileExists(exceptionFile)) {
      std::string expectedException = TestUtil::getFileContent(exceptionFile);
      EXPECT_EQ(std::string(errorWhat), expectedException);
    } else {
      FAIL() << "Expected no error, but got '" << errorWhat << "'";
    }
  }
}

// Test classes

class StdDataTests : public ::testing::TestWithParam<StdTestCase> {};
class StdIOTests : public ::testing::TestWithParam<StdTestCase> {};
class StdOSTests : public ::testing::TestWithParam<StdTestCase> {};
class StdTextTests : public ::testing::TestWithParam<StdTestCase> {};

// Test macros

TEST_P(StdDataTests, DataTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(StdIOTests, IOTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(StdOSTests, OSTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

TEST_P(StdTextTests, TextTests) { executeTest(GetParam()); } // NOLINT(cert-err58-cpp)

// Name resolver

struct NameResolver {
  template <class StdTestCase> std::string operator()(const ::testing::TestParamInfo<StdTestCase> &info) const {
    auto testCase = static_cast<StdTestCase>(info.param);
    return TestUtil::toCamelCase(testCase.testName);
  }
};

// Instantiations

const std::string dirSepString = std::string(1, FileUtil::DIR_SEPARATOR);                      // NOLINT(cert-err58-cpp)
const std::string testRefsBasePath = "." + dirSepString + "test-files" + dirSepString + "std"; // NOLINT(cert-err58-cpp)
const std::vector<StdTestSuite> testSuites = detectStdTestSuites(testRefsBasePath);            // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(StdDataTests, StdDataTests, ::testing::ValuesIn(testSuites[0]),
                         NameResolver()); // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(StdIOTests, StdIOTests, ::testing::ValuesIn(testSuites[1]), NameResolver()); // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(StdOSTests, StdOSTests, ::testing::ValuesIn(testSuites[2]), NameResolver()); // NOLINT(cert-err58-cpp)

INSTANTIATE_TEST_SUITE_P(StdTextTests, StdTextTests, ::testing::ValuesIn(testSuites[3]),
                         NameResolver()); // NOLINT(cert-err58-cpp)

// GCOV_EXCL_STOP