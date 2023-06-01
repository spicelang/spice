// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ExecutionEngine.h"

#include <llvm/ExecutionEngine/MCJIT.h>

namespace spice::compiler {

ExecutionEngine::ExecutionEngine(GlobalResourceManager &resourceManager, SourceFile *mainSourceFile)
    : CompilerPass(resourceManager, mainSourceFile) {
  const bool useLto = resourceManager.cliOptions.useLTO;
  llvm::EngineBuilder engineBuilder(std::move(useLto ? resourceManager.ltoModule : sourceFile->llvmModule));
  executionEngine = engineBuilder.create();
  assert(executionEngine != nullptr);

  // Disable the verifier if required
  if (resourceManager.cliOptions.disableVerifier)
    executionEngine->setVerifyModules(false);

  // Add all modules to the execution engine
  for (auto &sourceFile : resourceManager.sourceFiles)
    if (!sourceFile.second->mainFile)
      executionEngine->addModule(std::move(sourceFile.second->llvmModule));
}

/**
 * Run the program
 *
 * @return exit code of the program
 */
int ExecutionEngine::executeMainFct() {
  std::vector<std::string> argv = {"root"};
  const char *const *envp = {};

  llvm::Function *mainFct = executionEngine->FindFunctionNamed("main");
  assert(mainFct != nullptr);
  return executionEngine->runFunctionAsMain(mainFct, argv, envp);
}

} // namespace spice::compiler