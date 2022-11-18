// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <irgenerator/DebugInfoGenerator.h>
#include <irgenerator/OpRuleConversionsManager.h>
#include <irgenerator/StdFunctionManager.h>

// Forward declarations
class SourceFile;

class IRGenerator : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Friend classes
  friend class StdFunctionManager;
  friend class OpRuleConversionsManager;
  friend class DebugInfoGenerator;

  // Visitor methods
  std::any visitEntry(EntryNode *node) override;

  // Public methods
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "");
  [[nodiscard]] std::string getIRString() const;
  void dumpIR() const;

private:
  // Private members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;
  OpRuleConversionsManager conversionManager = OpRuleConversionsManager(resourceManager, this);
  const StdFunctionManager stdFunctionManager;
  DebugInfoGenerator diGenerator = DebugInfoGenerator(this);
  Scope *currentScope;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
};