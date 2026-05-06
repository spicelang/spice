// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CacheManager.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_set>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <util/SystemUtil.h>

#include <nlohmann/json.hpp>

namespace spice::compiler {

CacheManager::CacheManager(const CliOptions &cliOptions) : cliOptions(cliOptions), cacheDir(cliOptions.cacheDir) {}

std::string CacheManager::computeCacheKey(const std::string &sourceCode, const std::vector<std::string> &depCacheKeys) const {
  std::stringstream components;
  components << std::hex << std::hash<std::string>{}(sourceCode);
  components << static_cast<uint8_t>(cliOptions.buildMode);
  components << static_cast<uint8_t>(cliOptions.optLevel);
  components << static_cast<uint8_t>(cliOptions.instrumentation.sanitizer);
  components << cliOptions.instrumentation.generateDebugInfo;
  components << cliOptions.targetTriple.str();
  components << cliOptions.useLTO;
  // The output container influences codegen (PIC/PIE levels, DSO-local attributes for symbols,
  // etc.), so reusing an object emitted for a different container would produce wrong output.
  components << static_cast<uint8_t>(cliOptions.outputContainer);
  // Fold transitive dependency cache keys (sorted for determinism) into the key so that any
  // change in a dependency invalidates every dependent's cache entry too.
  std::vector<std::string> sortedDepKeys = depCacheKeys;
  std::ranges::sort(sortedDepKeys);
  for (const std::string &depKey : sortedDepKeys)
    components << depKey;
  return std::to_string(std::hash<std::string>{}(components.str()));
}

bool CacheManager::lookupSourceFile(SourceFile *sourceFile) const {
  const char *objectFileExtension = SystemUtil::getOutputFileExtension(cliOptions, OutputContainer::OBJECT_FILE);
  const std::filesystem::path metadataFilePath = cacheDir / (sourceFile->cacheKey + ".json");
  const std::filesystem::path objectFilePath = cacheDir / (sourceFile->cacheKey + "." + objectFileExtension);

  // Check if cache entry is available
  if (!exists(metadataFilePath) || !exists(objectFilePath))
    return false;

  // Read metadata
  std::ifstream metadataFile(metadataFilePath);
  if (!metadataFile)
    return false;

  nlohmann::json metadata;
  try {
    metadata = nlohmann::json::parse(metadataFile);
  } catch (nlohmann::detail::parse_error &) {
    return false;
  }

  // Verify all transitive dependency object files exist and collect their paths. We keep
  // these even though Spice imports register themselves via their own concludeCompilation,
  // because runtime modules (string-rt, memory-rt, ...) are pulled in implicitly during
  // symbol-table building - a stage that gets skipped for cache-restored files. Without
  // this list those runtime objects would be missing from the link. The linker dedupes the
  // overlap with deps that did register themselves.
  if (metadata.contains("dependencies")) {
    for (const auto &depKey : metadata["dependencies"]) {
      const std::string key = depKey.get<std::string>();
      const std::filesystem::path depObjectFilePath = cacheDir / (key + "." + objectFileExtension);
      if (!exists(depObjectFilePath))
        return false;
      sourceFile->cachedObjectFilePaths.push_back(depObjectFilePath);
    }
  }

  // Add this file's own object file last
  sourceFile->cachedObjectFilePaths.push_back(objectFilePath);

  // Restore linker flags and additional source paths
  if (metadata.contains("linkerFlags"))
    for (const auto &flag : metadata["linkerFlags"])
      sourceFile->sourceLinkerFlags.push_back(flag.get<std::string>());
  if (metadata.contains("additionalSourcePaths"))
    for (const auto &path : metadata["additionalSourcePaths"])
      sourceFile->sourceAdditionalSourcePaths.emplace_back(path.get<std::string>());

  return true;
}

void CacheManager::cacheSourceFile(const SourceFile *sourceFile) const {
  // Don't cache if LTO is enabled and this isn't the main file (no object file produced)
  if (cliOptions.useLTO && !sourceFile->isMainFile)
    return;

  // Determine the source object file path (mirrors runObjectEmitter's path logic)
  std::filesystem::path sourceObjFilePath = cliOptions.outputDir / sourceFile->filePath.filename();
  sourceObjFilePath.replace_extension("o");

  // Determine cache paths
  const char *objectFileExtension = SystemUtil::getOutputFileExtension(cliOptions, OutputContainer::OBJECT_FILE);
  const std::filesystem::path cachedObjectFilePath = cacheDir / (sourceFile->cacheKey + "." + objectFileExtension);
  const std::filesystem::path metadataFilePath = cacheDir / (sourceFile->cacheKey + ".json");

  // Verify source object file exists
  std::error_code error;
  if (!std::filesystem::exists(sourceObjFilePath, error) || error)
    return;

  // Ensure cache directory exists
  std::filesystem::create_directories(cacheDir, error);
  if (error)
    return;

  // Copy object file to cache
  std::filesystem::copy_file(sourceObjFilePath, cachedObjectFilePath, std::filesystem::copy_options::overwrite_existing, error);
  if (error)
    return;

  // Collect all transitive dependency cache keys, linker flags, and additional source paths.
  // We need the transitive list so that cache-restored files can replay the full linker input
  // even for implicit deps (e.g. runtime modules requested during symbol-table building, which
  // a cache hit skips). The linker dedupes the overlap with deps that register themselves.
  std::vector<std::string> depCacheKeys;
  std::vector<std::string> allLinkerFlags = sourceFile->sourceLinkerFlags;
  std::vector<std::string> allAdditionalSourcePaths;
  for (const auto &p : sourceFile->sourceAdditionalSourcePaths)
    allAdditionalSourcePaths.push_back(p.string());
  std::unordered_set<std::string> visited;
  std::queue<const SourceFile *> worklist;
  for (const SourceFile *dep : sourceFile->dependencies | std::views::values)
    worklist.push(dep);
  while (!worklist.empty()) {
    const SourceFile *dep = worklist.front();
    worklist.pop();
    if (!visited.insert(dep->cacheKey).second)
      continue;
    depCacheKeys.push_back(dep->cacheKey);
    allLinkerFlags.insert(allLinkerFlags.end(), dep->sourceLinkerFlags.begin(), dep->sourceLinkerFlags.end());
    for (const auto &p : dep->sourceAdditionalSourcePaths)
      allAdditionalSourcePaths.push_back(p.string());
    for (const SourceFile *transitiveDep : dep->dependencies | std::views::values)
      worklist.push(transitiveDep);
  }

  // Write metadata file
  nlohmann::json metadata;
  metadata["sourceFile"] = sourceFile->filePath.string();
  metadata["fileName"] = sourceFile->fileName;
  metadata["cacheKey"] = sourceFile->cacheKey;
  metadata["dependencies"] = depCacheKeys;
  metadata["linkerFlags"] = allLinkerFlags;
  metadata["additionalSourcePaths"] = allAdditionalSourcePaths;
  std::ofstream metadataStream(metadataFilePath);
  if (metadataStream)
    metadataStream << metadata.dump();
}

// Hash the content of a single linker input that's not produced by the Spice cache itself
// (e.g. C/C++ files referenced via @core.linker.additionalSource). Returns a sentinel that
// folds the path in if the file can't be opened, so a vanished file still produces a stable
// (but different) cache key.
std::string hashLinkedFile(const std::filesystem::path &path) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream)
    return "missing:" + path.string();
  std::stringstream content;
  content << stream.rdbuf();
  return std::to_string(std::hash<std::string>{}(content.str()));
}

std::string computeExecutableCacheKey(const std::vector<std::string> &objectFileCacheKeys,
                                      const std::vector<std::string> &linkerFlags,
                                      const std::vector<std::filesystem::path> &additionalSourcePaths,
                                      const CliOptions &cliOptions) {
  std::stringstream components;
  for (const std::string &key : objectFileCacheKeys)
    components << key;
  for (const std::string &flag : linkerFlags)
    components << flag;
  // Sort additional source paths so traversal order doesn't perturb the key, then fold in
  // path + content hash. Without this, edits to a referenced C/C++ source would leave every
  // Spice object cache key unchanged, and we'd serve a stale executable.
  std::vector<std::filesystem::path> sortedAdditionalSources = additionalSourcePaths;
  std::ranges::sort(sortedAdditionalSources);
  for (const std::filesystem::path &additionalSource : sortedAdditionalSources)
    components << additionalSource.string() << '\0' << hashLinkedFile(additionalSource);
  components << static_cast<uint8_t>(cliOptions.outputContainer);
  components << cliOptions.staticLinking;
  return std::to_string(std::hash<std::string>{}(components.str()));
}

bool CacheManager::lookupExecutable(const std::vector<std::string> &objectFileCacheKeys,
                                    const std::vector<std::string> &linkerFlags,
                                    const std::vector<std::filesystem::path> &additionalSourcePaths,
                                    std::filesystem::path &cachedExecutablePath) const {
  const std::string execCacheKey = computeExecutableCacheKey(objectFileCacheKeys, linkerFlags, additionalSourcePaths, cliOptions);

  // Determine expected extension
  const char *extension = SystemUtil::getOutputFileExtension(cliOptions, cliOptions.outputContainer);
  const std::string fileName = execCacheKey + (strlen(extension) > 0 ? "." + std::string(extension) : "");
  const std::filesystem::path execPath = cacheDir / fileName;

  if (!exists(execPath))
    return false;

  cachedExecutablePath = execPath;
  return true;
}

void CacheManager::cacheExecutable(const std::vector<std::string> &objFileCacheKeys, const std::vector<std::string> &linkerFlags,
                                   const std::vector<std::filesystem::path> &additionalSourcePaths,
                                   const std::filesystem::path &executablePath) const {
  const std::string execCacheKey = computeExecutableCacheKey(objFileCacheKeys, linkerFlags, additionalSourcePaths, cliOptions);

  // Determine cached file name
  const char *extension = SystemUtil::getOutputFileExtension(cliOptions, cliOptions.outputContainer);
  const std::string fileName = execCacheKey + (strlen(extension) > 0 ? "." + std::string(extension) : "");
  const std::filesystem::path cachedExecPath = cacheDir / fileName;

  // Verify executable exists
  std::error_code error;
  if (!std::filesystem::exists(executablePath, error) || error)
    return;

  // Ensure cache directory exists
  std::filesystem::create_directories(cacheDir, error);
  if (error)
    return;

  // Copy executable to cache
  std::filesystem::copy_file(executablePath, cachedExecPath, std::filesystem::copy_options::overwrite_existing, error);
}

void CacheManager::linkOrRestoreExecutable(GlobalResourceManager &resourceManager) const {
  const ExternalLinkerInterface &linker = resourceManager.linker;

  // Collect object file cache keys and any external linker inputs (e.g. C/C++ files added
  // via @core.linker.additionalSource) that participate in the executable cache key.
  std::vector<std::string> objectFileCacheKeys;
  std::vector<std::filesystem::path> additionalSourcePaths;
  for (const auto &sourceFile : resourceManager.sourceFiles | std::views::values) {
    objectFileCacheKeys.push_back(sourceFile->cacheKey);
    for (const std::filesystem::path &additionalSource : sourceFile->sourceAdditionalSourcePaths)
      additionalSourcePaths.push_back(additionalSource);
  }

  // Check if we have a cached executable
  std::filesystem::path cachedExecutablePath;
  if (!cliOptions.ignoreCache &&
      lookupExecutable(objectFileCacheKeys, linker.getLinkerFlags(), additionalSourcePaths, cachedExecutablePath)) {
    // Restore cached executable
    std::error_code ec;
    std::filesystem::create_directories(linker.outputPath.parent_path(), ec);
    std::filesystem::copy_file(cachedExecutablePath, linker.outputPath, std::filesystem::copy_options::overwrite_existing, ec);
  } else {
    // Link and cache the result
    linker.run();
    if (!cliOptions.ignoreCache)
      cacheExecutable(objectFileCacheKeys, linker.getLinkerFlags(), additionalSourcePaths, linker.outputPath);
  }
}

} // namespace spice::compiler
