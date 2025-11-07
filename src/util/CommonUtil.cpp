// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CommonUtil.h"

#include <SourceFile.h> // Must be included before windows.h due to symbol name ambiguities

#include <cxxabi.h>
#if OS_UNIX
#include <unistd.h>
#elif OS_WINDOWS
#include <windows.h>
#else
#error "Unsupported platform"
#endif

#include <llvm/TargetParser/Triple.h>

namespace spice::compiler {

/**
 * Search all occurrences of needle in haystack and replace them with the replacement
 *
 * @param haystack Input string
 * @param needle String to search
 * @param replacement String to replace
 */
void CommonUtil::replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement) {
  size_t start_pos = 0;
  while ((start_pos = haystack.find(needle, start_pos)) != std::string::npos) {
    haystack.replace(start_pos, needle.length(), replacement);
    start_pos += replacement.length();
  }
}

/**
 * Split the given haystack by the needle and return the last fragment
 *
 * @param haystack Input string
 * @param needle String to search
 * @return Last fragment
 */
std::string CommonUtil::getLastFragment(const std::string &haystack, const std::string &needle) {
  const size_t index = haystack.rfind(needle);
  return index == std::string::npos ? haystack : haystack.substr(index + needle.length());
}

/**
 * Trim the given input string
 *
 * @param input Input string
 * @return Trimmed string
 */
std::string CommonUtil::trim(const std::string &input) {
  const size_t first = input.find_first_not_of(' ');
  if (first == std::string::npos)
    return input;
  const size_t last = input.find_last_not_of(' ');
  const size_t newLength = last - first + 1;
  return input.substr(first, newLength);
}

/**
 * Split the given input string by spaces
 *
 * @param input Input string
 * @return Vector of fragments
 */
std::vector<std::string> CommonUtil::split(const std::string &input) {
  std::vector<std::string> result;
  std::istringstream stream(trim(input));

  std::string token;
  while (std::getline(stream, token, ' '))
    result.push_back(trim(token));

  return result;
}

/**
 * Get the memory page size of the current system
 *
 * @return Page size in bytes
 */
size_t CommonUtil::getSystemPageSize() {
#if OS_UNIX
  return static_cast<size_t>(sysconf(_SC_PAGESIZE));
#elif OS_WINDOWS
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return static_cast<size_t>(si.dwPageSize);
#else
#error "Unsupported platform"
#endif
}

/**
 * Return the given number of bytes in a human-readable format
 *
 * @return Human-readable size string
 */
std::string CommonUtil::formatBytes(const size_t bytes) {
  const char *units[] = {"B", "KB", "MB", "GB", "TB"};

  auto size = static_cast<double>(bytes);
  unsigned int unitIndex = 0;
  while (size >= 1024 && unitIndex < std::size(units) - 1) {
    size /= 1024;
    unitIndex++;
  }

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%.2f %s", size, units[unitIndex]);
  return {buffer};
}

/**
 * Demangle CXX type name
 *
 * @param mangledName Mangled CXX type name
 * @return Demangled name
 */
std::string CommonUtil::demangleTypeName(const char *mangledName) {
  int status;
  char *demangled = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
  if (status == 0) {
    std::string result(demangled);
    free(demangled);
    return result;
  }
  return mangledName;
}

/**
 * Check if the given string is a valid, mangled name
 *
 * @return
 */
bool CommonUtil::isValidMangledName(const std::string &mangledName) {
  int status;
  char *demangled = abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);
  free(demangled);
  return status == 0;
}

/**
 * Retrieve OS name without any version information from a LLVM target triple
 *
 * @param targetTriple LLVM target triple
 * @return OS name
 */
std::string CommonUtil::getOSNameFromTargetTriple(const llvm::Triple *targetTriple) {
  return targetTriple->getOSTypeName(targetTriple->getOS()).str();
}

/**
 * Generate a circular import message from the given source files
 *
 * @param sourceFiles Source files that form the circular dependency chain
 * @return Error message
 */
std::string CommonUtil::getCircularImportMessage(std::stack<const SourceFile *> &sourceFiles) {
  std::stringstream message;
  message << "*-----*\n";
  message << "|     |\n";
  while (!sourceFiles.empty()) {
    message << "|  " << sourceFiles.top()->fileName << "\n";
    message << "|     |\n";
    sourceFiles.pop();
  }
  message << "*-----*";
  return message.str();
}

/**
 * Generate the version info string for the Spice driver
 *
 * @return Version info string
 */
std::string CommonUtil::buildVersionInfo() {
  std::stringstream versionString;
  versionString << "Spice version: " << SPICE_VERSION << " " << SPICE_TARGET_OS << "/" << SPICE_TARGET_ARCH << "\n";
  versionString << "Git hash:      " << SPICE_GIT_HASH << "\n";
  versionString << "LLVM version:  " << LLVM_VERSION_STRING << "\n";
  versionString << "built by:      " << SPICE_BUILT_BY << "\n\n";
  versionString << "(c) Marc Auberer 2021-2025";
  return versionString.str();
}

} // namespace spice::compiler