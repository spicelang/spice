// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <atomic>
#include <filesystem>
#include <future>
#include <string>
#include <unordered_map>
#include <vector>

#include <util/SystemUtil.h>

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
  void startAdditionalSourceCompilation();
  void requestLibMathLinkage();
  [[nodiscard]] const std::vector<std::string> &getLinkerFlags() const { return linkerFlags; }
  [[nodiscard]] const std::vector<std::filesystem::path> &getLinkedFiles() const { return linkedFiles; }

  // Public members
  std::filesystem::path outputPath;

private:
  // Private structs
  struct AdditionalSourceCompilation {
    std::vector<std::string> args;
    std::filesystem::path objectFilePath;
    std::shared_future<ExecResult> result;
  };

  // Private methods
  void link() const;
  void archive() const;
  [[nodiscard]] std::vector<std::string> getInvokerArgs(const char *linkerName, const std::string &linkerPath,
                                                        bool isGccInvoker) const;
  [[nodiscard]] std::vector<std::string> getTrailingArgs() const;
  [[nodiscard]] std::vector<std::string> getAdditionalSourceCompileArgs(const std::filesystem::path &additionalSource,
                                                                        const std::filesystem::path &objectFilePath) const;
  [[nodiscard]] std::filesystem::path getAdditionalSourceObjectFilePath(const std::filesystem::path &additionalSource) const;
  [[nodiscard]] std::filesystem::path compileAdditionalSource(const std::filesystem::path &additionalSource) const;

  // Members
  const CliOptions &cliOptions;
  std::vector<std::filesystem::path> linkedFiles;
  std::vector<std::filesystem::path> additionalSourcePaths;
  std::vector<std::string> linkerFlags;
  // Expanding a linker flag may run commands (backtick substitutions), so every flag is only expanded once
  mutable std::unordered_map<std::string, std::vector<std::string>> expandedLinkerFlags;
  // Compilations of additional sources, that were started early to overlap with the back end. link() is const, but waits
  // for them and may add more, so they are mutable
  mutable std::unordered_map<std::string, AdditionalSourceCompilation> additionalSourceCompilations;
  // The IR generator requests libm linkage while emitting certain operators, so this flag is written from the back-end
  // worker threads. Everything else on this class is only touched from the single-threaded phases of the pipeline.
  std::atomic<bool> linkLibMath = false;
};

} // namespace spice::compiler