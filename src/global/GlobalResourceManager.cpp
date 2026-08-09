// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <cassert>
#include <thread>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <global/TypeNameDisambiguator.h>
#include <global/TypeRegistry.h>
#include <symboltablebuilder/Scope.h> // IWYU pragma: keep - Scope
#include <typechecker/FunctionManager.h>
#include <typechecker/InterfaceManager.h>
#include <typechecker/StructManager.h>
#include <util/FileUtil.h>

#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/SubtargetFeature.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(cliOptions), cacheManager(cliOptions), runtimeModuleManager(*this) {
  // Initialize the required LLVM targets
  if (cliOptions.isNativeTarget) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
  } else { // GCOV_EXCL_START
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
  } // GCOV_EXCL_STOP

  // Create cpu name and features strings
  cpuName = "generic";
  if (cliOptions.isNativeTarget && cliOptions.useCPUFeatures) {
    // Retrieve native CPU name and the supported CPU features
    cpuName = llvm::sys::getHostCPUName();
    llvm::SubtargetFeatures features;
    for (const auto &[feature, isEnabled] : llvm::sys::getHostCPUFeatures())
      features.AddFeature(feature, isEnabled);
    cpuFeatures = features.getString();
  }

  if (cliOptions.useLTO) {
    // Discard value names if not required
    ltoContext.setDiscardValueNames(!cliOptions.namesForIRValues);
    // Create lto module
    ltoModule = std::make_unique<llvm::Module>(LTO_FILE_NAME, ltoContext);
  }
}

GlobalResourceManager::~GlobalResourceManager() {
  // Notify all global components to prepare to destroy
  TypeRegistry::clear();
  TypeNameDisambiguator::clear();
  FunctionManager::cleanup();
  StructManager::cleanup();
  InterfaceManager::cleanup();
  // Cleanup all LLVM statics
  llvm::llvm_shutdown();
}

SourceFile *GlobalResourceManager::createSourceFile(SourceFile *parent, const std::string &depName,
                                                    const std::filesystem::path &path, bool isStdFile) {
  // Check if the source file was already added (e.g. by another source file that imports it)
  const std::string filePathStr = weakly_canonical(absolute(path)).string();

  // Create the new source file if it does not exist yet
  if (!sourceFiles.contains(filePathStr))
    sourceFiles.emplace(filePathStr, std::make_unique<SourceFile>(*this, parent, depName, path, isStdFile));

  return sourceFiles.at(filePathStr).get();
}

uint64_t GlobalResourceManager::getNextCustomTypeId() { return nextCustomTypeId++; }

/**
 * Determine how many source files may be compiled at the same time, based on the --jobs/-j cli option
 *
 * @return Number of compile jobs (always >= 1)
 */
size_t GlobalResourceManager::getCompileJobCount() const {
  // An explicit job count always wins
  if (cliOptions.compileJobCount > 0)
    return cliOptions.compileJobCount;

  // Otherwise derive the job count from the machine. hardware_concurrency may return 0 if it cannot be determined, in
  // which case we fall back to a serial back end.
  const unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
  return hardwareConcurrency > 0 ? hardwareConcurrency : 1;
}

/**
 * Get the worker pool for parallel compiler passes. The pool is created on first use and re-used afterwards.
 *
 * @param threadCount Number of worker threads to create the pool with
 * @return Thread pool
 */
ThreadPool &GlobalResourceManager::getThreadPool(size_t threadCount) {
  assert(threadCount > 0);
  if (!threadPool)
    threadPool = std::make_unique<ThreadPool>(threadCount);
  return *threadPool;
}

size_t GlobalResourceManager::getTotalLineCount() const {
  const auto acc = [](size_t sum, const auto &sourceFile) { return sum + FileUtil::getLineCount(sourceFile.second->filePath); };
  return std::accumulate(sourceFiles.begin(), sourceFiles.end(), 0, acc);
}

} // namespace spice::compiler