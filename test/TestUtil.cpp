// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#ifdef __unix__
#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif

#include "TestUtil.h"

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#ifdef OS_UNIX
#include <cstring> // Required by builds on Linux
#endif

/**
 * Get subdirectories of the given path
 *
 * @param basePath Path to a directory
 * @return Vector of subdirs
 */
std::vector<std::string> TestUtil::getSubdirs(const std::string &basePath) {
  std::vector<std::string> subdirs;
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(basePath.c_str())) != nullptr) {
    while ((ent = readdir(dir)) != nullptr) {
      if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
        subdirs.emplace_back(ent->d_name);
    }
    closedir(dir);
  }
  return subdirs;
}

/**
 * Retrieve the contents of a file as a string
 *
 * @param filePath File path
 * @return File contents as a string
 */
std::string TestUtil::getFileContent(const std::string &filePath) {
  std::ifstream inputFileStream;
  inputFileStream.open(filePath);
  std::ostringstream stringStream;
  stringStream << inputFileStream.rdbuf();
  return stringStream.str();
}

/**
 * Retrieve the contents of a file as a vector of line strings. Empty lines are omitted
 *
 * @param filePath File path
 * @return Vector of strings which are the lines of the file
 */
std::vector<std::string> TestUtil::getFileContentLinesVector(const std::string &filePath) {
  std::vector<std::string> lines;
  std::ifstream inputFileStream;
  inputFileStream.open(filePath);
  for (std::string line; std::getline(inputFileStream, line);) {
    if (!line.empty())
      lines.push_back(line);
  }
  return lines;
}

/**
 * Convert a string to camel case
 *
 * @param input Input string
 * @return Camel-cased string
 */
std::string TestUtil::toCamelCase(std::string input) {
  for (auto it = input.begin(); it != input.end(); it++) {
    if (*it == '-' || *it == '_') {
      it = input.erase(it);
      *it = (char)toupper(*it);
    }
  }
  return input;
}

/**
 * Get default executable name of the compiled binary
 *
 * @return Name of the executable including the file extension
 */
std::string TestUtil::getDefaultExecutableName() {
  std::string executableName = "./source";
#ifdef OS_WINDOWS
  executableName = ".\\source.exe";
#endif
  return executableName;
}

/**
 * Check if the update refs mode is enabled
 *
 * @return Enabled or not
 */
bool TestUtil::isUpdateRefsEnabled() { return updateRefs; }

// GCOV_EXCL_STOP