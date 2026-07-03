// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "LLVMObjectEmitter.h"

#include <SourceFile.h>
#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <global/GlobalResourceManager.h>
#include <util/RawStringOStream.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>

namespace spice::compiler {

LLVMObjectEmitter::LLVMObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile),
      module(cliOptions.useLTO ? *resourceManager.ltoModule : *sourceFile->llvmModule) {}

void LLVMObjectEmitter::emit(const std::filesystem::path &objectPath) const {
  const std::string objectPathString = objectPath.string();

  // Open file output stream
  std::error_code errorCode;
  llvm::raw_fd_ostream stream(objectPathString, errorCode, llvm::sys::fs::OF_None);
  if (errorCode)
    throw CompilerError(CANT_OPEN_OUTPUT_FILE, "File '" + objectPathString + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  constexpr auto fileType = llvm::CodeGenFileType::ObjectFile;
  if (sourceFile->targetMachine->addPassesToEmitFile(passManager, stream, nullptr, fileType, cliOptions.disableVerifier))
    throw CompilerError(WRONG_OUTPUT_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(module);
  stream.flush();
}

void LLVMObjectEmitter::getASMString(std::string &output) const {
  RawStringOStream ostream(output);
  llvm::legacy::PassManager passManager;
  constexpr auto fileType = llvm::CodeGenFileType::AssemblyFile;
  if (sourceFile->targetMachine->addPassesToEmitFile(passManager, ostream, nullptr, fileType, cliOptions.disableVerifier))
    throw CompilerError(WRONG_OUTPUT_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(module);
  ostream.flush();
}

} // namespace spice::compiler
