// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <llvm/Linker/Linker.h>

#include <CompilerPass.h>
#include <global/GlobalResourceManager.h>

namespace spice::compiler {

class BitcodeLinker : public CompilerPass {
public:
  // Constructors
  explicit BitcodeLinker(GlobalResourceManager &resourceManager)
      : CompilerPass(resourceManager), linker(*resourceManager.ltoModule) {}

  // Public methods
  void link();

private:
  // Private members
  llvm::Linker linker;
};

} // namespace spice::compiler