// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <llvm/IR/Module.h>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <driver/Driver.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

std::any IRGenerator::visitMainFctDef(const MainFctDefNode *node) {
  // Ignore main function definitions if this is not the main source file
  if (!sourceFile->isMainFile)
    return nullptr;

  // Do not generate main function if it is explicitly specified
  if (cliOptions.noEntryFct)
    return nullptr;

  // Change scope to function scope
  currentScope = node->bodyScope;
  assert(currentScope != nullptr);

  // Visit parameters
  std::vector<std::pair<std::string, SymbolTableEntry *>> paramInfoList;
  QualTypeList paramSymbolTypes;
  std::vector<llvm::Type *> paramTypes;
  if (node->takesArgs) {
    const size_t numOfParams = node->paramLst->params.size();
    paramInfoList.reserve(numOfParams);
    paramSymbolTypes.reserve(numOfParams);
    paramTypes.reserve(numOfParams);
    for (DeclStmtNode *param : node->paramLst->params) {
      // Get symbol table entry of param
      SymbolTableEntry *paramSymbol = node->bodyScope->lookupStrict(param->varName);
      assert(paramSymbol != nullptr);
      // Retrieve type of param
      auto paramType = any_cast<llvm::Type *>(visit(param->dataType));
      // Add it to the lists
      paramInfoList.emplace_back(param->varName, paramSymbol);
      paramSymbolTypes.push_back(paramSymbol->getQualType());
      paramTypes.push_back(paramType);
    }
  }

  // Build the function
  llvm::Type *returnType = builder.getInt32Ty();
  llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, paramTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, MAIN_FUNCTION_NAME, module);
  fct->setDSOLocal(true);

  // Add function attributes
  fct->addFnAttr(llvm::Attribute::NoInline);
  fct->addFnAttr(llvm::Attribute::NoUnwind);
  if (cliOptions.optLevel == OptLevel::O0)
    fct->addFnAttr(llvm::Attribute::OptimizeNone);
  else if (cliOptions.optLevel >= OptLevel::Os)
    fct->addFnAttr(llvm::Attribute::OptimizeForSize);
  fct->addFnAttr(llvm::Attribute::getWithUWTableKind(context, llvm::UWTableKind::Default));

  // Add debug info
  if (cliOptions.generateDebugInfo) {
    const auto nonConstNode = const_cast<MainFctDefNode *>(node);
    const Function spiceFunc = FunctionManager::createMainFunction(node->entry, paramSymbolTypes, nonConstNode);
    diGenerator.generateFunctionDebugInfo(fct, &spiceFunc);
    diGenerator.setSourceLocation(node);
  }

  // Create entry block
  llvm::BasicBlock *bEntry = createBlock();
  switchToBlock(bEntry, fct);

  // Reset alloca insert markers to this block
  allocaInsertBlock = bEntry;
  allocaInsertInst = nullptr;

  // Allocate result variable
  llvm::Value *resultAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
  // Update the symbol table entry
  SymbolTableEntry *resultEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultEntry != nullptr);
  resultEntry->updateAddress(resultAddress);
  // Generate debug info
  diGenerator.generateLocalVarDebugInfo(RETURN_VARIABLE_NAME, resultAddress);
  // Store the default result value
  insertStore(builder.getInt32(0), resultAddress);

  // Store function argument values
  for (auto &arg : fct->args()) {
    // Get information about the parameter
    const size_t argNumber = arg.getArgNo();
    auto [paramName, paramSymbol] = paramInfoList.at(argNumber);
    assert(paramSymbol != nullptr);
    // Allocate space for it
    llvm::Type *paramType = fctType->getParamType(argNumber);
    llvm::Value *paramAddress = insertAlloca(paramType, paramName);
    // Update the symbol table entry
    paramSymbol->updateAddress(paramAddress);
    // Store the value at the new address
    insertStore(&arg, paramAddress);
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(paramName, paramAddress, argNumber + 1);
  }

  // Visit function body
  visit(node->body);

  // Create return statement if the block is not terminated yet
  if (!blockAlreadyTerminated) {
    llvm::Value *result = insertLoad(fct->getReturnType(), resultEntry->getAddress());
    builder.CreateRet(result);
  }

  // Conclude debug info for function
  diGenerator.concludeFunctionDebugInfo();

  // Verify function
  verifyFunction(fct, node->codeLoc);

  // Change back to root scope
  currentScope = rootScope;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any IRGenerator::visitFctDef(const FctDefNode *node) {
  // Loop through manifestations
  manIdx = 0; // Reset the symbolTypeIndex
  for (Function *manifestation : node->manifestations) {
    assert(manifestation->entry != nullptr);

    // Check if the manifestation is substantiated or not public and not used by anybody
    const bool isPublic = manifestation->entry->getQualType().isPublic();
    if (!manifestation->isFullySubstantiated() || (!isPublic && !manifestation->used)) {
      manIdx++; // Increment symbolTypeIndex
      continue;
    }

    // Change to struct scope
    if (manifestation->isMethod()) {
      const QualType &thisType = manifestation->thisType;
      const std::string signature = Struct::getSignature(thisType.getSubType(), thisType.getTemplateTypes());
      currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + signature);
      assert(currentScope != nullptr);
    }

    // Change scope
    currentScope = currentScope->getChildScope(manifestation->getSignature(false));
    assert(currentScope != nullptr);

    // Get 'this' entry
    std::vector<std::pair<std::string, SymbolTableEntry *>> paramInfoList;
    std::vector<llvm::Type *> paramTypes;
    SymbolTableEntry *thisEntry = nullptr;
    if (manifestation->isMethod()) {
      thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
      assert(thisEntry != nullptr);
      paramInfoList.emplace_back(THIS_VARIABLE_NAME, thisEntry);
      paramTypes.push_back(builder.getPtrTy());
    }

    // Visit parameters
    size_t argIdx = 0;
    if (node->hasParams) {
      const size_t numOfParams = manifestation->paramList.size();
      paramInfoList.reserve(numOfParams);
      paramTypes.reserve(numOfParams);
      for (; argIdx < numOfParams; argIdx++) {
        const DeclStmtNode *param = node->paramLst->params.at(argIdx);
        // Get symbol table entry of param
        SymbolTableEntry *paramSymbol = currentScope->lookupStrict(param->varName);
        assert(paramSymbol != nullptr);
        const QualType paramSymbolType = manifestation->getParamTypes().at(argIdx);
        // Pass the information if captures are taken for function/procedure types
        if (paramSymbolType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && paramSymbolType.hasLambdaCaptures())
          paramSymbol->updateType(paramSymbol->getQualType().getWithLambdaCaptures(), true);
        // Retrieve type of param
        llvm::Type *paramType = paramSymbolType.toLLVMType(sourceFile);
        // Add it to the lists
        paramInfoList.emplace_back(param->varName, paramSymbol);
        paramTypes.push_back(paramType);
      }
    }

    // Get return type
    llvm::Type *returnType = manifestation->returnType.toLLVMType(sourceFile);

    // Check if function is explicitly inlined
    const bool explicitlyInlined = manifestation->entry->getQualType().isInline();
    // Get function linkage
    bool externalLinkage = isPublic;
    if (node->attrs && node->attrs->attrLst->hasAttr(ATTR_TEST))
      externalLinkage |= node->attrs->attrLst->getAttrValueByName(ATTR_TEST)->boolValue;
    const llvm::GlobalValue::LinkageTypes linkage = externalLinkage ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

    // Create function or implement declared function
    const std::string mangledName = manifestation->getMangledName();
    llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, paramTypes, false);
    module->getOrInsertFunction(mangledName, funcType);
    llvm::Function *func = module->getFunction(mangledName);
    node->entry->updateAddress(func);
    manifestation->llvmFunction = func;
    assert(func->empty());

    // Set attributes to function
    func->setDSOLocal(true);
    func->setLinkage(linkage);
    if (explicitlyInlined)
      func->addFnAttr(llvm::Attribute::AlwaysInline);

    // Set attributes to 'this' param
    if (manifestation->isMethod()) {
      func->addParamAttr(0, llvm::Attribute::NoUndef);
      func->addParamAttr(0, llvm::Attribute::NonNull);
      assert(thisEntry != nullptr);
      llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
      assert(structType != nullptr);
      func->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
      func->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
    }

    // Add debug info
    diGenerator.generateFunctionDebugInfo(func, manifestation);
    diGenerator.setSourceLocation(node);

    // Create entry block
    llvm::BasicBlock *bEntry = createBlock();
    switchToBlock(bEntry, func);

    // Reset alloca insert markers to this block
    allocaInsertBlock = bEntry;
    allocaInsertInst = nullptr;

    // Declare result variable
    llvm::Value *resultAddr = insertAlloca(returnType, RETURN_VARIABLE_NAME);
    SymbolTableEntry *resultEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
    assert(resultEntry != nullptr);
    resultEntry->updateAddress(resultAddr);
    // Generate debug info
    diGenerator.generateLocalVarDebugInfo(RETURN_VARIABLE_NAME, resultAddr);

    // Store function argument values
    for (auto &arg : func->args()) {
      // Get information about the parameter
      const size_t argNumber = arg.getArgNo();
      auto [paramName, paramSymbol] = paramInfoList.at(argNumber);
      assert(paramSymbol != nullptr);
      // Allocate space for it
      llvm::Type *paramType = funcType->getParamType(argNumber);
      llvm::Value *paramAddress = insertAlloca(paramType, paramName);
      // Update the symbol table entry
      paramSymbol->updateAddress(paramAddress);
      // Set source location
      diGenerator.setSourceLocation(paramSymbol->declNode);
      // Store the value at the new address
      insertStore(&arg, paramAddress);
      // Generate debug info to declare variable
      diGenerator.generateLocalVarDebugInfo(paramName, paramAddress, argNumber + 1);
    }

    // Store the default values for optional function args
    if (node->paramLst) {
      const std::vector<DeclStmtNode *> params = node->paramLst->params;
      for (; argIdx < params.size(); argIdx++)
        visit(params.at(argIdx));
    }

    // Visit function body
    visit(node->body);

    // Create return statement if the block is not terminated yet
    if (!blockAlreadyTerminated) {
      llvm::Value *result = insertLoad(returnType, resultEntry->getAddress());
      builder.CreateRet(result);
    }

    // Conclude debug info for function
    diGenerator.concludeFunctionDebugInfo();

    // Verify function
    verifyFunction(func, node->codeLoc);

    // Change to root scope
    currentScope = rootScope;

    manIdx++; // Increment symbolTypeIndex
  }
  manIdx = 0; // Reset the symbolTypeIndex

  // Ensure that we are at the root scope again
  assert(currentScope == rootScope);

  return nullptr;
}

std::any IRGenerator::visitProcDef(const ProcDefNode *node) {
  // Loop through manifestations
  manIdx = 0; // Reset the symbolTypeIndex
  for (Function *manifestation : node->manifestations) {
    assert(manifestation->entry != nullptr);

    // Check if the manifestation is substantiated or not public and not used by anybody
    const bool isPublic = manifestation->entry->getQualType().isPublic();
    if (!manifestation->isFullySubstantiated() || (!isPublic && !manifestation->used)) {
      manIdx++; // Increment symbolTypeIndex
      continue;
    }
    assert(manifestation->alreadyTypeChecked);

    // Change to struct scope
    if (manifestation->isMethod()) {
      const QualType &thisType = manifestation->thisType;
      const std::string signature = Struct::getSignature(thisType.getSubType(), thisType.getTemplateTypes());
      currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + signature);
      assert(currentScope != nullptr);
    }

    // Change scope
    currentScope = currentScope->getChildScope(manifestation->getSignature(false));
    assert(currentScope != nullptr);

    // Get 'this' entry
    std::vector<std::pair<std::string, SymbolTableEntry *>> paramInfoList;
    std::vector<llvm::Type *> paramTypes;
    SymbolTableEntry *thisEntry = nullptr;
    if (manifestation->isMethod()) {
      thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
      assert(thisEntry != nullptr);
      paramInfoList.emplace_back(THIS_VARIABLE_NAME, thisEntry);
      paramTypes.push_back(builder.getPtrTy());
    }

    // Visit parameters
    size_t argIdx = 0;
    if (node->hasParams) {
      const size_t numOfParams = manifestation->paramList.size();
      paramInfoList.reserve(numOfParams);
      paramTypes.reserve(numOfParams);
      for (; argIdx < numOfParams; argIdx++) {
        const DeclStmtNode *param = node->paramLst->params.at(argIdx);
        // Get symbol table entry of param
        SymbolTableEntry *paramSymbol = currentScope->lookupStrict(param->varName);
        assert(paramSymbol != nullptr);
        const QualType paramSymbolType = manifestation->getParamTypes().at(argIdx);
        // Pass the information if captures are taken for function/procedure types
        if (paramSymbolType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && paramSymbolType.hasLambdaCaptures())
          paramSymbol->updateType(paramSymbol->getQualType().getWithLambdaCaptures(), true);
        // Retrieve type of param
        llvm::Type *paramType = paramSymbolType.toLLVMType(sourceFile);
        // Add it to the lists
        paramInfoList.emplace_back(param->varName, paramSymbol);
        paramTypes.push_back(paramType);
      }
    }

    // Get return type
    llvm::Type *returnType = builder.getVoidTy();

    // Check if procedure is explicitly inlined
    const bool explicitlyInlined = manifestation->entry->getQualType().isInline();
    // Get procedure linkage
    const llvm::GlobalValue::LinkageTypes linkage = isPublic ? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;

    // Create procedure or implement declared procedure
    const std::string mangledName = manifestation->getMangledName();
    llvm::FunctionType *procType = llvm::FunctionType::get(returnType, paramTypes, false);
    module->getOrInsertFunction(mangledName, procType);
    llvm::Function *proc = module->getFunction(mangledName);
    node->entry->updateAddress(proc);
    manifestation->llvmFunction = proc;
    assert(proc->empty());

    // Set attributes to procedure
    proc->setDSOLocal(true);
    proc->setLinkage(linkage);
    if (explicitlyInlined)
      proc->addFnAttr(llvm::Attribute::AlwaysInline);

    // Set attributes to 'this' param
    if (manifestation->isMethod()) {
      proc->addParamAttr(0, llvm::Attribute::NoUndef);
      proc->addParamAttr(0, llvm::Attribute::NonNull);
      assert(thisEntry != nullptr);
      llvm::Type *structType = thisEntry->getQualType().getContained().toLLVMType(sourceFile);
      assert(structType != nullptr);
      proc->addDereferenceableParamAttr(0, module->getDataLayout().getTypeStoreSize(structType));
      proc->addParamAttr(0, llvm::Attribute::getWithAlignment(context, module->getDataLayout().getABITypeAlign(structType)));
    }

    // Add debug info
    diGenerator.generateFunctionDebugInfo(proc, manifestation);
    diGenerator.setSourceLocation(node);

    // Create entry block
    llvm::BasicBlock *bEntry = createBlock();
    switchToBlock(bEntry, proc);

    // Reset alloca insert markers to this block
    allocaInsertBlock = bEntry;
    allocaInsertInst = nullptr;

    // Store procedure argument values
    for (auto &arg : proc->args()) {
      // Get information about the parameter
      const size_t argNumber = arg.getArgNo();
      auto [paramName, paramSymbol] = paramInfoList.at(argNumber);
      assert(paramSymbol != nullptr);
      // Allocate space for it
      llvm::Type *paramType = procType->getParamType(argNumber);
      llvm::Value *paramAddress = insertAlloca(paramType, paramName);
      // Update the symbol table entry
      paramSymbol->updateAddress(paramAddress);
      // Set source location
      diGenerator.setSourceLocation(paramSymbol->declNode);
      // Store the value at the new address
      insertStore(&arg, paramAddress);
      // Generate debug info to declare variable
      diGenerator.generateLocalVarDebugInfo(paramName, paramAddress, argNumber + 1);
    }

    // Store the default values for optional procedure args
    if (node->paramLst) {
      const std::vector<DeclStmtNode *> params = node->paramLst->params;
      for (; argIdx < params.size(); argIdx++)
        visit(params.at(argIdx));
    }

    // Generate special ctor preamble before generating the body to store VTable, default field values, etc. if required
    if (node->isCtor)
      generateCtorBodyPreamble(currentScope);

    // Visit procedure body
    visit(node->body);

    // Create return statement if the block is not terminated yet
    if (!blockAlreadyTerminated)
      builder.CreateRetVoid();

    // Conclude debug info for procedure
    diGenerator.concludeFunctionDebugInfo();

    // Verify procedure
    verifyFunction(proc, node->codeLoc);

    // Change to root scope
    currentScope = rootScope;

    manIdx++; // Increment symbolTypeIndex
  }
  manIdx = 0; // Reset the symbolTypeIndex

  // Ensure that we are at the root scope again
  assert(currentScope == rootScope);

  return nullptr;
}

std::any IRGenerator::visitStructDef(const StructDefNode *node) {
  // Get all substantiated structs which result from this struct def
  std::vector<Struct *> manifestations = node->structManifestations;

  // Sort the manifestations to prevent generating the struct types in the wrong order (in case of dependencies between structs)
  const auto comp = [](const Struct *lhs, const Struct *rhs) { return lhs->manifestationIndex < rhs->manifestationIndex; };
  std::ranges::sort(manifestations, comp);

  for (Struct *spiceStruct : manifestations) {
    // Skip structs, that are not fully substantiated
    if (!spiceStruct->isFullySubstantiated())
      continue;

    // Do not generate this struct if it is private and used by nobody
    if (!spiceStruct->used && !spiceStruct->entry->getQualType().isPublic())
      continue;

    // Change scope to struct scope, specific to substantiation
    currentScope = spiceStruct->scope;
    assert(currentScope);

    // Set LLVM type to the struct entry
    const SymbolTableEntry *structEntry = spiceStruct->entry;
    assert(structEntry != nullptr);

    // Generate VTable if required
    if (node->emitVTable) {
      generateVTable(spiceStruct);
      deferredVTableInitializations.emplace_back([=, this]() { generateVTableInitializer(spiceStruct); }, false);
    }

    // Generate default ctor if required
    const QualType &thisType = structEntry->getQualType();
    const Function *ctorFunc = FunctionManager::lookup(currentScope, CTOR_FUNCTION_NAME, thisType, {}, true);
    if (ctorFunc != nullptr && ctorFunc->implicitDefault)
      generateDefaultCtor(ctorFunc);

    // Generate default copy ctor if required
    const ArgList args = {{thisType.toConstRef(node), false /* always non-temporary */}};
    const Function *copyCtorFunc = FunctionManager::lookup(currentScope, CTOR_FUNCTION_NAME, thisType, args, true);
    if (copyCtorFunc != nullptr && copyCtorFunc->implicitDefault)
      generateDefaultCopyCtor(copyCtorFunc);

    // Generate default dtor if required
    const Function *dtorFunc = FunctionManager::lookup(currentScope, DTOR_FUNCTION_NAME, thisType, {}, true);
    if (dtorFunc != nullptr && dtorFunc->implicitDefault)
      generateDefaultDtor(dtorFunc);

    // Return to root scope
    currentScope = rootScope;
    assert(currentScope);
  }

  return nullptr;
}

std::any IRGenerator::visitInterfaceDef(const InterfaceDefNode *node) {
  // Get all substantiated structs which result from this struct def
  std::vector<Interface *> manifestations = node->interfaceManifestations;

  // Sort the manifestations to prevent generating the struct types in the wrong order (in case of dependencies between structs)
  const auto comp = [](const Interface *lhs, const Interface *rhs) { return lhs->manifestationIndex < rhs->manifestationIndex; };
  std::ranges::sort(manifestations, comp);

  for (Interface *spiceInterface : manifestations) {
    // Skip interfaces, that are not fully substantiated
    if (!spiceInterface->isFullySubstantiated())
      continue;

    // Do not generate this interface if it is private and used by nobody
    if (!spiceInterface->used && !spiceInterface->entry->getQualType().isPublic())
      continue;

    // Generate VTable information
    generateVTable(spiceInterface);
    deferredVTableInitializations.emplace_back([=, this]() { generateVTableInitializer(spiceInterface); }, false);
  }

  return nullptr;
}

std::any IRGenerator::visitEnumDef(const EnumDefNode *node) {
  return nullptr; // Noop (enums are high-level semantic-only structures)
}

std::any IRGenerator::visitGenericTypeDef(const GenericTypeDefNode *node) {
  return nullptr; // Noop (generic types are high-level semantic-only structures)
}

std::any IRGenerator::visitAliasDef(const AliasDefNode *node) {
  return nullptr; // Noop (alias definitions are high-level semantic-only structures)
}

std::any IRGenerator::visitGlobalVarDef(const GlobalVarDefNode *node) {
  // Retrieve some information about the variable
  assert(node->entry != nullptr);
  const QualType &entryType = node->entry->getQualType();
  const bool isPublic = entryType.isPublic();
  const bool isConst = entryType.isConst();

  // Get correct type and linkage type
  const auto varType = std::any_cast<llvm::Type *>(visit(node->dataType));
  const auto linkage = isPublic ? llvm::GlobalValue::ExternalLinkage : llvm::GlobalValue::PrivateLinkage;

  // Create global var
  llvm::Value *varAddress = module->getOrInsertGlobal(node->varName, varType);
  llvm::GlobalVariable *var = module->getNamedGlobal(node->varName);
  // Set some attributes, based on the given information
  var->setLinkage(linkage);
  var->setConstant(isConst);

  // Set initializer
  if (node->hasValue) { // Set the constant value as variable initializer
    const auto constantValue = std::any_cast<llvm::Constant *>(visit(node->constant));
    var->setInitializer(constantValue);
  } else if (cliOptions.buildMode == DEBUG) { // Set the default value as variable initializer
    llvm::Constant *constantValue = getDefaultValueForSymbolType(node->entry->getQualType());
    var->setInitializer(constantValue);
  }

  node->entry->updateAddress(varAddress);

  // Add debug info
  diGenerator.generateGlobalVarDebugInfo(var, node->entry);

  return nullptr;
}

std::any IRGenerator::visitExtDecl(const ExtDeclNode *node) {
  // Get return type
  const Function *spiceFunc = node->extFunction;
  assert(spiceFunc != nullptr);
  llvm::Type *returnType = builder.getVoidTy();
  if (!spiceFunc->returnType.is(TY_DYN))
    returnType = spiceFunc->returnType.toLLVMType(sourceFile);

  // Get arg types
  std::vector<llvm::Type *> argTypes;
  argTypes.reserve(spiceFunc->paramList.size());
  for (const QualType &paramType : spiceFunc->getParamTypes())
    argTypes.push_back(paramType.toLLVMType(sourceFile));

  // Declare function
  llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, argTypes, node->isVarArg);
  const std::string mangledName = spiceFunc->getMangledName();
  module->getOrInsertFunction(mangledName, functionType);
  llvm::Function *fct = module->getFunction(mangledName);

  // Add noundef attribute to all parameters
  for (size_t i = 0; i < argTypes.size(); i++)
    fct->addParamAttr(i, llvm::Attribute::NoUndef);

  // If the function should be imported as dll, add the dll attribute
  if (node->attrs && node->attrs->attrLst->hasAttr(ATTR_CORE_LINKER_DLL))
    if (node->attrs->attrLst->getAttrValueByName(ATTR_CORE_LINKER_DLL)->boolValue)
      fct->setDLLStorageClass(llvm::GlobalValue::DLLImportStorageClass);

  return nullptr;
}

} // namespace spice::compiler