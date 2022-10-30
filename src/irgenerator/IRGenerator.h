// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

class IRGenerator : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Friend classes
  friend class StdFunctionManager;
  friend class OpRuleConversionsManager;

  // Public methods
  std::any visitEntry(EntryNode *node) override;

  [[nodiscard]] std::string getIRString() const;
  void dumpIR() const;

private:
  // Private members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;
};