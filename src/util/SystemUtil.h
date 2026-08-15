// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <array>
#include <filesystem>
#include <string>

namespace spice::compiler {

// Forward declarations
struct CliOptions;
enum class OutputContainer : uint8_t;

struct ExecResult {
  std::string output;
  int exitCode;
};

struct ExternalBinaryFinderResult {
  const char *name;
  std::string path;
};

// Directories that are searched for a linker invoker, linker or archiver executable
static constexpr std::array<const char *, 3> BINARY_SEARCH_DIRS = {"/usr/bin/", "/usr/local/bin/", "/bin/"};

// Supported linker invoker names, in order of preference
static constexpr auto LINKER_INVOKER_NAME_CLANG = "clang";
static constexpr auto LINKER_INVOKER_NAME_GCC = "gcc";
static constexpr std::array LINKER_INVOKER_NAMES = {LINKER_INVOKER_NAME_CLANG, LINKER_INVOKER_NAME_GCC};

// Supported linker names, in order of preference
static constexpr auto LINKER_NAME_MOLD = "mold";
static constexpr auto LINKER_NAME_LD_LLD = "ld.lld";
static constexpr auto LINKER_NAME_LD64_LLD = "ld64.ddl";
static constexpr auto LINKER_NAME_GOLD = "gold";
static constexpr auto LINKER_NAME_LD = "ld";
static constexpr auto LINKER_NAME_LLD = "lld";
static constexpr std::array LINKER_NAMES_UNIX = {LINKER_NAME_LD_LLD, LINKER_NAME_LD64_LLD, LINKER_NAME_GOLD, LINKER_NAME_LD};
static constexpr std::array LINKER_NAMES_WINDOWS = {LINKER_NAME_LLD, LINKER_NAME_LD};

// Supported archiver names, in order of preference
static constexpr auto ARCHIVER_NAME_LLVM_AR = "llvm-ar";
static constexpr auto ARCHIVER_NAME_GCC_AR = "gcc-ar";
static constexpr auto ARCHIVER_NAME_AR = "ar";
static constexpr auto ARCHIVER_NAME_LLVM_LIB = "llvm-lib";
static constexpr auto ARCHIVER_NAME_LIB = "lib";
static constexpr std::array ARCHIVER_NAMES_UNIX = {ARCHIVER_NAME_LLVM_AR, ARCHIVER_NAME_GCC_AR, ARCHIVER_NAME_AR};
static constexpr std::array ARCHIVER_NAMES_WINDOWS = {ARCHIVER_NAME_LLVM_LIB, ARCHIVER_NAME_LIB};

class SystemUtil {
public:
  static ExecResult exec(const std::string &command, bool redirectStdErrToStdOut = false);
  static int run(const std::string &executablePath);
  static bool isCommandAvailable(const std::string &cmd);
  static bool isGraphvizInstalled();
  static ExternalBinaryFinderResult findLinkerInvoker();
  static ExternalBinaryFinderResult findLinker(const CliOptions &cliOptions);
  static ExternalBinaryFinderResult findArchiver();
  static const char *getOutputFileExtension(const CliOptions &cliOptions, OutputContainer outputContainer);
  static std::filesystem::path getStdDir();
  static std::filesystem::path getBootstrapDir();
  static std::filesystem::path getSpiceBinDir();
  static size_t getSystemPageSize();
  static int transformStatusToExitCode(int status);
};

} // namespace spice::compiler
