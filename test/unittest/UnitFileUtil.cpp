// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <exception/CompilerError.h>
#include <util/FileUtil.h>

namespace spice::testing {

using namespace spice::compiler;

const auto TEST_FILE_NAME = "file-util-test-file.txt";
const std::filesystem::path TEST_FILE_PATH = std::filesystem::temp_directory_path() / TEST_FILE_NAME;
const std::string ERROR_MESSAGE = "[Error|Compiler]:\nI/O Error: Failed to open file: " + TEST_FILE_PATH.string();

TEST(FileUtilTest, WriteToAndReadFromFile) {
  const std::string expectedFileContent = "This is some test content";
  FileUtil::writeToFile(TEST_FILE_PATH, expectedFileContent);
  ASSERT_TRUE(exists(TEST_FILE_PATH));
  const std::string actualFileContent = FileUtil::getFileContent(TEST_FILE_PATH);
  ASSERT_EQ(expectedFileContent, actualFileContent);
  remove(TEST_FILE_PATH);
}

TEST(FileUtilTest, ReadFromFileNonExisting) {
  ASSERT_TRUE(!exists(TEST_FILE_PATH));
  try {
    FileUtil::getFileContent(TEST_FILE_PATH);
    FAIL();
  } catch (CompilerError &error) {
    ASSERT_EQ(ERROR_MESSAGE, error.what());
  }
}

TEST(FileUtilTest, GetLineCount) {
  const std::string expectedFileContent = "Line 1\nLine2\nLine3\n\nLine 5";
  FileUtil::writeToFile(TEST_FILE_PATH, expectedFileContent);
  ASSERT_TRUE(exists(TEST_FILE_PATH));
  const size_t lineCount = FileUtil::getLineCount(TEST_FILE_PATH);
  ASSERT_EQ(5, lineCount);
  remove(TEST_FILE_PATH);
}

TEST(FileUtilTest, GetLineCountNonExisting) {
  ASSERT_TRUE(!exists(TEST_FILE_PATH));
  try {
    FileUtil::getLineCount(TEST_FILE_PATH);
    FAIL();
  } catch (CompilerError &error) {
    ASSERT_EQ(ERROR_MESSAGE, error.what());
  }
}

} // namespace spice::testing