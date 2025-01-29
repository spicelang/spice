// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

std::any IRGenerator::visitStmtLst(const StmtLstNode *node) {
  // Generate instructions in the scope
  for (const StmtNode *stmt : node->statements) {
    if (!stmt)
      continue;
    // Check if we can cancel generating instructions for this code branch
    if (blockAlreadyTerminated || stmt->unreachable)
      break;
    // Visit child
    visit(stmt);
  }

  // Generate cleanup code of this scope, e.g. dtor calls for struct instances
  diGenerator.setSourceLocation(node->getNextOuterStmtLst()->closingBraceCodeLoc);
  generateScopeCleanup(node);

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
  const QualType varSymbolType = varEntry->getQualType();

  // Get LLVM type of variable
  llvm::Type *varTy = varSymbolType.toLLVMType(sourceFile);

  // Check if the declaration is with an assignment or the default value
  llvm::Value *varAddress = nullptr;
  if (node->hasAssignment) { // Assignment
    if (node->calledCopyCtor) {
      // Allocate memory
      varAddress = insertAlloca(varTy);
      varEntry->updateAddress(varAddress);
      // Call copy ctor
      llvm::Value *rhsAddress = resolveAddress(node->assignExpr);
      assert(rhsAddress != nullptr);
      generateCtorOrDtorCall(varEntry, node->calledCopyCtor, {rhsAddress});
    } else {
      // Assign rhs to lhs
      [[maybe_unused]] const LLVMExprResult assignResult = doAssignment(varAddress, varEntry, node->assignExpr, node, true);
      assert(assignResult.entry == varEntry);
      varAddress = varEntry->getAddress();
      varEntry->updateAddress(varAddress);
    }
  } else { // Default value
    // Allocate memory
    varAddress = insertAlloca(varTy);
    varEntry->updateAddress(varAddress);

    if (node->calledInitCtor) {
      // Call no-args constructor
      generateCtorOrDtorCall(varEntry, node->calledInitCtor, {});
    } else if (!node->isForEachItem && cliOptions.buildMode == DEBUG) {
      assert(!node->isCtorCallRequired);
      // Retrieve default value for lhs symbol type and store it
      llvm::Constant *defaultValue = getDefaultValueForSymbolType(varSymbolType);
      insertStore(defaultValue, varAddress);
    }
  }
  assert(varAddress != nullptr);

  // Attach the variable name to the LLVM value.
  varAddress->setName(varEntry->name);

  // Generate debug info for variable declaration
  diGenerator.setSourceLocation(node);
  diGenerator.generateLocalVarDebugInfo(node->varName, varAddress);

  return nullptr;
}

std::any IRGenerator::visitQualifierLst(const QualifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitModAttr(const ModAttrNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitTopLevelDefinitionAttr(const TopLevelDefinitionAttrNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitCaseConstant(const CaseConstantNode *node) {
  if (node->constant)
    return visit(node->constant);

  const SymbolTableEntry *constantEntry = node->entry;
  return getConst(constantEntry->declNode->getCompileTimeValue(), node->getEvaluatedSymbolType(manIdx), node);
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) {
  diGenerator.setSourceLocation(node);

  llvm::Value *returnValue = nullptr;
  if (node->hasReturnValue) { // Return value is attached to the return statement
    const AssignExprNode* returnExpr = node->assignExpr;
    const QualType &returnType = node->getEvaluatedSymbolType(manIdx);
    if (node->calledCopyCtor) {
      // Perform a copy
      llvm::Value* originalAddress = resolveAddress(returnExpr);
      llvm::Type* returnTy = returnType.toLLVMType(sourceFile);
      llvm::Value* newAddress = insertAlloca(returnTy);
      generateCtorOrDtorCall(newAddress, node->calledCopyCtor, {originalAddress});
      returnValue = insertLoad(returnTy, newAddress);
    } else {
      returnValue = returnType.isRef() ? resolveAddress(returnExpr) : resolveValue(returnExpr);
    }
  } else { // Try to load result variable value
    const SymbolTableEntry *resultEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (resultEntry != nullptr) {
      llvm::Type *resultSTy = resultEntry->getQualType().toLLVMType(sourceFile);
      llvm::Value *returnValueAddr = resultEntry->getAddress();
      returnValue = insertLoad(resultSTy, returnValueAddr);
    }
  }

  // Terminate the block (with scope cleanup)
  terminateBlock(node->getParentScopeNode());

  // Create return instruction
  if (returnValue != nullptr) {
    // Return with value
    builder.CreateRet(returnValue);
  } else {
    // Return without value
    builder.CreateRetVoid();
  }

  return nullptr;
}

std::any IRGenerator::visitBreakStmt(const BreakStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Jump to destination block
  const size_t blockIdx = breakBlocks.size() - node->breakTimes;
  insertJump(breakBlocks.at(blockIdx));

  return nullptr;
}

std::any IRGenerator::visitContinueStmt(const ContinueStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Jump to destination block
  const size_t blockIdx = continueBlocks.size() - node->continueTimes;
  insertJump(continueBlocks.at(blockIdx));

  return nullptr;
}

std::any IRGenerator::visitFallthroughStmt(const FallthroughStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Jump to destination block
  insertJump(fallthroughBlocks.top());

  return nullptr;
}

std::any IRGenerator::visitAssertStmt(const AssertStmtNode *node) {
  // Only generate assertions in debug build mode or in test mode
  if (cliOptions.buildMode != DEBUG && !cliOptions.testMode)
    return nullptr;

  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string &codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = createBlock("assert.then." + codeLine);
  llvm::BasicBlock *bExit = createBlock("assert.exit." + codeLine);

  // Visit the assignExpr
  llvm::Value *condValue = resolveValue(node->assignExpr);

  // Create condition check
  insertCondJump(condValue, bExit, bThen, LIKELY);

  // Switch to then block
  switchToBlock(bThen);
  // Create constant for error message
  const std::string errorMsg = "Assertion failed: Condition '" + node->expressionString + "' evaluated to false.\n";
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