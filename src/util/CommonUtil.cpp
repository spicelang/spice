// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CommonUtil.h"

#include <cxxabi.h>

#include <SourceFile.h>

#ifdef OS_WINDOWS
#include <windows.h>
#elif OS_UNIX
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

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
  size_t first = input.find_first_not_of(' ');
  if (std::string::npos == first)
    return input;
  size_t last = input.find_last_not_of(' ');
  return input.substr(first, (last - first + 1));
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
#ifdef OS_WINDOWS
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return static_cast<size_t>(si.dwPageSize);
#elif OS_UNIX
  return static_cast<size_t>(sysconf(_SC_PAGESIZE));
#else
#error "Unsupported platform"
#endif
}

/**
 * Check if the given string is a valid mangled name
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
 * Generate a circular import message from the given source files
 *
 * @param sourceFiles Source files building the circular dependency chain
 * @return Error message
 */
std::string CommonUtil::getCircularImportMessage(const std::vector<const SourceFile *> &sourceFiles) {
  std::stringstream message;
  message << "*-----*\n";
  message << "|     |\n";
  for (size_t i = 0; i < sourceFiles.size(); i++) {
    if (i != 0)
      message << "|     |\n";
    message << "|  " << sourceFiles.at(i)->fileName.c_str() << "\n";
  }
  message << "|     |\n";
  message << "*-----*";
  return message.str();
}

/**
 * Generate the version info string for the Spice driver
 *
 * @return Version info string
 */
std::string CommonUtil::getVersionInfo() {
  std::stringstream versionString;
  versionString << "spice version " << SPICE_VERSION << " " << SPICE_TARGET_OS << "/" << SPICE_TARGET_ARCH << "\n";
  versionString << "built by: " << SPICE_BUILT_BY << "\n\n";
  versionString << "(c) Marc Auberer 2021-2024";
  return versionString.str();
}

} // namespace spice::compiler