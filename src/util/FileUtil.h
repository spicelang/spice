// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>

namespace spice::compiler {

struct ExecResult {
  std::string output;
  int exitCode;
};

/**
 * Util class for file-related work
 */
class FileUtil {
public:
  static void writeToFile(const std::filesystem::path &fileName, const std::string &fileContent);
  static std::string getFileContent(const std::filesystem::path &filePath);
  static ExecResult exec(const std::string &cmd);
  static bool isCommandAvailable(const std::string &cmd);
  static std::filesystem::path getStdDir();
  static std::filesystem::path getBootstrapDir();
  static std::filesystem::path getSpiceBinDir();
};

} // namespace spice::compiler