// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <exception/CompilerError.h>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>

namespace spice::compiler {

namespace {

std::string toUtf8String(const std::filesystem::path &path) {
  const std::u8string utf8Path = path.u8string();
  return std::string(utf8Path.begin(), utf8Path.end());
}

std::filesystem::path fromUtf8String(const llvm::StringRef utf8Path) { return std::u8string(utf8Path.begin(), utf8Path.end()); }

} // namespace

/**
 * Creates a file and writes fileContent to it.
 *
 * @param filePath File path
 * @param fileContent String to write into the file
 */
void FileUtil::writeToFile(const std::filesystem::path &filePath, const std::string &fileContent) {
  std::ofstream file(filePath);
  if (!file)                                                                    // LCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string()); // LCOV_EXCL_LINE
  file << fileContent;
  file.flush();
  file.close();
}

/**
 * Retrieve the contents of a file as a string
 *
 * @param filePath File path
 * @return File contents as a string
 */
std::string FileUtil::getFileContent(const std::filesystem::path &filePath) {
  std::ifstream file(filePath);
  if (!file)                                                                    // LCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string()); // LCOV_EXCL_LINE
  std::stringstream stringStream;
  stringStream << file.rdbuf();
  file.close();
  return stringStream.str();
}

/**
 * Retrieve the number of lines in a file
 *
 * @param filePath File path
 * @return Number of lines
 */
size_t FileUtil::getLineCount(const std::filesystem::path &filePath) {
  std::ifstream file(filePath);
  if (!file)
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string());
  size_t lineCount = 0;
  std::string line;
  while (std::getline(file, line))
    lineCount++;
  file.close();
  return lineCount;
}

/**
 * Resolve the given path to an absolute path without symlinks and '.'/'..' components, like std::filesystem::canonical().
 * Unlike the latter, this also resolves symlinks on Windows, where libstdc++ cannot detect them and only makes the path
 * absolute. This keeps the resolved paths identical across all platforms.
 *
 * @param path Path to resolve
 * @return Canonical path
 * @throws std::filesystem::filesystem_error if the path does not exist or cannot be resolved
 */
std::filesystem::path FileUtil::canonical(const std::filesystem::path &path) {
  llvm::SmallString<256> realPath;
  if (const std::error_code errorCode = llvm::sys::fs::real_path(toUtf8String(path), realPath))
    throw std::filesystem::filesystem_error("Failed to resolve path", path, errorCode);
  return fromUtf8String(realPath.str()).lexically_normal();
}

/**
 * Resolve the longest existing prefix of the given path canonically and append the rest of the path lexically normalized,
 * like std::filesystem::weakly_canonical(). Symlinks are resolved on all platforms (see canonical()).
 *
 * @param path Path to resolve
 * @return Weakly canonical path
 */
std::filesystem::path FileUtil::weaklyCanonical(const std::filesystem::path &path) {
  std::filesystem::path existingPrefix;
  std::filesystem::path remainder;
  for (const std::filesystem::path &component : path) {
    std::error_code errorCode;
    if (remainder.empty() && exists(existingPrefix / component, errorCode))
      existingPrefix /= component;
    else
      remainder /= component;
  }
  if (existingPrefix.empty())
    return path.lexically_normal();
  const std::filesystem::path canonicalPrefix = canonical(existingPrefix);
  return remainder.empty() ? canonicalPrefix : (canonicalPrefix / remainder).lexically_normal();
}

/**
 * Make the given path relative to the given base path, like std::filesystem::relative(). Both paths are resolved weakly
 * canonical first, which resolves symlinks on all platforms (see canonical()).
 *
 * @param path Path to make relative
 * @param base Base path, the current working directory by default
 * @return Relative path, or an empty path if there is no relative path (e.g. the paths are on different drives)
 */
std::filesystem::path FileUtil::relative(const std::filesystem::path &path, const std::filesystem::path &base) {
  return weaklyCanonical(path).lexically_relative(weaklyCanonical(base));
}

} // namespace spice::compiler
