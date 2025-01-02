// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SourceFile.h>

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;

class ObjectEmitter : CompilerPass {
public:
  // Constructors
  ObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  void emit(const std::filesystem::path &objectPath) const;
  void getASMString(std::string &output) const;

private:
  // Private members
  llvm::Module &module;
};

} // namespace spice::compiler