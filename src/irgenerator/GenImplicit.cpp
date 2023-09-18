// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <irgenerator/NameMangling.h>
#include <model/Function.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

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
    llvm::Value *thisPtr = builder.CreateLoad(builder.getPtrTy(), thisVar->getAddress());
    // Add field offset
    structPtr = builder.CreateInBoundsGEP(thisType, thisPtr, {builder.getInt32(0), builder.getInt32(entry->orderIndex)});
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

llvm::Function *IRGenerator::generateImplicitProcedure(const std::function<void()> &generateBody, const Function *spiceFunc) {
  // Only focus on method procedures
  const ASTNode *node = spiceFunc->entry->declNode;
  assert(spiceFunc->isMethodProcedure());

  // Only generate if used
  if (!spiceFunc->used)
    return nullptr;

  // Retrieve return type
  llvm::Type *returnType = builder.getVoidTy();

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

void IRGenerator::generateDefaultDefaultCtor(const Function *ctorFunction) {
  assert(ctorFunction->implicitDefault && ctorFunction->name == CTOR_FUNCTION_NAME);

  const std::function<void()> generateBody = [&]() {
    // Retrieve struct scope
    Scope *structScope = ctorFunction->bodyScope->parent;
    assert(structScope != nullptr);

    // Get struct address
    SymbolTableEntry *thisEntry = ctorFunction->bodyScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    llvm::Value *thisAddress = thisEntry->getAddress();
    assert(thisAddress != nullptr);
    llvm::Value *thisAddressLoaded = nullptr;
    llvm::Type *structType = thisEntry->getType().getBaseType().toLLVMType(context, structScope);

    const size_t fieldCount = structScope->getFieldCount();
    for (size_t i = 0; i < fieldCount; i++) {
      SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
      assert(fieldSymbol != nullptr && fieldSymbol->isField());
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
      if (fieldNode->defaultValue() != nullptr) {
        assert(fieldNode->defaultValue()->hasCompileTimeValue());
        // Retrieve field address
        if (!thisAddressLoaded)
          thisAddressLoaded = builder.CreateLoad(builder.getPtrTy(), thisAddress);
        llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
        llvm::Value *fieldAddress = builder.CreateInBoundsGEP(structType, thisAddressLoaded, indices);
        // Store default value
        const CompileTimeValue compileTimeValue = fieldNode->defaultValue()->getCompileTimeValue();
        llvm::Value *value = getConst(compileTimeValue, fieldType, fieldNode->defaultValue());
        builder.CreateStore(value, fieldAddress);
      }
    }
  };
  generateImplicitProcedure(generateBody, ctorFunction);
}

void IRGenerator::generateDefaultDefaultCopyCtor(const Function *copyCtorFunction) {
  assert(copyCtorFunction->implicitDefault && copyCtorFunction->name == CTOR_FUNCTION_NAME);

  const std::function<void()> generateBody = [&]() {
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
            thisAddressLoaded = builder.CreateLoad(builder.getPtrTy(), thisAddress);
          llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
          llvm::Value *fieldAddress = builder.CreateInBoundsGEP(structType, thisAddressLoaded, indices);
          generateCtorOrDtorCall(fieldSymbol, ctorFct, {fieldAddress});
        }
      }
    }
  };
  generateImplicitProcedure(generateBody, copyCtorFunction);
}

void IRGenerator::generateDefaultDefaultDtor(const Function *dtorFunction) {
  assert(dtorFunction->implicitDefault && dtorFunction->name == DTOR_FUNCTION_NAME);

  const std::function<void()> generateBody = [&]() {
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
          thisAddressLoaded = builder.CreateLoad(builder.getPtrTy(), thisAddress);
        llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
        llvm::Value *fieldAddress = builder.CreateInBoundsGEP(structType, thisAddressLoaded, indices);
        // Call dealloc function
        generateDeallocCall(fieldAddress);
      }
    }
  };
  generateImplicitProcedure(generateBody, dtorFunction);
}

} // namespace spice::compiler