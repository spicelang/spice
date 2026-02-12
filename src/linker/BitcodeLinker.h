// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>

#include <llvm/Linker/Linker.h>

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;

class BitcodeLinker : public CompilerPass {
public:
  // Constructors
  explicit BitcodeLinker(GlobalResourceManager &resourceManager);

  // Public methods
  void link();

private:
  // Private members
  llvm::Linker linker;
};

} // namespace spice::compiler