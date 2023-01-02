// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SourceFile.h>
#include <ast/ASTVisitor.h>

#include <llvm/Passes/OptimizationLevel.h>

namespace spice::compiler {

class IROptimizer : private CompilerPass {
public:
  // Constructors
  IROptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
      : CompilerPass(resourceManager, sourceFile), module(sourceFile->llvmModule.get()) {}

  // Public methods
  void optimize();
  [[nodiscard]] std::string getOptimizedIRString() const;
  void dumpOptimizedIR() const;

private:
  // Private members
  llvm::Module *module;

  // Private methods
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};

} // namespace spice::compiler