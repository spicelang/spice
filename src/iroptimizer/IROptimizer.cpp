// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IROptimizer.h"

#include <llvm/Analysis/ModuleSummaryAnalysis.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>

#include <driver/Driver.h>

namespace spice::compiler {

void IROptimizer::prepare() {
  llvm::PipelineTuningOptions pto;
  if (!resourceManager.cliOptions.testMode)
    si.registerCallbacks(pic, &moduleAnalysisMgr);
  passBuilder = std::make_unique<llvm::PassBuilder>(sourceFile->targetMachine.get(), pto, std::nullopt, &pic);

  functionAnalysisMgr.registerPass([&] { return passBuilder->buildDefaultAAPipeline(); });

  passBuilder->registerModuleAnalyses(moduleAnalysisMgr);
  passBuilder->registerCGSCCAnalyses(cgsccAnalysisMgr);
  passBuilder->registerFunctionAnalyses(functionAnalysisMgr);
  passBuilder->registerLoopAnalyses(loopAnalysisMgr);
  passBuilder->crossRegisterProxies(loopAnalysisMgr, functionAnalysisMgr, cgsccAnalysisMgr, moduleAnalysisMgr);
}

void IROptimizer::optimizeDefault() {
  if (cliOptions.printDebugOutput && cliOptions.dumpSettings.dumpIR && !cliOptions.dumpSettings.dumpToFiles)
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " ...\n"; // GCOV_EXCL_LINE

  // Run passes
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildPerModuleDefaultPipeline(llvmOptLevel);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(*sourceFile->llvmModule, moduleAnalysisMgr);
}

void IROptimizer::optimizePreLink() {
  if (cliOptions.printDebugOutput && cliOptions.dumpSettings.dumpIR && !cliOptions.dumpSettings.dumpToFiles) // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " (pre-link) ...\n";      // GCOV_EXCL_LINE

  // Run passes
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTOPreLinkDefaultPipeline(llvmOptLevel);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(*sourceFile->llvmModule, moduleAnalysisMgr);

  // Generate module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  moduleSummaryIndexAnalysis.run(*sourceFile->llvmModule, moduleAnalysisMgr);
}

void IROptimizer::optimizePostLink() {
  if (cliOptions.printDebugOutput && cliOptions.dumpSettings.dumpIR && !cliOptions.dumpSettings.dumpToFiles) // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " (post-link) ...\n";     // GCOV_EXCL_LINE
  llvm::Module &ltoModule = *resourceManager.ltoModule;

  // Compute module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  llvm::ModuleSummaryIndex moduleSummaryIndex = moduleSummaryIndexAnalysis.run(ltoModule, moduleAnalysisMgr);
  moduleSummaryIndex.setWithWholeProgramVisibility();

  // Run passes
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTODefaultPipeline(llvmOptLevel, &moduleSummaryIndex);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(ltoModule, moduleAnalysisMgr);
}

llvm::OptimizationLevel IROptimizer::getLLVMOptLevelFromSpiceOptLevel() const {
  switch (cliOptions.optLevel) {
  case O1:
    return llvm::OptimizationLevel::O1;
  case O2:
    return llvm::OptimizationLevel::O2;
  case O3:
    return llvm::OptimizationLevel::O3;
  case Os:
    return llvm::OptimizationLevel::Os;
  case Oz:
    return llvm::OptimizationLevel::Oz;
  default:
    return llvm::OptimizationLevel::O0;
  }
}

} // namespace spice::compiler