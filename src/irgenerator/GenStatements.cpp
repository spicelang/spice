// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

std::any IRGenerator::visitStmtLst(const StmtLstNode *node) {
  // Generate instructions in the scope
  for (const ASTNode *stmt : node->children) {
    if (!stmt)
      continue;
    // Check if we can cancel generating instructions for this code branch
    if (blockAlreadyTerminated || stmt->unreachable)
      break;
    // Visit child
    visit(stmt);
  }

  // Generate cleanup code of this scope, e.g. dtor calls for struct instances
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
  const SymbolType varSymbolType = varEntry->getType();

  // Get LLVM type of variable
  Scope *accessScope = currentScope;
  if (varSymbolType.is(TY_STRUCT))
    accessScope = varSymbolType.getBodyScope()->parent;
  llvm::Type *varTy = varSymbolType.toLLVMType(context, accessScope);

  // Check if right side is dyn array. If this is the case we have an empty array initializer and need the default value
  const bool rhsIsDynArray = node->hasAssignment && node->assignExpr()->getEvaluatedSymbolType(manIdx).isArrayOf(TY_DYN);

  // Check if the declaration is with an assignment or the default value
  llvm::Value *varAddress = nullptr;
  if (node->hasAssignment && !rhsIsDynArray) { // Assignment
    LLVMExprResult assignResult = doAssignment(varAddress, varEntry, node->assignExpr(), true);
    assert(assignResult.entry == varEntry);
    varAddress = assignResult.entry->getAddress();
  } else { // Default value
    // Allocate memory
    varAddress = insertAlloca(varTy);

    if (node->defaultCtor) {
      // Call default constructor
      const std::string mangledName = NameMangling::mangleFunction(*node->defaultCtor);
      llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), builder.getPtrTy(), false);
      module->getOrInsertFunction(mangledName, fctType);
      llvm::Function *fct = module->getFunction(mangledName);
      assert(fct != nullptr);
      builder.CreateCall({fctType, fct}, varAddress);
    } else if (!node->isForEachItem) {
      // Retrieve default value for lhs symbol type and store it
      llvm::Constant *defaultValue = getDefaultValueForSymbolType(varSymbolType);
      builder.CreateStore(defaultValue, varAddress);
    }

    // Update address in symbol table
    varEntry->updateAddress(varAddress);
  }
  assert(varAddress != nullptr);

  // Attach the variable name to the LLVM value.
  varAddress->setName(varEntry->name);

  // Generate debug info for variable declaration
  if (cliOptions.generateDebugInfo) {
    diGenerator.setSourceLocation(node);
    diGenerator.generateLocalVarDebugInfo(node->varName, varAddress, SIZE_MAX, true);
  }

  return nullptr;
}

std::any IRGenerator::visitSpecifierLst(const SpecifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) {
  diGenerator.setSourceLocation(node);

  llvm::Value *returnValue = nullptr;
  if (node->hasReturnValue) { // Return value is attached to the return statement
    if (node->getEvaluatedSymbolType(manIdx).isRef())
      returnValue = resolveAddress(node->assignExpr());
    else
      returnValue = resolveValue(node->assignExpr());
  } else { // Try to load return variable value
    SymbolTableEntry *resultEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (resultEntry != nullptr) {
      llvm::Type *resultSTy = resultEntry->getType().toLLVMType(context, currentScope);
      llvm::Value *returnValueAddr = resultEntry->getAddress();
      returnValue = builder.CreateLoad(resultSTy, returnValueAddr);
    }
  }

  // Generate scope cleanup code
  generateScopeCleanup(node->getParentScopeNode());

  // Set block to terminated
  blockAlreadyTerminated = true;

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

std::any IRGenerator::visitAssertStmt(const AssertStmtNode *node) {
  // Only generate assertions with -O0 or in test mode
  if (cliOptions.optLevel != O0 && !cliOptions.testMode)
    return nullptr;

  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string &codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = createBlock("assert.then." + codeLine);
  llvm::BasicBlock *bExit = createBlock("assert.exit." + codeLine);

  // Visit the assignExpr
  llvm::Value *condValue = resolveValue(node->assignExpr());

  // Create condition check
  insertCondJump(condValue, bExit, bThen, LIKELY);

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