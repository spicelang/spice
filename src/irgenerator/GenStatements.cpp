// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

std::any IRGenerator::visitStmtLst(const StmtLstNode *node) {
  for (const ASTNode *child : node->children) {
    // Check if we can cancel generating instructions for this code branch
    if (blockAlreadyTerminated || node->unreachable)
      return nullptr;
    // Visit child
    visit(child);
  }
  return nullptr;
}

std::any IRGenerator::visitTypeAltsLst(const TypeAltsLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitDeclStmt(const DeclStmtNode *node) { return ParallelizableASTVisitor::visitDeclStmt(node); }

std::any IRGenerator::visitSpecifierLst(const SpecifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) { return ParallelizableASTVisitor::visitReturnStmt(node); }

std::any IRGenerator::visitBreakStmt(const BreakStmtNode *node) { return ParallelizableASTVisitor::visitBreakStmt(node); }

std::any IRGenerator::visitContinueStmt(const ContinueStmtNode *node) {
  return ParallelizableASTVisitor::visitContinueStmt(node);
}

std::any IRGenerator::visitAssertStmt(const AssertStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Only generate assertions with -O0
  if (cliOptions.optLevel == 0)
    return nullptr;

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = createBlock("assert.then." + codeLine);
  llvm::BasicBlock *bEnd = createBlock("assert.end." + codeLine);

  // Visit the assignExpr
  llvm::Value *condValue = resolveValue(node->assignExpr());
  llvm::Function *parentFct = builder.GetInsertBlock()->getParent();

  // Create condition check
  insertCondJump(condValue, bThen, bEnd);

  // Switch to then block
  switchToBlock(bThen);
  // Print the exception message
  const std::string errorMsg = "Assertion failed: Condition '" + node->expressionString + "' evaluated to false.";
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();
  builder.CreateCall(printfFct, builder.CreateGlobalStringPtr(errorMsg));
  // Generate call to exit()
  llvm::Function *exitFct = stdFunctionManager.getExitFct();
  builder.CreateCall(exitFct, builder.getInt32(0));
  // Create unreachable instruction
  builder.CreateUnreachable();

  // Switch to exit block
  switchToBlock(bEnd);

  return nullptr;
}