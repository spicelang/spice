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
  void run() const;
  void cleanup() const;
  void addObjectFilePath(const std::filesystem::path &path);
  void addLinkerFlag(const std::string &flag);
  void addAdditionalSourcePath(std::filesystem::path additionalSource);
  void requestLibMathLinkage();

  // Public members
  std::filesystem::path outputPath;

private:
  // Private methods
  void link() const;
  void archive() const;

  // Members
  const CliOptions &cliOptions;
  std::vector<std::filesystem::path> objectFilePaths;
  std::vector<std::string> linkerFlags;
  bool linkLibMath = false;
};

} // namespace spice::compiler