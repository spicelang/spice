// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <cli/CLIInterface.h>

namespace spice::compiler {

class ExternalLinkerInterface {
public:
  // Constructors
  explicit ExternalLinkerInterface(const CliOptions &cliOptions) : cliOptions(cliOptions), outputPath(cliOptions.outputPath){};

  // Public methods
  void prepare();
  void link() const;
  void addObjectFilePath(const std::string &objectFilePath);
  void addLinkerFlag(const std::string &linkerFlag);

  // Public members
  std::filesystem::path outputPath;

private:
  // Members
  const CliOptions &cliOptions;
  std::vector<std::string> objectFilePaths;
  std::vector<std::string> linkerFlags = {"-fuse-ld=lld", "-flto"};
};

} // namespace spice::compiler