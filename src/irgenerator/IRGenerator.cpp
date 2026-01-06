// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <SourceFile.h>
#include <global/GlobalResourceManager.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

IRGenerator::IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), context(cliOptions.useLTO ? resourceManager.ltoContext : sourceFile->context),
      builder(sourceFile->builder), module(sourceFile->llvmModule.get()), conversionManager(sourceFile, this),
      stdFunctionManager(sourceFile, resourceManager, module) {
  // Attach information to the module
  module->setTargetTriple(cliOptions.targetTriple);
  module->setDataLayout(sourceFile->targetMachine->createDataLayout());
  module->setPICLevel(llvm::PICLevel::BigPIC);
  module->setPIELevel(llvm::PIELevel::Large);
  module->setUwtable(llvm::UWTableKind::Default);
  module->setFramePointer(llvm::FramePointerKind::All);

  // Add module identifier metadata
  llvm::NamedMDNode *identifierMetadata = module->getOrInsertNamedMetadata("llvm.ident");
  identifierMetadata->addOperand(llvm::MDNode::get(context, llvm::MDString::get(context, PRODUCER_STRING)));

  // Initialize debug info generator
  if (cliOptions.instrumentation.generateDebugInfo)
    diGenerator.initialize(sourceFile->fileName, sourceFile->fileDir);
}

std::any IRGenerator::visitEntry(const EntryNode *node) {
  // Generate IR
  visitChildren(node);

  // Generate test main if required
  if (sourceFile->isMainFile && cliOptions.generateTestMain)
    generateTestMain();

  // Execute deferred VTable initializations
  for (DeferredLogic &deferredVTableInit : deferredVTableInitializations)
    deferredVTableInit.execute();

  // Finalize debug info generator
  diGenerator.finalize();

  // Verify module
  verifyModule(node->codeLoc);

  return nullptr;
}

llvm::AllocaInst *IRGenerator::insertAlloca(llvm::Type *llvmType, std::string varName) {
  if (!cliOptions.namesForIRValues)
    varName = "";

  if (allocaInsertInst != nullptr) { // If there is already an alloca inst, insert right after that
    llvm::AllocaInst *allocaInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInst->setDebugLoc(llvm::DebugLoc());
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else { // This is the first alloca inst in the current function -> insert at the entry block
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder.GetInsertBlock();
    builder.SetInsertPoint(allocaInsertBlock, allocaInsertBlock->begin());

    // Allocate the size of the given LLVM type
    allocaInsertInst = builder.CreateAlloca(llvmType, nullptr, varName);
    allocaInsertInst->setDebugLoc(llvm::DebugLoc());

    // Restore old basic block
    builder.SetInsertPoint(currentBlock);
  }

  // Insert lifetime start marker
  if (cliOptions.useLifetimeMarkers) {
    const uint64_t sizeInBytes = module->getDataLayout().getTypeAllocSize(llvmType);
    builder.CreateLifetimeStart(allocaInsertInst, builder.getInt64(sizeInBytes));
  }

  return allocaInsertInst;
}

llvm::AllocaInst *IRGenerator::insertAlloca(const QualType &qualType, const std::string &varName) {
  llvm::Type *llvmType = qualType.toLLVMType(sourceFile);
  llvm::AllocaInst *alloca = insertAlloca(llvmType, varName);

  // Insert type metadata
  if (cliOptions.useTBAAMetadata)
    mdGenerator.generateTypeMetadata(allocaInsertInst, qualType);

  return alloca;
}

llvm::LoadInst *IRGenerator::insertLoad(llvm::Type *llvmType, llvm::Value *ptr, bool isVolatile,
                                        const std::string &varName) const {
  assert(ptr->getType()->isPointerTy());
  return builder.CreateLoad(llvmType, ptr, isVolatile, cliOptions.namesForIRValues ? varName : "");
}

llvm::LoadInst *IRGenerator::insertLoad(const QualType &qualType, llvm::Value *ptr, bool isVolatile, const std::string &varName) {
  llvm::Type *llvmType = qualType.toLLVMType(sourceFile);
  llvm::LoadInst *load = insertLoad(llvmType, ptr, isVolatile, varName);
  if (cliOptions.useTBAAMetadata)
    mdGenerator.generateTBAAMetadata(load, qualType);
  return load;
}

llvm::StoreInst *IRGenerator::insertStore(llvm::Value *val, llvm::Value *ptr, bool isVolatile) const {
  assert(ptr->getType()->isPointerTy());
  return builder.CreateStore(val, ptr, isVolatile);
}

void IRGenerator::insertStore(llvm::Value *val, llvm::Value *ptr, const QualType &qualType, bool isVolatile) {
  llvm::StoreInst *store = insertStore(val, ptr, isVolatile);
  if (cliOptions.useTBAAMetadata)
    mdGenerator.generateTBAAMetadata(store, qualType);
}

llvm::Value *IRGenerator::insertInBoundsGEP(llvm::Type *type, llvm::Value *basePtr, llvm::ArrayRef<llvm::Value *> indices,
                                            std::string varName) const {
  assert(basePtr->getType()->isPointerTy());
  assert(!indices.empty());
  assert(std::ranges::all_of(indices, [](const llvm::Value *index) {
    const llvm::Type *indexType = index->getType();
    return indexType->isIntegerTy(32) || indexType->isIntegerTy(64);
  }));

  if (!cliOptions.namesForIRValues)
    varName = "";

  // Insert GEP
  return builder.CreateInBoundsGEP(type, basePtr, indices, varName);
}

llvm::Value *IRGenerator::insertStructGEP(llvm::Type *type, llvm::Value *basePtr, unsigned int index, std::string varName) const {
  assert(basePtr->getType()->isPointerTy());

  if (!cliOptions.namesForIRValues)
    varName = "";

  // If we use index 0 we can use the base pointer directly
  if (index == 0)
    return basePtr;

  // Insert GEP
  return builder.CreateStructGEP(type, basePtr, index, varName);
}

llvm::Value *IRGenerator::resolveValue(const ExprNode *node) {
  // Visit the given AST node
  auto exprResult = any_cast<LLVMExprResult>(visit(node));
  return resolveValue(node, exprResult);
}

llvm::Value *IRGenerator::resolveValue(const ExprNode *node, LLVMExprResult &exprResult) {
  return resolveValue(node->getEvaluatedSymbolType(manIdx), exprResult);
}

llvm::Value *IRGenerator::resolveValue(const QualType &qualType, LLVMExprResult &exprResult) {
  // Check if the value is already present
  if (exprResult.value != nullptr)
    return exprResult.value;

  // Check if a constant is present
  if (exprResult.constant != nullptr) {
    materializeConstant(exprResult);
    return exprResult.value;
  }

  assert(exprResult.ptr != nullptr || exprResult.refPtr != nullptr);

  // De-reference if reference type
  const bool isVolatile = exprResult.entry && exprResult.entry->isVolatile;
  if (exprResult.refPtr != nullptr && exprResult.ptr == nullptr)
    exprResult.ptr = insertLoad(builder.getPtrTy(), exprResult.refPtr, isVolatile);

  // Load the value from the pointer
  const QualType referencedType = qualType.removeReferenceWrapper();
  exprResult.value = insertLoad(referencedType, exprResult.ptr, isVolatile);

  return exprResult.value;
}

llvm::Value *IRGenerator::resolveAddress(const ASTNode *node) {
  // Visit the given AST node
  auto exprResult = any_cast<LLVMExprResult>(visit(node));
  return resolveAddress(exprResult);
}

llvm::Value *IRGenerator::resolveAddress(LLVMExprResult &exprResult) {
  // Check if an address is already present
  if (exprResult.ptr != nullptr)
    return exprResult.ptr;

  // Check if the reference address is already present
  const bool isVolatile = exprResult.entry && exprResult.entry->isVolatile;
  if (exprResult.refPtr != nullptr && exprResult.ptr == nullptr) {
    exprResult.ptr = insertLoad(builder.getPtrTy(), exprResult.refPtr, isVolatile);
    return exprResult.ptr;
  }

  // If not, store the value or constant
  materializeConstant(exprResult);
  assert(exprResult.value != nullptr);
  exprResult.ptr = insertAlloca(exprResult.value->getType(), exprResult.entry ? exprResult.entry->name : "");
  insertStore(exprResult.value, exprResult.ptr, isVolatile);

  return exprResult.ptr;
}

llvm::Constant *IRGenerator::getDefaultValueForSymbolType(const QualType &symbolType) { // NOLINT(misc-no-recursion)
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
  if (symbolType.is(TY_STRING)) {
    llvm::GlobalVariable *globalString = builder.CreateGlobalString("", "");
    if (cliOptions.comparableOutput)
      globalString->setAlignment(llvm::Align(4));
    return globalString;
  }

  // Bool
  if (symbolType.is(TY_BOOL))
    return builder.getFalse();

  // Pointer or reference
  if (symbolType.isOneOf({TY_PTR, TY_REF}))
    return llvm::Constant::getNullValue(builder.getPtrTy());

  // Array
  if (symbolType.isArray()) {
    // Get array size
    const size_t arraySize = symbolType.getArraySize();

    // Get default value for item
    llvm::Constant *defaultItemValue = getDefaultValueForSymbolType(symbolType.getContained());

    // Retrieve array and item type
    llvm::Type *itemType = symbolType.getContained().toLLVMType(sourceFile);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(itemType, arraySize);

    // Create a constant array with n times the default value
    const std::vector itemConstants(arraySize, defaultItemValue);
    return llvm::ConstantArray::get(arrayType, itemConstants);
  }

  // Function or procedure
  if (symbolType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    if (!llvmTypes.fatPtrType)
      llvmTypes.fatPtrType = llvm::StructType::get(context, {builder.getPtrTy(), builder.getPtrTy()});

    llvm::Constant *ptrDefaultValue = getDefaultValueForSymbolType(QualType(TY_PTR));
    return llvm::ConstantStruct::get(llvmTypes.fatPtrType, {ptrDefaultValue, ptrDefaultValue});
  }

  // Struct
  if (symbolType.is(TY_STRUCT)) {
    // Retrieve field count
    Scope *structScope = symbolType.getBodyScope();
    assert(structScope != nullptr);
    const size_t fieldCount = structScope->getFieldCount();

    // Get default values for all fields of the struct
    std::vector<llvm::Constant *> fieldConstants;
    fieldConstants.reserve(fieldCount);

    // Add default value for each struct field
    for (size_t i = 0; i < fieldCount; i++) {
      // Get entry of the field
      const SymbolTableEntry *fieldEntry = structScope->lookupField(i);
      assert(fieldEntry != nullptr && fieldEntry->isField());

      // Retrieve default field value
      llvm::Constant *defaultFieldValue;
      if (const auto fieldNode = dynamic_cast<FieldNode *>(fieldEntry->declNode); fieldNode && fieldNode->defaultValue)
        defaultFieldValue = getConst(fieldNode->defaultValue->getCompileTimeValue(), fieldEntry->getQualType(), fieldNode);
      else
        defaultFieldValue = getDefaultValueForSymbolType(fieldEntry->getQualType());

      fieldConstants.push_back(defaultFieldValue);
    }

    const auto structType = reinterpret_cast<llvm::StructType *>(symbolType.toLLVMType(sourceFile));
    return llvm::ConstantStruct::get(structType, fieldConstants);
  }

  // Interface
  if (symbolType.is(TY_INTERFACE)) {
    const auto structType = reinterpret_cast<llvm::StructType *>(symbolType.toLLVMType(sourceFile));
    return llvm::ConstantStruct::get(structType, llvm::Constant::getNullValue(builder.getPtrTy()));
  }

  throw CompilerError(INTERNAL_ERROR, "Cannot determine default value for symbol type"); // GCOV_EXCL_LINE
}

llvm::Constant *IRGenerator::getConst(const CompileTimeValue &compileTimeValue, const QualType &type, const ASTNode *node) const {
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

  if (type.is(TY_STRING)) {
    const std::string &stringValue = resourceManager.compileTimeStringValues.at(compileTimeValue.stringValueOffset);
    return createGlobalStringConst(ANON_GLOBAL_STRING_NAME, stringValue, node->codeLoc);
  }

  if (type.is(TY_BOOL))
    return builder.getInt1(compileTimeValue.boolValue);

  if (type.is(TY_PTR))
    return llvm::Constant::getNullValue(builder.getPtrTy());

  throw CompilerError(UNHANDLED_BRANCH, "Constant fall-through"); // GCOV_EXCL_LINE
}

llvm::BasicBlock *IRGenerator::createBlock(const std::string &blockName /*=""*/) const {
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

void IRGenerator::terminateBlock(const StmtLstNode *stmtLstNode) {
  diGenerator.setSourceLocation(stmtLstNode->closingBraceCodeLoc);
  generateScopeCleanup(stmtLstNode);
  blockAlreadyTerminated = true;
}

void IRGenerator::insertJump(llvm::BasicBlock *targetBlock) {
  if (blockAlreadyTerminated)
    return;
  builder.CreateBr(targetBlock);
  blockAlreadyTerminated = true;
}

void IRGenerator::insertCondJump(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock,
                                 Likelihood likelihood /*=UNSPECIFIED*/) {
  if (blockAlreadyTerminated)
    return;
  llvm::BranchInst *jumpInst = builder.CreateCondBr(condition, trueBlock, falseBlock);
  blockAlreadyTerminated = true;

  if (likelihood != Likelihood::UNSPECIFIED)
    mdGenerator.generateBranchWeightsMetadata(jumpInst, likelihood);
}

void IRGenerator::verifyFunction(const llvm::Function *fct, const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.instrumentation.generateDebugInfo)
    return;

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*fct, &oss))                     // LCOV_EXCL_LINE
    throw CompilerError(codeLoc, INVALID_FUNCTION, output); // LCOV_EXCL_LINE
}

void IRGenerator::verifyModule(const CodeLoc &codeLoc) const {
  // Skip the verifying step if the verifier was disabled manually or debug info is emitted
  if (cliOptions.disableVerifier || cliOptions.instrumentation.generateDebugInfo)
    return;

  // Verify module
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyModule(*module, &oss))                  // LCOV_EXCL_LINE
    throw CompilerError(codeLoc, INVALID_MODULE, output); // LCOV_EXCL_LINE
}

LLVMExprResult IRGenerator::doAssignment(const ASTNode *lhsNode, const ExprNode *rhsNode, const ASTNode *node) {
  // Get entry of left side
  auto [value, constant, ptr, refPtr, entry, _] = std::any_cast<LLVMExprResult>(visit(lhsNode));
  llvm::Value *lhsAddress = entry != nullptr && entry->getQualType().isRef() ? refPtr : ptr;
  return doAssignment(lhsAddress, entry, rhsNode, node);
}

LLVMExprResult IRGenerator::doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, const ExprNode *rhsNode,
                                         const ASTNode *node, bool isDecl) {
  // Get symbol type of right side
  const QualType &rhsSType = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
  return doAssignment(lhsAddress, lhsEntry, rhs, rhsSType, node, isDecl);
}

LLVMExprResult IRGenerator::doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, LLVMExprResult &rhs,
                                         const QualType &rhsSType, const ASTNode *node, bool isDecl) {
  // Deduce some information about the assignment
  const bool isRefAssign = lhsEntry != nullptr && lhsEntry->getQualType().isRef();
  const bool needsCopy = !isRefAssign && rhsSType.removeReferenceWrapper().is(TY_STRUCT) && !rhs.isTemporary();

  if (isRefAssign) {
    assert(lhsEntry != nullptr);
    if (isDecl) { // Reference gets initially assigned
      // Get address of right side
      llvm::Value *rhsAddress = resolveAddress(rhs);
      assert(rhsAddress != nullptr);

      // Store lhs pointer to rhs
      llvm::Value *refAddress = insertAlloca(builder.getPtrTy());
      lhsEntry->updateAddress(refAddress);
      insertStore(rhsAddress, refAddress);

      return LLVMExprResult{.value = rhsAddress, .ptr = refAddress, .entry = lhsEntry};
    }

    // Reference to reference assignment (only for struct fields that are not initialized yet)
    // These are only allowed inside a ctor body. In other cases, the value of the reference gets assigned, not the ref itself.
    const bool isInitialFieldRefAssign = isInCtorBody && rhsSType.isRef() && rhs.entry && lhsEntry->isField();
    // Assigning the result variable
    const bool isReturnValAssign = lhsEntry->name == RETURN_VARIABLE_NAME;
    if (isInitialFieldRefAssign || isReturnValAssign) {
      // Get address of right side
      llvm::Value *referencedAddress = resolveAddress(rhs);
      assert(referencedAddress != nullptr);

      // Store the rhs* to the lhs**
      insertStore(referencedAddress, lhsAddress);

      return LLVMExprResult{.value = referencedAddress, .ptr = lhsAddress, .entry = lhsEntry};
    }

    // Load referenced address
    lhsAddress = insertLoad(builder.getPtrTy(), lhsAddress);
  }

  if (isDecl && rhsSType.is(TY_STRUCT) && rhs.isTemporary()) {
    assert(lhsEntry != nullptr);
    // Directly set the address to the lhs entry (temp stealing)
    llvm::Value *rhsAddress = resolveAddress(rhs);
    lhsEntry->updateAddress(rhsAddress);
    rhs.entry = lhsEntry;
    return rhs;
  }

  // Allocate new memory if the lhs address does not exist
  if (!lhsAddress) {
    assert(lhsEntry != nullptr);
    lhsAddress = insertAlloca(lhsEntry->getQualType());
    lhsEntry->updateAddress(lhsAddress);
  }

  // Check if we try to assign an array by value to a pointer. Here we have to store the address of the first element to the lhs
  if (lhsEntry && lhsEntry->getQualType().isPtr() && rhsSType.isArray() && rhsSType.getArraySize() != ARRAY_SIZE_UNKNOWN) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhs);
    assert(rhsAddress != nullptr);
    llvm::Type *elementTy = rhsSType.toLLVMType(sourceFile);
    llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(0)};
    llvm::Value *firstItemAddress = insertInBoundsGEP(elementTy, rhsAddress, indices);
    insertStore(firstItemAddress, lhsAddress);
    return LLVMExprResult{.value = rhsAddress, .ptr = lhsAddress, .entry = lhsEntry};
  }

  // Check if we need to copy the rhs to the lhs. This happens for structs
  if (needsCopy) {
    // Get address of right side
    llvm::Value *rhsAddress = resolveAddress(rhs);
    assert(rhsAddress != nullptr);

    const QualType rhsSTypeNonRef = rhsSType.removeReferenceWrapper().toNonConst();
    if (rhsSTypeNonRef.isTriviallyCopyable(node)) {
      // Create shallow copy
      llvm::Type *rhsType = rhsSTypeNonRef.toLLVMType(sourceFile);
      const std::string copyName = lhsEntry ? lhsEntry->name : "";
      generateShallowCopy(rhsAddress, rhsType, lhsAddress, lhsEntry && lhsEntry->isVolatile);
    } else {
      // Check if we have a copy ctor
      Scope *structScope = rhsSTypeNonRef.getBodyScope();
      const ArgList args = {{rhsSTypeNonRef.toConstRef(node), rhs.isTemporary()}};
      const Function *copyCtor = FunctionManager::lookup(structScope, CTOR_FUNCTION_NAME, rhsSTypeNonRef, args, true);
      if (copyCtor != nullptr) {
        // Call copy ctor
        generateCtorOrDtorCall(lhsAddress, copyCtor, {rhsAddress});
      } else {
        const std::string structName = rhsSTypeNonRef.getName();
        const std::string msg = "Cannot copy struct '" + structName + "', as it is not trivially copyable and has no copy ctor";
        throw SemanticError(node, COPY_CTOR_REQUIRED, msg);
      }
    }
    return LLVMExprResult{.ptr = lhsAddress, .entry = lhsEntry};
  }

  // Optimization: If we have the address of both sides, we can do a memcpy instead of loading and storing the value
  llvm::Value *rhsValue = nullptr;
  if (rhsSType.is(TY_STRUCT) && rhs.value == nullptr && rhs.constant == nullptr) {
    // Create shallow copy
    const QualType rhsSTypeNonRef = rhsSType.removeReferenceWrapper().toNonConst();
    llvm::Type *rhsType = rhsSTypeNonRef.toLLVMType(sourceFile);
    llvm::Value *rhsAddress = resolveAddress(rhs);
    assert(rhsAddress != nullptr);
    const std::string copyName = lhsEntry ? lhsEntry->name : "";
    generateShallowCopy(rhsAddress, rhsType, lhsAddress, lhsEntry && lhsEntry->isVolatile);
  } else {
    // We can load the value from the right side and store it to the left side
    // Retrieve value of the right side
    rhsValue = resolveValue(rhsSType, rhs);
    // Store the value to the address
    insertStore(rhsValue, lhsAddress, rhsSType);
  }

  return LLVMExprResult{.value = rhsValue, .ptr = lhsAddress, .entry = lhsEntry};
}

void IRGenerator::generateShallowCopy(llvm::Value *oldAddress, llvm::Type *varType, llvm::Value *targetAddress,
                                      bool isVolatile) const {
  // Retrieve size to copy
  const llvm::TypeSize typeSize = module->getDataLayout().getTypeAllocSize(varType);

  // Create values for memcpy intrinsic
  llvm::Value *structSize = builder.getInt64(typeSize);
  llvm::Value *copyVolatile = builder.getInt1(isVolatile);

  // Call memcpy intrinsic to execute the shallow copy
  llvm::Function *memcpyFct = stdFunctionManager.getMemcpyIntrinsic();
  assert(targetAddress != nullptr);
  builder.CreateCall(memcpyFct, {targetAddress, oldAddress, structSize, copyVolatile});
}

void IRGenerator::autoDeReferencePtr(llvm::Value *&ptr, QualType &symbolType) {
  while (symbolType.isPtr() || symbolType.isRef()) {
    ptr = insertLoad(symbolType, ptr);
    symbolType = symbolType.getContained();
  }
}

llvm::GlobalVariable *IRGenerator::createGlobalConst(const std::string &baseName, llvm::Constant *constant) const {
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

llvm::GlobalVariable *IRGenerator::createGlobalStringConst(const std::string &baseName, const std::string &value) const {
  // Get unused name
  const std::string globalName = getUnusedGlobalName(baseName);
  // Create global
  builder.CreateGlobalString(value, globalName, 0, module);
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);
  // If the output should be comparable, fix alignment to 4 bytes
  if (cliOptions.comparableOutput)
    global->setAlignment(llvm::Align(4));
  return global;
}

llvm::GlobalVariable *IRGenerator::createGlobalStringConst(const std::string &baseName, const std::string &value,
                                                           const CodeLoc &codeLoc) const {
  llvm::GlobalVariable *global = createGlobalStringConst(baseName, value);
  // Create debug info
  if (cliOptions.instrumentation.generateDebugInfo)
    diGenerator.generateGlobalStringDebugInfo(global, global->getName().str(), value.length(), codeLoc);
  return global;
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

void IRGenerator::materializeConstant(LLVMExprResult &exprResult) {
  // Skip results, that do not contain a constant or already have a value
  if (exprResult.value != nullptr || exprResult.constant == nullptr)
    return;

  // Default case: the value to the constant
  exprResult.value = exprResult.constant;
}

std::string IRGenerator::getIRString(llvm::Module *llvmModule, const CliOptions &cliOptions) {
  assert(llvmModule != nullptr); // Make sure the module hasn't been moved away
  const bool eliminateTarget = cliOptions.comparableOutput && cliOptions.isNativeTarget;

  // Backup target triple and data layout
  const llvm::Triple targetTriple = llvmModule->getTargetTriple();
  const std::string targetDataLayout = llvmModule->getDataLayoutStr();
  // Remove target triple and data layout
  if (eliminateTarget) {
    llvmModule->setTargetTriple(llvm::Triple());
    llvmModule->setDataLayout("");
  }

  // Get IR string
  std::string output;
  llvm::raw_string_ostream oss(output);
  llvmModule->print(oss, nullptr);

  // Restore target triple and data layout
  if (eliminateTarget) {
    llvmModule->setTargetTriple(targetTriple);
    llvmModule->setDataLayout(targetDataLayout);
  }

  return output;
}

/**
 * Returns the operator function list for the current manifestation and the given node
 *
 * @param node Node to retrieve the op fct pointer list from
 * @return Op fct pointer list
 */
const std::vector<const Function *> &IRGenerator::getOpFctPointers(const ASTNode *node) const {
  assert(node->getOpFctPointers()->size() > manIdx);
  return node->getOpFctPointers()->at(manIdx);
}

} // namespace spice::compiler