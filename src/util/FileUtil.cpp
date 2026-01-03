// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <exception/CompilerError.h>

namespace spice::compiler {

/**
 * Creates a file and writes fileContent to it.
 *
 * @param filePath File path
 * @param fileContent String to write into the file
 */
void FileUtil::writeToFile(const std::filesystem::path &filePath, const std::string &fileContent) {
  std::ofstream file(filePath);
  if (!file)
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string());
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
  if (!file)                                                                    // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string()); // GCOV_EXCL_LINE
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

} // namespace spice::compiler