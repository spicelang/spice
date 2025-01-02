// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "BitcodeLinker.h"

#include <SourceFile.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

void BitcodeLinker::link() {
  // Link all source file modules in
  for (const auto &[name, sourceFile] : resourceManager.sourceFiles)
    linker.linkInModule(std::move(sourceFile->llvmModule), llvm::Linker::None);
}

} // namespace spice::compiler