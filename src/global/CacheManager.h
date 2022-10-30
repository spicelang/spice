// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

// Forward declarations
class SourceFile;

class CacheManager {
public:
  // Constructors
  CacheManager(const std::string &cacheDir) : cacheDir(cacheDir) {}

  // Public methods
  bool lookupSourceFile(SourceFile *sourceFile);
  void cacheSourceFile(const SourceFile *sourceFile);

private:
  // Private members
  const std::string &cacheDir;
};