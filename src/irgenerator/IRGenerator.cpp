// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>

#include <llvm/BinaryFormat/Dwarf.h>

IRGenerator::IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), context(resourceManager.context), builder(resourceManager.builder),
      module(sourceFile->llvmModule.get()), stdFunctionManager(StdFunctionManager(resourceManager, sourceFile->llvmModule.get())),
      currentScope(sourceFile->globalScope.get()) {
  // Attach information to the module
  module->setTargetTriple(resourceManager.cliOptions.targetTriple);
  module->setDataLayout(resourceManager.targetMachine->createDataLayout());

  // Initialize debug info generator
  if (cliOptions.generateDebugInfo)
    diGenerator.initializeDIBuilder(sourceFile->fileName, sourceFile->fileDir);
}

std::any IRGenerator::visitEntry(EntryNode *node) { return nullptr; }

llvm::Value *IRGenerator::insertAlloca(llvm::Type *llvmType, const std::string &varName) {
  if (allocaInsertInst != nullptr) { // If there is already an alloca inst, insert right after that
    llvm::AllocaInst *allocaInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInst->setDebugLoc(llvm::DebugLoc());
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else { // This is the first alloca inst in the current function -> insert at the entry block
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder.GetInsertBlock();
    builder.SetInsertPoint(allocaInsertBlock);

    allocaInsertInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInsertInst->setDebugLoc(llvm::DebugLoc());

    // Restore old basic block
    builder.SetInsertPoint(currentBlock);
  }
  return static_cast<llvm::Value *>(allocaInsertInst);
}

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