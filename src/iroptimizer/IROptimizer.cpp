// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "IROptimizer.h"

#include <llvm/Analysis/ModuleSummaryAnalysis.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>

#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>

namespace spice::compiler {

void IROptimizer::prepare() {
  passBuilder = std::make_unique<llvm::PassBuilder>(resourceManager.targetMachine.get());

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
  llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildPerModuleDefaultPipeline(llvmOptLevel);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(*module, moduleAnalysisMgr);
}

void IROptimizer::optimizePreLink() {
  if (cliOptions.printDebugOutput && cliOptions.dumpSettings.dumpIR && !cliOptions.dumpSettings.dumpToFiles) // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " (pre-link) ...\n";      // GCOV_EXCL_LINE

  // Run passes
  llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTOPreLinkDefaultPipeline(llvmOptLevel);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(*module, moduleAnalysisMgr);

  // Generate module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  moduleSummaryIndexAnalysis.run(*sourceFile->llvmModule, moduleAnalysisMgr);
}

void IROptimizer::optimizePostLink(llvm::Module &ltoModule) {
  if (cliOptions.printDebugOutput && cliOptions.dumpSettings.dumpIR && !cliOptions.dumpSettings.dumpToFiles) // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " (post-link) ...\n";     // GCOV_EXCL_LINE

  // Compute module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  llvm::ModuleSummaryIndex moduleSummaryIndex = moduleSummaryIndexAnalysis.run(ltoModule, moduleAnalysisMgr);
  moduleSummaryIndex.setWithWholeProgramVisibility();

  // Run passes
  llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTODefaultPipeline(llvmOptLevel, &moduleSummaryIndex);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(ltoModule, moduleAnalysisMgr);
}

std::string IROptimizer::getOptimizedIRString(llvm::Module *module) const {
  if (!module)
    module = this->module;
  assert(module != nullptr); // Ensure that the module was not moved away
  std::string output;
  llvm::raw_string_ostream oss(output);
  module->print(oss, nullptr);
  return oss.str();
}

llvm::OptimizationLevel IROptimizer::getLLVMOptLevelFromSpiceOptLevel() const {
  switch (cliOptions.optLevel) {
  case OptLevel::O1:
    return llvm::OptimizationLevel::O1;
  case OptLevel::O2:
    return llvm::OptimizationLevel::O2;
  case OptLevel::O3:
    return llvm::OptimizationLevel::O3;
  case OptLevel::Os:
    return llvm::OptimizationLevel::Os;
  case OptLevel::Oz:
    return llvm::OptimizationLevel::Oz;
  default:
    return llvm::OptimizationLevel::O0;
  }
}

} // namespace spice::compiler