// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <cli/CLIInterface.h>

// Forward declarations
class ThreadFactory;

class LinkerInterface {
public:
  // Constructors
  explicit LinkerInterface(const ThreadFactory &threadFactory, const CliOptions &cliOptions)
      : threadFactory(threadFactory), cliOptions(cliOptions), outputPath(cliOptions.outputPath){};

  // Public methods
  void link() const;
  void addObjectFilePath(const std::string &objectFilePath);
  void addLinkerFlag(const std::string &linkerFlag);

  // Public members
  std::string outputPath;

private:
  // Members
  const ThreadFactory &threadFactory;
  const CliOptions &cliOptions;
  std::vector<std::string> objectFilePaths;
  std::vector<std::string> linkerFlags = {"-no-pie", "-flto"};
};