// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>

namespace spice::compiler {

/**
 * Util class for file-related work
 */
class FileUtil {
public:
  static void writeToFile(const std::filesystem::path &filePath, const std::string &fileContent);
  static std::string getFileContent(const std::filesystem::path &filePath);
  static size_t getLineCount(const std::filesystem::path &filePath);
};

} // namespace spice::compiler