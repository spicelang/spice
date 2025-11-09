// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include "TestUtil.h"

#include <dirent.h>
#if OS_UNIX
#include <cstring> // Required by builds on Unix
#endif

#include <gtest/gtest.h>

#include <util/CommonUtil.h>
#include <util/FileUtil.h>

#include "../driver/Driver.h"

namespace spice::testing {

using namespace spice::compiler;

extern TestDriverCliOptions testDriverCliOptions;

/**
 * Collect the test cases in a particular test suite
 *
 * @param suiteName Name of the test suite
 * @param useSubDirs Use subdirectories as test cases
 * @return Vector of tests cases
 */
std::vector<TestCase> TestUtil::collectTestCases(const char *suiteName, bool useSubDirs) {
  const std::filesystem::path suitePath = std::filesystem::path(PATH_TEST_FILES) / suiteName;

  std::vector<TestCase> testCases;
  testCases.reserve(EXPECTED_NUMBER_OF_TESTS);

  if (useSubDirs) {
    // Collect subdirectories of the given suite
    const std::vector<std::string> testGroupDirs = getSubdirs(suitePath);

    // Convert them to test cases
    for (const std::string &groupDirName : testGroupDirs) {
      const std::filesystem::path groupPath = suitePath / groupDirName;
      for (const std::string &caseDirName : getSubdirs(groupPath)) {
        const std::filesystem::path testPath = groupPath / caseDirName;
        const TestCase tc = {toCamelCase(groupDirName), toCamelCase(caseDirName), testPath};
        testCases.push_back(tc);
      }
    }
  } else {
    // Collect test cases
    for (const std::string &caseDirName : getSubdirs(suitePath)) {
      const std::filesystem::path testPath = suitePath / caseDirName;
      const TestCase tc = {toCamelCase(suiteName), toCamelCase(caseDirName), testPath};
      testCases.push_back(tc);
    }
  }

  assert(testCases.size() <= EXPECTED_NUMBER_OF_TESTS);
  return testCases;
}

/**
 * Check if the expected output matches the actual output
 *
 * @param originalRefPath Path to the reference file
 * @param getActualOutput Callback to execute the required steps to get the actual test output
 * @param modifyOutputFct Callback to modify the output before comparing it with the reference
 * @param x86Only Only compare/update ref file on x86_64
 *
 * @return True, if the ref file was found
 */
bool TestUtil::checkRefMatch(const std::filesystem::path &originalRefPath, GetOutputFct getActualOutput,
                             ModifyOutputFct modifyOutputFct, [[maybe_unused]] bool x86Only) {
  for (const std::filesystem::path &refPath : expandRefPaths(originalRefPath)) {
    if (testDriverCliOptions.isVerbose)
      std::cout << "Checking for ref file: " << refPath << " - ";
    if (!exists(refPath)) {
      if (testDriverCliOptions.isVerbose)
        std::cout << "not found" << std::endl;
      continue;
    }
    if (testDriverCliOptions.isVerbose)
      std::cout << "ok" << std::endl;

    // Get actual output
    std::string actualOutput = getActualOutput();

#ifndef ARCH_X86_64
    // Cancel early, before comparing or updating the refs
    if (x86Only && refPath == originalRefPath)
      return true;
#endif

    if (testDriverCliOptions.updateRefs) { // Update refs
      FileUtil::writeToFile(refPath, actualOutput);
    } else { // Check refs
      std::string expectedOutput = FileUtil::getFileContent(refPath);
      modifyOutputFct(expectedOutput, actualOutput);
      EXPECT_EQ(expectedOutput, actualOutput) << "Output does not match the reference file: " << refPath;
    }
    return true;
  }
  return false;
}

/**
 * Check if a variant of the requested ref file was found
 *
 * @param originalRefPath Path to the reference file
 * @return True, if the ref file was found
 */
bool TestUtil::doesRefExist(const std::filesystem::path &originalRefPath) {
  const std::array<std::filesystem::path, 3> refPaths = expandRefPaths(originalRefPath);
  return std::ranges::any_of(refPaths, [](const std::filesystem::path &refPath) { return exists(refPath); });
}

/**
 * Handle a test error
 *
 * @param testCase Testcase which has produced the error
 * @param error Exception with error message
 */
void TestUtil::handleError(const TestCase &testCase, const std::exception &error) {
  std::string errorWhat = error.what();
  CommonUtil::replaceAll(errorWhat, "\\", "/");

  // Fail if no ref file exists
  const std::filesystem::path refPath = testCase.testPath / REF_NAME_ERROR_OUTPUT;
  if (!doesRefExist(refPath))
    FAIL() << "Expected no error, but got: " + errorWhat;

  // Check if the exception message matches the expected output
  checkRefMatch(refPath, [&] { return errorWhat; });
}

/**
 * Get subdirectories of the given path
 *
 * @param basePath Path to a directory
 * @return Vector of subdirs
 */
std::vector<std::string> TestUtil::getSubdirs(const std::filesystem::path &basePath) {
  std::vector<std::string> subdirs;
  if (DIR *dir = opendir(basePath.string().c_str()); dir != nullptr) {
    dirent *ent;
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
std::string TestUtil::toCamelCase(std::string input) {
  for (auto it = input.begin(); it != input.end(); ++it) {
    if (*it == '-' || *it == '_') {
      it = input.erase(it);
      *it = static_cast<char>(toupper(*it));
    }
  }
  return input;
}

/**
 * Check if the provided test case is disabled
 *
 * @param testCase Test case to check
 * @return Disabled or not
 */
bool TestUtil::isDisabled(const TestCase &testCase) {
  if (exists(testCase.testPath / CTL_SKIP_DISABLED))
    return true;
  if (testDriverCliOptions.isGitHubActions && exists(testCase.testPath / CTL_SKIP_GH))
    return true;
#ifdef OS_WINDOWS
  if (exists(testCase.testPath / CTL_SKIP_WINDOWS))
    return true;
#elifdef OS_MACOS
  if (exists(testCase.testPath / CTL_SKIP_MACOS))
    return true;
#endif
  return false;
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
    if (const size_t lineStart = gdbOutput.rfind('\n', pos); lineStart != std::string::npos)
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

std::array<std::filesystem::path, 3> TestUtil::expandRefPaths(const std::filesystem::path &refPath) {
  const std::filesystem::path parent = refPath.parent_path();
  const std::string stem = refPath.stem().string();
  const std::string ext = refPath.extension().string();
  // Construct array of files to search for
  const std::string osFileName = stem + "-" + SPICE_TARGET_OS + ext;
  const std::string osArchFileName = stem + "-" + SPICE_TARGET_OS + "-" + SPICE_TARGET_ARCH + ext;
  return {parent / osArchFileName, parent / osFileName, refPath};
}

} // namespace spice::testing

// GCOV_EXCL_STOP