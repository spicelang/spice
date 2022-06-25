// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <cli/CliInterface.h>
#include <exception/ErrorFactory.h>
#include <util/ThreadFactory.h>

class LinkerInterface {
public:
  // Constructors
  explicit LinkerInterface(const ErrorFactory &errorFactory, const ThreadFactory &threadFactory, CliOptions &cliOptions)
      : err(errorFactory), threadFactory(threadFactory), cliOptions(cliOptions){};

  // Public methods
  void link();
  void addObjectFilePath(const std::string &objectFilePath);
  void addLinkerFlag(const std::string &linkerFlag);
  void setOutputPath(const std::string &outputPath);

private:
  // Members
  const ErrorFactory &err;
  const ThreadFactory &threadFactory;
  const CliOptions &cliOptions;
  std::vector<std::string> objectFilePaths;
  std::vector<std::string> linkerFlags = {"-no-pie"};
  std::string outputPath;
};