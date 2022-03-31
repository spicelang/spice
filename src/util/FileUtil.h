// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <cli/CliInterface.h>
#include <exception/ErrorFactory.h>

/**
 * Util class for file-related work
 */
class FileUtil {
public:
  static std::string getImportPath(const std::string &importPath, const std::string &sourceFile, const ErrorFactory *err,
                                   const antlr4::Token &token, const CliOptions *cliOptions);
  static bool fileExists(const std::string &filePath);
  static void deleteFile(const std::string &filePath);
  static bool dirExists(const std::string &dirPath);
  static bool createDirs(const std::string &dirPath);
  static std::string getFileName(const std::string &filePath);
  static std::string getFileDir(const std::string &filePath);
  static std::string exec(const std::string &cmd);
  static char getDirSeparator();
  static std::string getSpiceBinDir();
};