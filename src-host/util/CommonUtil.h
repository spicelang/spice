// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

// Forward declarations
namespace llvm {
class Triple;
} // namespace llvm

namespace spice::compiler {

// Which compiler implementation this is. Tells the host compiler (src-host/) and the self-hosted compiler (src/) apart
const char *const COMPILER_IMPLEMENTATION = "host";

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string getLastFragment(const std::string &haystack, const std::string &needle);
  static std::string trim(const std::string &input);
  static std::vector<std::string> split(const std::string &input);
  static std::string formatBytes(size_t bytes);
  static std::string demangleTypeName(const char *mangledName);
  static bool isValidMangledName(const std::string &mangledName);
  static int getCurrentYear();
  static std::string buildVersionInfo();
};

} // namespace spice::compiler