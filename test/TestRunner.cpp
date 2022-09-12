// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include "TestUtil.h"
#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/LexerParserError.h>
#include <exception/SemanticError.h>
#include <linker/LinkerInterface.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>
#include <util/ThreadFactory.h>

void execTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase))
    GTEST_SKIP();

  // Create fake cli options
  std::string sourceFilePath = testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SOURCE;
  llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
  CliOptions options = {/* mainSourceFile= */ sourceFilePath,
                        /* targetTriple= */ targetTriple.getTriple(),
                        /* targetArch= */ std::string(targetTriple.getArchName()),
                        /* targetVendor= */ std::string(targetTriple.getVendorName()),
                        /* targetOs= */ std::string(targetTriple.getOSName()),
                        /* outputDir= */ ".",
                        /* outputPath= */ ".",
                        /* printDebugOutput= */ false,
                        /* dumpCST= */ false,
                        /* dumpAST= */ false,
                        /* dumpIR= */ false,
                        /* dumpAssembly= */ false,
                        /* dumpSymbolTables= */ false,
                        /* optLevel= */ 0,
                        /* generateDebugInfo= */ false,
                        /* disableVerifier= */ false,
                        /* testMode= */ true};

  // Prepare resources
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  ThreadFactory threadFactory = ThreadFactory();
  LinkerInterface linker = LinkerInterface(threadFactory, options);

  try {
    // Create source file instance for main source file
    SourceFile mainSourceFile =
        SourceFile(&context, &builder, threadFactory, linker, options, nullptr, "root", options.mainSourceFile, false);

    // Check CST
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_PARSE_TREE, [&]() {
      std::string actualOutput;
      mainSourceFile.visualizeCST(&actualOutput);
      return actualOutput;
    });

    // Build AST
    mainSourceFile.buildAST();

    // Check AST
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SYNTAX_TREE, [&]() {
      std::string actualOutput;
      mainSourceFile.visualizeAST(&actualOutput);
      return actualOutput;
    });

    // Execute pre-analyzer and semantic analysis
    mainSourceFile.preAnalyze();
    mainSourceFile.analyze();
    mainSourceFile.reAnalyze();

    // Fail if an error was expected
    if (FileUtil::fileExists(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_ERROR_OUTPUT))
      FAIL() << "Expected error, but got no error";

    // Check SymbolTable
    TestUtil::checkRefMatch(testCase.testPath + FileUtil::DIR_SEPARATOR + REF_NAME_SYMBOL_TABLE,
                            [&]() { return mainSourceFile.symbolTable->toJSON().dump(2); });

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

class AnalyzerTests : public testing::TestWithParam<TestCase> {};
TEST_P(AnalyzerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, AnalyzerTests, testing::ValuesIn(TestUtil::collectTestCases("analyzer")), TestUtil::NameResolver());

class GeneratorTests : public testing::TestWithParam<TestCase> {};
TEST_P(GeneratorTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, GeneratorTests, testing::ValuesIn(TestUtil::collectTestCases("generator")), TestUtil::NameResolver());

class StdTests : public testing::TestWithParam<TestCase> {};
TEST_P(StdTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, StdTests, testing::ValuesIn(TestUtil::collectTestCases("std")), TestUtil::NameResolver());

// GCOV_EXCL_STOP