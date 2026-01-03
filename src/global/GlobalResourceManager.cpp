// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <SourceFile.h>
#include <global/TypeRegistry.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/StructManager.h>
#include <util/FileUtil.h>

#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/SubtargetFeature.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(cliOptions), cacheManager(cliOptions.cacheDir), runtimeModuleManager(*this) {
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

  // Create lto module
  if (cliOptions.useLTO)
    ltoModule = std::make_unique<llvm::Module>(LTO_FILE_NAME, ltoContext);
}

GlobalResourceManager::~GlobalResourceManager() {
  // Notify all global components to prepare to destroy
  TypeRegistry::clear();
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
    sourceFiles.insert({filePathStr, std::make_unique<SourceFile>(*this, parent, depName, path, isStdFile)});

  return sourceFiles.at(filePathStr).get();
}

uint64_t GlobalResourceManager::getNextCustomTypeId() { return nextCustomTypeId++; }

size_t GlobalResourceManager::getTotalLineCount() const {
  const auto acc = [](size_t sum, const auto &sourceFile) { return sum + FileUtil::getLineCount(sourceFile.second->filePath); };
  return std::accumulate(sourceFiles.begin(), sourceFiles.end(), 0, acc);
}

} // namespace spice::compiler