// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "BitcodeLinker.h"

namespace spice::compiler {

void BitcodeLinker::link() {
  // Link all source file modules in
  for (const auto &sourceFile : resourceManager.sourceFiles)
    linker.linkInModule(std::move(sourceFile.second->llvmModule), llvm::Linker::None);
}

} // namespace spice::compiler