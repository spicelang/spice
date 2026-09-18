// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <atomic>
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
  void addFileToLinkage(const std::filesystem::path &path);
  void addLinkerFlag(const std::string &flag);
  void addAdditionalSourcePath(std::filesystem::path additionalSource);
  void requestLibMathLinkage();
  [[nodiscard]] const std::vector<std::string> &getLinkerFlags() const { return linkerFlags; }
  [[nodiscard]] const std::vector<std::filesystem::path> &getLinkedFiles() const { return linkedFiles; }

  // Public members
  std::filesystem::path outputPath;

private:
  // Private methods
  void link() const;
  void archive() const;

  // Members
  const CliOptions &cliOptions;
  std::vector<std::filesystem::path> linkedFiles;
  std::vector<std::string> linkerFlags;
  // The IR generator requests libm linkage while emitting certain operators, so this flag is written from the back-end
  // worker threads. Everything else on this class is only touched from the single-threaded phases of the pipeline.
  std::atomic<bool> linkLibMath = false;
};

} // namespace spice::compiler