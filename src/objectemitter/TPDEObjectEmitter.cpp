// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TPDEObjectEmitter.h"

#include <fstream>
#include <memory>
#include <vector>

#include <exception/CompilerError.h>

#include <llvm/IR/Module.h>
#include <llvm/TargetParser/Triple.h>

#include <tpde-llvm/LLVMCompiler.hpp>

namespace spice::compiler {

TPDEObjectEmitter::TPDEObjectEmitter(llvm::Module &module) : module(module) {}

void TPDEObjectEmitter::emit(const std::filesystem::path &objectPath) const {
  const std::string objectPathString = objectPath.string();

  // Create a TPDE compiler for the module's target triple
  const llvm::Triple triple(module.getTargetTriple());
  const std::unique_ptr<tpde_llvm::LLVMCompiler> compiler = tpde_llvm::LLVMCompiler::create(triple);
  if (!compiler)
    throw CompilerError(WRONG_OUTPUT_TYPE,
                        "The TPDE backend does not support target triple '" + triple.str() + "'"); // GCOV_EXCL_LINE

  // Compile to an in-memory ELF object
  std::vector<uint8_t> objBytes;
  if (!compiler->compile_to_elf(module, objBytes))
    throw CompilerError(WRONG_OUTPUT_TYPE, "TPDE failed to compile module '" + module.getName().str() + "'");

  // Write to the requested path
  std::ofstream out(objectPathString, std::ios::binary);
  if (!out)
    throw CompilerError(CANT_OPEN_OUTPUT_FILE, "File '" + objectPathString + "' could not be opened"); // GCOV_EXCL_LINE
  out.write(reinterpret_cast<const char *>(objBytes.data()), static_cast<std::streamsize>(objBytes.size()));
  out.flush();
}

void TPDEObjectEmitter::getASMString(std::string &output) const {
  // TPDE emits ELF bytes directly and does not expose an assembly listing.
  output = "; Assembly listing is not available under the TPDE backend.\n"
           "; Use --backend=llvm to obtain assembly output.\n";
}

} // namespace spice::compiler
