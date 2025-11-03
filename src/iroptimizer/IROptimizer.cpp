// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IROptimizer.h"

#include <llvm/Analysis/ModuleSummaryAnalysis.h>
#include <llvm/Transforms/Instrumentation/AddressSanitizer.h>
#include <llvm/Transforms/Instrumentation/MemorySanitizer.h>
#include <llvm/Transforms/Instrumentation/ThreadSanitizer.h>

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
  if (cliOptions.printDebugOutput && cliOptions.dump.dumpIR && !cliOptions.dump.dumpToFiles)          // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(static_cast<uint8_t>(cliOptions.optLevel)) // GCOV_EXCL_LINE
              << " ...\n";                                                                            // GCOV_EXCL_LINE

  // Prepare pipeline
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildPerModuleDefaultPipeline(llvmOptLevel);

  // Add optional passes
  addInstrumentationPassToPipeline(modulePassMgr);

  // Run pipeline
  modulePassMgr.run(*sourceFile->llvmModule, moduleAnalysisMgr);
}

void IROptimizer::optimizePreLink() {
  if (cliOptions.printDebugOutput && cliOptions.dump.dumpIR && !cliOptions.dump.dumpToFiles)          // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(static_cast<uint8_t>(cliOptions.optLevel)) // GCOV_EXCL_LINE
              << " (pre-link) ...\n";                                                                 // GCOV_EXCL_LINE

  // Prepare pipeline
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTOPreLinkDefaultPipeline(llvmOptLevel);

  // Run pipeline
  modulePassMgr.run(*sourceFile->llvmModule, moduleAnalysisMgr);

  // Generate module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  moduleSummaryIndexAnalysis.run(*sourceFile->llvmModule, moduleAnalysisMgr);
}

void IROptimizer::optimizePostLink() {
  if (cliOptions.printDebugOutput && cliOptions.dump.dumpIR && !cliOptions.dump.dumpToFiles)          // GCOV_EXCL_LINE
    std::cout << "\nOptimizing on level " + std::to_string(static_cast<uint8_t>(cliOptions.optLevel)) // GCOV_EXCL_LINE
              << " (post-link) ...\n";                                                                // GCOV_EXCL_LINE
  llvm::Module &ltoModule = *resourceManager.ltoModule;

  // Compute module summary index
  llvm::ModuleSummaryIndexAnalysis moduleSummaryIndexAnalysis;
  llvm::ModuleSummaryIndex moduleSummaryIndex = moduleSummaryIndexAnalysis.run(ltoModule, moduleAnalysisMgr);
  moduleSummaryIndex.setWithWholeProgramVisibility();

  // Prepare pipeline
  const llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder->buildLTODefaultPipeline(llvmOptLevel, &moduleSummaryIndex);

  // Add optional passes
  addInstrumentationPassToPipeline(modulePassMgr);

  // Run pipeline
  modulePassMgr.run(ltoModule, moduleAnalysisMgr);
}

void IROptimizer::addInstrumentationPassToPipeline(llvm::ModulePassManager &modulePassMgr) const {
  switch (cliOptions.instrumentation.sanitizer) {
  case Sanitizer::NONE: {
    return;
  }
  case Sanitizer::ADDRESS: {
    llvm::AddressSanitizerOptions asanOptions;
    asanOptions.UseAfterScope = true;
    modulePassMgr.addPass(llvm::AddressSanitizerPass(asanOptions));
    break;
  }
  case Sanitizer::THREAD: {
    modulePassMgr.addPass(llvm::ModuleThreadSanitizerPass());
    modulePassMgr.addPass(llvm::createModuleToFunctionPassAdaptor(llvm::ThreadSanitizerPass()));
    break;
  }
  case Sanitizer::MEMORY: {
    llvm::MemorySanitizerOptions msanOptions;
    msanOptions.EagerChecks = true;
    modulePassMgr.addPass(llvm::MemorySanitizerPass(msanOptions));
    break;
  }
  }
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