// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <exception/CompilerError.h>
#include <util/FileUtil.h>

// LCOV_EXCL_START

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

class FileUtilPathTest : public ::testing::Test {
protected:
  std::filesystem::path testDir;
  std::filesystem::path realDir;
  std::filesystem::path linkDir;

  void SetUp() override {
    const auto *testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    testDir = std::filesystem::temp_directory_path() / (std::string("spice-file-util-") + testInfo->name());
    std::filesystem::remove_all(testDir);
    realDir = testDir / "real" / "nested";
    linkDir = testDir / "link-parent" / "link";
    create_directories(realDir);
    create_directories(linkDir.parent_path());
    FileUtil::writeToFile(realDir / "file.txt", "content");
    FileUtil::writeToFile(testDir / "other.txt", "content");
    // Creating symlinks requires special privileges on Windows
    std::error_code errorCode;
    std::filesystem::create_directory_symlink(realDir, linkDir, errorCode);
    if (errorCode)
      GTEST_SKIP() << "Could not create a directory symlink: " << errorCode.message();
  }

  void TearDown() override {
    std::error_code errorCode;
    std::filesystem::remove_all(testDir, errorCode);
  }
};

TEST_F(FileUtilPathTest, CanonicalResolvesSymlinks) {
  // Compare against FileUtil::canonical() of the real path, as std::filesystem::canonical() does not expand short (8.3)
  // names on Windows, which a temp dir path may contain
  const std::filesystem::path expected = FileUtil::canonical(realDir / "file.txt");
  ASSERT_TRUE(expected.is_absolute());
  ASSERT_EQ(expected, FileUtil::canonical(linkDir / "file.txt"));
  ASSERT_EQ(expected, FileUtil::canonical(linkDir / "." / "file.txt"));
}

TEST_F(FileUtilPathTest, CanonicalNonExisting) {
  ASSERT_THROW(FileUtil::canonical(testDir / "non-existing.txt"), std::filesystem::filesystem_error);
}

TEST_F(FileUtilPathTest, WeaklyCanonicalNonExistingRemainder) {
  const std::filesystem::path expected = FileUtil::canonical(realDir) / "missing" / "file.txt";
  ASSERT_EQ(expected, FileUtil::weaklyCanonical(linkDir / "missing" / "dir" / ".." / "file.txt"));
  ASSERT_EQ(std::filesystem::path("missing/file.txt").lexically_normal(), FileUtil::weaklyCanonical("missing/./file.txt"));
}

TEST_F(FileUtilPathTest, RelativeResolvesSymlinks) {
  // Through the symlink, the file lives three levels below the test dir, not two
  const std::filesystem::path relativePath = FileUtil::relative(testDir / "other.txt", linkDir / "file.txt");
  ASSERT_EQ("../../../other.txt", relativePath.generic_string());
  ASSERT_EQ("../real/nested/file.txt", FileUtil::relative(linkDir / "file.txt", testDir / "other.txt").generic_string());
}

} // namespace spice::testing

// LCOV_EXCL_STOP