// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

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
  auto exprResult = any_cast<LLVMExprResult>(visit(node));
  return resolveValue(node, exprResult, accessScope);
}

llvm::Value *IRGenerator::resolveValue(const ASTNode *node, LLVMExprResult &exprResult, Scope *accessScope /*=nullptr*/) {
  return resolveValue(node->getEvaluatedSymbolType(manIdx), exprResult, accessScope);
}

llvm::Value *IRGenerator::resolveValue(const SymbolType &symbolType, LLVMExprResult &exprResult,
                                       Scope *accessScope /*=nullptr*/) {
  // Check if the value is already present
  if (exprResult.value != nullptr)
    return exprResult.value;

  // Check if a constant is present
  if (exprResult.constant != nullptr) {
    materializeConstant(exprResult);
    return exprResult.value;
  }

  assert(exprResult.ptr != nullptr || exprResult.refPtr != nullptr);

  // Set access scope to current scope if nullptr gets passed
  if (!accessScope)
    accessScope = currentScope;

  // De-reference if reference type
  SymbolType referencedType = symbolType.removeReferenceWrapper();
  if (exprResult.refPtr != nullptr && exprResult.ptr == nullptr)
    exprResult.ptr = builder.CreateLoad(builder.getPtrTy(), exprResult.refPtr);

  // Load the value from the pointer
  llvm::Type *valueTy = referencedType.toLLVMType(context, accessScope);
  exprResult.value = builder.CreateLoad(valueTy, exprResult.ptr);

  return exprResult.value;
}

llvm::Value *IRGenerator::resolveAddress(const ASTNode *node, bool storeVolatile /*=false*/) {
  // Visit the given AST node
  auto exprResult = any_cast<LLVMExprResult>(visit(node));
  return resolveAddress(exprResult, storeVolatile);
}

llvm::Value *IRGenerator::resolveAddress(LLVMExprResult &exprResult, bool storeVolatile /*=false*/) {
  // Check if an address is already present
  if (exprResult.ptr != nullptr)
    return exprResult.ptr;

  // Check if the reference address is already present
  if (exprResult.refPtr != nullptr && exprResult.ptr == nullptr) {
    exprResult.ptr = builder.CreateLoad(builder.getPtrTy(), exprResult.refPtr);
    return exprResult.ptr;
  }

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

  // Pointer or reference
  if (symbolType.isOneOf({TY_PTR, TY_REF, TY_FUNCTION, TY_PROCEDURE}))
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
    // Retrieve struct type
    Scope *structScope = symbolType.getBodyScope();
    assert(structScope != nullptr);
    const size_t fieldCount = structScope->getFieldCount();
    auto structType = reinterpret_cast<llvm::StructType *>(symbolType.toLLVMType(context, structScope));

    // Get default values for all fields of the struct
    std::vector<llvm::Constant *> fieldConstants;
    fieldConstants.reserve(fieldCount);
    for (size_t i = 0; i < fieldCount; i++) {
      // Get entry of the field
      SymbolTableEntry *fieldEntry = structScope->symbolTable.lookupStrictByIndex(i);
      assert(fieldEntry != nullptr);

      // Retrieve field node
      const auto fieldNode = dynamic_cast<FieldNode *>(fieldEntry->declNode);
      assert(fieldNode != nullptr);

      // Retrieve default field value
      llvm::Constant *defaultFieldValue;
      if (fieldNode->defaultValue())
        defaultFieldValue = getConst(fieldNode->defaultValue()->getCompileTimeValue(), fieldEntry->getType(), fieldNode);
      else
        defaultFieldValue = getDefaultValueForSymbolType(fieldEntry->getType());

      fieldConstants.push_back(defaultFieldValue);
    }
    return llvm::ConstantStruct::get(structType, fieldConstants);
  }

  throw CompilerError(INTERNAL_ERROR, "Cannot determine default value for symbol type"); // GCOV_EXCL_LINE
}

llvm::Constant *IRGenerator::getConst(const CompileTimeValue &compileTimeValue, const SymbolType &type, const ASTNode *node) {
  if (type.is(TY_DOUBLE))
    return llvm::ConstantFP::get(context, llvm::APFloat(compileTimeValue.doubleValue));

  if (type.is(TY_INT))
    return builder.getInt32(compileTimeValue.intValue);

  if (type.is(TY_SHORT))
    return builder.getInt16(compileTimeValue.shortValue);

  if (type.is(TY_LONG))
    return builder.getInt64(compileTimeValue.longValue);

  if (type.isOneOf({TY_BYTE, TY_CHAR}))
    return builder.getInt8(compileTimeValue.charValue);

  if (type.is(TY_STRING))
    return createGlobalStringConst(ANON_GLOBAL_STRING_NAME, compileTimeValue.stringValue, node->codeLoc);

  if (type.is(TY_BOOL))
    return builder.getInt1(compileTimeValue.boolValue);

  throw CompilerError(UNHANDLED_BRANCH, "Constant fall-through"); // GCOV_EXCL_LINE
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
  parentFct->insert(parentFct->end(), block);
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

void IRGenerator::insertCondJump(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock,
                                 Likeliness likeliness /*=UNSPECIFIED*/) {
  if (blockAlreadyTerminated)
    return;
  llvm::BranchInst *jumpInst = builder.CreateCondBr(condition, trueBlock, falseBlock);
  blockAlreadyTerminated = true;

  if (likeliness != UNSPECIFIED) {
    const bool likely = likeliness == LIKELY;
    llvm::Metadata *name = llvm::MDString::get(context, "branch_weights");
    llvm::Metadata *trueBranchWeight = llvm::ConstantAsMetadata::get(builder.getInt32(likely ? 2000 : 1));
    llvm::Metadata *falseBranchWeight = llvm::ConstantAsMetadata::get(builder.getInt32(likely ? 1 : 2000));
    auto profMetadata = llvm::MDNode::get(context, {name, trueBranchWeight, falseBranchWeight});
    jumpInst->setMetadata("prof", profMetadata);
  }
}

void IRGenerator::verifyFunction(llvm::Function *fct, const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.generateDebugInfo)
    return;

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*fct, &oss))
    throw CompilerError(codeLoc, INVALID_FUNCTION, output);
}

void IRGenerator::verifyModule(const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.generateDebugInfo)
    return;

  // Verify module
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyModule(*module, &oss))
    throw CompilerError(codeLoc, INVALID_MODULE, output);
}

LLVMExprResult IRGenerator::doAssignment(const ASTNode *lhsNode, const ASTNode *rhsNode) {
  // Get entry of left side
  auto lhs = std::any_cast<LLVMExprResult>(visit(lhsNode));
  llvm::Value *lhsAddress = lhs.entry != nullptr && lhs.entry->getType().isRef() ? lhs.refPtr : lhs.ptr;
  return doAssignment(lhsAddress, lhs.entry, rhsNode);
}

LLVMExprResult IRGenerator::doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, const ASTNode *rhsNode,
                                         bool isDecl) {
  // Get symbol type of right side
  const SymbolType &rhsSType = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
  return doAssignment(lhsAddress, lhsEntry, rhs, rhsSType, isDecl);
}

LLVMExprResult IRGenerator::doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, LLVMExprResult &rhs,
                                         const SymbolType &rhsSType, bool isDecl) {
  // Deduce some information about the assignment
  const bool isRefAssign = lhsEntry != nullptr && lhsEntry->getType().isRef();
  const bool needsShallowCopy = !isDecl && !isRefAssign && rhsSType.is(TY_STRUCT);

  if (isRefAssign) {
    if (isDecl) { // Reference gets initially assigned
      // Get address of right side
      llvm::Value *rhsAddress = resolveAddress(rhs);
      assert(rhsAddress != nullptr);

      // Store lhs pointer to rhs
      llvm::Value *refAddress = insertAlloca(builder.getPtrTy());
      lhsEntry->updateAddress(refAddress);
      builder.CreateStore(rhsAddress, refAddress);

      return LLVMExprResult{.value = rhsAddress, .ptr = refAddress, .entry = lhsEntry};
    }

    // Reference to reference assignment (only for struct fields that are not initialized yet)
    if (rhsSType.isRef() && rhs.entry && lhsEntry->isField()) {
      // Get address of right side
      llvm::Value *referencedAddress = resolveAddress(rhs);
      assert(referencedAddress != nullptr);

      // Store the rhs* to the lhs**
      builder.CreateStore(referencedAddress, lhsAddress);

      return LLVMExprResult{.value = referencedAddress, .ptr = lhsAddress, .entry = lhsEntry};
    }

    // Load referenced address
    lhsAddress = builder.CreateLoad(builder.getPtrTy(), lhsAddress);
  }

  // Check if we need to copy the rhs to the lhs. This happens for structs
  if (needsShallowCopy) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhs);
    assert(rhsAddress != nullptr);
    // Create shallow copy
    llvm::Type *rhsType = rhsSType.toLLVMType(context, currentScope);
    const std::string copyName = lhsEntry ? lhsEntry->name : "";
    llvm::Value *newAddress = createShallowCopy(rhsAddress, rhsType, lhsAddress, copyName, lhsEntry && lhsEntry->isVolatile);
    // Set address of lhs to the copy
    if (lhsEntry && lhsEntry->scope->type != SCOPE_STRUCT && lhsEntry->scope->type != SCOPE_INTERFACE)
      lhsEntry->updateAddress(newAddress);
    return LLVMExprResult{.ptr = newAddress, .entry = lhsEntry};
  }

  if (isDecl && rhsSType.is(TY_STRUCT)) {
    materializeConstant(rhs);

    // Directly set the address to the lhs entry
    lhsEntry->updateAddress(resolveAddress(rhs, lhsEntry->isVolatile));

    // If we have value, store it to the address
    if (rhs.value) {
      assert(rhs.ptr != nullptr);
      builder.CreateStore(rhs.value, rhs.ptr);
    }

    rhs.entry = lhsEntry;
    return rhs;
  }

  // We can load the value from the right side and store it to the left side
  // Retrieve value of the right side
  llvm::Value *rhsValue = resolveValue(rhsSType, rhs, currentScope);
  // Allocate new memory if the lhs address does not exist
  if (!lhsAddress) {
    assert(lhsEntry != nullptr);
    lhsAddress = insertAlloca(lhsEntry->getType().toLLVMType(context, currentScope));
    lhsEntry->updateAddress(lhsAddress);
  }
  // Store the value to the address
  builder.CreateStore(rhsValue, lhsAddress);
  return LLVMExprResult{.value = rhsValue, .ptr = lhsAddress, .entry = lhsEntry};
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
  while (symbolType.isPtr() || symbolType.isRef()) {
    ptr = builder.CreateLoad(symbolType.toLLVMType(context, accessScope), ptr);
    symbolType = symbolType.getContainedTy();
  }
}

void IRGenerator::generateScopeCleanup(const StmtLstNode *node) const {
  // Do not clean up if the block is already terminated
  if (blockAlreadyTerminated)
    return;

  // Call all dtor functions
  for (auto [entry, dtor] : node->dtorFunctions.at(manIdx))
    generateDtorCall(entry, dtor, node);
}

void IRGenerator::generateDtorCall(SymbolTableEntry *entry, Function *dtor, const StmtLstNode *node) const {
  assert(dtor != nullptr);

  // Retrieve metadata for the function
  const std::string mangledName = NameMangling::mangleFunction(*dtor);

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), builder.getPtrTy(), false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Retrieve address of the struct variable. For fields this is the 'this' variable, otherwise use the normal address
  llvm::Value *structPtr;
  if (entry->isField()) {
    // Take 'this' var as base pointer
    SymbolTableEntry *thisVar = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisVar != nullptr);
    llvm::Value *baseAddress = thisVar->getAddress();
    // Add field offset
    llvm::Type *fieldType = entry->getType().toLLVMType(context, currentScope);
    llvm::ArrayRef<llvm::Value *> indices = {builder.getInt32(0), builder.getInt32(entry->orderIndex)};
    structPtr = builder.CreateInBoundsGEP(fieldType, baseAddress, indices);
  } else {
    structPtr = entry->getAddress();
  }
  assert(structPtr != nullptr);

  // Generate function call
  builder.CreateCall(callee, structPtr);
}

llvm::GlobalVariable *IRGenerator::createGlobalConst(const std::string &baseName, llvm::Constant *constant) {
  // Get unused name
  const std::string globalName = getUnusedGlobalName(baseName);
  // Create global
  module->getOrInsertGlobal(globalName, constant->getType());
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);
  // Set initializer to the given constant
  global->setInitializer(constant);
  global->setConstant(true);
  global->setLinkage(llvm::GlobalValue::PrivateLinkage);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  return global;
}

llvm::Constant *IRGenerator::createGlobalStringConst(const std::string &baseName, const std::string &value,
                                                     const CodeLoc &codeLoc) {
  // Get unused name
  const std::string globalName = getUnusedGlobalName(baseName);
  // Create global
  llvm::Constant *globalAddr = builder.CreateGlobalStringPtr(value, globalName, 0, module);
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);
  // Create debug info
  if (cliOptions.generateDebugInfo)
    diGenerator.generateGlobalStringDebugInfo(global, globalName, value.length(), codeLoc);
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
  while (srcSTy.isPtr()) {
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

void IRGenerator::materializeConstant(LLVMExprResult &exprResult) {
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
  assert(!scope->isGenericScope);
  // Adjust members of the new scope
  scope->parent = currentScope;
  scope->symbolTable.parent = &currentScope->symbolTable;
  // Set the scope
  currentScope = scope;
}

/**
 * Returns the operator function list for the current manifestation and the given node
 *
 * @param node Node to retrieve the op fct pointer list from
 * @return Op fct pointer list
 */
const std::vector<const Function *> &IRGenerator::getOpFctPointers(const ASTNode *node) const {
  assert(node->opFct.size() > manIdx);
  return node->opFct.at(manIdx);
}

} // namespace spice::compiler