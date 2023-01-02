// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

class EscapeAnalyzer : private CompilerPass, public ASTVisitor {
public:
  // Constructor
  EscapeAnalyzer(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

private:
  // Private members
};

} // namespace spice::compiler