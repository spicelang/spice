// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "FileUtil.h"
#include "exception/CompilerError.h"

#include <array>
#include <filesystem>
#include <iostream>

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
ExecResult FileUtil::exec(const std::string &command) {
#ifdef _WIN32
  const std::string quotedCommand = "\"" + command + "\"";
  FILE *pipe = _popen(quotedCommand.c_str(), "r");
#else
  FILE *pipe = popen(command.c_str(), "r");
#endif

  if (!pipe)                                                                // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to execute command: " + command); // GCOV_EXCL_LINE

  std::array<char, 128> buffer{};
  std::stringstream result;
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    result << buffer.data();

#ifdef _WIN32
  const int exitCode = _pclose(pipe);
#else
  const int exitCode = pclose(pipe);
#endif
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
#ifdef OS_UNIX
  if (std::filesystem::exists(std::filesystem::path("/usr/lib/spice/std/")))
    return std::filesystem::path("/usr/lib/spice/std/");
#endif
  if (std::getenv("SPICE_STD_DIR")) {
    std::filesystem::path stdPath(std::getenv("SPICE_STD_DIR"));
    if (std::filesystem::exists(stdPath))
      return stdPath;
  }
  return ""; // GCOV_EXCL_LINE
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