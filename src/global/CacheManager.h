// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>

namespace spice::compiler {

// Forward declarations
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
  bool lookupSourceFile(const SourceFile *sourceFile) const;
  void cacheSourceFile(const SourceFile *sourceFile);

private:
  // Private members
  const CliOptions &cliOptions;
  const std::filesystem::path &cacheDir;
};

} // namespace spice::compiler