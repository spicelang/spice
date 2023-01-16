// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <exception/IRError.h>

#include <llvm/BinaryFormat/Dwarf.h>
#include <llvm/IR/Verifier.h>

namespace spice::compiler {

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

  // Check if the value is already present
  if (exprResult.value != nullptr)
    return exprResult.value;

  // Check if a constant is present
  if (exprResult.constant != nullptr) {
    materializeConstant(exprResult);
    return exprResult.value;
  }

  // If not, load the value from the pointer
  assert(exprResult.ptr != nullptr);
  llvm::Type *valueTy = node->getEvaluatedSymbolType(manIdx).toLLVMType(context, accessScope);
  exprResult.value = builder.CreateLoad(valueTy, exprResult.ptr);

  return exprResult.value;
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
  materializeConstant(exprResult);
  assert(exprResult.value != nullptr);
  exprResult.ptr = insertAlloca(exprResult.value->getType(), exprResult.entry ? exprResult.entry->name : "");
  builder.CreateStore(exprResult.value, exprResult.ptr, storeVolatile);

  return exprResult.ptr;
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
  if (symbolType.is(TY_PTR))
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
    Scope *accessScope = symbolType.getStructBodyScope()->parent;
    SymbolTableEntry *structEntry = accessScope->lookup(symbolType.getSubType());
    assert(structEntry != nullptr);

    // Retrieve struct type
    Scope *structScope = structEntry->getType().getStructBodyScope();
    assert(structScope != nullptr);
    const size_t fieldCount = structScope->getFieldCount();
    auto structType = reinterpret_cast<llvm::StructType *>(structEntry->getType().toLLVMType(context, structScope));

    // Get default values for all fields of the struct
    std::vector<llvm::Constant *> fieldConstants;
    fieldConstants.reserve(fieldCount);
    for (size_t i = 0; i < fieldCount; i++) {
      // Get entry of the field
      SymbolTableEntry *fieldEntry = structScope->symbolTable.lookupByIndex(i);
      assert(fieldEntry != nullptr);

      // Retrieve default field value
      llvm::Constant *defaultFieldValue = getDefaultValueForSymbolType(fieldEntry->getType());

      fieldConstants.push_back(defaultFieldValue);
    }
    return llvm::ConstantStruct::get(structType, fieldConstants);
  }

  throw std::runtime_error("Internal compiler error: Cannot determine default value for symbol type"); // GCOV_EXCL_LINE
}

llvm::BasicBlock *IRGenerator::createBlock(const std::string &blockName /*=""*/) {
  return llvm::BasicBlock::Create(context, blockName);
}

void IRGenerator::switchToBlock(llvm::BasicBlock *block, llvm::Function *parentFct /*=nullptr*/) {
  assert(block->getParent() == nullptr); // Ensure that the block was not added to a function already
  // If no parent function were passed, use the current function
  if (!parentFct)
    parentFct = builder.GetInsertBlock()->getParent();
  // Append block to current function
  parentFct->getBasicBlockList().push_back(block);
  // Set insert point to the block
  builder.SetInsertPoint(block);
  blockAlreadyTerminated = false;
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
  return doAssignment(lhs.ptr, lhs.entry, rhsNode);
}

ExprResult IRGenerator::doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, const ASTNode *rhsNode, bool isDecl) {
  // Get symbol types of left and right side
  const SymbolType &lhsSType = lhsEntry->getType();
  const SymbolType &rhsSType = rhsNode->getEvaluatedSymbolType(manIdx);

  // Deduce some information about the assignment
  const bool isRefAssign = lhsEntry != nullptr && lhsSType.isReference();
  const bool needsShallowCopy = !isDecl && !isRefAssign && rhsSType.is(TY_STRUCT);

  if (isRefAssign) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhsNode);
    assert(rhsAddress != nullptr);

    // Allocate space for the reference and store the address
    llvm::Value *refAddress = insertAlloca(rhsAddress->getType());
    builder.CreateStore(rhsAddress, refAddress);
    if (lhsEntry)
      lhsEntry->updateAddress(refAddress);
    return ExprResult{.ptr = refAddress, .value = rhsAddress, .entry = lhsEntry};
  }

  if (isDecl && rhsSType.is(TY_STRUCT)) {
    auto result = std::any_cast<ExprResult>(visit(rhsNode));
    materializeConstant(result);
    lhsEntry->updateAddress(resolveAddress(result, lhsEntry->isVolatile));
    result.entry = lhsEntry;
    return result;
  }

  // Check if we need to copy the rhs to the lhs. This happens for structs
  if (needsShallowCopy) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhsNode);
    assert(rhsAddress != nullptr);
    // Create shallow copy
    llvm::Type *rhsType = rhsSType.toLLVMType(context, currentScope);
    const std::string copyName = lhsEntry ? lhsEntry->name : "";
    llvm::Value *newAddress = createShallowCopy(rhsAddress, rhsType, lhsAddress, copyName, lhsEntry && lhsEntry->isVolatile);
    // Set address of lhs to the copy
    if (lhsEntry)
      lhsEntry->updateAddress(newAddress);
    return ExprResult{.ptr = newAddress, .entry = lhsEntry};
  }

  // We can load the value from the right side and store it to the left side
  // Retrieve value of the right side
  llvm::Value *rhsValue = resolveValue(rhsNode);
  // Allocate new memory if the lhs address does not exist
  if (!lhsAddress) {
    assert(lhsEntry != nullptr);
    lhsAddress = insertAlloca(lhsSType.toLLVMType(context, currentScope));
    if (lhsEntry)
      lhsEntry->updateAddress(lhsAddress);
  }
  // Store the value to the address
  builder.CreateStore(rhsValue, lhsAddress);
  return ExprResult{.ptr = lhsAddress, .value = rhsValue, .entry = lhsEntry};
}

llvm::Value *IRGenerator::createShallowCopy(llvm::Value *oldAddress, llvm::Type *varType, llvm::Value *targetAddress,
                                            const std::string &name /*=""*/, bool isVolatile /*=false*/) {
  // Retrieve size to copy
  const llvm::TypeSize typeSize = module->getDataLayout().getTypeAllocSize(varType);

  // Create values for memcpy intrinsic
  llvm::Value *structSize = builder.getInt64(typeSize);
  llvm::Value *copyVolatile = builder.getInt1(isVolatile);

  // If no target address was provided, allocate new space
  if (!targetAddress)
    targetAddress = insertAlloca(varType, name);

  // Call memcpy intrinsic to execute the shallow copy
  llvm::Function *memcpyFct = stdFunctionManager.getMemcpyIntrinsic();
  builder.CreateCall(memcpyFct, {targetAddress, oldAddress, structSize, copyVolatile});

  return targetAddress;
}

void IRGenerator::autoDeReferencePtr(llvm::Value *&ptr, SymbolType &symbolType, Scope *accessScope) const {
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

llvm::Value *IRGenerator::doImplicitCast(llvm::Value *src, SymbolType dstSTy, SymbolType srcSTy) {
  assert(srcSTy != dstSTy); // We only need to cast implicitly, if the types do not match exactly

  // Unpack the pointers until a pointer of another type is met
  size_t loadCounter = 0;
  while (srcSTy.isPointer()) {
    src = builder.CreateLoad(srcSTy.toLLVMType(context, currentScope), src);
    srcSTy = srcSTy.getContainedTy();
    dstSTy = dstSTy.getContainedTy();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstSTy.isArray() && srcSTy.isArray()) { // Special case that is used for passing arrays as pointer to functions
    llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
    src = builder.CreateInBoundsGEP(srcSTy.toLLVMType(context, currentScope), src, indices);
  } else {
    src = builder.CreateLoad(srcSTy.toLLVMType(context, currentScope), src);
    src = builder.CreateBitCast(src, dstSTy.toLLVMType(context, currentScope));
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(src->getType());
    builder.CreateStore(src, newActualArg);
    src = newActualArg;
  }
  return src;
}

void IRGenerator::materializeConstant(ExprResult &exprResult) {
  // Skip results, that do not contain a constant or already have a value
  if (exprResult.constant == nullptr || exprResult.value != nullptr)
    return;

  llvm::Type *constantType = exprResult.constant->getType();
  if (constantType->isArrayTy() || constantType->isStructTy()) {
    // Insert alloca for local variable
    llvm::Value *localAddr = insertAlloca(constantType);

    // If no address is given, we simply store the constant
    if (exprResult.ptr != nullptr) {
      // Get the size of the type in bytes
      const size_t instanceSize = module->getDataLayout().getTypeAllocSize(constantType);

      // Copy the constant to the local address via llvm.memcpy
      llvm::Function *memcpyIntrinsic = stdFunctionManager.getMemcpyIntrinsic();
      llvm::Value *args[4] = {localAddr, exprResult.ptr, builder.getInt64(instanceSize), builder.getFalse()};
      builder.CreateCall(memcpyIntrinsic, args);
    }

    // Set the pointer to the newly created local variable
    exprResult.ptr = localAddr;
  }

  // Default case: the value to the constant
  exprResult.value = exprResult.constant;
}

std::string IRGenerator::getIRString() const {
  std::string output;
  llvm::raw_string_ostream oss(output);
  module->print(oss, nullptr);
  return oss.str();
}

/**
 * Change to the passed scope.
 * For nested scopes in generic functions/procedures it is important to have the right parent for symbol lookups
 * Therefore, changeToScope sets the children's parent to the old scope to always have the right parent
 *
 * @param scope Scope to change to
 * @param scopeType Expected type of the given scope
 */
void IRGenerator::changeToScope(Scope *scope, const ScopeType scopeType) {
  assert(scope != nullptr);
  assert(scope->type == scopeType);
  // Adjust members of the new scope
  scope->parent = currentScope;
  scope->symbolTable.parent = &currentScope->symbolTable;
  // Set the scope
  currentScope = scope;
}

} // namespace spice::compiler