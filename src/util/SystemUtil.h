// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>

namespace spice::compiler {

// Forward declarations
struct CliOptions;

struct ExecResult {
  std::string output;
  int exitCode;
};

struct ExternalBinaryFinderResult {
  const char *name;
  std::string path;
};

class SystemUtil {
public:
  static ExecResult exec(const std::string &command, bool redirectStdErrToStdOut = false);
  static bool isCommandAvailable(const std::string &cmd);
  static bool isGraphvizInstalled();
  static ExternalBinaryFinderResult findLinkerInvoker();
  static ExternalBinaryFinderResult findLinker(const CliOptions &cliOptions);
  static std::filesystem::path getStdDir();
  static std::filesystem::path getBootstrapDir();
  static std::filesystem::path getSpiceBinDir();
};

} // namespace spice::compiler
