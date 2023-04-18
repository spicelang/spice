// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <SourceFile.h>
#include <exception/IRError.h>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(threadFactory, cliOptions), cacheManager(cliOptions.cacheDir), runtimeModuleManager(*this) {
  // Initialize the required LLVM targets
  if (cliOptions.isNativeTarget) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
  } else { // GCOV_EXCL_START
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
  } // GCOV_EXCL_STOP

  // Search after selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  // Create target machine for LLVM
  llvm::TargetOptions opt;
  std::optional rm = std::optional<llvm::Reloc::Model>();
  targetMachine = target->createTargetMachine(cliOptions.targetTriple, "generic", "", opt, rm);
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