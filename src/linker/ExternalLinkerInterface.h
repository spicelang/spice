// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace spice::compiler {

// Forward declarations
struct CliOptions;

class ExternalLinkerInterface {
public:
  // Constructors
  explicit ExternalLinkerInterface(const CliOptions &cliOptions);

  // Avoid copies
  ExternalLinkerInterface(const ExternalLinkerInterface &) = delete;
  ExternalLinkerInterface &operator=(const ExternalLinkerInterface &) = delete;

  // Public methods
  void prepare();
  void link() const;
  void addObjectFilePath(const std::string &objectFilePath);
  void addLinkerFlag(const std::string &flag);
  void addAdditionalSourcePath(std::filesystem::path additionalSource);
  void requestLibMathLinkage();

  // Public members
  std::filesystem::path outputPath;

private:
  // Members
  const CliOptions &cliOptions;
  std::vector<std::string> objectFilePaths;
  std::vector<std::string> linkerFlags;
  bool linkLibMath = false;
};

} // namespace spice::compiler