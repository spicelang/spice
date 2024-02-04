// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

namespace spice::compiler {

// Forward declarations
class SourceFile;

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string getLastFragment(const std::string &haystack, const std::string &needle);
  static std::string trim(const std::string &input);
  static std::vector<std::string> split(const std::string &input);
  static size_t getSystemPageSize();
  static bool isValidMangledName(const std::string &mangledName);
  static std::string getCircularImportMessage(const std::vector<const SourceFile *> &sourceFiles);
  static std::string getVersionInfo();
};

} // namespace spice::compiler