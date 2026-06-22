// Copyright (c) 2021-2026 ChilliBits. All rights reserved.
// LCOV_EXCL_START

#include <gtest/gtest.h>

#include <exception/CompilerError.h>
#include <util/FileUtil.h>

namespace spice::testing {

using namespace spice::compiler;

class FileUtilTest : public ::testing::Test {
protected:
  std::filesystem::path testFilePath;
  std::string errorMessage;

  void SetUp() override {
    const auto *testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    const std::string fileName = std::string("spice-file-util-") + testInfo->name() + ".txt";
    testFilePath = std::filesystem::temp_directory_path() / fileName;
    errorMessage = "[Error|Compiler]:\nI/O Error: Failed to open file: " + testFilePath.string();
    std::error_code ec;
    std::filesystem::remove(testFilePath, ec);
  }

  void TearDown() override {
    std::error_code ec;
    std::filesystem::remove(testFilePath, ec);
  }
};

TEST_F(FileUtilTest, WriteToAndReadFromFile) {
  const std::string expectedFileContent = "This is some test content";
  FileUtil::writeToFile(testFilePath, expectedFileContent);
  ASSERT_TRUE(exists(testFilePath));
  const std::string actualFileContent = FileUtil::getFileContent(testFilePath);
  ASSERT_EQ(expectedFileContent, actualFileContent);
}

TEST_F(FileUtilTest, ReadFromFileNonExisting) {
  ASSERT_TRUE(!exists(testFilePath));
  try {
    FileUtil::getFileContent(testFilePath);
    FAIL();
  } catch (CompilerError &error) {
    ASSERT_EQ(errorMessage, error.what());
  }
}

TEST_F(FileUtilTest, GetLineCount) {
  const std::string expectedFileContent = "Line 1\nLine2\nLine3\n\nLine 5";
  FileUtil::writeToFile(testFilePath, expectedFileContent);
  ASSERT_TRUE(exists(testFilePath));
  const size_t lineCount = FileUtil::getLineCount(testFilePath);
  ASSERT_EQ(5, lineCount);
}

TEST_F(FileUtilTest, GetLineCountNonExisting) {
  ASSERT_TRUE(!exists(testFilePath));
  try {
    FileUtil::getLineCount(testFilePath);
    FAIL();
  } catch (CompilerError &error) {
    ASSERT_EQ(errorMessage, error.what());
  }
}

} // namespace spice::testing

// LCOV_EXCL_STOP