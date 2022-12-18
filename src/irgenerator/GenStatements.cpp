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

std::any IRGenerator::visitDeclStmt(const DeclStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Get variable entry
  SymbolTableEntry *varEntry = currentScope->lookupStrict(node->varName);
  assert(varEntry != nullptr);
  const SymbolType varSymbolType = varEntry->getType();

  // Get LLVM type of variable
  llvm::Type *varTy = varSymbolType.toLLVMType(context, currentScope);

  // Check if the declaration is with an assignment or the default value
  llvm::Value *varAddress = nullptr;
  if (node->hasAssignment) { // Assignment
    ExprResult assignResult = doAssignment(varEntry, node->assignExpr());
    assert(assignResult.entry == varEntry);
    assert(assignResult.ptr == varEntry->getAddress());
    varAddress = assignResult.entry->getAddress();
  } else { // Default value
    // Retrieve default value for lhs symbol type
    llvm::Value *defaultValue = getDefaultValueForSymbolType(varSymbolType);
    // Allocate memory and store the default value there
    varAddress = insertAlloca(varTy);
    builder.CreateStore(defaultValue, varAddress);
    // Update address in symbol table
    varEntry->updateAddress(varAddress);
  }
  assert(varAddress != nullptr);

  // Generate debug info for variable declaration
  if (cliOptions.generateDebugInfo)
    diGenerator.generateDeclDebugInfo(node->codeLoc, node->varName, varAddress, SIZE_MAX, true);

  return nullptr;
}

std::any IRGenerator::visitSpecifierLst(const SpecifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) { return ParallelizableASTVisitor::visitReturnStmt(node); }

std::any IRGenerator::visitBreakStmt(const BreakStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->breakTimes; i++)
    breakBlocks.pop();

  // Jump to destination block
  insertJump(breakBlocks.top());

  return nullptr;
}

std::any IRGenerator::visitContinueStmt(const ContinueStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->continueTimes; i++)
    continueBlocks.pop();

  // Jump to destination block
  insertJump(continueBlocks.top());

  return nullptr;
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