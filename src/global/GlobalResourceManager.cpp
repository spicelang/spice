// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <SourceFile.h>
#include <exception/IRError.h>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(threadFactory, cliOptions), cacheManager(cliOptions.cacheDir), runtimeModuleManager(*this) {
  // Initialize the required LLVM targets
  if (cliOptions.isNativeTarget) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
  } else { // GCOV_EXCL_START
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
  } // GCOV_EXCL_STOP

  // Search after selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  // Create target machine for LLVM
  llvm::TargetOptions opt;
  std::string cpuName = "generic";
  std::stringstream featureString;
  if (cliOptions.isNativeTarget) {
    // Retrieve native CPU name and the supported CPU features
    cpuName = llvm::sys::getHostCPUName();
    llvm::StringMap<bool> hostFeatures;
    llvm::sys::getHostCPUFeatures(hostFeatures);
    for (const auto &feature : hostFeatures) {
      if (featureString.rdbuf()->in_avail() > 0)
        featureString << ",";
      featureString << (feature.second ? "+" : "-") << feature.first().str();
    }
  }

  targetMachine = target->createTargetMachine(cliOptions.targetTriple, cpuName, featureString.str(), opt, llvm::Reloc::PIC_);
}

GlobalResourceManager::~GlobalResourceManager() {
  // Delete all source files
  for (auto &sourceFile : sourceFiles)
    delete sourceFile.second;
}

SourceFile *GlobalResourceManager::createSourceFile(SourceFile *parent, const std::string &dependencyName,
                                                    const std::string &path, bool isStdFile) {
  // Check if the source file was already added (e.g. by another source file that imports it)
  if (sourceFiles.contains(path))
    return sourceFiles.at(path);

  // Create the new source file
  auto newSourceFile = new SourceFile(*this, parent, dependencyName, path, isStdFile);
  sourceFiles.insert({path, newSourceFile});
  return newSourceFile;
}

} // namespace spice::compiler