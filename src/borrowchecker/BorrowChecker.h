// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

#define BCResult bool

class BorrowChecker : private CompilerPass, public ASTVisitor<BCResult> {
public:
  // Constructors
  BorrowChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

private:
  // Private members
};