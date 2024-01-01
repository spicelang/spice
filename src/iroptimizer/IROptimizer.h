// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>

#include <CompilerPass.h>
#include <SourceFile.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

class IROptimizer : private CompilerPass {
public:
  // Constructors
  IROptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
      : CompilerPass(resourceManager, sourceFile), module(sourceFile->llvmModule.get()) {}

  // Public methods
  void prepare();
  void optimizeDefault();
  void optimizePreLink();
  void optimizePostLink(llvm::Module &ltoModule);
  [[nodiscard]] std::string getOptimizedIRString(llvm::Module *module = nullptr) const;

private:
  // Private members
  llvm::LoopAnalysisManager loopAnalysisMgr;
  llvm::FunctionAnalysisManager functionAnalysisMgr;
  llvm::CGSCCAnalysisManager cgsccAnalysisMgr;
  llvm::ModuleAnalysisManager moduleAnalysisMgr;
  std::unique_ptr<llvm::PassBuilder> passBuilder;
  llvm::Module *module;

  // Private methods
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};

} // namespace spice::compiler