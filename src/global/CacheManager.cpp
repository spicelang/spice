// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CacheManager.h"

#include <SourceFile.h>
#include <util/FileUtil.h>

bool CacheManager::lookupSourceFile(SourceFile *sourceFile) {
  std::string symbolTableFilePath = cacheDir + FileUtil::DIR_SEPARATOR + sourceFile->cacheKey + ".bson";
  std::string objectFilePath = cacheDir + FileUtil::DIR_SEPARATOR + sourceFile->cacheKey + ".o";

  // Check if cache entry is available
  if (!FileUtil::fileExists(symbolTableFilePath) || !FileUtil::fileExists(objectFilePath))
    return false;

  // Load symbol table

  // Set object file path

  return true;
}

void CacheManager::cacheSourceFile(const SourceFile */*sourceFile*/) {
  // Cache symbol table

  // Cache object file
}