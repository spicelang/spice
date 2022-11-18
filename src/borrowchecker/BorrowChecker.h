// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

class BorrowChecker : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  BorrowChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

private:
  // Private members
};