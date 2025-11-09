// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <array>
#include <filesystem>
#include <iostream> // IWYU pragma: keep (usage in Windows-only code)

#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <exception/LinkerError.h>
#include <util/CommonUtil.h>

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
  if (!file)
    throw CompilerError(IO_ERROR, "Failed to open file: " + filePath.string());
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
 * Execute external command. Used to execute compiled binaries
 *
 * @param command Command to execute
 * @param redirectStdErrToStdOut Redirect StdErr to StdOut
 * @return Result struct
 */
ExecResult FileUtil::exec(const std::string &command, bool redirectStdErrToStdOut) {
#if OS_UNIX
  std::string redirectedCommand = command;
  if (redirectStdErrToStdOut)
    redirectedCommand += " 2>&1"; // Redirect stderr to stdout
  FILE *pipe = popen(redirectedCommand.c_str(), "r");
#elif OS_WINDOWS
  std::string redirectedCommand = command;
  if (redirectStdErrToStdOut)
    redirectedCommand = "\"" + command + " 2>&1\""; // Redirect stderr to stdout
  FILE *pipe = _popen(redirectedCommand.c_str(), "r");
#else
#error "Unsupported platform"
#endif

  if (!pipe)                                                                // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Failed to execute command: " + command); // GCOV_EXCL_LINE

  std::array<char, 128> buffer{};
  std::stringstream result;
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    result << buffer.data();

#if OS_UNIX
  const int exitCode = pclose(pipe) / 256;
#elif OS_WINDOWS
  const int exitCode = _pclose(pipe);
#else
#error "Unsupported platform"
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
#if OS_UNIX
  const std::string checkCmd = "which " + cmd + " > /dev/null 2>&1";
#elif OS_WINDOWS
  const std::string checkCmd = "where " + cmd + " > nul 2>&1";
#else
#error "Unsupported platform"
#endif
  return std::system(checkCmd.c_str()) == 0;
}

/**
 * Checks if Graphviz is installed on the system
 *
 * @return Present or not
 */
bool FileUtil::isGraphvizInstalled() { return std::system("dot -V") == 0; }

/**
 * Search for a supported linker invoker on the system and return the executable name or path.
 * This function may throw a LinkerError if no linker invoker is found.
 *
 * @return Name and path to the linker invoker executable
 */
ExternalBinaryFinderResult FileUtil::findLinkerInvoker() {
#if OS_UNIX
  for (const char *linkerInvokerName : {"clang", "gcc"})
    for (const std::string path : {"/usr/bin/", "/usr/local/bin/", "/bin/"})
      if (std::filesystem::exists(path + linkerInvokerName))
        return ExternalBinaryFinderResult{linkerInvokerName, path + linkerInvokerName};
#elif OS_WINDOWS
  for (const char *linkerInvokerName : {"clang", "gcc"})
    if (isCommandAvailable(std::string(linkerInvokerName) + " -v"))
      return ExternalBinaryFinderResult{linkerInvokerName, linkerInvokerName};
#else
#error "Unsupported platform"
#endif
  const auto msg = "No supported linker invoker was found on the system. Supported are: clang and gcc"; // LCOV_EXCL_LINE
  throw LinkerError(LINKER_NOT_FOUND, msg);                                                             // LCOV_EXCL_LINE
}

/**
 * Search for a supported linker on the system and return the executable name or path.
 * This function may throw a LinkerError if no linker is found.
 *
 * @return Name and path to the linker executable
 */
ExternalBinaryFinderResult FileUtil::findLinker([[maybe_unused]] const CliOptions &cliOptions) {
#if OS_UNIX
  std::vector<const char *> linkerList;
  linkerList.reserve(5);
  // mold does only support linking for unix and darwin
  if (!cliOptions.targetTriple.isOSWindows())
    linkerList.push_back("mold");
  linkerList.push_back("ld.lld");
  linkerList.push_back("ld64.ddl");
  linkerList.push_back("gold");
  linkerList.push_back("ld");

  for (const char *linkerName : linkerList)
    for (const std::string path : {"/usr/bin/", "/usr/local/bin/", "/bin/"})
      if (std::filesystem::exists(path + linkerName))
        return ExternalBinaryFinderResult{linkerName, path + linkerName};
#elif OS_WINDOWS
  for (const char *linkerName : {"lld", "ld"})
    if (isCommandAvailable(std::string(linkerName) + " -v"))
      return ExternalBinaryFinderResult{linkerName, linkerName};
#else
#error "Unsupported platform"
#endif
  const auto msg = "No supported linker was found on the system. Supported are: mold, lld, gold and ld"; // LCOV_EXCL_LINE
  throw LinkerError(LINKER_NOT_FOUND, msg);                                                              // LCOV_EXCL_LINE
}

/**
 * Retrieve the dir, where the standard library lives.
 * Returns an empty string if the std was not found.
 *
 * @return Std directory
 */
std::filesystem::path FileUtil::getStdDir() {
#if OS_UNIX
  if (exists(std::filesystem::path("/usr/lib/spice/std/")))
    return {"/usr/lib/spice/std/"};
#endif
  if (std::getenv("SPICE_STD_DIR"))
    if (const std::filesystem::path stdPath(std::getenv("SPICE_STD_DIR")); exists(stdPath))
      return stdPath;
  const auto errMsg = "Standard library could not be found. Check if the env var SPICE_STD_DIR exists"; // GCOV_EXCL_LINE
  throw CompilerError(STD_NOT_FOUND, errMsg);                                                           // GCOV_EXCL_LINE
}

/**
 * Retrieve the dir, where the bootstrap compiler lives.
 * Returns an empty string if the bootstrap compiler was not found.
 *
 * @return
 */
std::filesystem::path FileUtil::getBootstrapDir() {
  if (std::getenv("SPICE_BOOTSTRAP_DIR")) {
    if (const std::filesystem::path stdPath(std::getenv("SPICE_BOOTSTRAP_DIR")); exists(stdPath))
      return stdPath;
  }
  const auto errMsg = "Bootstrap compiler could not be found. Check if the env var SPICE_BOOTSTRAP_DIR exists"; // GCOV_EXCL_LINE
  throw CompilerError(BOOTSTRAP_NOT_FOUND, errMsg);                                                             // GCOV_EXCL_LINE
}

/**
 * Retrieve the dir, where output binaries should go when installing them
 *
 * @return Installation directory
 */
std::filesystem::path FileUtil::getSpiceBinDir() {
#if OS_UNIX
  return "/usr/local/bin/";
#elif OS_WINDOWS
  const char *userProfile = std::getenv("USERPROFILE");
  assert(userProfile != nullptr && strlen(userProfile) > 0);
  return std::filesystem::path(userProfile) / "spice" / "bin";
#else
#error "Unsupported platform"
#endif
}

} // namespace spice::compiler