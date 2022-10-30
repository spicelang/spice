// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <llvm/BinaryFormat/Dwarf.h>

IRGenerator::IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), context(resourceManager.context), builder(resourceManager.builder),
      module(sourceFile->llvmModule.get()) {
  // Attach information to the module
  module->setTargetTriple(resourceManager.cliOptions.targetTriple);
  module->setDataLayout(resourceManager.targetMachine->createDataLayout());

  // Initialize debug info generator
  if (cliOptions.generateDebugInfo) {
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
    module->addModuleFlag(llvm::Module::Error, "PIC Level", llvm::PICLevel::BigPIC);

    auto identifierMetadata = module->getOrInsertNamedMetadata("llvm.ident");
    llvm::MDNode *n = llvm::MDNode::get(context, llvm::MDString::get(context, "spice version " + std::string(SPICE_VERSION)));
    identifierMetadata->addOperand(n);

    initializeDIBuilder(sourceFile->fileName, sourceFile->fileDir);
  }
}

std::any IRGenerator::visitEntry(EntryNode *node) { return nullptr; }

std::string IRGenerator::getIRString() const {
  std::string output;
  llvm::raw_string_ostream oss(output);
  module->print(oss, nullptr);
  return oss.str();
}

void IRGenerator::dumpIR() const {
  module->print(llvm::outs(), nullptr); // GCOV_EXCL_LINE
  llvm::outs().flush();                 // GCOV_EXCL_LINE
}
