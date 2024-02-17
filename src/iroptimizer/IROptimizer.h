// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>

#include <CompilerPass.h>
#include <SourceFile.h>
#include <ast/ASTVisitor.h>
#include <global/GlobalResourceManager.h>

namespace spice::compiler {

class IROptimizer : private CompilerPass {
public:
  // Constructors
  IROptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
      : CompilerPass(resourceManager, sourceFile),
        si(resourceManager.context, false, resourceManager.cliOptions.testMode, llvm::PrintPassOptions(false, true, false)) {}

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
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};

} // namespace spice::compiler