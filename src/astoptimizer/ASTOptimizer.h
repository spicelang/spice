// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

/**
 * Jobs:
 * - Remove obviously unnecessary nodes from the AST
 *
 * The following optimizations are made:
 * - Remove empty if, else if and else branches
 * - Remove empty while, do-while, for and foreach loops
 * - Remove empty anonymous, thread and unsafe blocks
 */
class ASTOptimizer : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  ASTOptimizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

  // Public methods
  std::any visitThreadDef(ThreadDefNode *node) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  std::any visitForLoop(ForLoopNode *node) override;
  std::any visitForeachLoop(ForeachLoopNode *node) override;
  std::any visitWhileLoop(WhileLoopNode *node) override;
  std::any visitDoWhileLoop(DoWhileLoopNode *node) override;
  std::any visitIfStmt(IfStmtNode *node) override;
  std::any visitElseStmt(ElseStmtNode *node) override;
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
};

} // namespace spice::compiler