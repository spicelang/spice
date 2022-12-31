// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"
#include "symboltablebuilder/SymbolTableBuilder.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

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
  SymbolTableEntry *varEntry = node->entries.at(manIdx);
  assert(varEntry != nullptr);
  const SymbolType varSymbolType = varEntry->getType();

  // Get LLVM type of variable
  llvm::Type *varTy = varSymbolType.toLLVMType(context, currentScope);

  // Check if right side is dyn array. If this is the case we have an empty array initializer and need the default value
  const bool rhsIsDynArray = node->hasAssignment && node->assignExpr()->getEvaluatedSymbolType(manIdx).isArrayOf(TY_DYN);

  // Check if the declaration is with an assignment or the default value
  llvm::Value *varAddress = nullptr;
  if (node->hasAssignment && !rhsIsDynArray) { // Assignment
    ExprResult assignResult = doAssignment(varEntry, node->assignExpr());
    assert(assignResult.entry == varEntry);
    assert(assignResult.ptr == varEntry->getAddress());
    varAddress = assignResult.entry->getAddress();
  } else { // Default value
    // Allocate memory
    varAddress = insertAlloca(varTy);
    // Retrieve default value for lhs symbol type and store it
    llvm::Constant *defaultValue = getDefaultValueForSymbolType(varSymbolType);
    builder.CreateStore(defaultValue, varAddress);
    // Update address in symbol table
    varEntry->updateAddress(varAddress);
  }
  assert(varAddress != nullptr);

  // Attach the variable name to the LLVM value
  varAddress->setName(varEntry->name);

  // Generate debug info for variable declaration
  if (cliOptions.generateDebugInfo)
    diGenerator.generateLocalVarDebugInfo(node->codeLoc, node->varName, varAddress, SIZE_MAX, true);

  return nullptr;
}

std::any IRGenerator::visitSpecifierLst(const SpecifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) {
  diGenerator.setSourceLocation(node);

  llvm::Value *returnValue = nullptr;
  if (node->hasReturnValue) { // Return value is attached to the return statement
    returnValue = resolveValue(node->assignExpr());
  } else { // Try to load return variable value
    SymbolTableEntry *resultEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (resultEntry != nullptr) {
      llvm::Type *resultSTy = resultEntry->getType().toLLVMType(context, currentScope);
      llvm::Value *returnValueAddr = resultEntry->getAddress();
      returnValue = builder.CreateLoad(resultSTy, returnValueAddr);
    }
  }

  // Set block to terminated
  blockAlreadyTerminated = true;

  // Create return instruction
  if (returnValue != nullptr) { // Return value
    builder.CreateRet(returnValue);
  } else { // Return without value
    builder.CreateRetVoid();
  }

  return nullptr;
}

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
  const std::string &codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = createBlock("assert.then." + codeLine);
  llvm::BasicBlock *bExit = createBlock("assert.exit." + codeLine);

  // Visit the assignExpr
  llvm::Value *condValue = resolveValue(node->assignExpr());
  llvm::Function *parentFct = builder.GetInsertBlock()->getParent();

  // Create condition check
  insertCondJump(condValue, bThen, bExit);

  // Switch to then block
  switchToBlock(bThen);
  // Create constant for error message
  const std::string errorMsg = "Assertion failed: Condition '" + node->expressionString + "' evaluated to false.";
  llvm::Constant *globalString = builder.CreateGlobalStringPtr(errorMsg, getUnusedGlobalName(ANON_GLOBAL_STRING_NAME));
  // Print the error message
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();
  builder.CreateCall(printfFct, globalString);
  // Generate call to exit()
  llvm::Function *exitFct = stdFunctionManager.getExitFct();
  builder.CreateCall(exitFct, builder.getInt32(EXIT_FAILURE));
  // Create unreachable instruction
  builder.CreateUnreachable();

  // Switch to exit block
  switchToBlock(bExit);

  return nullptr;
}

} // namespace spice::compiler