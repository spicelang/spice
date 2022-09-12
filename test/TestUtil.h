// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#pragma once

#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <util/FileUtil.h>

#ifdef __unix__
#define OS_UNIX
const char *const PATH_TEST_FILES = "./test-files/";
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
const char *const PATH_TEST_FILES = ".\\test-files\\";
#endif
const unsigned int EXPECTED_NUMBER_OF_TESTS = 250;
const unsigned int IR_FILE_SKIP_LINES = 4; // Skip the first couple of lines, because they contain target dependent information
extern bool updateRefs;

const char *const REF_NAME_SOURCE = "source.spice";
const char *const REF_NAME_SYNTAX_TREE = "syntax-tree.ast";
const char *const REF_NAME_SYMBOL_TABLE = "symbol-table.json";
const char *const REF_NAME_IR = "ir-code.ll";
const char *const REF_NAME_IR_O1 = "ir-code-O1.ll";
const char *const REF_NAME_IR_O2 = "ir-code-O2.ll";
const char *const REF_NAME_IR_O3 = "ir-code-O3.ll";
const char *const REF_NAME_IR_OS = "ir-code-Os.ll";
const char *const REF_NAME_IR_OZ = "ir-code-Oz.ll";
const char *const REF_NAME_EXECUTION_OUTPUT = "cout.out";
const char *const REF_NAME_ERROR_OUTPUT = "exception.out";
const char *const REF_NAME_EXIT_CODE = "exit-code.out";

const char *const CTL_SKIP_DISABLED = "disabled";
const char *const CTL_SKIP_GH = "skip-gh-actions";
const char *const CTL_NAME_LINKER_FLAGS = "linker-flags.txt";

struct TestCase {
  const std::string testSuite;
  const std::string testName;
  const std::string testPath;
};

// Typedefs
typedef const std::function<std::string(void)> &GetOutputFct;
typedef const std::function<void(std::string &expectedOutput, std::string &actualOutput)> &ModifyOutputFct;

class TestUtil {
public:
  // Public structs
  struct NameResolver {
    template <class T> std::string operator()(const testing::TestParamInfo<T> &info) const {
      T testCase = static_cast<T>(info.param);
      return TestUtil::toCamelCase(testCase.testSuite) + TestUtil::toCamelCase(testCase.testName);
    }
  };

  // Public static methods
  static std::vector<TestCase> collectTestCases(const std::string &suiteName);
  static void checkRefMatch(const std::string &refPath, GetOutputFct getActualOutput, ModifyOutputFct modifyOutput = {});
  static std::vector<std::string> getSubdirs(const std::string &basePath);
  static std::string getFileContent(const std::string &filePath);
  static std::vector<std::string> getFileContentLinesVector(const std::string &filePath);
  static std::string toCamelCase(std::string input);
  static std::string getDefaultExecutableName();
  static bool isUpdateRefsEnabled();
  static bool isDisabled(const TestCase &testCase);
};

// GCOV_EXCL_STOP