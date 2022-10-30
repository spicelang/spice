// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

class ObjectEmitter : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  ObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
      : CompilerPass(resourceManager, sourceFile), module(sourceFile->llvmModule.get()) {}

  // Public methods
  void emit() const;
  void dumpAsm() const;

private:
  // Private members
  llvm::Module *module;
};