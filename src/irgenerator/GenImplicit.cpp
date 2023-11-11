// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <irgenerator/NameMangling.h>
#include <model/Function.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

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
  const std::string mangledName = NameMangling::mangleFunction(*ctorOrDtor);

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
  // Issue call
  llvm::Function *deallocFct = stdFunctionManager.getDeallocBytePtrRefFct();
  builder.CreateCall(deallocFct, variableAddress);
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
  const std::string mangledName = NameMangling::mangleFunction(*spiceFunc);
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
  const std::string mangledName = NameMangling::mangleFunction(*spiceProc);
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
  std::vector<Function *> testFunctions;
  sourceFile->collectTestFunctions(testFunctions);

  // Prepare printf function
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();

  // Prepare success and error messages
  llvm::Constant *successMessage = createGlobalStringConst("successMessage", "[passed]\n", *rootScope->codeLoc);
  llvm::Constant *errorMessage = createGlobalStringConst("errorMessage", "[failed]\n", *rootScope->codeLoc);

  // Prepare test main function
  Function testMain(MAIN_FUNCTION_NAME, nullptr, SymbolType(TY_DYN), SymbolType(TY_INT), {}, {}, nullptr);
  testMain.implicitDefault = true;

  // Generate
  const std::function<void(void)> generateBody = [&]() {
    // Prepare result variable
    llvm::Type *i32Ty = builder.getInt32Ty();
    llvm::Value *overallResult = insertAlloca(i32Ty, RETURN_VARIABLE_NAME);
    builder.CreateStore(builder.getTrue(), overallResult);

    // Generate a call to each test function
    for (Function *testFunction : testFunctions) {
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
      assert(!testSkipAttr || !testSkipAttr->boolValue); // All skipped tests must be filtered out
      const CompileTimeValue *testNameAttr = attrs->getAttrValueByName(ATTR_TEST_NAME);
      const std::string testName = testNameAttr ? testNameAttr->stringValue : testFunction->name;

      // Print test name
      builder.CreateCall(printfFct, createGlobalStringConst("testName", testName + " ", testFunction->getDeclCodeLoc()));

      // Call test function
      llvm::Function *callee = module->getFunction(NameMangling::mangleFunction(*testFunction));
      assert(callee != nullptr);
      llvm::Value *testCaseResult = builder.CreateCall(callee);

      // Update result variable
      llvm::Value *oldResult = insertLoad(i32Ty, overallResult);
      llvm::Value *newResult = builder.CreateAnd(oldResult, builder.CreateZExt(testCaseResult, i32Ty));
      builder.CreateStore(newResult, overallResult);

      // Print success or error message
      llvm::Value *message = builder.CreateSelect(testCaseResult, successMessage, errorMessage);
      builder.CreateCall(printfFct, message);
    }

    // Return result
    llvm::Value *finalResult = insertLoad(i32Ty, overallResult);
    builder.CreateRet(finalResult);
  };
  generateImplicitFunction(generateBody, &testMain);
}

} // namespace spice::compiler