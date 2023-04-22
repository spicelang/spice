// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ObjectEmitter.h"
#include "util/FileUtil.h"

#include <exception/IRError.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>

namespace spice::compiler {

void ObjectEmitter::emit() const {
  const std::string &objectFile = sourceFile->objectFilePath;

  // GCOV_EXCL_START
  if (cliOptions.printDebugOutput && cliOptions.dumpAssembly)
    resourceManager.tout.println("\nEmitting object file for triplet '" + cliOptions.targetTriple + "' to path: " + objectFile);
  // GCOV_EXCL_STOP

  // Lock the mutex
  resourceManager.objectEmitLock.lock();

  // Open file output stream
  std::error_code errorCode;
  llvm::raw_fd_ostream stream(objectFile, errorCode, llvm::sys::fs::OF_None);
  if (errorCode)                                                                           // GCOV_EXCL_LINE
    throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + objectFile + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  // GCOV_EXCL_START
  if (resourceManager.targetMachine->addPassesToEmitFile(passManager, stream, nullptr, llvm::CGFT_ObjectFile,
                                                         cliOptions.disableVerifier))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");
  // GCOV_EXCL_STOP

  // Emit object file
  passManager.run(module);
  stream.flush();

  // Unlock the mutex
  resourceManager.objectEmitLock.unlock();
}

std::string ObjectEmitter::getASMString() const {
  const std::string asmFile = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + "assembly.asm";
  std::error_code errorCode;
  llvm::raw_fd_ostream ostream(asmFile, errorCode, llvm::sys::fs::OF_None);
  if (errorCode)                                                                        // GCOV_EXCL_LINE
    throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + asmFile + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  // GCOV_EXCL_START
  if (resourceManager.targetMachine->addPassesToEmitFile(passManager, ostream, nullptr, llvm::CGFT_AssemblyFile,
                                                         cliOptions.disableVerifier))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");
  // GCOV_EXCL_STOP

  // Emit object file
  passManager.run(module);
  ostream.flush();

  // Read contents of temp file to return it
  std::ifstream inputStream(asmFile);
  assert(inputStream.good());
  std::stringstream buffer;
  buffer << inputStream.rdbuf();

  return buffer.str();
}

void ObjectEmitter::dumpAsm() const {
  llvm::legacy::PassManager passManager;
  // GCOV_EXCL_START
  if (resourceManager.targetMachine->addPassesToEmitFile(passManager, llvm::outs(), nullptr, llvm::CGFT_AssemblyFile,
                                                         cliOptions.disableVerifier))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");
  // GCOV_EXCL_STOP

  // Emit object file
  passManager.run(module);
  llvm::outs().flush();
}

} // namespace spice::compiler