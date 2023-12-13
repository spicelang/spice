// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include "TestUtil.h"

#include <dirent.h>

#include <gtest/gtest.h>

#include "util/CommonUtil.h"
#include "util/FileUtil.h"

using namespace spice::compiler;

#ifdef OS_UNIX
#include <cstring> // Required by builds on Linux
#endif

/**
 * Collect the test cases in a particular test suite
 *
 * @param suitePath Path to the test suite
 * @param useSubDirs Use subdirectories as test cases
 * @return Vector of tests cases
 */
std::vector<TestCase> TestUtil::collectTestCases(const char *suiteName, bool useSubDirs) {
  const std::filesystem::path suitePath = std::filesystem::path(PATH_TEST_FILES) / suiteName;

  std::vector<TestCase> testCases;
  testCases.reserve(EXPECTED_NUMBER_OF_TESTS);

  if (useSubDirs) {
    // Collect subdirectories of the given suite
    const std::vector<std::string> testGroupDirs = TestUtil::getSubdirs(suitePath);

    // Convert them to test cases
    for (const std::string &groupDirName : testGroupDirs) {
      const std::filesystem::path groupPath = suitePath / groupDirName;
      for (const std::string &caseDirName : TestUtil::getSubdirs(groupPath)) {
        const std::filesystem::path testPath = groupPath / caseDirName;
        const TestCase tc = {toCamelCase(groupDirName), toCamelCase(caseDirName), testPath};
        testCases.push_back(tc);
      }
    }
  } else {
    // Collect test cases
    for (const std::string &caseDirName : TestUtil::getSubdirs(suitePath)) {
      const std::filesystem::path testPath = suitePath / caseDirName;
      const TestCase tc = {toCamelCase(suiteName), toCamelCase(caseDirName), testPath};
      testCases.push_back(tc);
    }
  }

  return testCases;
}

/**
 * Check if the expected output matches the actual output
 *
 * @param refPath Path to the reference file
 * @param callback Callback to execute the required steps to get the actual test output
 *
 * @return True, if the ref file was found
 */
bool TestUtil::checkRefMatch(const std::filesystem::path &refPath, GetOutputFct getActualOutput,
                             ModifyOutputFct modifyOutputFct) {
  // Cancel if the ref file does not exist
  if (!std::filesystem::exists(refPath))
    return false;

  // Get actual output
  std::string actualOutput = getActualOutput();
  if (updateRefs) { // Update refs
    FileUtil::writeToFile(refPath, actualOutput);
  } else { // Check refs
    std::string expectedOutput = FileUtil::getFileContent(refPath);
    modifyOutputFct(expectedOutput, actualOutput);
    EXPECT_EQ(expectedOutput, actualOutput) << "Output does not match the reference file: " << refPath;
  }

  return true;
}

/**
 * Handle an test error
 *
 * @param testCase Testcase which has produced the error
 * @param error Exception with error message
 */
void TestUtil::handleError(const TestCase &testCase, const std::exception &error) {
  std::string errorWhat = error.what();
  CommonUtil::replaceAll(errorWhat, "\\", "/");

  // Fail if no ref file exists
  const std::filesystem::path refPath = testCase.testPath / REF_NAME_ERROR_OUTPUT;
  if (!std::filesystem::exists(refPath))
    FAIL() << "Expected no error, but got: " + errorWhat;

  // Check if the exception message matches the expected output
  TestUtil::checkRefMatch(testCase.testPath / REF_NAME_ERROR_OUTPUT, [&]() { return errorWhat; });
}

/**
 * Get subdirectories of the given path
 *
 * @param basePath Path to a directory
 * @return Vector of subdirs
 */
std::vector<std::string> TestUtil::getSubdirs(const std::filesystem::path &basePath) {
  std::vector<std::string> subdirs;
  struct dirent *ent;
  if (DIR *dir = opendir(basePath.string().c_str()); dir != nullptr) {
    while ((ent = readdir(dir)) != nullptr) {
      if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
        subdirs.emplace_back(ent->d_name);
    }
    closedir(dir);
  }
  return subdirs;
}

/**
 * Retrieve the contents of a file as a vector of line strings. Empty lines are omitted
 *
 * @param filePath File path
 * @return Vector of strings which are the lines of the file
 */
std::vector<std::string> TestUtil::getFileContentLinesVector(const std::filesystem::path &filePath) {
  std::vector<std::string> lines;
  std::ifstream inputFileStream;
  inputFileStream.open(filePath);
  for (std::string line; std::getline(inputFileStream, line);) {
    if (!line.empty())
      lines.push_back(line);
  }
  return lines;
}

/**
 * Convert a string to camel case
 *
 * @param input Input string
 * @return Camel-cased string
 */
std::string TestUtil::toCamelCase(const std::string& input) {
  std::stringstream ss(input);
  std::string token;
  std::string camelCase;

  while (std::getline(ss, token, '-')) {
    camelCase += token[0];
    camelCase += token.substr(1);
  }

  return camelCase;
}

/**
 * Check if the provided test case is disabled
 *
 * @param testCase Test case to check
 * @param isGHActions Running tests with GitHub Actions
 * @return Disabled or not
 */
bool TestUtil::isDisabled(const TestCase &testCase, bool isGHActions) {
  if (std::filesystem::exists(testCase.testPath / CTL_SKIP_DISABLED))
    return true;
  if (isGHActions && std::filesystem::exists(testCase.testPath / CTL_SKIP_GH))
    return true;
  return false;
}

/**
 * Removes the first n lines of the IR code to not compare target dependent code
 *
 * @param irCode IR code to modify
 */
void TestUtil::eraseIRModuleHeader(std::string &irCode) {
  for (unsigned int i = 0; i < IR_FILE_SKIP_LINES; i++)
    irCode.erase(0, irCode.find('\n') + 1);
}

/**
 * Removes the first n lines of the GDB output to not compare target dependent code
 *
 * @param gdbOutput GDB output to modify
 */
void TestUtil::eraseGDBHeader(std::string &gdbOutput) {
  // Remove header
  size_t pos = gdbOutput.find(GDB_READING_SYMBOLS_MESSAGE);
  if (pos != std::string::npos) {
    const size_t lineStart = gdbOutput.rfind('\n', pos);
    if (lineStart != std::string::npos)
      gdbOutput.erase(0, lineStart + 1);
  }

  // Remove inferior message
  pos = gdbOutput.find(GDB_INFERIOR_MESSAGE);
  if (pos != std::string::npos)
    gdbOutput.erase(pos);
}

/**
 * Remove lines, containing a certain substring to make the IR string comparable
 *
 * @param irCode IR code to modify
 * @param needle Substring to search for
 */
void TestUtil::eraseLinesBySubstring(std::string &irCode, const char *const needle) {
  std::string::size_type pos = 0;
  while ((pos = irCode.find(needle, pos)) != std::string::npos) {
    // Find the start of the line that contains the substring
    std::string::size_type lineStart = irCode.rfind('\n', pos);
    if (lineStart == std::string::npos)
      lineStart = 0;
    else
      lineStart++; // move past the '\n'

    // Find the end of the line that contains the substring
    std::string::size_type lineEnd = irCode.find('\n', pos);
    if (lineEnd == std::string::npos)
      lineEnd = irCode.length();

    // Erase the line
    irCode.erase(lineStart, lineEnd - lineStart);
  }
}

// GCOV_EXCL_STOP