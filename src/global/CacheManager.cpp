// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CacheManager.h"

#include <SourceFile.h>

namespace spice::compiler {

bool CacheManager::lookupSourceFile(const SourceFile *sourceFile) const {
  const std::filesystem::path symbolTableFilePath = cacheDir / (sourceFile->cacheKey + ".bson");
  const std::filesystem::path objectFilePath = cacheDir / (sourceFile->cacheKey + ".o");

  // Check if cache entry is available
  if (!exists(symbolTableFilePath) || !exists(objectFilePath))
    return false;

  // Load symbol table

  // Set object file path

  return true;
}

void CacheManager::cacheSourceFile(const SourceFile * /*sourceFile*/) {
  // Cache symbol table

  // Cache object file
}

} // namespace spice::compiler