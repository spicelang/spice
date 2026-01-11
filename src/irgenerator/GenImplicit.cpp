// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <model/Function.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

// String placeholders for builtin testing output
static const char *const TEST_ALL_START_MSG = "[==========] Running %d test(s) from %d source file(s)\n";
static const char *const TEST_ALL_END_MSG = "[==========] Ran %d test(s) from %d source file(s)\n";
static const char *const TEST_FILE_START_MSG = "[----------] Running %d test(s) from %s\n";
static const char *const TEST_FILE_END_MSG = "[----------] Ran %d test(s) from %s\n\n";
static const char *const TEST_CASE_RUN_MSG = "[ RUN      ] %s\n";
static const char *const TEST_CASE_SUCCESS_MSG = "\033[1m\033[32m[ PASSED   ]\033[0m\033[22m %s\n";
static const char *const TEST_CASE_FAILED_MSG = "\033[1m\033[31m[ FAILED   ]\033[0m\033[22m %s\n";
static const char *const TEST_CASE_SKIPPED_MSG = "\033[1m\033[33m[ SKIPPED  ]\033[0m\033[22m %s\n";

llvm::Value *IRGenerator::doImplicitCast(llvm::Value *src, QualType dstSTy, QualType srcSTy) {
  assert(srcSTy != dstSTy); // We only need to cast implicitly, if the types do not match exactly

  // Unpack the pointers until a pointer of another type is met
  size_t loadCounter = 0;
  while (srcSTy.isPtr()) {
    src = insertLoad(srcSTy, src);
    srcSTy = srcSTy.getContained();
    dstSTy = dstSTy.getContained();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstSTy.isArray() && srcSTy.isArray()) { // Special case that is used for passing arrays as pointer to functions
    llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(0)};
    src = insertInBoundsGEP(srcSTy.toLLVMType(sourceFile), src, indices);
  } else {
    src = insertLoad(srcSTy, src);
    src = builder.CreateBitCast(src, dstSTy.toLLVMType(sourceFile));
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(srcSTy);
    insertStore(src, newActualArg);
    src = newActualArg;
  }
  return src;
}

void IRGenerator::generateScopeCleanup(const StmtLstNode *node) const {
  // Do not clean up if the block is already terminated
  if (blockAlreadyTerminated)
    return;

  // Call all dtor functions
  const auto &[dtorFunctionsToCall, heapVarsToFree] = node->resourcesToCleanup.at(manIdx);
  for (auto [entry, dtor] : dtorFunctionsToCall)
    generateCtorOrDtorCall(entry, dtor, {});

  // Deallocate all heap variables that go out of scope and are currently owned
  for (const SymbolTableEntry *entry : heapVarsToFree)
    generateDeallocCall(entry->getAddress());

  // Generate lifetime end markers
  if (cliOptions.useLifetimeMarkers) {
    for (const SymbolTableEntry *var : currentScope->getVarsGoingOutOfScope()) {
      llvm::Value *address = var->getAddress();
      if (address != nullptr)
        builder.CreateLifetimeEnd(address);
    }
  }
}

void IRGenerator::generateFctDecl(const Function *fct, const std::vector<llvm::Value *> &args) const {
  // Retrieve metadata for the function
  const std::string mangledName = fct->getMangledName();

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    std::vector<llvm::Type *> paramTypes;
    for (const llvm::Value *argValue : args)
      paramTypes.push_back(argValue->getType());
    llvm::Type *returnType = fct->isFunction() ? fct->returnType.toLLVMType(sourceFile) : builder.getVoidTy();
    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    module->getOrInsertFunction(mangledName, fctType);

    if (fct->isMethod()) {
      // Get callee function
      llvm::Function *callee = module->getFunction(mangledName);
      assert(callee != nullptr);

      // Set attributes to 'this' param
      // Get 'this' entry
      const SymbolTableEntry *thisEntry = fct->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
      assert(thisEntry != nullptr);
      llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
      assert(structType != nullptr);
      callee->addParamAttr(0, llvm::Attribute::NoUndef);
      callee->addParamAttr(0, llvm::Attribute::NonNull);
      callee->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
      callee->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
    }
  }
}

llvm::CallInst *IRGenerator::generateFctCall(const Function *fct, const std::vector<llvm::Value *> &args) const {
  // Retrieve metadata for the function
  const std::string mangledName = fct->getMangledName();

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Generate function call
  llvm::CallInst *callInst = builder.CreateCall(callee, args);

  // Set attributes to 'this' param
  if (fct->isMethod()) {
    // Get 'this' entry
    const SymbolTableEntry *thisEntry = fct->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
    assert(structType != nullptr);
    callInst->addParamAttr(0, llvm::Attribute::NoUndef);
    callInst->addParamAttr(0, llvm::Attribute::NonNull);
    callInst->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
    callInst->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
  }

  return callInst;
}

llvm::Value *IRGenerator::generateFctDeclAndCall(const Function *fct, const std::vector<llvm::Value *> &args) const {
  generateFctDecl(fct, args);
  return generateFctCall(fct, args);
}

void IRGenerator::generateProcDeclAndCall(const Function *proc, const std::vector<llvm::Value *> &args) const {
  generateFctDecl(proc, args);
  (void)generateFctCall(proc, args);
}

void IRGenerator::generateCtorOrDtorCall(const SymbolTableEntry *entry, const Function *ctorOrDtor,
                                         const std::vector<llvm::Value *> &args) const {
  // Retrieve address of the struct variable. For fields this is the 'this' variable, otherwise use the normal address
  llvm::Value *structAddr;
  if (entry->isField()) {
    // Take 'this' var as base pointer
    const SymbolTableEntry *thisVar = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisVar != nullptr);
    assert(thisVar->getQualType().isPtr() && thisVar->getQualType().getContained().is(TY_STRUCT));
    llvm::Type *thisType = thisVar->getQualType().getContained().toLLVMType(sourceFile);
    llvm::Value *thisPtr = insertLoad(builder.getPtrTy(), thisVar->getAddress());
    // Add field offset
    structAddr = insertStructGEP(thisType, thisPtr, entry->orderIndex);
  } else {
    structAddr = entry->getAddress();
    // For optional parameter initializers we need this exception
    if (!structAddr)
      return;
  }
  assert(structAddr != nullptr);
  generateCtorOrDtorCall(structAddr, ctorOrDtor, args);
}

void IRGenerator::generateCtorOrDtorCall(llvm::Value *structAddr, const Function *ctorOrDtor,
                                         const std::vector<llvm::Value *> &args) const {
  // Build parameter list
  std::vector argValues = {structAddr};
  argValues.insert(argValues.end(), args.begin(), args.end());

  // Generate function call
  generateProcDeclAndCall(ctorOrDtor, argValues);
}

void IRGenerator::generateDeallocCall(llvm::Value *variableAddress) const {
  // Abort if the address is not set. This can happen when leaving the scope of a dtor, which already freed the heap memory
  if (!variableAddress)
    return;

  // In case of string runtime, call free manually. Otherwise, use the memory_rt implementation of sDealloc()
  if (sourceFile->isStringRT()) {
    llvm::Function *freeFct = stdFunctionManager.getFreeFct();
    builder.CreateCall(freeFct, variableAddress);
  } else {
    llvm::Function *deallocFct = stdFunctionManager.getDeallocBytePtrRefFct();
    builder.CreateCall(deallocFct, variableAddress);
  }
}

llvm::Function *IRGenerator::generateImplicitFunction(const std::function<void()> &generateBody, const Function *spiceFunc) {
  // Only focus on method procedures
  const ASTNode *node = spiceFunc->entry->declNode;
  assert(spiceFunc->isFunction());

  // Only generate if used
  if (!spiceFunc->used)
    return nullptr;

  // Retrieve return type
  llvm::Type *returnType = spiceFunc->returnType.toLLVMType(sourceFile);

  // Get 'this' entry
  std::vector<llvm::Type *> paramTypes;
  SymbolTableEntry *thisEntry = nullptr;
  if (spiceFunc->isMethod()) {
    thisEntry = spiceFunc->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    paramTypes.push_back(builder.getPtrTy());
  }

  // Get parameter types
  for (const auto &[qualType, isOptional] : spiceFunc->paramList) {
    assert(!isOptional);
    paramTypes.push_back(qualType.toLLVMType(sourceFile));
  }

  // Get function linkage
  const bool isPublic = spiceFunc->entry->getQualType().isPublic();
  const llvm::GlobalValue::LinkageTypes linkage = isPublic ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

  // Create function
  const std::string mangledName = spiceFunc->getMangledName();
  llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, paramTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, mangledName, module);
  fct->setLinkage(linkage);
  fct->addFnAttr(llvm::Attribute::MustProgress);
  fct->addFnAttr(llvm::Attribute::NoUnwind);
  if (cliOptions.optLevel == OptLevel::O0) {
    fct->addFnAttr(llvm::Attribute::OptimizeNone);
    fct->addFnAttr(llvm::Attribute::NoInline);
  } else if (cliOptions.optLevel >= OptLevel::Os) {
    fct->addFnAttr(llvm::Attribute::OptimizeForSize);
  }
  fct->addFnAttr(llvm::Attribute::getWithUWTableKind(context, llvm::UWTableKind::Default));

  // Set attributes to 'this' param
  if (spiceFunc->isMethod()) {
    fct->addParamAttr(0, llvm::Attribute::NoUndef);
    fct->addParamAttr(0, llvm::Attribute::NonNull);
    assert(thisEntry != nullptr);
    llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
    assert(structType != nullptr);
    fct->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
    fct->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
  }

  // Add debug info
  diGenerator.generateFunctionDebugInfo(fct, spiceFunc);
  if (node != nullptr)
    diGenerator.setSourceLocation(node);

  // Change to body scope
  changeToScope(spiceFunc->getScopeName(), ScopeType::FUNC_PROC_BODY);

  // Create entry block
  llvm::BasicBlock *bEntry = createBlock();
  switchToBlock(bEntry, fct);

  // Reset alloca insert markers to this block
  allocaInsertBlock = bEntry;
  allocaInsertInst = nullptr;

  // Store first argument to 'this' symbol
  if (spiceFunc->isMethod()) {
    assert(thisEntry != nullptr);
    // Allocate space for the parameter
    llvm::Value *thisAddress = insertAlloca(paramTypes.front(), THIS_VARIABLE_NAME);
    // Update the symbol table entry
    thisEntry->updateAddress(thisAddress);
    // Store the value at the new address
    insertStore(fct->arg_begin(), thisAddress);
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(THIS_VARIABLE_NAME, thisAddress, 1);
  }

  // Generate body
  generateBody();

  // Conclude debug info for function
  diGenerator.concludeFunctionDebugInfo();

  // Verify function
  // Use the code location of the declaration node if available. Otherwise, (e.g. in case of test main) use an artificial code loc
  const CodeLoc codeLoc = node != nullptr ? node->codeLoc : CodeLoc(1, 1, sourceFile);
  verifyFunction(fct, codeLoc);

  // Change to parent scope
  changeToParentScope(ScopeType::FUNC_PROC_BODY);

  return fct;
}

llvm::Function *IRGenerator::generateImplicitProcedure(const std::function<void()> &generateBody, const Function *spiceProc) {
  // Only focus on method procedures
  const ASTNode *node = spiceProc->entry->declNode;
  assert(node != nullptr);
  assert(spiceProc->isProcedure());

  // Only generate if used
  if (!spiceProc->used)
    return nullptr;

  // Get 'this' entry
  std::vector<llvm::Type *> paramTypes;
  SymbolTableEntry *thisEntry = nullptr;
  if (spiceProc->isMethod()) {
    thisEntry = spiceProc->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    paramTypes.push_back(builder.getPtrTy());
  }

  // Get parameter types
  for (const auto &[qualType, isOptional] : spiceProc->paramList) {
    assert(!isOptional);
    paramTypes.push_back(qualType.toLLVMType(sourceFile));
  }

  // Get function linkage
  const bool isPublic = spiceProc->entry->getQualType().isPublic();
  const llvm::GlobalValue::LinkageTypes linkage = isPublic ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

  // Create function
  const std::string mangledName = spiceProc->getMangledName();
  llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), paramTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, mangledName, module);
  fct->setLinkage(linkage);
  fct->addFnAttr(llvm::Attribute::MustProgress);
  fct->addFnAttr(llvm::Attribute::NoUnwind);
  if (cliOptions.optLevel == OptLevel::O0) {
    fct->addFnAttr(llvm::Attribute::OptimizeNone);
    fct->addFnAttr(llvm::Attribute::NoInline);
  } else if (cliOptions.optLevel >= OptLevel::Os) {
    fct->addFnAttr(llvm::Attribute::OptimizeForSize);
  }
  fct->addFnAttr(llvm::Attribute::getWithUWTableKind(context, llvm::UWTableKind::Default));

  // Set attributes to 'this' param
  if (spiceProc->isMethod()) {
    fct->addParamAttr(0, llvm::Attribute::NoUndef);
    fct->addParamAttr(0, llvm::Attribute::NonNull);
    assert(thisEntry != nullptr);
    llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
    assert(structType != nullptr);
    fct->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
    fct->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
  }

  // Add debug info
  diGenerator.generateFunctionDebugInfo(fct, spiceProc);
  diGenerator.setSourceLocation(node);

  // Change to body scope
  changeToScope(spiceProc->getScopeName(), ScopeType::FUNC_PROC_BODY);

  // Create entry block
  llvm::BasicBlock *bEntry = createBlock();
  switchToBlock(bEntry, fct);

  // Reset alloca insert markers to this block
  allocaInsertBlock = bEntry;
  allocaInsertInst = nullptr;

  // Store first argument to 'this' symbol
  if (spiceProc->isMethod()) {
    assert(thisEntry != nullptr);
    // Allocate space for the parameter
    llvm::Value *thisAddress = insertAlloca(paramTypes.front(), THIS_VARIABLE_NAME);
    // Update the symbol table entry
    thisEntry->updateAddress(thisAddress);
    // Store the value at the new address
    insertStore(fct->arg_begin(), thisAddress);
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(THIS_VARIABLE_NAME, thisAddress, 1);
  }

  // Generate body
  generateBody();

  // Create return instruction
  builder.CreateRetVoid();

  // Conclude debug info for function
  diGenerator.concludeFunctionDebugInfo();

  // Verify function
  verifyFunction(fct, node->codeLoc);

  // Change to parent scope
  changeToParentScope(ScopeType::FUNC_PROC_BODY);

  return fct;
}

void IRGenerator::generateCtorBodyPreamble(Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  const SymbolTableEntry *thisEntry = bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisPtrPtr = thisEntry->getAddress();
  assert(thisPtrPtr != nullptr);
  llvm::Value *thisPtr = nullptr;
  const QualType structSymbolType = thisEntry->getQualType().getBase();
  llvm::Type *structType = structSymbolType.toLLVMType(sourceFile);

  // Store VTable to first struct field if required
  const Struct *spiceStruct = structSymbolType.getStruct(nullptr);
  assert(spiceStruct != nullptr);
  if (spiceStruct->vTableData.vtable != nullptr) {
    assert(spiceStruct->vTableData.vtableType != nullptr);
    // Store VTable to field address at index 0
    thisPtr = insertLoad(builder.getPtrTy(), thisPtrPtr);
    llvm::Value *indices[3] = {builder.getInt64(0), builder.getInt32(0), builder.getInt32(2)};
    llvm::Value *gepResult = insertInBoundsGEP(spiceStruct->vTableData.vtableType, spiceStruct->vTableData.vtable, indices);
    insertStore(gepResult, thisPtr);
  }

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t fieldIdx = 0; fieldIdx < fieldCount; fieldIdx++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(fieldIdx);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    // Call ctor for struct fields
    const QualType &fieldType = fieldSymbol->getQualType();
    const auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);
    if (fieldType.is(TY_STRUCT)) {
      // Lookup ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      if (const Function *ctorFunction = FunctionManager::lookup(matchScope, CTOR_FUNCTION_NAME, fieldType, {}, false)) {
        if (!thisPtr)
          thisPtr = insertLoad(builder.getPtrTy(), thisPtrPtr);
        llvm::Value *fieldAddress = insertStructGEP(structType, thisPtr, fieldIdx);
        generateCtorOrDtorCall(fieldAddress, ctorFunction, {});
      }
      continue;
    }

    // Store default field values
    if (fieldNode->defaultValue != nullptr || cliOptions.buildMode != BuildMode::RELEASE) {
      // Retrieve field address
      if (!thisPtr)
        thisPtr = insertLoad(builder.getPtrTy(), thisPtrPtr);
      llvm::Value *fieldAddress = insertStructGEP(structType, thisPtr, fieldIdx);
      // Retrieve default value
      llvm::Value *value;
      if (fieldNode->defaultValue != nullptr) {
        // To resolve the default value, we need to temporarily change to the manifestation of the current struct instantiation
        const size_t oldManIdx = manIdx; // Save manifestation index
        manIdx = spiceStruct->manifestationIndex;
        value = resolveValue(fieldNode->defaultValue);
        manIdx = oldManIdx; // Restore manifestation index
      } else {
        assert(cliOptions.buildMode == BuildMode::DEBUG || cliOptions.buildMode == BuildMode::TEST);
        value = getDefaultValueForSymbolType(fieldType);
      }
      // Store default value
      insertStore(value, fieldAddress);
    }
  }
}

void IRGenerator::generateDefaultCtor(const Function *ctorFunction) {
  assert(ctorFunction->implicitDefault && ctorFunction->name == CTOR_FUNCTION_NAME);
  const std::function<void()> generateBody = [&] { generateCtorBodyPreamble(ctorFunction->bodyScope); };
  generateImplicitProcedure(generateBody, ctorFunction);
}

void IRGenerator::generateCopyCtorBodyPreamble(const Function *copyCtorFunction) {
  // Retrieve struct scope
  Scope *structScope = copyCtorFunction->bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  const SymbolTableEntry *thisEntry = copyCtorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisPtrPtr = thisEntry->getAddress();
  assert(thisPtrPtr != nullptr);
  llvm::Value *thisPtr = nullptr;
  llvm::Type *structType = thisEntry->getQualType().getBase().toLLVMType(sourceFile);

  // Retrieve the value of the original struct, which is the only function parameter
  llvm::Value *originalThisPtr = builder.GetInsertBlock()->getParent()->getArg(1);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t fieldIdx = 0; fieldIdx < fieldCount; fieldIdx++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(fieldIdx);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());

    // Retrieve the address of the original field (copy source)
    llvm::Value *originalFieldAddress = insertStructGEP(structType, originalThisPtr, fieldIdx);

    const QualType &fieldType = fieldSymbol->getQualType();

    // Call copy ctor for struct fields
    if (fieldType.is(TY_STRUCT) && !fieldType.isTriviallyCopyable(nullptr)) {
      // Lookup copy ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      const ArgList args = {{fieldType.toConstRef(nullptr), false /* we have the field as storage */}};
      const Function *copyCtor = FunctionManager::lookup(matchScope, CTOR_FUNCTION_NAME, fieldType, args, false);
      assert(copyCtor != nullptr);
      generateCtorOrDtorCall(fieldSymbol, copyCtor, {originalFieldAddress});
      continue;
    }

    // Retrieve the address of the new field (copy dest)
    if (!thisPtr)
      thisPtr = insertLoad(builder.getPtrTy(), thisPtrPtr);
    llvm::Value *fieldAddress = insertStructGEP(structType, thisPtr, fieldIdx);

    // For owning heap fields, copy the underlying heap storage
    if (fieldType.isHeap()) {
      assert(fieldType.isPtr());
      llvm::Type *pointeeType = fieldType.getContained().toLLVMType(sourceFile);

      // Retrieve original heap address
      llvm::Value *originalHeapAddress = insertLoad(builder.getPtrTy(), originalFieldAddress);

      // Insert check for nullptr
      llvm::BasicBlock *bThen = createBlock("nullptrcheck.then");
      llvm::BasicBlock *bExit = createBlock("nullptrcheck.exit");
      llvm::Value *condValue = builder.CreateICmpNE(originalHeapAddress, llvm::Constant::getNullValue(builder.getPtrTy()));
      insertCondJump(condValue, bThen, bExit);

      // Fill then block
      switchToBlock(bThen);

      // Allocate new space on the heap
      llvm::Function *unsafeAllocFct = stdFunctionManager.getAllocUnsafeLongFct();
      const size_t typeSizeInBytes = module->getDataLayout().getTypeSizeInBits(pointeeType) / 8;
      llvm::ConstantInt *typeSize = builder.getInt64(typeSizeInBytes);
      llvm::Value *newHeapAddress = builder.CreateCall(unsafeAllocFct, {typeSize});
      insertStore(newHeapAddress, fieldAddress);

      // Copy data from the old heap storage to the new one
      generateShallowCopy(originalHeapAddress, pointeeType, newHeapAddress, false);
      insertJump(bExit);

      // Switch to exit block
      switchToBlock(bExit);

      continue;
    }

    // Shallow copy
    llvm::Type *type = fieldType.toLLVMType(sourceFile);
    generateShallowCopy(originalFieldAddress, type, fieldAddress, false);
  }
}

void IRGenerator::generateDefaultCopyCtor(const Function *copyCtorFunction) {
  assert(copyCtorFunction->implicitDefault && copyCtorFunction->name == CTOR_FUNCTION_NAME);
  const std::function<void()> generateBody = [&] { generateCopyCtorBodyPreamble(copyCtorFunction); };
  generateImplicitProcedure(generateBody, copyCtorFunction);
}

void IRGenerator::generateDtorBodyPreamble(const Function *dtorFunction) const {
  // Retrieve struct scope
  Scope *structScope = dtorFunction->bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  const SymbolTableEntry *thisEntry = dtorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisPtrPtr = thisEntry->getAddress();
  assert(thisPtrPtr != nullptr);
  llvm::Value *thisPtr = nullptr;
  llvm::Type *structType = thisEntry->getQualType().getBase().toLLVMType(sourceFile);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    const size_t fieldIdx = fieldCount - 1 - i; // Destruct fields in reverse order
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(fieldIdx);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());

    // Call dtor for struct fields
    const QualType &fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      // Lookup dtor function and generate call if found
      if (const Function *dtorFct = FunctionManager::lookup(fieldType.getBodyScope(), DTOR_FUNCTION_NAME, fieldType, {}, false))
        generateCtorOrDtorCall(fieldSymbol, dtorFct, {});
      continue;
    }

    // Deallocate fields, that are stored on the heap
    if (fieldType.isHeap()) {
      // Retrieve field address
      if (!thisPtr)
        thisPtr = insertLoad(builder.getPtrTy(), thisPtrPtr);
      llvm::Value *fieldAddress = insertStructGEP(structType, thisPtr, fieldIdx);
      // Call dealloc function
      generateDeallocCall(fieldAddress);
    }
  }
}

void IRGenerator::generateDefaultDtor(const Function *dtorFunction) {
  assert(dtorFunction->implicitDefault && dtorFunction->name == DTOR_FUNCTION_NAME);
  const std::function<void()> generateBody = [&] { generateDtorBodyPreamble(dtorFunction); };
  generateImplicitProcedure(generateBody, dtorFunction);
}

void IRGenerator::generateTestMain() {
  // Collect all test functions
  std::vector<const std::vector<const Function *> *> tests;
  for (const auto &sourceFile : resourceManager.sourceFiles | std::views::values)
    if (!sourceFile->testFunctions.empty())
      tests.push_back(&sourceFile->testFunctions);

  // Prepare printf function
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();

  // Prepare success and error messages
  llvm::Constant *allStartMsg = createGlobalStringConst("allStartMsg", TEST_ALL_START_MSG, *rootScope->codeLoc);
  llvm::Constant *allEndMsg = createGlobalStringConst("allEndMsg", TEST_ALL_END_MSG, *rootScope->codeLoc);
  llvm::Constant *fileStartMsg = createGlobalStringConst("fileStartMsg", TEST_FILE_START_MSG, *rootScope->codeLoc);
  llvm::Constant *fileEndMsg = createGlobalStringConst("fileEndMsg", TEST_FILE_END_MSG, *rootScope->codeLoc);
  llvm::Constant *runMsg = createGlobalStringConst("runMsg", TEST_CASE_RUN_MSG, *rootScope->codeLoc);
  llvm::Constant *successMsg = createGlobalStringConst("successMsg", TEST_CASE_SUCCESS_MSG, *rootScope->codeLoc);
  llvm::Constant *errorMsg = createGlobalStringConst("errorMsg", TEST_CASE_FAILED_MSG, *rootScope->codeLoc);
  llvm::Constant *skippedMsg = createGlobalStringConst("skippedMsg", TEST_CASE_SKIPPED_MSG, *rootScope->codeLoc);

  // Prepare entry for test main
  QualType functionType(TY_FUNCTION);
  functionType.setQualifiers(TypeQualifiers::of(TY_FUNCTION));
  functionType.makePublic();
  SymbolTableEntry entry(MAIN_FUNCTION_NAME, functionType, rootScope, nullptr, 0, false);

  // Prepare test main function
  Function testMain(MAIN_FUNCTION_NAME, &entry, QualType(TY_DYN), QualType(TY_INT), {}, {}, nullptr);
  testMain.used = true; // Mark as used to prevent removal
  testMain.implicitDefault = true;
  testMain.mangleFunctionName = false;

  // Prepare scope
  rootScope->createChildScope(testMain.getScopeName(), ScopeType::FUNC_PROC_BODY, nullptr);

  // Generate
  const std::function<void()> generateBody = [&] {
    // Prepare result variable
    std::vector<llvm::Value *> testCaseResults;
    testCaseResults.reserve(tests.size());

    // Print start message
    const auto accFct = [&](size_t sum, const std::vector<const Function *> *innerVector) { return sum + innerVector->size(); };
    const size_t totalTestCount = std::accumulate(tests.begin(), tests.end(), 0, accFct);
    builder.CreateCall(printfFct, {allStartMsg, builder.getInt32(totalTestCount), builder.getInt32(tests.size())});

    // Generate a call to each test function
    for (const std::vector<const Function *> *testSuite : tests) {
      // Print test suite prologue
      const std::string fileName = testSuite->front()->bodyScope->sourceFile->fileName;
      llvm::Constant *fileNameValue = createGlobalStringConst("fileName", fileName, testSuite->front()->getDeclCodeLoc());
      builder.CreateCall(printfFct, {fileStartMsg, builder.getInt32(testSuite->size()), fileNameValue});

      for (const Function *testFunction : *testSuite) {
        assert(testFunction->isNormalFunction());
        assert(testFunction->paramList.empty());

        // Retrieve attribute list for the test function
        assert(testFunction->declNode->isFctOrProcDef());
        const auto fctDefNode = spice_pointer_cast<FctDefBaseNode *>(testFunction->declNode);
        assert(fctDefNode->attrs != nullptr);
        const AttrLstNode *attrs = fctDefNode->attrs->attrLst;
        assert(attrs->getAttrValueByName(ATTR_TEST)->boolValue); // The test attribute must be present
        const CompileTimeValue *testSkipAttr = attrs->getAttrValueByName(ATTR_TEST_SKIP);
        const bool skipTest = testSkipAttr && testSkipAttr->boolValue;
        const CompileTimeValue *testNameAttr = attrs->getAttrValueByName(ATTR_TEST_NAME);

        // Prepare test name
        std::stringstream testName;
        testName << testFunction->name;
        if (testNameAttr)
          testName << " (" << resourceManager.compileTimeStringValues.at(testNameAttr->stringValueOffset) << ")";

        // Print test case run message
        llvm::Constant *testNameValue = createGlobalStringConst("testName", testName.str(), testFunction->getDeclCodeLoc());
        builder.CreateCall(printfFct, {runMsg, testNameValue});

        if (skipTest) {
          // Print test case skip message
          builder.CreateCall(printfFct, {skippedMsg, testNameValue});
          continue;
        }

        // Test function is not defined in the current module -> declare it
        const std::string mangledName = testFunction->getMangledName();
        if (!module->getFunction(mangledName)) {
          assert(testFunction->returnType.is(TY_BOOL));
          assert(testFunction->paramList.empty());
          llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getInt1Ty(), {}, false);
          module->getOrInsertFunction(mangledName, fctType);
        }

        // Call test function
        llvm::Function *callee = module->getFunction(mangledName);
        assert(callee != nullptr);
        llvm::Value *testCaseResult = builder.CreateCall(callee);
        testCaseResults.push_back(testCaseResult);

        // Print test case result message
        llvm::Value *message = builder.CreateSelect(testCaseResult, successMsg, errorMsg);
        builder.CreateCall(printfFct, {message, testNameValue});
      }

      // Print test suite epilogue
      builder.CreateCall(printfFct, {fileEndMsg, builder.getInt32(testSuite->size()), fileNameValue});
    }

    // Print end message
    builder.CreateCall(printfFct, {allEndMsg, builder.getInt32(totalTestCount), builder.getInt32(tests.size())});

    // Compute overall result
    llvm::Value *overallResult = builder.getTrue();
    for (llvm::Value *testCaseResult : testCaseResults)
      overallResult = builder.CreateAnd(overallResult, testCaseResult);

    // Return code must be 0 for success and 1 for failure, so we need to invert the result and zero extend to 32 bit
    llvm::Value *overallResultNegated = builder.CreateNot(overallResult);
    llvm::Value *exitCode = builder.CreateZExt(overallResultNegated, builder.getInt32Ty());
    builder.CreateRet(exitCode);
  };
  generateImplicitFunction(generateBody, &testMain);
}

} // namespace spice::compiler