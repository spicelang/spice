// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <exception/IRError.h>

#include <llvm/IR/Verifier.h>

namespace spice::compiler {

std::any IRGenerator::visitThreadDef(const ThreadDefNode *node) {
  diGenerator.setSourceLocation(node);

  // Create LLVM function for thread body
  const std::string fctName = "_thread" + std::to_string(resourceManager.threadFactory.getNextFunctionSuffix());
  llvm::Type *voidPtrTy = builder.getInt8PtrTy();
  llvm::FunctionType *threadFctTy = llvm::FunctionType::get(voidPtrTy, voidPtrTy, false);
  llvm::Function *threadFct = llvm::Function::Create(threadFctTy, llvm::Function::PrivateLinkage, fctName, module);

  // Change scope
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);

  // Collect arg names, types and addresses from captures of the nested scope
  std::vector<std::string> argStructFieldNames;
  std::vector<llvm::Type *> argStructFieldTypes;
  std::vector<llvm::Value *> argStructFieldPointers;
  for (const auto &[name, capture] : currentScope->symbolTable.captures) {
    argStructFieldNames.push_back(name);
    argStructFieldTypes.push_back(capture.capturedEntry->getType().toLLVMType(context, currentScope)->getPointerTo());
    argStructFieldPointers.push_back(capture.capturedEntry->getAddress());
  }

  // Create arg struct instance
  llvm::StructType *argStructTy = llvm::StructType::get(context, argStructFieldTypes, false);
  llvm::Value *argStruct = insertAlloca(argStructTy);

  // Fill the struct with the argument values
  for (size_t i = 0; i < argStructFieldNames.size(); i++) {
    llvm::Value *fieldAddress = builder.CreateStructGEP(argStructTy, argStruct, i);
    builder.CreateStore(argStructFieldPointers[i], fieldAddress);
  }

  // Get current basic block to return to later
  llvm::BasicBlock *bOriginal = builder.GetInsertBlock();

  // Create entry block for thread function
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bEntry = allocaInsertBlock = createBlock("thread.entry." + codeLine);
  switchToBlock(bEntry, threadFct);

  llvm::BasicBlock *allocaInsertBlockBackup = allocaInsertBlock;
  llvm::Instruction *allocaInsertInstBackup = allocaInsertInst;
  allocaInsertInst = nullptr;

  // Store function args
  llvm::Value *recArgStructPtr = builder.CreatePointerCast(threadFct->args().begin(), argStructTy->getPointerTo());
  size_t i = 0;
  for (const auto &[name, capture] : currentScope->symbolTable.captures) {
    const std::string argName = argStructFieldNames.at(i);
    llvm::Value *memAddress = builder.CreateStructGEP(argStructTy, recArgStructPtr, i);
    llvm::Type *loadType = capture.capturedEntry->getType().toLLVMType(context, currentScope)->getPointerTo();
    memAddress = builder.CreateLoad(loadType, memAddress);
    // Push address to each capture to ensure that the address is valid and known to the inner function
    capture.capturedEntry->pushAddress(memAddress);
    i++;
  }

  // Visit body
  visit(node->body());

  // Pop address from each capture to ensure that the address is valid and known to the outer function
  for (const auto &capture : currentScope->symbolTable.captures)
    capture.second.capturedEntry->popAddress();

  // Change scope back
  currentScope = node->bodyScope->parent;
  assert(currentScope != nullptr);

  // Insert return statement with nullptr value
  llvm::Value *voidPtrNull = llvm::Constant::getNullValue(voidPtrTy);
  builder.CreateRet(voidPtrNull);

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*threadFct, &oss))
    throw IRError(node->codeLoc, INVALID_FUNCTION, oss.str());

  // Change back to the original block
  switchToBlock(bOriginal);

  // Restore alloca insert block and inst
  allocaInsertBlock = allocaInsertBlockBackup;
  allocaInsertInst = allocaInsertInstBackup;

  // Create call to pthread_create
  llvm::Function *ptCreateFct = stdFunctionManager.getPthreadCreate();
  llvm::Value *pthread = builder.CreateAlloca(voidPtrTy); // Caution: Do not replace with insertAlloca() due to thread safety!
  builder.CreateCall(ptCreateFct, {pthread, voidPtrNull, threadFct, builder.CreatePointerCast(argStruct, voidPtrTy)});

  // Return the thread id as i8**
  return ExprResult{.ptr = pthread};
}

std::any IRGenerator::visitUnsafeBlockDef(const UnsafeBlockDefNode *node) {
  diGenerator.setSourceLocation(node);

  // Change scope
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);

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
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);
  breakBlocks.push(bExit);
  continueBlocks.push(bTail);

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
  breakBlocks.pop();
  continueBlocks.pop();

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
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);
  breakBlocks.push(bExit);
  continueBlocks.push(bTail);

  // Call .reset on iterator
  // ToDo: implement
  // Create jump from original to head node
  insertJump(bHead);

  // Switch to head block
  switchToBlock(bHead);
  // Call .isValid on iterator
  // ToDo: implement
  llvm::Value *condValue = nullptr;
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
  // Call .next on iterator
  // ToDo: implement
  // Create jump from tail to head block
  insertJump(bHead);

  // Switch to exit block
  switchToBlock(bExit);

  // Pop basic blocks from break and continue stacks
  breakBlocks.pop();
  continueBlocks.pop();

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
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);
  breakBlocks.push(bExit);
  continueBlocks.push(bHead);

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
  breakBlocks.pop();
  continueBlocks.pop();

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
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);
  breakBlocks.push(bExit);
  continueBlocks.push(bFoot);

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
  breakBlocks.pop();
  continueBlocks.pop();

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
  currentScope = node->thenBodyScope;
  assert(currentScope != nullptr);

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
    currentScope = node->elseBodyScope;
    assert(currentScope != nullptr);

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