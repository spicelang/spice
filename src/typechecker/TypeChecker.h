// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <typechecker/OpRuleManager.h>

class TypeChecker : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

  // Friend classes
  friend class OpRuleManager;

private:
  // Private members
  OpRuleManager opRuleManager = OpRuleManager(this);
  Scope *currentScope = sourceFile->globalScope.get();
};
