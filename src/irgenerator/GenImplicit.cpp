// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <model/Function.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

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
    src = insertLoad(srcSTy.toLLVMType(sourceFile), src);
    srcSTy = srcSTy.getContained();
    dstSTy = dstSTy.getContained();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstSTy.isArray() && srcSTy.isArray()) { // Special case that is used for passing arrays as pointer to functions
    llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(0)};
    src = insertInBoundsGEP(srcSTy.toLLVMType(sourceFile), src, indices);
  } else {
    src = insertLoad(srcSTy.toLLVMType(sourceFile), src);
    src = builder.CreateBitCast(src, dstSTy.toLLVMType(sourceFile));
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(src->getType());
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
  const StmtLstNode::ResourcesForManifestationToCleanup& resourcesToCleanup = node->resourcesToCleanup.at(manIdx);
  for (auto [entry, dtor] : resourcesToCleanup.dtorFunctionsToCall)
    generateCtorOrDtorCall(entry, dtor, {});

  // Deallocate all heap variables that go out of scope and are currently owned
  //for (SymbolTableEntry *entry : resourcesToCleanup.heapVarsToFree)
  //  generateDeallocCall(entry->getAddress());

  // Generate lifetime end markers
  if (cliOptions.useLifetimeMarkers) {
    std::vector<SymbolTableEntry *> vars = currentScope->getVarsGoingOutOfScope();
    for (SymbolTableEntry *var : vars) {
      llvm::Value *address = var->getAddress();
      if (address == nullptr)
        continue;
      const uint64_t sizeInBytes = module->getDataLayout().getTypeAllocSize(var->getQualType().toLLVMType(sourceFile));
      builder.CreateLifetimeEnd(address, builder.getInt64(sizeInBytes));
    }
  }
}

llvm::Value *IRGenerator::generateFctCall(const Function *fct, const std::vector<llvm::Value *> &args) const {
  // Retrieve metadata for the function
  const std::string mangledName = fct->getMangledName();

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    std::vector<llvm::Type *> paramTypes;
    for (const llvm::Value *argValue : args)
      paramTypes.push_back(argValue->getType());
    llvm::Type *returnType = fct->returnType.toLLVMType(sourceFile);
    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Generate function call
  return builder.CreateCall(callee, args);
}

void IRGenerator::generateProcCall(const Function *proc, std::vector<llvm::Value *> &args) const {
  // Retrieve metadata for the function
  const std::string mangledName = proc->getMangledName();

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    std::vector<llvm::Type *> paramTypes;
    for (const llvm::Value *argValue : args)
      paramTypes.push_back(argValue->getType());
    llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), paramTypes, false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Generate function call
  builder.CreateCall(callee, args);
}

void IRGenerator::generateCtorOrDtorCall(SymbolTableEntry *entry, const Function *ctorOrDtor,
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
    llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(entry->orderIndex)};
    structAddr = insertInBoundsGEP(thisType, thisPtr, indices);
  } else {
    structAddr = entry->getAddress();
  }
  assert(structAddr != nullptr);
  generateCtorOrDtorCall(structAddr, ctorOrDtor, args);
}

void IRGenerator::generateCtorOrDtorCall(llvm::Value *structAddr, const Function *ctorOrDtor,
                                         const std::vector<llvm::Value *> &args) const {
  // Build parameter list
  std::vector<llvm::Value *> argValues = {structAddr};
  argValues.insert(argValues.end(), args.begin(), args.end());

  // Generate function call
  generateProcCall(ctorOrDtor, argValues);
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

llvm::Function *IRGenerator::generateImplicitFunction(const std::function<void(void)> &generateBody, const Function *spiceFunc) {
  // Only focus on method procedures
  const ASTNode *node = spiceFunc->entry->declNode;
  assert(spiceFunc->isFunction());

  // Only generate if used
  if (!spiceFunc->used)
    return nullptr;

  // Retrieve return type
  llvm::Type *returnType = spiceFunc->returnType.toLLVMType(sourceFile);

  // Get 'this' entry
  std::vector<std::pair<std::string, SymbolTableEntry *>> paramInfoList;
  std::vector<llvm::Type *> paramTypes;
  SymbolTableEntry *thisEntry = nullptr;
  if (spiceFunc->isMethod()) {
    thisEntry = spiceFunc->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    paramInfoList.emplace_back(THIS_VARIABLE_NAME, thisEntry);
    paramTypes.push_back(builder.getPtrTy());
  }

  // Get parameter types
  for (const Param &param : spiceFunc->paramList) {
    assert(!param.isOptional);
    paramTypes.push_back(param.qualType.toLLVMType(sourceFile));
  }

  // Get function linkage
  const bool isPublic = spiceFunc->entry->getQualType().isPublic();
  llvm::GlobalValue::LinkageTypes linkage = isPublic ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

  // Create function
  const std::string mangledName = spiceFunc->getMangledName();
  llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, paramTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, mangledName, module);
  fct->setLinkage(linkage);
  fct->setDoesNotRecurse();

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
  changeToScope(spiceFunc->getSignature(false), ScopeType::FUNC_PROC_BODY);

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
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(THIS_VARIABLE_NAME, thisAddress, 1);
    // Store the value at the new address
    insertStore(fct->arg_begin(), thisAddress);
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

llvm::Function *IRGenerator::generateImplicitProcedure(const std::function<void(void)> &generateBody, const Function *spiceProc) {
  // Only focus on method procedures
  const ASTNode *node = spiceProc->entry->declNode;
  assert(node != nullptr);
  assert(spiceProc->isProcedure());

  // Only generate if used
  if (!spiceProc->used)
    return nullptr;

  // Get 'this' entry
  std::vector<std::pair<std::string, SymbolTableEntry *>> paramInfoList;
  std::vector<llvm::Type *> paramTypes;
  SymbolTableEntry *thisEntry = nullptr;
  if (spiceProc->isMethod()) {
    thisEntry = spiceProc->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    paramInfoList.emplace_back(THIS_VARIABLE_NAME, thisEntry);
    paramTypes.push_back(builder.getPtrTy());
  }

  // Get parameter types
  for (const Param &param : spiceProc->paramList) {
    assert(!param.isOptional);
    paramTypes.push_back(param.qualType.toLLVMType(sourceFile));
  }

  // Get function linkage
  const bool isPublic = spiceProc->entry->getQualType().isPublic();
  llvm::GlobalValue::LinkageTypes linkage = isPublic ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

  // Create function
  const std::string mangledName = spiceProc->getMangledName();
  llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), paramTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, mangledName, module);
  fct->setLinkage(linkage);
  fct->setDoesNotRecurse();

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
  changeToScope(spiceProc->getSignature(false), ScopeType::FUNC_PROC_BODY);

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
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(THIS_VARIABLE_NAME, thisAddress, 1);
    // Store the value at the new address
    insertStore(fct->arg_begin(), thisAddress);
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
  SymbolTableEntry *thisEntry = bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisAddress = thisEntry->getAddress();
  assert(thisAddress != nullptr);
  llvm::Value *thisAddressLoaded = nullptr;
  QualType structSymbolType = thisEntry->getQualType().getBase();
  llvm::Type *structType = structSymbolType.toLLVMType(sourceFile);

  // Store VTable to first struct field if required
  Struct *spiceStruct = structSymbolType.getStruct(nullptr);
  assert(spiceStruct != nullptr);
  if (spiceStruct->vTableData.vtable != nullptr) {
    assert(spiceStruct->vTableData.vtableType != nullptr);
    // Store VTable to field address at index 0
    thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
    llvm::Value *indices[3] = {builder.getInt64(0), builder.getInt32(0), builder.getInt32(2)};
    llvm::Value *gepResult = insertInBoundsGEP(spiceStruct->vTableData.vtableType, spiceStruct->vTableData.vtable, indices);
    insertStore(gepResult, thisAddressLoaded);
  }

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    // Call ctor for struct fields
    const QualType &fieldType = fieldSymbol->getQualType();
    auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);
    if (fieldType.is(TY_STRUCT)) {
      // Lookup ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      const Function *ctorFunction = FunctionManager::lookup(matchScope, CTOR_FUNCTION_NAME, fieldType, {}, false);
      if (ctorFunction)
        generateCtorOrDtorCall(fieldSymbol, ctorFunction, {});

      continue;
    }

    // Store default field values
    if (fieldNode->defaultValue() != nullptr || cliOptions.buildMode == BuildMode::DEBUG) {
      // Retrieve field address
      if (!thisAddressLoaded)
        thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
      llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(i)};
      llvm::Value *fieldAddress = insertInBoundsGEP(structType, thisAddressLoaded, indices);
      // Retrieve default value
      llvm::Value *value;
      if (fieldNode->defaultValue() != nullptr) {
        // To resolve the default value, we need to temporarily change to the manifestation of the current struct instantiation
        const size_t oldManIdx = manIdx; // Save manifestation index
        manIdx = spiceStruct->manifestationIndex;
        value = resolveValue(fieldNode->defaultValue());
        manIdx = oldManIdx; // Restore manifestation index
      } else {
        assert(cliOptions.buildMode == BuildMode::DEBUG);
        value = getDefaultValueForSymbolType(fieldType);
      }
      // Store default value
      insertStore(value, fieldAddress);
    }
  }
}

void IRGenerator::generateDefaultCtor(const Function *ctorFunction) {
  assert(ctorFunction->implicitDefault && ctorFunction->name == CTOR_FUNCTION_NAME);
  const std::function<void(void)> generateBody = [&]() { generateCtorBodyPreamble(ctorFunction->bodyScope); };
  generateImplicitProcedure(generateBody, ctorFunction);
}

void IRGenerator::generateCopyCtorBodyPreamble(const Function *copyCtorFunction) {
  // Retrieve struct scope
  Scope *structScope = copyCtorFunction->bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  SymbolTableEntry *thisEntry = copyCtorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisAddress = thisEntry->getAddress();
  assert(thisAddress != nullptr);
  llvm::Value *thisAddressLoaded = nullptr;
  llvm::Type *structType = thisEntry->getQualType().getBase().toLLVMType(sourceFile);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    // Call copy ctor for struct fields
    const QualType &fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      // Lookup copy ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      const ArgList args = {{fieldType.toConstRef(nullptr), false /* we have the field as storage */}};
      const Function *ctorFct = FunctionManager::lookup(matchScope, CTOR_FUNCTION_NAME, fieldType, args, false);
      if (ctorFct) {
        // Retrieve field address
        if (!thisAddressLoaded)
          thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
        llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(i)};
        llvm::Value *fieldAddress = insertInBoundsGEP(structType, thisAddressLoaded, indices);
        generateCtorOrDtorCall(fieldSymbol, ctorFct, {fieldAddress});
      }
    }
  }
}

void IRGenerator::generateDefaultCopyCtor(const Function *copyCtorFunction) {
  assert(copyCtorFunction->implicitDefault && copyCtorFunction->name == CTOR_FUNCTION_NAME);
  const std::function<void(void)> generateBody = [&]() { generateCopyCtorBodyPreamble(copyCtorFunction); };
  generateImplicitProcedure(generateBody, copyCtorFunction);
}

void IRGenerator::generateDtorBodyPreamble(const Function *dtorFunction) {
  // Retrieve struct scope
  Scope *structScope = dtorFunction->bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  SymbolTableEntry *thisEntry = dtorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisAddress = thisEntry->getAddress();
  assert(thisAddress != nullptr);
  llvm::Value *thisAddressLoaded = nullptr;
  llvm::Type *structType = thisEntry->getQualType().getBase().toLLVMType(sourceFile);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    // Call dtor for struct fields
    const QualType &fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      // Lookup dtor function and generate call if found
      const Function *dtorFct = FunctionManager::lookup(fieldType.getBodyScope(), DTOR_FUNCTION_NAME, fieldType, {}, false);
      if (dtorFct)
        generateCtorOrDtorCall(fieldSymbol, dtorFct, {});
      continue;
    }

    // Deallocate fields, that are stored on the heap
    if (fieldType.isHeap()) {
      // Retrieve field address
      if (!thisAddressLoaded)
        thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
      llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(i)};
      llvm::Value *fieldAddress = insertInBoundsGEP(structType, thisAddressLoaded, indices);
      // Call dealloc function
      generateDeallocCall(fieldAddress);
    }
  }
}

void IRGenerator::generateDefaultDtor(const Function *dtorFunction) {
  assert(dtorFunction->implicitDefault && dtorFunction->name == DTOR_FUNCTION_NAME);
  const std::function<void(void)> generateBody = [&]() { generateDtorBodyPreamble(dtorFunction); };
  generateImplicitProcedure(generateBody, dtorFunction);
}

void IRGenerator::generateTestMain() {
  // Collect all test functions
  std::vector<const std::vector<const Function *> *> tests;
  for (const auto &sourceFile : resourceManager.sourceFiles)
    if (!sourceFile.second->testFunctions.empty())
      tests.push_back(&sourceFile.second->testFunctions);

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
  functionType.setSpecifiers(TypeSpecifiers::of(TY_FUNCTION));
  functionType.makePublic();
  SymbolTableEntry entry(MAIN_FUNCTION_NAME, functionType, rootScope, nullptr, 0, false);

  // Prepare test main function
  Function testMain(MAIN_FUNCTION_NAME, &entry, QualType(TY_DYN), QualType(TY_INT), {}, {}, nullptr);
  testMain.used = true; // Mark as used to prevent removal
  testMain.implicitDefault = true;
  testMain.mangleFunctionName = false;

  // Prepare scope
  rootScope->createChildScope(testMain.getSignature(false), ScopeType::FUNC_PROC_BODY, nullptr);

  // Generate
  const std::function<void(void)> generateBody = [&]() {
    // Prepare result variable
    llvm::Type *i32Ty = builder.getInt32Ty();
    llvm::Value *overallResult = insertAlloca(i32Ty, RETURN_VARIABLE_NAME);
    insertStore(builder.getTrue(), overallResult);

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
        auto fctDefNode = spice_pointer_cast<FctDefBaseNode *>(testFunction->declNode);
        assert(fctDefNode->attrs() != nullptr);
        const AttrLstNode *attrs = fctDefNode->attrs()->attrLst();
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

        // Update result variable
        llvm::Value *oldResult = insertLoad(i32Ty, overallResult);
        llvm::Value *newResult = builder.CreateAnd(oldResult, builder.CreateZExt(testCaseResult, i32Ty));
        insertStore(newResult, overallResult);

        // Print test case result message
        llvm::Value *message = builder.CreateSelect(testCaseResult, successMsg, errorMsg);
        builder.CreateCall(printfFct, {message, testNameValue});
      }

      // Print test suite epilogue
      builder.CreateCall(printfFct, {fileEndMsg, builder.getInt32(testSuite->size()), fileNameValue});
    }

    // Print end message
    builder.CreateCall(printfFct, {allEndMsg, builder.getInt32(totalTestCount), builder.getInt32(tests.size())});

    // Return result
    llvm::Value *finalResult = insertLoad(i32Ty, overallResult);
    builder.CreateRet(finalResult);
  };
  generateImplicitFunction(generateBody, &testMain);
}

} // namespace spice::compiler