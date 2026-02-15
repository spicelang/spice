// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "SystemUtil.h"

#include <array>
#include <iostream> // IWYU pragma: keep (usage in Windows-only code)
#include <vector>
#if OS_UNIX
#include <unistd.h>
#elif OS_WINDOWS
#include <windows.h>
#else
#error "Unsupported platform"
#endif

#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <exception/LinkerError.h>

#include <llvm/TargetParser/Triple.h>

namespace spice::compiler {

/**
 * Execute external command. Used to execute compiled binaries
 *
 * @param command Command to execute
 * @param redirectStdErrToStdOut Redirect StdErr to StdOut
 * @return Result struct
 */
ExecResult SystemUtil::exec(const std::string &command, bool redirectStdErrToStdOut) {
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
bool SystemUtil::isCommandAvailable(const std::string &cmd) {
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
bool SystemUtil::isGraphvizInstalled() { return isCommandAvailable("dot"); }

/**
 * Search for a supported linker invoker on the system and return the executable name or path.
 * This function may throw a LinkerError if no linker invoker is found.
 *
 * @return Name and path to the linker invoker executable
 */
ExternalBinaryFinderResult SystemUtil::findLinkerInvoker() {
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
  throw LinkerError(LINKER_INVOKER_NOT_FOUND, msg);                                                     // LCOV_EXCL_LINE
}

/**
 * Search for a supported linker on the system and return the executable name or path.
 * This function may throw a LinkerError if no linker is found.
 *
 * @param cliOptions Command line options
 * @return Name and path to the linker executable
 */
ExternalBinaryFinderResult SystemUtil::findLinker([[maybe_unused]] const CliOptions &cliOptions) {
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
 * Search for a supported archiver on the system and return the executable name or path.
 * This function may throw a LinkerError if no archiver is found.
 *
 * @return Name and path to the archiver executable
 */
ExternalBinaryFinderResult SystemUtil::findArchiver() {
#if OS_UNIX
  for (const char *archiverName : {"llvm-ar", "gcc-ar", "ar"})
    for (const std::string path : {"/usr/bin/", "/usr/local/bin/", "/bin/"})
      if (std::filesystem::exists(path + archiverName))
        return ExternalBinaryFinderResult{archiverName, path + archiverName};
#elif OS_WINDOWS
  for (const char *archiverName : {"llvm-lib", "lib"})
    if (isCommandAvailable(std::string(archiverName) + " -v"))
      return ExternalBinaryFinderResult{archiverName, archiverName};
#else
#error "Unsupported platform"
#endif
  const auto msg = "No supported archiver was found on the system. Supported are: llvm-ar and ar"; // LCOV_EXCL_LINE
  throw LinkerError(ARCHIVER_NOT_FOUND, msg);                                                      // LCOV_EXCL_LINE
}

/**
 * Retrieve the file extension of the produced output file, depending on target container format and target OS
 *
 * @param cliOptions Command line options
 * @param outputContainer Output container
 * @return File extension
 */
const char *SystemUtil::getOutputFileExtension(const CliOptions &cliOptions, OutputContainer outputContainer) {
  static constexpr auto OUTPUT_CONTAINER_COUNT = static_cast<size_t>(OutputContainer::MAX);
  static constexpr std::array<const char *, OUTPUT_CONTAINER_COUNT> OC_EXT_MAP_WASM = {"wasm", "o", "a", "so"};
  static constexpr std::array<const char *, OUTPUT_CONTAINER_COUNT> OC_EXT_MAP_MACOS = {"", "o", "a", "dylib"};
  static constexpr std::array<const char *, OUTPUT_CONTAINER_COUNT> OC_EXT_MAP_WINDOWS = {"exe", "obj", "lib", "dll"};
  static constexpr std::array<const char *, OUTPUT_CONTAINER_COUNT> OC_EXT_MAP_LINUX = {"", "o", "a", "so"};

  const auto outputContainerCasted = static_cast<uint8_t>(outputContainer);
  if (cliOptions.targetTriple.isWasm())
    return OC_EXT_MAP_WASM[outputContainerCasted];
  if (cliOptions.targetTriple.isOSDarwin())
    return OC_EXT_MAP_MACOS[outputContainerCasted];
  if (cliOptions.targetTriple.isOSWindows())
    return OC_EXT_MAP_WINDOWS[outputContainerCasted];
  return OC_EXT_MAP_LINUX[outputContainerCasted];
}

/**
 * Retrieve the dir, where the standard library lives.
 * Returns an empty string if the std was not found.
 *
 * @return Std directory
 */
std::filesystem::path SystemUtil::getStdDir() {
#if OS_UNIX
  if (exists(std::filesystem::path("/usr/lib/spice/std/")))
    return "/usr/lib/spice/std/";
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
std::filesystem::path SystemUtil::getBootstrapDir() {
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
std::filesystem::path SystemUtil::getSpiceBinDir() {
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

/**
 * Get the memory page size of the current system
 *
 * @return Page size in bytes
 */
size_t SystemUtil::getSystemPageSize() {
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

} // namespace spice::compiler
