// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <exception/IRError.h>

#include <llvm/IR/Verifier.h>

namespace spice::compiler {

std::any IRGenerator::visitUnsafeBlockDef(const UnsafeBlockDefNode *node) {
  diGenerator.setSourceLocation(node);

  // Change scope
  changeToScope(node->bodyScope, SCOPE_UNSAFE_BODY);

  // Visit instructions in the block
  visit(node->body());

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitForLoop(const ForLoopNode *node) {
  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bHead = createBlock("for.head." + codeLine);
  llvm::BasicBlock *bBody = createBlock("for.body." + codeLine);
  llvm::BasicBlock *bTail = createBlock("for.tail." + codeLine);
  llvm::BasicBlock *bExit = createBlock("for.exit." + codeLine);

  // Change scope
  changeToScope(node->bodyScope, SCOPE_FOR_BODY);

  // Save the blocks for break and continue
  breakBlocks.push_back(bExit);
  continueBlocks.push_back(bTail);

  // Init statement
  visit(node->initDecl());
  // Create jump from original to head block
  insertJump(bHead);

  // Switch to head block
  switchToBlock(bHead);
  // Condition evaluation
  llvm::Value *condValue = resolveValue(node->condAssign());
  // Create conditional jump from head to body or exit block
  insertCondJump(condValue, bBody, bExit);

  // Switch to body block
  switchToBlock(bBody);
  // Visit body
  visit(node->body());
  // Create jump from body to tail block
  insertJump(bTail);

  // Switch to tail block
  switchToBlock(bTail);
  // Inc statement
  visit(node->incAssign());
  // Create jump from tail to head
  insertJump(bHead);

  // Switch to exit block
  switchToBlock(bExit);

  // Pop basic blocks from break and continue stacks
  assert(breakBlocks.back() == bExit);
  breakBlocks.pop_back();
  assert(continueBlocks.back() == bTail);
  continueBlocks.pop_back();

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitForeachLoop(const ForeachLoopNode *node) {
  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bHead = createBlock("foreach.head." + codeLine);
  llvm::BasicBlock *bBody = createBlock("foreach.body." + codeLine);
  llvm::BasicBlock *bTail = createBlock("foreach.tail." + codeLine);
  llvm::BasicBlock *bExit = createBlock("foreach.exit." + codeLine);

  // Change scope
  changeToScope(node->bodyScope, SCOPE_FOREACH_BODY);

  // Save the blocks for break and continue
  breakBlocks.push_back(bExit);
  continueBlocks.push_back(bTail);

  // Resolve iterator
  AssignExprNode *iteratorAssignNode = node->iteratorAssign();
  SymbolType iteratorType = iteratorAssignNode->getEvaluatedSymbolType(manIdx).removeReferenceWrapper();
  const SymbolType itemSTy = iteratorType.getTemplateTypes().front();
  const SymbolType itemRefSTy = itemSTy.toReference(node);
  assert(!node->getFct || itemRefSTy == node->getFct->returnType);
  assert(!node->getIdxFct || itemRefSTy == node->getIdxFct->returnType.getTemplateTypes().back());
  llvm::Value *iterator = resolveAddress(iteratorAssignNode);

  // Visit idx variable declaration if required
  const DeclStmtNode *idxDeclNode = node->idxVarDecl();
  const bool hasIdx = idxDeclNode != nullptr;
  SymbolTableEntry *idxEntry;
  llvm::Value *idxAddress;
  if (hasIdx) {
    visit(idxDeclNode);
    // Get address of idx variable
    idxEntry = idxDeclNode->entries.at(manIdx);
    idxAddress = idxEntry->getAddress();
    assert(idxAddress != nullptr);
  }

  // Visit item variable declaration
  const DeclStmtNode *itemDeclNode = node->itemVarDecl();
  visit(itemDeclNode);
  // Get address of item variable
  SymbolTableEntry *itemEntry = itemDeclNode->entries.at(manIdx);
  llvm::Value *itemAddress = itemEntry->getAddress();
  assert(itemAddress != nullptr);

  // Create jump from original to head block
  insertJump(bHead);

  // Switch to head block
  switchToBlock(bHead);
  // Call .isValid() on iterator
  assert(node->isValidFct);
  llvm::Function *isValidFct = stdFunctionManager.getIteratorIsValidFct(node->isValidFct);
  llvm::Value *isValid = builder.CreateCall(isValidFct, iterator);
  // Create conditional jump from head to body or exit block
  insertCondJump(isValid, bBody, bExit);

  // Switch to body block
  switchToBlock(bBody);
  // Get the current iterator values
  if (hasIdx) {
    // Allocate space to save pair
    llvm::Type *pairTy = node->getIdxFct->returnType.toLLVMType(context, currentScope);
    llvm::Value *pairPtr = insertAlloca(pairTy, "pair_addr");
    // Call .getIdx() on iterator
    assert(node->getIdxFct);
    llvm::Function *getIdxFct = stdFunctionManager.getIteratorGetIdxFct(node->getIdxFct, currentScope);
    llvm::Value *pair = builder.CreateCall(getIdxFct, iterator);
    pair->setName("pair");
    builder.CreateStore(pair, pairPtr);
    // Store idx to idx var
    llvm::Value *idxAddrInPair = builder.CreateStructGEP(pairTy, pairPtr, 0, "idx_addr");
    LLVMExprResult idxResult = {.ptr = idxAddrInPair};
    doAssignment(idxAddress, idxEntry, idxResult, SymbolType(TY_LONG), false);
    // Store item to item var
    llvm::Value *itemAddrInPair = builder.CreateStructGEP(pairTy, pairPtr, 1, "item_addr");
    LLVMExprResult itemResult = {.refPtr = itemAddrInPair};
    doAssignment(itemAddress, itemEntry, itemResult, itemRefSTy, false);
  } else {
    // Call .get() on iterator
    assert(node->getFct);
    llvm::Function *getFct = stdFunctionManager.getIteratorGetFct(node->getFct);
    llvm::Value *getItemPtr = builder.CreateCall(getFct, iterator);
    LLVMExprResult getResult = {.ptr = getItemPtr};
    doAssignment(itemAddress, itemEntry, getResult, itemRefSTy, false);
  }
  // Visit body
  visit(node->body());
  // Create jump from body to tail block
  insertJump(bTail);

  // Switch to tail block
  switchToBlock(bTail);
  // Call .next() on iterator
  assert(node->nextFct);
  llvm::Function *nextFct = stdFunctionManager.getIteratorNextFct(node->nextFct);
  builder.CreateCall(nextFct, iterator);
  // Create jump from tail to head block
  insertJump(bHead);

  // Switch to exit block
  switchToBlock(bExit);

  // Pop basic blocks from break and continue stacks
  assert(breakBlocks.back() == bExit);
  breakBlocks.pop_back();
  assert(continueBlocks.back() == bTail);
  continueBlocks.pop_back();

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitWhileLoop(const WhileLoopNode *node) {
  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bHead = createBlock("while.head." + codeLine);
  llvm::BasicBlock *bBody = createBlock("while.body." + codeLine);
  llvm::BasicBlock *bExit = createBlock("while.exit." + codeLine);

  // Change scope
  changeToScope(node->bodyScope, SCOPE_WHILE_BODY);

  // Save the blocks for break and continue
  breakBlocks.push_back(bExit);
  continueBlocks.push_back(bHead);

  // Jump to head block
  insertJump(bHead);

  // Switch to head block
  switchToBlock(bHead);
  // Evaluate condition
  llvm::Value *condValue = resolveValue(node->condition());
  // Jump to body or exit block, depending on the condition
  insertCondJump(condValue, bBody, bExit);

  // Switch to body block
  switchToBlock(bBody);
  // Visit body
  visit(node->body());
  // Create jump to head block
  insertJump(bHead);

  // Switch to exit block
  switchToBlock(bExit);

  // Pop basic blocks from break and continue stacks
  assert(breakBlocks.back() == bExit);
  breakBlocks.pop_back();
  assert(continueBlocks.back() == bHead);
  continueBlocks.pop_back();

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitDoWhileLoop(const DoWhileLoopNode *node) {
  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bBody = createBlock("dowhile.body." + codeLine);
  llvm::BasicBlock *bFoot = createBlock("dowhile.foot." + codeLine);
  llvm::BasicBlock *bExit = createBlock("dowhile.exit." + codeLine);

  // Change scope
  changeToScope(node->bodyScope, SCOPE_WHILE_BODY);

  // Save the blocks for break and continue
  breakBlocks.push_back(bExit);
  continueBlocks.push_back(bFoot);

  // Jump to body block
  insertJump(bBody);

  // Switch to body block
  switchToBlock(bBody);
  // Visit body
  visit(node->body());
  // Create jump to foot block
  insertJump(bFoot);

  // Switch to head block
  switchToBlock(bFoot);
  // Evaluate condition
  llvm::Value *condValue = resolveValue(node->condition());
  // Jump to body or exit block, depending on the condition
  insertCondJump(condValue, bBody, bExit);

  // Switch to exit block
  switchToBlock(bExit);

  // Pop basic blocks from break and continue stacks
  assert(breakBlocks.back() == bExit);
  breakBlocks.pop_back();
  assert(continueBlocks.back() == bFoot);
  continueBlocks.pop_back();

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitIfStmt(const IfStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Create blocks
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = createBlock("if.then." + codeLine);
  llvm::BasicBlock *bElse = node->elseStmt() ? createBlock("if.else." + codeLine) : nullptr;
  llvm::BasicBlock *bExit = createBlock("if.exit." + codeLine);

  // Change scope
  changeToScope(node->thenBodyScope, SCOPE_IF_ELSE_BODY);

  // Retrieve condition value
  llvm::Value *condValue = resolveValue(node->condition());
  // Check if condition is fulfilled
  insertCondJump(condValue, bThen, node->elseStmt() ? bElse : bExit);

  // Switch to then block
  switchToBlock(bThen);
  // Visit then body
  visit(node->thenBody());
  // Create jump from then to end block
  insertJump(bExit);

  // Change scope back
  currentScope = node->thenBodyScope->parent;
  assert(currentScope != nullptr);

  if (node->elseStmt()) {
    // Switch to else block
    switchToBlock(bElse);
    // Visit else block
    visit(node->elseStmt());
    // Create jump from else to end block
    insertJump(bExit);
  }

  // Switch to exit block
  switchToBlock(bExit);

  // Return conditional value as result for the if stmt
  return condValue;
}

std::any IRGenerator::visitElseStmt(const ElseStmtNode *node) {
  diGenerator.setSourceLocation(node);

  if (node->ifStmt()) { // It is an else if branch
    visit(node->ifStmt());
  } else { // It is an else branch
    // Change scope
    changeToScope(node->elseBodyScope, SCOPE_IF_ELSE_BODY);

    // Generate IR for nested statements
    visit(node->body());

    // Change scope back
    currentScope = node->elseBodyScope->parent;
    assert(currentScope != nullptr);
  }

  return nullptr;
}

std::any IRGenerator::visitAnonymousBlockStmt(const AnonymousBlockStmtNode *node) {
  diGenerator.setSourceLocation(node);

  // Change scope
  node->bodyScope->parent = currentScope;                           // Needed for nested scopes in generic functions
  node->bodyScope->symbolTable.parent = &currentScope->symbolTable; // Needed for nested scopes in generic functions
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);

  // Visit instructions in the block
  visit(node->body());

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

} // namespace spice::compiler