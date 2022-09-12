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
#include <util/FileUtil.h>

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

  // Create source file instance for main source file
  SourceFile mainSourceFile = SourceFile(options, nullptr, "root", options.mainSourceFile, false);
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