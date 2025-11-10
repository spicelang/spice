// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <util/FileUtil.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(FileUtilTest, WriteToAndReadFromFile) {
  const auto fileName = "write-to-file-test.txt";
  const std::string expectedFileContent = "This is some test content";
  const std::filesystem::path filePath = std::filesystem::temp_directory_path() / fileName;
  FileUtil::writeToFile(filePath, expectedFileContent);
  ASSERT_TRUE(exists(filePath));
  const std::string actualFileContent = FileUtil::getFileContent(filePath);
  ASSERT_EQ(expectedFileContent, actualFileContent);
}



} // namespace spice::testing