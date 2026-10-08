// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

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
  static std::filesystem::path canonical(const std::filesystem::path &path);
  static std::filesystem::path weaklyCanonical(const std::filesystem::path &path);
  static std::filesystem::path relative(const std::filesystem::path &path,
                                        const std::filesystem::path &base = std::filesystem::current_path());
};

} // namespace spice::compiler