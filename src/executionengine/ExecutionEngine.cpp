// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ExecutionEngine.h"

#include <llvm/ExecutionEngine/MCJIT.h>

namespace spice::compiler {

ExecutionEngine::ExecutionEngine(GlobalResourceManager &resourceManager, SourceFile *mainSourceFile)
    : CompilerPass(resourceManager, mainSourceFile) {
  llvm::EngineBuilder engineBuilder(std::move(sourceFile->llvmModule));
  executionEngine = engineBuilder.create();
  assert(executionEngine != nullptr);

  // Disable the verifier if required
  if (resourceManager.cliOptions.disableVerifier)
    executionEngine->setVerifyModules(false);

  // Add all modules to the execution engine
  std::vector<SourceFile *> sourceFiles;
  sourceFile->collectAllSourceFiles(sourceFiles);
  for (SourceFile *sourceFile : sourceFiles)
    executionEngine->addModule(std::move(sourceFile->llvmModule));
}

/**
 * Run the program
 *
 * @return exit code of the program
 */
int ExecutionEngine::executeMainFct() {
  std::vector<std::string> argv;
  const char * const *envp = {};

  llvm::Function *mainFct = executionEngine->FindFunctionNamed("main");
  assert(mainFct != nullptr);
  return executionEngine->runFunctionAsMain(mainFct, argv, envp);
}

} // namespace spice::compiler