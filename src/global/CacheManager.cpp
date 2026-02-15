// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CacheManager.h"

#include <SourceFile.h>
#include <driver/Driver.h>
#include <util/SystemUtil.h>

namespace spice::compiler {

CacheManager::CacheManager(const CliOptions &cliOptions) : cliOptions(cliOptions), cacheDir(cliOptions.cacheDir) {}

bool CacheManager::lookupSourceFile(const SourceFile *sourceFile) const {
  const std::filesystem::path symbolTableFilePath = cacheDir / (sourceFile->cacheKey + ".bson");
  const char *objectFileExtension = SystemUtil::getOutputFileExtension(cliOptions, OutputContainer::OBJECT_FILE);
  const std::filesystem::path objectFilePath = cacheDir / (sourceFile->cacheKey + objectFileExtension);

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