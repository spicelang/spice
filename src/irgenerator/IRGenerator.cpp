// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <exception/IRError.h>

#include <llvm/BinaryFormat/Dwarf.h>
#include <llvm/IR/Verifier.h>

IRGenerator::IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), context(resourceManager.context), builder(resourceManager.builder),
      module(sourceFile->llvmModule.get()), stdFunctionManager(StdFunctionManager(resourceManager, sourceFile->llvmModule.get())),
      currentScope(sourceFile->globalScope.get()) {
  // Attach information to the module
  module->setTargetTriple(resourceManager.cliOptions.targetTriple);
  module->setDataLayout(resourceManager.targetMachine->createDataLayout());

  // Initialize debug info generator
  if (cliOptions.generateDebugInfo)
    diGenerator.initialize(sourceFile->fileName, sourceFile->fileDir);
}

std::any IRGenerator::visitEntry(const EntryNode *node) {
  // Generate IR
  visitChildren(node);

  // Finalize debug info generator
  diGenerator.finalize();

  // Verify module
  verifyModule(node->codeLoc);

  return nullptr;
}

llvm::Value *IRGenerator::insertAlloca(llvm::Type *llvmType, const std::string &varName) {
  if (allocaInsertInst != nullptr) { // If there is already an alloca inst, insert right after that
    llvm::AllocaInst *allocaInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInst->setDebugLoc(llvm::DebugLoc());
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else { // This is the first alloca inst in the current function -> insert at the entry block
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder.GetInsertBlock();
    builder.SetInsertPoint(allocaInsertBlock);

    // Allocate the size of the given LLVM type
    allocaInsertInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInsertInst->setDebugLoc(llvm::DebugLoc());

    // Restore old basic block
    builder.SetInsertPoint(currentBlock);
  }
  return static_cast<llvm::Value *>(allocaInsertInst);
}

llvm::Value *IRGenerator::resolveValue(const ASTNode *node, Scope *accessScope /*=nullptr*/) {
  // Visit the given AST node
  auto exprResult = any_cast<ExprResult>(visit(node));
  return resolveValue(node, exprResult);
}

llvm::Value *IRGenerator::resolveValue(const ASTNode *node, ExprResult &exprResult, Scope *accessScope /*=nullptr*/) {
  // Set access scope to current scope if nullptr gets passed
  if (!accessScope)
    accessScope = currentScope;

  // Check if a constant or a value is already present
  if (exprResult.constant != nullptr)
    return exprResult.constant;
  if (exprResult.value != nullptr)
    return exprResult.value;

  // If not, load the value from the pointer
  assert(exprResult.ptr != nullptr);
  llvm::Type *valueTy = node->getEvaluatedSymbolType(manIdx).toLLVMType(context, accessScope);
  return builder.CreateLoad(valueTy, exprResult.ptr);
}

llvm::Value *IRGenerator::resolveAddress(const ASTNode *node, bool storeVolatile /*=false*/) {
  // Visit the given AST node
  auto exprResult = any_cast<ExprResult>(visit(node));
  return resolveAddress(exprResult);
}

llvm::Value *IRGenerator::resolveAddress(ExprResult &exprResult, bool storeVolatile /*=false*/) {
  // Check if an address is already present
  if (exprResult.ptr != nullptr)
    return exprResult.ptr;

  if (exprResult.entry)
    storeVolatile |= exprResult.entry->isVolatile;

  // If not, store the value or constant
  llvm::Value *value = exprResult.constant ?: exprResult.value;
  assert(value != nullptr);
  llvm::Value *address = insertAlloca(value->getType(), exprResult.entry ? exprResult.entry->name : "");
  builder.CreateStore(value, address, storeVolatile);
  return address;
}

llvm::Constant *IRGenerator::getDefaultValueForSymbolType(const SymbolType &symbolType) { // NOLINT(misc-no-recursion)
  // Double
  if (symbolType.is(TY_DOUBLE))
    return llvm::ConstantFP::get(context, llvm::APFloat(0.0));

  // Int
  if (symbolType.is(TY_INT))
    return builder.getInt32(0);

  // Short
  if (symbolType.is(TY_SHORT))
    return builder.getInt16(0);

  // Long
  if (symbolType.is(TY_LONG))
    return builder.getInt64(0);

  // Byte or char
  if (symbolType.isOneOf({TY_BYTE, TY_CHAR}))
    return builder.getInt8(0);

  // String
  if (symbolType.is(TY_STRING))
    return builder.CreateGlobalStringPtr("", "", 0, module);

  // Bool
  if (symbolType.is(TY_BOOL))
    return builder.getFalse();

  // Pointer
  if (symbolType.is(TY_PTR) || (symbolType.is(TY_ARRAY) && symbolType.getArraySize() == ARRAY_SIZE_DYNAMIC))
    return llvm::Constant::getNullValue(builder.getPtrTy());

  // Array
  if (symbolType.is(TY_ARRAY)) {
    // Get array size
    const size_t arraySize = symbolType.getArraySize();

    // Get default value for item
    llvm::Constant *defaultItemValue = getDefaultValueForSymbolType(symbolType.getContainedTy());

    // Retrieve array and item type
    llvm::Type *itemType = symbolType.getContainedTy().toLLVMType(context, currentScope);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(itemType, arraySize);

    // Create a constant array with n times the default value
    std::vector<llvm::Constant *> itemConstants(arraySize, defaultItemValue);
    return llvm::ConstantArray::get(arrayType, itemConstants);
  }

  // Struct
  if (symbolType.is(TY_STRUCT)) {
    // Get struct entry
    SymbolTableEntry *structEntry = currentScope->lookup(symbolType.getSubType());
    assert(structEntry != nullptr);

    // Retrieve struct type
    Scope *structScope = structEntry->getType().getStructBodyScope();
    assert(structScope != nullptr);
    const size_t fieldCount = structScope->getFieldCount();
    auto structType = reinterpret_cast<llvm::StructType *>(structEntry->getType().toLLVMType(context, structScope));

    // Allocate space for the struct in memory
    llvm::Value *structAddress = insertAlloca(structType);

    // Get default values for all fields of the struct
    std::vector<llvm::Constant *> fieldConstants;
    fieldConstants.reserve(fieldCount);
    for (size_t i = 0; i < fieldCount; i++) {
      // Get entry of the field
      SymbolTableEntry *fieldEntry = structScope->symbolTable.lookupByIndex(i);
      assert(fieldEntry != nullptr);

      // Retrieve default field value
      llvm::Constant *defaultFieldValue = getDefaultValueForSymbolType(fieldEntry->getType());

      // Store the default value at the correct offset
      llvm::Value *fieldAddress = builder.CreateStructGEP(structType, structAddress, i);
      builder.CreateStore(defaultFieldValue, fieldAddress);

      fieldConstants.push_back(defaultFieldValue);
    }
    return llvm::ConstantStruct::get(structType, fieldConstants);
  }

  throw std::runtime_error("Internal compiler error: Cannot determine default value for symbol type"); // GCOV_EXCL_LINE
}

llvm::BasicBlock *IRGenerator::createBlock(const std::string &blockName, llvm::Function *parentFct /*=nullptr*/) {
  // Create block
  llvm::BasicBlock *block = llvm::BasicBlock::Create(context, blockName);
  // If no parent function were passed, use the current function
  if (!parentFct)
    parentFct = builder.GetInsertBlock()->getParent();
  // Append to current function
  parentFct->getBasicBlockList().push_back(block);
  return block;
}

void IRGenerator::insertJump(llvm::BasicBlock *targetBlock) {
  if (blockAlreadyTerminated)
    return;
  builder.CreateBr(targetBlock);
  blockAlreadyTerminated = true;
}

void IRGenerator::insertCondJump(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) {
  if (blockAlreadyTerminated)
    return;
  builder.CreateCondBr(condition, trueBlock, falseBlock);
  blockAlreadyTerminated = true;
}

void IRGenerator::switchToBlock(llvm::BasicBlock *block) {
  builder.SetInsertPoint(block);
  blockAlreadyTerminated = false;
}

void IRGenerator::verifyFunction(llvm::Function *fct, const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.generateDebugInfo)
    return;

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*fct, &oss))
    throw IRError(codeLoc, INVALID_FUNCTION, output);
}

void IRGenerator::verifyModule(const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.generateDebugInfo)
    return;

  // Verify module
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyModule(*module, &oss))
    throw IRError(codeLoc, INVALID_MODULE, output);
}

ExprResult IRGenerator::doAssignment(const ASTNode *lhsNode, const ASTNode *rhsNode) {
  // Get entry of left side
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));
  assert(lhs.entry != nullptr);

  return doAssignment(lhs.entry, rhsNode);
}

ExprResult IRGenerator::doAssignment(SymbolTableEntry *lhsEntry, const ASTNode *rhsNode) {
  assert(lhsEntry != nullptr);

  // Get symbol types of left and right side
  const SymbolType lhsSType = lhsEntry->getType();
  const SymbolType rhsSType = rhsNode->getEvaluatedSymbolType(manIdx);

  // Deduce some information about the assignment
  const bool isRefAssign = lhsSType.isReference();
  const bool needsShallowCopy = !isRefAssign && lhsSType.is(TY_STRUCT);

  if (isRefAssign) { // We simply set the address of lhs to the address of rhs
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhsNode);
    assert(rhsAddress != nullptr);

    // Set address of rhs to lhs entry
    lhsEntry->updateAddress(rhsAddress);
    return ExprResult{.ptr = rhsAddress, .entry = lhsEntry};
  }

  // Check if we need to copy the rhs to the lhs. This happens for structs
  if (needsShallowCopy) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhsNode);
    assert(rhsAddress != nullptr);
    // Create shallow copy
    llvm::Value *newAddress = createShallowCopy(rhsAddress, rhsAddress->getType(), lhsEntry->name, lhsEntry->isVolatile);
    // Set address of lhs to the copy
    lhsEntry->updateAddress(newAddress);
    return ExprResult{.ptr = newAddress, .entry = lhsEntry};
  }

  // We can load the value from the right side and store it to the left side
  // Retrieve value of the right side
  llvm::Value *rhsValue = resolveValue(rhsNode);
  // Retrieve address of the lhs side
  llvm::Value *lhsAddress = lhsEntry->getAddress();
  // Allocate new memory if the lhs address does not exist
  if (!lhsAddress) {
    lhsAddress = insertAlloca(lhsSType.toLLVMType(context, currentScope));
    lhsEntry->updateAddress(lhsAddress);
  }
  // Store the value to the address
  builder.CreateStore(rhsValue, lhsAddress);
  return ExprResult{.ptr = lhsAddress, .value = rhsValue, .entry = lhsEntry};
}

llvm::Value *IRGenerator::createShallowCopy(llvm::Value *oldAddress, llvm::Type *varType, const std::string &name /*=""*/,
                                            bool isVolatile /*=false*/) {
  // Retrieve size to copy
  const unsigned int typeSize = module->getDataLayout().getTypeSizeInBits(varType);

  // Create values for memcpy intrinsic
  llvm::Value *structSize = builder.getInt64(typeSize);
  llvm::Value *copyVolatile = builder.getInt1(isVolatile);
  llvm::Value *newAddress = insertAlloca(varType, name);

  // Call memcpy intrinsic to execute the shallow copy
  llvm::Function *memcpyFct = stdFunctionManager.getMemcpyIntrinsic();
  builder.CreateCall(memcpyFct, {newAddress, oldAddress, structSize, copyVolatile});

  return newAddress;
}

void IRGenerator::autoDeReferencePtr(llvm::Value *ptr, SymbolType symbolType, Scope *accessScope) const {
  while (symbolType.isPointer()) {
    ptr = builder.CreateLoad(symbolType.toLLVMType(context, accessScope), ptr);
    symbolType = symbolType.getContainedTy();
  }
}

llvm::Value *IRGenerator::createGlobalConstant(const std::string &baseName, llvm::Constant *constant) {
  // Get unused name
  const std::string globalName = getUnusedGlobalName(baseName);
  // Create global
  llvm::Value *globalAddr = module->getOrInsertGlobal(globalName, constant->getType());
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);
  // Set initializer to the given constant
  global->setInitializer(constant);
  global->setConstant(true);
  global->setLinkage(llvm::GlobalValue::PrivateLinkage);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  return globalAddr;
}

std::string IRGenerator::getUnusedGlobalName(const std::string &baseName) const {
  // Find an unused global name
  std::string globalName;
  unsigned int suffixNumber = 0;
  do {
    globalName = baseName + std::to_string(suffixNumber);
    suffixNumber++;
  } while (module->getNamedGlobal(globalName) != nullptr);
  return globalName;
}

std::string IRGenerator::getIRString() const {
  std::string output;
  llvm::raw_string_ostream oss(output);
  module->print(oss, nullptr);
  return oss.str();
}