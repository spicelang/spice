// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

std::any TypeChecker::visitFctDefPrepare(FctDefNode *node) {
  std::vector<GenericType> usedGenericTypes;

  // Change to function scope
  currentScope = node->fctScope;

  // Check if all control paths in the function return
  if (!node->returnsOnAllControlPaths())
    throw SemanticError(node, NOT_ALL_CONTROL_PATHS_RETURN, "Not all control paths have a return statement");

  // Retrieve function template types
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = node->structScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  SymbolType thisType(TY_DYN); // If the function is not a method, the default this type is TY_DYN
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    SymbolTableEntry *structEntry = node->structScope->parent->lookupStrict(node->structName);
    assert(structEntry != nullptr);
    thisType = structEntry->getType();
    thisPtrType = thisType.toPointer(node);
    // Collect template types of 'this' type
    for (const SymbolType &templateType : thisType.getTemplateTypes())
      usedGenericTypes.emplace_back(templateType);
  }

  // Set type of 'this' variable
  if (node->isMethod) {
    SymbolTableEntry *thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    thisEntry->updateType(thisPtrType, false);
  }

  // Retrieve return type
  auto returnType = std::any_cast<SymbolType>(visit(node->returnType()));
  if (returnType.is(TY_DYN))
    throw SemanticError(node, UNEXPECTED_DYN_TYPE_SA, "Dyn return types are not allowed");
  // Set return type to the result variable
  SymbolTableEntry *resultVarEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultVarEntry != nullptr);
  resultVarEntry->updateType(returnType, false);
  resultVarEntry->used = true;

  // Visit parameters
  std::vector<std::string> paramNames;
  ParamList paramTypes;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      paramNames.push_back(param.name);
      paramTypes.push_back({param.type, param.isOptional});
      if (!param.type.is(TY_GENERIC))
        continue;
      // Check if the type is present in the template for generic types
      if (std::none_of(usedGenericTypes.begin(), usedGenericTypes.end(), [&](const GenericType &t) { return t == param.type; }))
        throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic argument type not included in function template types");
    }
  }

  // Leave function body scope
  currentScope = node->fctScope->parent;

  // Update type of function entry
  SymbolTableEntry *functionEntry = currentScope->lookupStrict(node->getTemporaryName());
  assert(functionEntry != nullptr);
  functionEntry->updateType(SymbolType(TY_FUNCTION), false);

  // Build function object
  const Function spiceFunc(node->functionName, functionEntry, thisType, returnType, paramTypes, usedGenericTypes, node);
  currentScope->insertFunction(spiceFunc, &node->fctManifestations);

  // Rename / duplicate the original child scope to reflect the substantiated versions of the function
  const std::vector<Function> substantiatedFunctions = spiceFunc.substantiateOptionalParams();
  currentScope->renameChildScope(node->getScopeId(), substantiatedFunctions.front().getSignature());
  for (int i = 1; i < substantiatedFunctions.size(); i++)
    currentScope->copyChildScope(substantiatedFunctions.front().getSignature(), substantiatedFunctions[i].getSignature());

  // Change to the root scope
  currentScope = rootScope;

  return nullptr;
}

std::any TypeChecker::visitProcDefPrepare(ProcDefNode *node) {
  std::vector<GenericType> usedGenericTypes;

  // Change to procedure scope
  currentScope = node->procScope;

  // Retrieve procedure template types
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = node->structScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  SymbolType thisType(TY_DYN); // If the procedure is not a method, the default this type is TY_DYN
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    SymbolTableEntry *structEntry = node->structScope->parent->lookupStrict(node->structName);
    assert(structEntry != nullptr);
    thisType = structEntry->getType();
    thisPtrType = thisType.toPointer(node);
    // Collect template types of 'this' type
    for (const SymbolType &templateType : thisType.getTemplateTypes())
      usedGenericTypes.emplace_back(templateType);
  }

  // Set type of 'this' variable
  if (node->isMethod) {
    SymbolTableEntry *thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    thisEntry->updateType(thisPtrType, false);
  }

  // Visit parameters
  std::vector<std::string> paramNames;
  ParamList paramTypes;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      paramNames.push_back(param.name);
      paramTypes.push_back({param.type, param.isOptional});
      if (!param.type.is(TY_GENERIC))
        continue;
      // Check if the type is present in the template for generic types
      if (std::none_of(usedGenericTypes.begin(), usedGenericTypes.end(), [&](const GenericType &t) { return t == param.type; }))
        throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic argument type not included in procedure template types");
    }
  }

  // Leave procedure body scope
  currentScope = node->procScope->parent;

  // Update type of procedure entry
  SymbolTableEntry *procedureEntry = currentScope->lookupStrict(node->getTemporaryName());
  assert(procedureEntry != nullptr);
  procedureEntry->updateType(SymbolType(TY_PROCEDURE), false);

  // Build procedure object
  const Function spiceProc(node->procedureName, procedureEntry, thisType, SymbolType(TY_DYN), paramTypes, usedGenericTypes, node);
  currentScope->insertFunction(spiceProc, &node->procManifestations);

  // Rename / duplicate the original child block to reflect the substantiated versions of the procedure
  const std::vector<Function> substantiatedProcedures = spiceProc.substantiateOptionalParams();
  currentScope->renameChildScope(node->getScopeId(), substantiatedProcedures.front().getSignature());
  for (int i = 1; i < substantiatedProcedures.size(); i++)
    currentScope->copyChildScope(substantiatedProcedures.front().getSignature(), substantiatedProcedures[i].getSignature());

  // Change to the root scope
  currentScope = rootScope;

  return nullptr;
}

std::any TypeChecker::visitStructDefPrepare(StructDefNode *node) {
  std::vector<SymbolType> usedTemplateTypes;
  std::vector<GenericType> usedTemplateTypesGeneric;

  // Retrieve struct template types
  if (node->isGeneric) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedTemplateTypes.push_back(*genericType);
      usedTemplateTypesGeneric.push_back(*genericType);
    }
  }

  // Retrieve implemented interfaces
  std::vector<SymbolType> interfaceTypes;
  if (node->hasInterfaces) {
    for (DataTypeNode *dataType : node->interfaceTypeLst()->dataTypes()) {
      // Visit interface type
      auto interfaceType = std::any_cast<SymbolType>(visit(dataType));
      // Check if it is an interface type
      if (!interfaceType.is(TY_INTERFACE))
        throw SemanticError(dataType, EXPECTED_INTERFACE_TYPE, "Expected interface type, got " + interfaceType.getName());
      // Add to interface types
      interfaceTypes.push_back(interfaceType);
    }
  }

  // Update type of struct entry
  SymbolTableEntry *structEntry = rootScope->lookupStrict(node->structName);
  assert(structEntry != nullptr);
  const SymbolType structType(TY_STRUCT, node->structName, {}, usedTemplateTypes);
  structEntry->updateType(structType, false);

  // Change to struct scope
  currentScope = node->structScope;

  // Retrieve field types
  std::vector<SymbolType> fieldTypes;
  for (const auto &field : node->fields()) {
    // Visit field type
    auto fieldType = std::any_cast<SymbolType>(visit(field->dataType()));

    // Add to field types
    fieldTypes.push_back(fieldType);

    // Update type of field entry
    SymbolTableEntry *fieldEntry = currentScope->lookupStrict(field->fieldName);
    assert(fieldEntry != nullptr);
    fieldEntry->updateType(fieldType, false);

    if (!fieldType.isBaseType(TY_GENERIC))
      continue;

    // Check if the template type list contains this type
    if (std::none_of(usedTemplateTypesGeneric.begin(), usedTemplateTypesGeneric.end(),
                     [&](const GenericType &t) { return t == fieldType.getBaseType(); }))
      throw SemanticError(field->dataType(), GENERIC_TYPE_NOT_IN_TEMPLATE, "Generic field type not included in struct template");
  }

  // Change to the root scope
  currentScope = rootScope;

  // Build struct object
  Struct spiceStruct(node->structName, structEntry, fieldTypes, usedTemplateTypesGeneric, interfaceTypes, node);
  node->spiceStruct = currentScope->insertStruct(spiceStruct);
  spiceStruct.structScope = node->structScope;

  return nullptr;
}

std::any TypeChecker::visitInterfaceDefPrepare(InterfaceDefNode *node) {
  // Update type of interface entry
  SymbolType interfaceType(TY_INTERFACE, node->interfaceName);
  SymbolTableEntry *interfaceEntry = currentScope->lookupStrict(node->interfaceName);
  assert(interfaceEntry != nullptr);
  interfaceEntry->updateType(interfaceType, false);

  // Change to interface scope
  currentScope = node->interfaceScope;

  // Visit signatures
  std::vector<Function *> signatures;
  signatures.reserve(node->signatures().size());
  for (SignatureNode *signature : node->signatures()) {
    auto method = std::any_cast<std::vector<Function *> *>(visit(signature));
    assert(method != nullptr);
    signatures.insert(signatures.end(), method->begin(), method->end());
  }

  // Change to root scope
  currentScope = rootScope;

  // Build interface object
  Interface i(node->interfaceName, node->entry->specifiers, signatures, node);
  rootScope->insertInterface(i);
  i.interfaceScope = node->interfaceScope;

  return nullptr;
}

std::any TypeChecker::visitEnumDefPrepare(EnumDefNode *node) {
  // Get enum entry
  SymbolTableEntry *enumEntry = rootScope->lookupStrict(node->enumName);
  assert(enumEntry != nullptr);

  // Change to enum scope
  currentScope = node->enumScope;

  // Loop through all items with values
  std::vector<std::string> names;
  std::vector<uint32_t> values;
  for (const EnumItemNode *enumItem : node->itemLst()->items()) {
    // Save name
    names.push_back(enumItem->itemName);
    // Check for duplicate value
    if (enumItem->hasValue) {
      if (std::find(values.begin(), values.end(), enumItem->itemValue) != values.end())
        throw SemanticError(enumItem, DUPLICATE_ENUM_ITEM_VALUE, "Duplicate enum item value, please use another");
      values.push_back(enumItem->itemValue);
    }
  }

  // Loop through all items without values
  uint32_t nextValue = 0;
  SymbolType intSymbolType(TY_INT);
  for (EnumItemNode *enumItem : node->itemLst()->items()) {
    // Update type of enum item entry
    SymbolTableEntry *itemEntry = currentScope->lookupStrict(enumItem->itemName);
    assert(itemEntry != nullptr);
    itemEntry->updateType(intSymbolType, false);
    // Fill in value if not filled yet
    if (!enumItem->hasValue) {
      while (std::find(values.begin(), values.end(), nextValue) != values.end())
        nextValue++;
      enumItem->itemValue = nextValue;
      values.push_back(nextValue);
    }
  }

  return nullptr;
}

std::any TypeChecker::visitGenericTypeDefPrepare(GenericTypeDefNode *node) {
  // Retrieve type conditions
  std::vector<SymbolType> typeConditions;
  typeConditions.reserve(node->typeAltsLst()->dataTypes().size());
  for (const auto &typeAlt : node->typeAltsLst()->dataTypes()) {
    auto typeCondition = std::any_cast<SymbolType>(visit(typeAlt));
    typeConditions.push_back(typeCondition);
  }

  // Add generic type to the scope
  const GenericType genericType(node->typeName, typeConditions);
  currentScope->insertGenericType(genericType);

  return nullptr;
}

std::any TypeChecker::visitGlobalVarDefPrepare(GlobalVarDefNode *node) {
  // Insert variable name to symbol table
  auto globalVarType = std::any_cast<SymbolType>(visit(node->dataType()));

  if (node->constant()) { // Variable is initialized here
    SymbolType rhsType = std::any_cast<ExprResult>(visit(node->constant())).type;
    if (globalVarType.is(TY_DYN)) { // Perform type inference
      globalVarType = rhsType;
    } else if (globalVarType != rhsType) { // Check if types are matching
      throw SemanticError(node->constant(), OPERATOR_WRONG_DATA_TYPE,
                          "Expected " + globalVarType.getName() + ", but got " + rhsType.getName());
    }
  }

  // Check if the type is still missing
  if (globalVarType.is(TY_DYN))
    throw SemanticError(node->dataType(), GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope to initialize the variable
  if (!globalVarType.isPrimitive())
    throw SemanticError(node->dataType(), GLOBAL_OF_INVALID_TYPE, "Spice does only global variables of primitive type");

  // Update type of global var entry
  SymbolTableEntry *globalVarEntry = rootScope->lookupStrict(node->varName);
  assert(globalVarEntry != nullptr);
  globalVarEntry->updateType(globalVarType, false);

  return nullptr;
}

std::any TypeChecker::visitExtDeclPrepare(ExtDeclNode *node) {
  // Collect argument types
  ParamList argTypes;
  if (node->hasArgs) {
    argTypes.reserve(node->argTypeLst()->dataTypes().size());
    for (DataTypeNode *arg : node->argTypeLst()->dataTypes()) {
      // Visit argument
      auto argType = std::any_cast<SymbolType>(visit(arg));
      // Check if the argument type is 'dyn'
      if (argType.is(TY_DYN))
        throw SemanticError(arg, UNEXPECTED_DYN_TYPE_SA, "Dyn data type is not allowed as arg type for external functions");
      // Save argument
      argTypes.push_back({argType, false});
    }
  }

  SymbolType dynType(TY_DYN);
  Function spiceFunc;
  if (node->returnType()) { // External function
    // Visit return type
    auto returnType = std::any_cast<SymbolType>(visit(node->returnType()));
    // Check if return type is dyn
    if (returnType.is(TY_DYN))
      throw SemanticError(node->returnType(), UNEXPECTED_DYN_TYPE_SA, "dyn is not allowed as return type for external functions");
    spiceFunc = Function(node->extFunctionName, node->entry, /*thisType=*/dynType, returnType, argTypes, {}, node);
  } else { // External procedure
    spiceFunc = Function(node->extFunctionName, node->entry, /*thisType=*/dynType, /*returnType=*/dynType, argTypes, {}, node);
  }

  // Add function to current scope
  std::vector<Function *> manifestations;
  currentScope->insertFunction(spiceFunc, &manifestations);
  assert(!manifestations.empty());
  node->externalFunction = manifestations.front();

  return nullptr;
}