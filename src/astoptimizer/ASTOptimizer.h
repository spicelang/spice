// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

#define ASTOResult bool

/**
 * Jobs:
 * - Remove obviously unnecessary nodes from the AST
 *
 * The following optimizations are made:
 * - Remove empty if and else branches
 */
class ASTOptimizer : private CompilerPass, public ASTVisitor<ASTOResult> {
public:
  // Constructors
  ASTOptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

  // Public methods
  ASTOResult visitIfStmt(IfStmtNode *node) override;

private:
  // Private members
};