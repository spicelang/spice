// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ObjectEmitter.h"

#include <exception/IRError.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>

void ObjectEmitter::emit() const {
  const std::string &objectFile = sourceFile->objectFilePath;

  // GCOV_EXCL_START
  if (cliOptions.printDebugOutput && cliOptions.dumpAssembly)
    std::cout << "\nEmitting object file for triplet '" << cliOptions.targetTriple << "' to path: " << objectFile << "\n";
  // GCOV_EXCL_STOP

  // Open file output stream
  std::error_code errorCode;
  llvm::raw_fd_ostream dest(objectFile, errorCode, llvm::sys::fs::OF_None);
  if (errorCode)
    throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + objectFile + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  if (resourceManager.targetMachine->addPassesToEmitFile(passManager, dest, nullptr, llvm::CGFT_ObjectFile))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(*module);
  dest.flush();
}

void ObjectEmitter::dumpAsm() const {
  llvm::legacy::PassManager passManager;
  if (resourceManager.targetMachine->addPassesToEmitFile(passManager, llvm::outs(), nullptr, llvm::CGFT_AssemblyFile))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(*module);
  llvm::outs().flush();
}