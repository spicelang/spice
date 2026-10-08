// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "BitcodeLinker.h"

#include <SourceFile.h>
#include <global/GlobalResourceManager.h>

#include <llvm/IR/Module.h> // IWYU pragma: keep

namespace spice::compiler {

BitcodeLinker::BitcodeLinker(GlobalResourceManager &resourceManager)
    : CompilerPass(resourceManager), linker(*resourceManager.ltoModule) {}

void BitcodeLinker::link() {
  // Link all source file modules in. Use the creation order, because the order of the source file map depends on the hash
  // function, and the link order shows in the LTO module
  for (SourceFile *sourceFile : resourceManager.sourceFilesInCreationOrder)
    linker.linkInModule(std::move(sourceFile->llvmModule), llvm::Linker::None);
}

} // namespace spice::compiler