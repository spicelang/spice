// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
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

llvm::Value *IRGenerator::doImplicitCast(llvm::Value *src, SymbolType dstSTy, SymbolType srcSTy) {
  assert(srcSTy != dstSTy); // We only need to cast implicitly, if the types do not match exactly

  // Unpack the pointers until a pointer of another type is met
  size_t loadCounter = 0;
  while (srcSTy.isPtr()) {
    src = insertLoad(srcSTy.toLLVMType(context, currentScope), src);
    srcSTy = srcSTy.getContainedTy();
    dstSTy = dstSTy.getContainedTy();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstSTy.isArray() && srcSTy.isArray()) { // Special case that is used for passing arrays as pointer to functions
    llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
    src = insertInBoundsGEP(srcSTy.toLLVMType(context, currentScope), src, indices);
  } else {
    src = insertLoad(srcSTy.toLLVMType(context, currentScope), src);
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

void IRGenerator::generateScopeCleanup(const StmtLstNode *node) const {
  // Do not clean up if the block is already terminated
  if (blockAlreadyTerminated)
    return;

  // Call all dtor functions
  for (auto [entry, dtor] : node->dtorFunctions.at(manIdx))
    generateCtorOrDtorCall(entry, dtor, {});
}

void IRGenerator::generateCtorOrDtorCall(SymbolTableEntry *entry, const Function *ctorOrDtor,
                                         const std::vector<llvm::Value *> &args) const {
  assert(ctorOrDtor != nullptr);

  // Retrieve metadata for the function
  const std::string mangledName = ctorOrDtor->getMangledName();

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    std::vector<llvm::Type *> paramTypes = {builder.getPtrTy()};
    for (llvm::Value *argValue : args)
      paramTypes.push_back(argValue->getType());
    llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), paramTypes, false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Retrieve address of the struct variable. For fields this is the 'this' variable, otherwise use the normal address
  llvm::Value *structPtr;
  if (entry->isField()) {
    // Take 'this' var as base pointer
    const SymbolTableEntry *thisVar = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisVar != nullptr);
    assert(thisVar->getType().isPtr() && thisVar->getType().getContainedTy().is(TY_STRUCT));
    llvm::Type *thisType = thisVar->getType().getContainedTy().toLLVMType(context, currentScope);
    llvm::Value *thisPtr = insertLoad(builder.getPtrTy(), thisVar->getAddress());
    // Add field offset
    structPtr = insertInBoundsGEP(thisType, thisPtr, {builder.getInt32(0), builder.getInt32(entry->orderIndex)});
  } else {
    structPtr = entry->getAddress();
  }
  assert(structPtr != nullptr);

  // Build parameter list
  std::vector<llvm::Value *> argValues = {structPtr};
  argValues.insert(argValues.end(), args.begin(), args.end());

  // Generate function call
  builder.CreateCall(callee, argValues);
}

void IRGenerator::generateDeallocCall(llvm::Value *variableAddress) const {
  // In case of string runtime, call free manually. Otherwise, use the memory_rt implementation of sDealloc()
  if (sourceFile->isStringRT()) {
    llvm::Function *freeFct = stdFunctionManager.getFreeFctPtr();
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
  llvm::Type *returnType = spiceFunc->returnType.toLLVMType(context, currentScope);

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
    paramTypes.push_back(param.type.toLLVMType(context, currentScope));
  }

  // Get function linkage
  const bool isPublic = spiceFunc->entry->getType().specifiers.isPublic;
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
    llvm::Type *structType = thisEntry->getType().getContainedTy().toLLVMType(context, currentScope);
    assert(structType != nullptr);
    fct->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
    fct->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
  }

  // Add debug info
  diGenerator.generateFunctionDebugInfo(fct, spiceFunc);
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
    builder.CreateStore(fct->arg_begin(), thisAddress);
  }

  // Generate body
  generateBody();

  // Conclude debug info for function
  diGenerator.concludeFunctionDebugInfo();

  // Verify function
  verifyFunction(fct, node->codeLoc);

  // Change to parent scope
  changeToParentScope(ScopeType::FUNC_PROC_BODY);

  return fct;
}

llvm::Function *IRGenerator::generateImplicitProcedure(const std::function<void(void)> &generateBody, const Function *spiceProc) {
  // Only focus on method procedures
  const ASTNode *node = spiceProc->entry->declNode;
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
    paramTypes.push_back(param.type.toLLVMType(context, currentScope));
  }

  // Get function linkage
  const bool isPublic = spiceProc->entry->getType().specifiers.isPublic;
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
    llvm::Type *structType = thisEntry->getType().getContainedTy().toLLVMType(context, currentScope);
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
    builder.CreateStore(fct->arg_begin(), thisAddress);
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
  SymbolType structSymbolType = thisEntry->getType().getBaseType();
  llvm::Type *structType = structSymbolType.toLLVMType(context, structScope);

  // Store VTable to first struct field if required
  Struct *spiceStruct = structSymbolType.getStruct(nullptr);
  assert(spiceStruct != nullptr);
  if (spiceStruct->vtable != nullptr) {
    assert(spiceStruct->vtableType != nullptr);
    // Store VTable to field address at index 0
    thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
    llvm::Value *indices[3] = {builder.getInt32(0), builder.getInt32(0), builder.getInt32(2)};
    llvm::Value *gepResult = insertInBoundsGEP(spiceStruct->vtableType, spiceStruct->vtable, indices);
    builder.CreateStore(gepResult, thisAddressLoaded);
  }

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    const SymbolType &fieldType = fieldSymbol->getType();
    auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);

    // Call ctor for struct fields
    if (fieldType.is(TY_STRUCT)) {
      // Lookup ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      const Function *ctorFunction = FunctionManager::lookupFunction(matchScope, CTOR_FUNCTION_NAME, fieldType, {}, false);
      if (ctorFunction)
        generateCtorOrDtorCall(fieldSymbol, ctorFunction, {});

      continue;
    }

    // Store default field values
    if (fieldNode->defaultValue() != nullptr || cliOptions.buildMode == BuildMode::DEBUG) {
      // Retrieve field address
      if (!thisAddressLoaded)
        thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
      llvm::Value *fieldAddress = insertInBoundsGEP(structType, thisAddressLoaded, indices);
      // Retrieve default value
      llvm::Value *value;
      if (fieldNode->defaultValue() != nullptr) {
        assert(fieldNode->defaultValue()->hasCompileTimeValue());
        const CompileTimeValue compileTimeValue = fieldNode->defaultValue()->getCompileTimeValue();
        value = getConst(compileTimeValue, fieldType, fieldNode->defaultValue());
      } else {
        assert(cliOptions.buildMode == BuildMode::DEBUG);
        value = getDefaultValueForSymbolType(fieldType);
      }
      // Store default value
      builder.CreateStore(value, fieldAddress);
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
  llvm::Type *structType = thisEntry->getType().getBaseType().toLLVMType(context, structScope);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    const SymbolType &fieldType = fieldSymbol->getType();

    // Call copy ctor for struct fields
    if (fieldType.is(TY_STRUCT)) {
      // Lookup copy ctor function and call if available
      Scope *matchScope = fieldType.getBodyScope();
      std::vector<SymbolType> paramTypes = {fieldType.toConstReference(nullptr)};
      const Function *ctorFct = FunctionManager::lookupFunction(matchScope, CTOR_FUNCTION_NAME, fieldType, paramTypes, false);
      if (ctorFct) {
        // Retrieve field address
        if (!thisAddressLoaded)
          thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
        llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
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

void IRGenerator::generateDtorBodyPreamble(const spice::compiler::Function *dtorFunction) {
  // Retrieve struct scope
  Scope *structScope = dtorFunction->bodyScope->parent;
  assert(structScope != nullptr);

  // Get struct address
  SymbolTableEntry *thisEntry = dtorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
  assert(thisEntry != nullptr);
  llvm::Value *thisAddress = thisEntry->getAddress();
  assert(thisAddress != nullptr);
  llvm::Value *thisAddressLoaded = nullptr;
  llvm::Type *structType = thisEntry->getType().getBaseType().toLLVMType(context, structScope);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    const SymbolType &fieldType = fieldSymbol->getType();

    // Call dtor for struct fields
    if (fieldType.is(TY_STRUCT)) {
      // Lookup dtor function
      Scope *matchScope = fieldType.getBodyScope();
      const Function *dtorFunction = FunctionManager::lookupFunction(matchScope, DTOR_FUNCTION_NAME, fieldType, {}, false);

      // Generate call to dtor
      if (dtorFunction)
        generateCtorOrDtorCall(fieldSymbol, dtorFunction, {});

      continue;
    }

    // Deallocate fields, that are stored on the heap
    if (fieldType.isHeap()) {
      // Retrieve field address
      if (!thisAddressLoaded)
        thisAddressLoaded = insertLoad(builder.getPtrTy(), thisAddress);
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
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
  SymbolType functionType(TY_FUNCTION);
  functionType.specifiers = TypeSpecifiers::of(TY_FUNCTION);
  functionType.specifiers.isPublic = true;
  SymbolTableEntry entry(MAIN_FUNCTION_NAME, functionType, rootScope, nullptr, 0, false);

  // Prepare test main function
  Function testMain(MAIN_FUNCTION_NAME, &entry, SymbolType(TY_DYN), SymbolType(TY_INT), {}, {}, nullptr);
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
    builder.CreateStore(builder.getTrue(), overallResult);

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
        const CompileTimeValue *testValue = attrs->getAttrValueByName(ATTR_TEST);
        assert(testValue->boolValue); // The test attribute must be present
        const CompileTimeValue *testSkipAttr = attrs->getAttrValueByName(ATTR_TEST_SKIP);
        const bool skipTest = testSkipAttr && testSkipAttr->boolValue;
        const CompileTimeValue *testNameAttr = attrs->getAttrValueByName(ATTR_TEST_NAME);

        // Prepare test name
        std::stringstream testName;
        testName << testFunction->name;
        if (testNameAttr)
          testName << " (" << testNameAttr->stringValue << ")";

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
        builder.CreateStore(newResult, overallResult);

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