// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SourceFile.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

namespace spice::compiler {

class ExecutionEngine : public CompilerPass {
public:
  // Constructors
  ExecutionEngine(GlobalResourceManager &resourceManager, SourceFile *mainSourceFile);
  ExecutionEngine(const ExecutionEngine &) = delete;

  // Public methods
  int executeMainFct();

private:
  // Private members
  llvm::ExecutionEngine *executionEngine;
};

} // namespace spice::compiler