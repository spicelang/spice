// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CacheManager.h"

#include <SourceFile.h>
#include <util/FileUtil.h>

namespace spice::compiler {

bool CacheManager::lookupSourceFile(SourceFile *sourceFile) {
  std::filesystem::path symbolTableFilePath = cacheDir / (sourceFile->cacheKey + ".bson");
  std::filesystem::path objectFilePath = cacheDir / (sourceFile->cacheKey + ".o");

  // Check if cache entry is available
  if (!std::filesystem::exists(symbolTableFilePath) || !std::filesystem::exists(objectFilePath))
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