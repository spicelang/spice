// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

#define EAResult bool

class EscapeAnalyzer : private CompilerPass, public ASTVisitor<EAResult> {
public:
  // Constructor
  EscapeAnalyzer(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

private:
  // Private members
};