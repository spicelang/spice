// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

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
#ifdef _WIN32
  static const char DIR_SEPARATOR = '\\';
#else
  static const char DIR_SEPARATOR = '/';
#endif

  static bool fileExists(const std::string &filePath);
  static void deleteFile(const std::string &filePath);
  static bool dirExists(const std::string &dirPath);
  static bool createDirs(const std::string &dirPath);
  static void writeToFile(const std::string &fileName, const std::string &fileContent);
  static std::string getFileName(const std::string &filePath);
  static std::string getFileDir(const std::string &filePath);
  static ExecResult exec(const std::string &cmd);
  static bool isCommandAvailable(const std::string &cmd);
  static std::string getStdDir();
  static std::string getSpiceBinDir();
};

} // namespace spice::compiler