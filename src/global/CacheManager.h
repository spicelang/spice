// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>

namespace spice::compiler {

// Forward declarations
class SourceFile;

class CacheManager {
public:
  // Constructors
  explicit CacheManager(const std::filesystem::path &cacheDir) : cacheDir(cacheDir) {}
  CacheManager(const CacheManager &) = delete;

  // Public methods
  bool lookupSourceFile(const SourceFile *sourceFile) const;
  void cacheSourceFile(const SourceFile *sourceFile);

private:
  // Private members
  const std::filesystem::path &cacheDir;
};

} // namespace spice::compiler