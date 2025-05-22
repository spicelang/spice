// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "BitcodeLinker.h"

#include <SourceFile.h>

#include <llvm/IR/Module.h> // IWYU pragma: keep

namespace spice::compiler {

void BitcodeLinker::link() {
  // Link all source file modules in
  for (const auto &sourceFile : resourceManager.sourceFiles | std::views::values)
    linker.linkInModule(std::move(sourceFile->llvmModule), llvm::Linker::None);
}

} // namespace spice::compiler