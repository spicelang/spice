// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <exception/IRError.h>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(threadFactory, cliOptions), cacheManager(cliOptions.cacheDir) {
  // Search after selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  llvm::TargetOptions opt;
  llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
  targetMachine = target->createTargetMachine(cliOptions.targetTriple, "generic", "", opt, rm);
}