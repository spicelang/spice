// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace spice::compiler {

// Forward declarations
class ExternalLinkerInterface;
class GlobalResourceManager;
class SourceFile;
struct CliOptions;

class CacheManager {
public:
  // Constructors
  explicit CacheManager(const CliOptions &cliOptions);

  // Prevent copy
  CacheManager(const CacheManager &) = delete;
  CacheManager &operator=(const CacheManager &) = delete;

  // Public methods
  std::string computeCacheKey(const std::string &sourceCode, const std::vector<std::string> &depCacheKeys = {}) const;
  bool lookupSourceFile(SourceFile *sourceFile) const;
  void cacheSourceFile(const SourceFile *sourceFile) const;
  bool lookupExecutable(const std::vector<std::string> &objectFileCacheKeys, const std::vector<std::string> &linkerFlags,
                        const std::vector<std::filesystem::path> &additionalSourcePaths,
                        std::filesystem::path &cachedExecutablePath) const;
  void cacheExecutable(const std::vector<std::string> &objFileCacheKeys, const std::vector<std::string> &linkerFlags,
                       const std::vector<std::filesystem::path> &additionalSourcePaths,
                       const std::filesystem::path &executablePath) const;
  void linkOrRestoreExecutable(GlobalResourceManager &resourceManager) const;

private:
  // Private members
  const CliOptions &cliOptions;
  const std::filesystem::path &cacheDir;
};

} // namespace spice::compiler