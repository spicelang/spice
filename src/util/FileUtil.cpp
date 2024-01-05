// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "FileUtil.h"
#include "exception/CompilerError.h"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>

#include <util/CommonUtil.h>

namespace spice::compiler {

/**
 * Creates a file and writes fileContent to it.
 *
 * @param fileName File name
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
  if (!file)
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string());
  std::stringstream stringStream;
  stringStream << file.rdbuf();
  file.close();
  return stringStream.str();
}

/**
 * Execute external command. Used to execute compiled binaries
 *
 * @param cmd Command to execute
 * @return Result struct
 */
ExecResult FileUtil::exec(const std::string &cmd) {
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)                                                            // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to execute command: " + cmd); // GCOV_EXCL_LINE
  char buffer[128];
  std::stringstream result;
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != nullptr)
      result << buffer;
  }
  int exitCode = pclose(pipe) / 256;
  return {result.str(), exitCode};
}

/**
 * Checks if a certain command is available on the computer
 *
 * @param cmd Command to search for
 * @return Present or not
 */
bool FileUtil::isCommandAvailable(const std::string &cmd) {
#if OS_WINDOWS
  const std::string checkCmd = "where " + cmd + " > nul 2>&1";
#elif OS_UNIX
  const std::string checkCmd = "which " + cmd + " > /dev/null 2>&1";
#else
#error "Unsupported platform"
#endif
  return std::system(checkCmd.c_str()) != 0;
}

/**
 * Retrieve the dir, where the standard library lives.
 * Returns an empty string if the std was not found.
 *
 * @return Std directory
 */
std::filesystem::path FileUtil::getStdDir() {
#if OS_WINDOWS
  if (std::getenv("SPICE_STD_DIR")) {
    std::filesystem::path stdPath(std::getenv("SPICE_STD_DIR"));
    if (std::filesystem::exists(stdPath))
      return stdPath;
  }
#elif OS_UNIX
  if (std::filesystem::exists(std::filesystem::path("/usr/lib/spice/std/")))
    return std::filesystem::path("/usr/lib/spice/std/");
#else
#error "Unsupported platform"
#endif
  return "";
}

/**
 * Retrieve the dir, where output binaries should go when installing them
 *
 * @return Installation directory
 */
std::filesystem::path FileUtil::getSpiceBinDir() {
#if OS_WINDOWS
  return std::filesystem::path(std::getenv("USERPROFILE")) / "spice" / "bin";
#elif OS_UNIX
  return "/usr/local/bin/";
#else
#error "Unsupported platform"
#endif
}

} // namespace spice::compiler