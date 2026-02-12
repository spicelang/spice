// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>

#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>

namespace spice::compiler {

class IROptimizer final : CompilerPass {
public:
  // Constructors
  IROptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  void prepare();
  void optimizeDefault();
  void optimizePreLink();
  void optimizePostLink();

private:
  // Private members
  llvm::LoopAnalysisManager loopAnalysisMgr;
  llvm::FunctionAnalysisManager functionAnalysisMgr;
  llvm::CGSCCAnalysisManager cgsccAnalysisMgr;
  llvm::ModuleAnalysisManager moduleAnalysisMgr;
  llvm::StandardInstrumentations si;
  llvm::PassInstrumentationCallbacks pic;
  std::unique_ptr<llvm::PassBuilder> passBuilder;

  // Private methods
  void addInstrumentationPassToPipeline(llvm::ModulePassManager& modulePassMgr) const;
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};

} // namespace spice::compiler