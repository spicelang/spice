// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any IRGenerator::visitValue(const ValueNode *node) {
  diGenerator.setSourceLocation(node);

  // Constant
  if (node->constant()) {
    // Visit constant value
    auto constantValue = std::any_cast<llvm::Constant *>(visit(node->constant()));
    return ExprResult{.constant = constantValue};
  }

  // Function call
  if (node->functionCall())
    return visit(node->functionCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  if (node->isNil) {
    // Retrieve type of the nil constant
    auto nilType = any_cast<llvm::Type *>(visit(node->nilType()));
    // Create constant nil value
    llvm::Constant *nilValue = llvm::Constant::getNullValue(nilType);
    // Return it
    return ExprResult{.constant = nilValue};
  }

  throw std::runtime_error("Internal compiler error: Value fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitConstant(const ConstantNode *node) {
  const CompileTimeValue &compileTimeValue = node->getCompileTimeValue();

  // Value is a double constant
  if (node->type == ConstantNode::TYPE_DOUBLE) {
    llvm::Constant *value = llvm::ConstantFP::get(context, llvm::APFloat(compileTimeValue.doubleValue));
    return value;
  }

  // Value is an integer constant
  if (node->type == ConstantNode::TYPE_INT) {
    llvm::Type *intTy = builder.getInt32Ty();
    llvm::Constant *constInt = llvm::ConstantInt::get(intTy, compileTimeValue.intValue, node->isSigned);
    return constInt;
  }

  // Value is a short constant
  if (node->type == ConstantNode::TYPE_SHORT) {
    llvm::Type *shortTy = builder.getInt16Ty();
    llvm::Constant *constShort = llvm::ConstantInt::get(shortTy, compileTimeValue.shortValue, node->isSigned);
    return constShort;
  }

  // Value is a long constant
  if (node->type == ConstantNode::TYPE_LONG) {
    llvm::Type *longTy = builder.getInt64Ty();
    llvm::Constant *constLong = llvm::ConstantInt::get(longTy, compileTimeValue.longValue, node->isSigned);
    return constLong;
  }

  // Value is a char constant
  if (node->type == ConstantNode::TYPE_CHAR) {
    llvm::Type *charTy = builder.getInt8Ty();
    llvm::Constant *constChar = llvm::ConstantInt::get(charTy, compileTimeValue.charValue, node->isSigned);
    return constChar;
  }

  // Value is a string constant
  if (node->type == ConstantNode::TYPE_STRING) {
    const std::string globalName = getUnusedGlobalName(ANON_GLOBAL_STRING_NAME);
    llvm::Constant *constString = builder.CreateGlobalStringPtr(compileTimeValue.stringValue, globalName, 0, module);
    return constString;
  }

  // Value is a boolean constant
  if (node->type == ConstantNode::TYPE_BOOL) {
    llvm::Constant *constBool = compileTimeValue.boolValue ? builder.getTrue() : builder.getFalse();
    return constBool;
  }

  throw std::runtime_error("Internal compiler error: Constant fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitFunctionCall(const FunctionCallNode *node) {
  diGenerator.setSourceLocation(node);

  const FunctionCallNode::FunctionCallData &data = node->data.at(manIdx);

  Function *spiceFunc = data.callee;
  Scope *accessScope = data.calleeParentScope;
  const std::string &mangledName = data.callee->external ? node->fqFunctionName : spiceFunc->getMangledName();
  std::vector<llvm::Value *> argValues;

  // Get this type
  llvm::Value *thisPtr = nullptr;
  if (data.isMethodCall) {
    // Retrieve entry of the first fragment
    SymbolTableEntry *firstFragmentEntry = currentScope->lookup(node->functionNameFragments.front());
    assert(firstFragmentEntry != nullptr && firstFragmentEntry->getType().isBaseType(TY_STRUCT));
    Scope *structScope = firstFragmentEntry->getType().getStructBodyScope();

    // Get address of the referenced variable / struct instance
    thisPtr = firstFragmentEntry->getAddress();

    // Auto de-reference 'this' pointer
    SymbolType firstFragmentType = firstFragmentEntry->getType();
    autoDeReferencePtr(thisPtr, firstFragmentType, structScope->parent);
    llvm::Type *structTy = firstFragmentEntry->getType().getBaseType().toLLVMType(context, structScope->parent);

    // Traverse through structs - the first fragment is already looked up and the last one is the function name
    for (size_t i = 1; i < node->functionNameFragments.size() - 1; i++) {
      const std::string identifier = node->functionNameFragments.at(i);
      // Retrieve field entry
      SymbolTableEntry *fieldEntry = structScope->lookupStrict(identifier);
      assert(fieldEntry != nullptr && fieldEntry->getType().is(TY_STRUCT));
      // Get struct type and scope
      structScope = fieldEntry->getType().getStructBodyScope();
      assert(structScope != nullptr);
      // Get address of field
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(fieldEntry->orderIndex)};
      thisPtr = builder.CreateInBoundsGEP(structTy, thisPtr, indices);
      // Auto de-reference pointer and get new struct type
      SymbolType fieldEntryType = fieldEntry->getType();
      autoDeReferencePtr(thisPtr, fieldEntryType, structScope->parent);
      structTy = fieldEntry->getType().getBaseType().toLLVMType(context, structScope->parent);
    }

    // Add 'this' pointer to the front of the argument list
    argValues.push_back(thisPtr);
  }

  if (data.isConstructorCall) {
    llvm::Type *thisType = spiceFunc->thisType.toLLVMType(context, spiceFunc->thisType.getStructBodyScope());
    thisPtr = insertAlloca(thisType);

    // Add 'this' pointer to the front of the argument list
    argValues.push_back(thisPtr);
  }

  // Get arg values
  if (node->hasArgs) {
    argValues.reserve(spiceFunc->paramList.size());
    for (AssignExprNode *argNode : node->argLst()->args()) {
      llvm::Value *argValue = resolveValue(argNode);
      argValues.push_back(argValue);
    }
  }

  // Function is not defined in the current module -> declare it
  // This can happen when:
  // 1) If this is an imported source file
  // 2) This is a down-call to a function, which is defined later in the same file
  if (data.isImported || data.isDownCall) {
    // Get returnType
    llvm::Type *returnType = builder.getVoidTy();
    if (!spiceFunc->returnType.is(TY_DYN))
      returnType = spiceFunc->returnType.toLLVMType(context, accessScope);

    // Get arg types
    std::vector<llvm::Type *> argTypes;
    if (data.isMethodCall)
      argTypes.push_back(spiceFunc->thisType.toLLVMType(context, accessScope)->getPointerTo());
    for (const SymbolType &paramType : spiceFunc->getParamTypes())
      argTypes.push_back(paramType.toLLVMType(context, accessScope));

    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Generate function call
  llvm::Value *resultValue = builder.CreateCall(callee, argValues);

  if (data.isConstructorCall)
    return ExprResult{.ptr = thisPtr};

  return ExprResult{.value = resultValue};
}

std::any IRGenerator::visitArrayInitialization(const ArrayInitializationNode *node) {
  // Return immediately if the initialization is empty
  if (node->actualSize == 0)
    return ExprResult{.node = node};

  // Visit array items
  bool canBeConstant = true;
  std::vector<ExprResult> itemResults;
  itemResults.reserve(node->actualSize);
  for (AssignExprNode *itemNode : node->itemLst()->args()) {
    auto item = std::any_cast<ExprResult>(visit(itemNode));
    canBeConstant &= item.constant != nullptr;
    item.node = itemNode;
    itemResults.push_back(item);
  }

  // Get LLVM type of item and array
  assert(!itemResults.empty());
  const SymbolType &firstItemSTy = node->itemLst()->args().front()->getEvaluatedSymbolType(manIdx);
  llvm::Type *itemType = firstItemSTy.toLLVMType(context, currentScope);
  llvm::ArrayType *arrayType = llvm::ArrayType::get(itemType, node->actualSize);

  if (canBeConstant) { // All items are constants, so we can create a global constant array
    // Collect constants
    std::vector<llvm::Constant *> constants;
    for (const ExprResult &exprResult : itemResults) {
      // Delete potential constant globals, that were already created a layer below
      if (exprResult.constant->getType()->isArrayTy())
        module->getNamedGlobal(exprResult.ptr->getName())->eraseFromParent();
      constants.push_back(exprResult.constant);
    }

    // Create global array
    llvm::Constant *constantArray = llvm::ConstantArray::get(arrayType, constants);
    llvm::Value *arrayAddr = createGlobalConstant(ANON_GLOBAL_ARRAY_NAME, constantArray);

    return ExprResult{.ptr = arrayAddr, .constant = constantArray};
  } else { // We have non-immediate values as items, so we need to take normal arrays as fallback
    llvm::Value *arrayAddr = insertAlloca(arrayType);

    // Retrieve address of first item
    llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
    llvm::Value *firstItemAddress = builder.CreateInBoundsGEP(arrayType, arrayAddr, indices);

    // Store all array items at their corresponding offsets
    llvm::Value *currentItemAddress = firstItemAddress;
    for (size_t i = 0; i < itemResults.size(); i++) {
      ExprResult &exprResult = itemResults[i];
      llvm::Value *itemValue = resolveValue(exprResult.node, exprResult);
      // Retrieve current item address
      if (i >= 1)
        currentItemAddress = builder.CreateInBoundsGEP(itemType, currentItemAddress, builder.getInt32(1));
      // Store the item value
      const bool storeVolatile = exprResult.entry != nullptr && exprResult.entry->isVolatile;
      builder.CreateStore(itemValue, currentItemAddress, storeVolatile);
    }

    return ExprResult{.ptr = arrayAddr};
  }
}

std::any IRGenerator::visitStructInstantiation(const StructInstantiationNode *node) {
  // Get struct object
  const Struct *spiceStruct = node->instantiatedStructs.at(manIdx);
  assert(spiceStruct != nullptr);
  const size_t numFields = spiceStruct->fieldTypes.size();

  // Get struct type
  assert(spiceStruct->entry != nullptr);
  llvm::StructType *structType = spiceStruct->entry->getStructLLVMType();
  assert(structType != nullptr);

  if (!node->fieldLst()) {
    llvm::Constant *constantStruct = getDefaultValueForSymbolType(spiceStruct->entry->getType());
    return ExprResult{.constant = constantStruct};
  }

  // Visit struct field values
  bool canBeConstant = true;
  std::vector<ExprResult> fieldValueResults;
  fieldValueResults.reserve(numFields);
  for (AssignExprNode *fieldValueNode : node->fieldLst()->args()) {
    auto fieldValue = std::any_cast<ExprResult>(visit(fieldValueNode));
    canBeConstant &= fieldValue.constant != nullptr;
    fieldValue.node = fieldValueNode;
    fieldValueResults.push_back(fieldValue);
  }

  if (canBeConstant) { // All field values are constants, so we can create a global constant struct instantiation
    // Collect constants
    std::vector<llvm::Constant *> constants;
    for (const ExprResult &exprResult : fieldValueResults) {
      // Delete potential constant globals, that were already created a layer below
      if (exprResult.constant->getType()->isStructTy())
        module->getNamedGlobal(exprResult.ptr->getName())->eraseFromParent();
      constants.push_back(exprResult.constant);
    }

    // Create global struct
    llvm::Constant *constantStruct = llvm::ConstantStruct::get(structType, constants);
    llvm::Value *structAddr = createGlobalConstant(ANON_GLOBAL_STRUCT_NAME, constantStruct);

    return ExprResult{.ptr = structAddr, .constant = constantStruct};
  } else { // We have at least one non-immediate value, so we need to take normal struct instantiation as fallback
    llvm::Value *structAddr = insertAlloca(structType);

    // Store all field values at their corresponding offsets
    llvm::Value *indices[2] = {builder.getInt32(0), nullptr};
    for (size_t i = 0; i < fieldValueResults.size(); i++) {
      ExprResult &exprResult = fieldValueResults.at(i);
      // Get field value
      llvm::Value *itemValue = resolveValue(exprResult.node, exprResult);
      // Get field address
      indices[1] = builder.getInt32(i);
      llvm::Value *currentFieldAddress = builder.CreateInBoundsGEP(structType, structAddr, indices);
      // Store the item value
      const bool storeVolatile = exprResult.entry != nullptr && exprResult.entry->isVolatile;
      builder.CreateStore(itemValue, currentFieldAddress, storeVolatile);
    }

    return ExprResult{.ptr = structAddr};
  }
}

std::any IRGenerator::visitDataType(const DataTypeNode *node) {
  diGenerator.setSourceLocation(node);

  // Retrieve symbol type
  SymbolType symbolType = node->getEvaluatedSymbolType(manIdx);
  assert(!symbolType.is(TY_DYN)); // Symbol type should not be dyn anymore at this point

  return symbolType.toLLVMType(context, currentScope);
}

} // namespace spice::compiler