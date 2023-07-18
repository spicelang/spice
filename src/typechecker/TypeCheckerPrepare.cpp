// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

std::any TypeChecker::visitMainFctDefPrepare(MainFctDefNode *node) {
  // Mark unreachable statements
  node->returnsOnAllControlPaths(nullptr);

  // Retrieve return type
  SymbolType returnType(TY_INT);

  // Change to function body scope
  currentScope = node->fctScope;

  // Set type of 'result' variable to int
  SymbolTableEntry *resultEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultEntry != nullptr);
  resultEntry->updateType(returnType, false);
  resultEntry->used = true;

  // Retrieve param types
  std::vector<SymbolType> paramTypes;
  if (node->takesArgs) {
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList)
      paramTypes.push_back(param.type);
  }

  // Prepare type of function
  SymbolType functionType(TY_FUNCTION);
  functionType.setFunctionReturnType(returnType);
  functionType.setFunctionParamTypes(paramTypes);

  // Update main function symbol type
  SymbolTableEntry *functionEntry = rootScope->lookupStrict(node->getSignature());
  assert(functionEntry != nullptr);
  functionEntry->updateType(functionType, false);
  functionEntry->used = true;

  // Leave main function body scope
  currentScope = rootScope;

  return nullptr;
}

std::any TypeChecker::visitFctDefPrepare(FctDefNode *node) {
  // Check if name is dtor
  if (node->fctName->name == DTOR_FUNCTION_NAME)
    throw SemanticError(node, DTOR_MUST_BE_PROCEDURE, "Destructors are not allowed to be of type function");

  // Check if all control paths in the function return
  if (!node->returnsOnAllControlPaths(nullptr))
    throw SemanticError(node, MISSING_RETURN_STMT, "Not all control paths of this function have a return statement");

  // Change to function scope
  currentScope = node->fctScope;
  assert(currentScope->type == SCOPE_FUNC_PROC_BODY);

  // Retrieve function template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = node->fctScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  SymbolType thisType(TY_DYN); // If the function is not a method, the default this type is TY_DYN
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    Scope *structParentScope = node->structScope->parent;
    SymbolTableEntry *structEntry = structParentScope->lookupStrict(node->fctName->structName);
    assert(structEntry != nullptr);
    // Set struct to used
    structEntry->used = true;
    /*auto manifestations = StructManager::getManifestationList(structParentScope, structEntry->getDeclCodeLoc());
    if (manifestations)
      for (auto &manifestation : *manifestations)
        manifestation.second.used = true;*/
    // Get type and ptr type
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
  HANDLE_UNRESOLVED_TYPE_PTR(returnType)
  if (returnType.is(TY_DYN))
    throw SemanticError(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed");
  if (!returnType.isCoveredByGenericTypeList(usedGenericTypes))
    throw SemanticError(node->returnType(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                        "Generic return type not included in the template type list of the function");

  // Visit parameters
  std::vector<std::string> paramNames;
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      paramNames.push_back(param.name);
      paramTypes.push_back(param.type);
      paramList.push_back({param.type, param.isOptional});
      // Check if the type is present in the template for generic types
      if (!param.type.isCoveredByGenericTypeList(usedGenericTypes))
        throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic param type not included in the template type list of the function");
    }
  }

  // Leave function body scope
  currentScope = node->fctScope->parent;
  assert(currentScope->type == SCOPE_GLOBAL || currentScope->type == SCOPE_STRUCT);

  // Prepare type of function
  SymbolType functionType(TY_FUNCTION);
  functionType.specifiers = node->functionSpecifiers;
  functionType.setFunctionReturnType(returnType);
  functionType.setFunctionParamTypes(paramTypes);

  // Update type of function entry
  SymbolTableEntry *functionEntry = currentScope->lookupStrict(node->getSymbolTableEntryName());
  assert(functionEntry != nullptr);
  functionEntry->updateType(functionType, false);

  // Build function object
  const Function spiceFunc(node->fctName->name, functionEntry, thisType, returnType, paramList, usedGenericTypes, node,
                           /*external=*/false);
  FunctionManager::insertFunction(currentScope, spiceFunc, &node->fctManifestations);

  // Check function attributes
  if (node->attrs()) {
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLE); attr != nullptr)
      node->fctManifestations.front()->mangleFunctionName = attr->value()->compileTimeValue.boolValue;
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLED_NAME); attr != nullptr)
      node->fctManifestations.front()->predefinedMangledName = attr->value()->compileTimeValue.stringValue;
  }

  // Rename / duplicate the original child scope to reflect the substantiated versions of the function
  currentScope->renameChildScope(node->getScopeId(), node->fctManifestations.front()->getSignature(false));
  for (size_t i = 1; i < node->fctManifestations.size(); i++)
    currentScope->copyChildScope(node->fctManifestations.front()->getSignature(false),
                                 node->fctManifestations.at(i)->getSignature(false));

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == SCOPE_GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitProcDefPrepare(ProcDefNode *node) {
  // Mark unreachable statements
  node->returnsOnAllControlPaths(nullptr);

  // Check if dtor and has params
  if (node->hasParams && node->procName->name == DTOR_FUNCTION_NAME)
    throw SemanticError(node, DTOR_WITH_PARAMS, "It is not allowed to specify parameters for destructors");

  // Change to procedure scope
  currentScope = node->procScope;
  assert(currentScope->type == SCOPE_FUNC_PROC_BODY);

  // Retrieve procedure template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = node->procScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  SymbolType thisType(TY_DYN); // If the procedure is not a method, the default this type is TY_DYN
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    Scope *structParentScope = node->structScope->parent;
    SymbolTableEntry *structEntry = structParentScope->lookupStrict(node->procName->structName);
    assert(structEntry != nullptr);
    // Set struct to used
    structEntry->used = true;
    /*auto manifestations = StructManager::getManifestationList(structParentScope, structEntry->getDeclCodeLoc());
    if (manifestations)
      for (auto &manifestation : *manifestations)
        manifestation.second.used = true;*/
    // Get type and ptr type
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
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      paramNames.push_back(param.name);
      paramTypes.push_back(param.type);
      paramList.push_back({param.type, param.isOptional});
      // Check if the type is present in the template for generic types
      if (!param.type.isCoveredByGenericTypeList(usedGenericTypes))
        throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic param type not included in the template type list of the procedure");
    }
  }

  // Leave procedure body scope
  currentScope = node->procScope->parent;
  assert(currentScope->type == SCOPE_GLOBAL || currentScope->type == SCOPE_STRUCT);

  // Prepare type of procedure
  SymbolType procedureType(TY_PROCEDURE);
  procedureType.specifiers = node->procedureSpecifiers;
  procedureType.setFunctionParamTypes(paramTypes);

  // Update type of procedure entry
  SymbolTableEntry *procedureEntry = currentScope->lookupStrict(node->getSymbolTableEntryName());
  assert(procedureEntry != nullptr);
  procedureEntry->updateType(procedureType, false);

  // Build procedure object
  const Function spiceProc(node->procName->name, procedureEntry, thisType, SymbolType(TY_DYN), paramList, usedGenericTypes, node,
                           /*external=*/false);
  FunctionManager::insertFunction(currentScope, spiceProc, &node->procManifestations);

  // Check procedure attributes
  if (node->attrs()) {
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLE); attr != nullptr)
      node->procManifestations.front()->mangleFunctionName = attr->value()->compileTimeValue.boolValue;
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLED_NAME); attr != nullptr)
      node->procManifestations.front()->predefinedMangledName = attr->value()->compileTimeValue.stringValue;
  }

  // Rename / duplicate the original child block to reflect the substantiated versions of the procedure
  currentScope->renameChildScope(node->getScopeId(), node->procManifestations.front()->getSignature(false));
  for (size_t i = 1; i < node->procManifestations.size(); i++)
    currentScope->copyChildScope(node->procManifestations.front()->getSignature(false),
                                 node->procManifestations.at(i)->getSignature(false));

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == SCOPE_GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitStructDefPrepare(StructDefNode *node) {
  std::vector<SymbolType> usedTemplateTypes;
  std::vector<GenericType> templateTypesGeneric;

  // Retrieve struct template types
  if (node->hasTemplateTypes) {
    usedTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    templateTypesGeneric.reserve(node->templateTypeLst()->dataTypes().size());
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedTemplateTypes.push_back(*genericType);
      templateTypesGeneric.push_back(*genericType);
    }
  }

  // Retrieve implemented interfaces
  std::vector<SymbolType> interfaceTypes;
  if (node->hasInterfaces) {
    interfaceTypes.reserve(node->interfaceTypeLst()->dataTypes().size());
    for (DataTypeNode *dataType : node->interfaceTypeLst()->dataTypes()) {
      // Visit interface type
      auto interfaceType = std::any_cast<SymbolType>(visit(dataType));
      if (interfaceType.is(TY_UNRESOLVED))
        continue;
      // Check if it is an interface type
      if (!interfaceType.is(TY_INTERFACE))
        throw SemanticError(dataType, EXPECTED_INTERFACE_TYPE, "Expected interface type, got " + interfaceType.getName());
      // Add to interface types
      interfaceTypes.push_back(interfaceType);
    }
  }

  // Update type of struct entry
  assert(node->entry != nullptr);
  SymbolType structType(TY_STRUCT, node->structName, {.bodyScope = node->structScope}, usedTemplateTypes);
  structType.specifiers = node->structSpecifiers;
  node->entry->updateType(structType, false);

  // Change to struct scope
  currentScope = node->structScope;
  assert(currentScope->type == SCOPE_STRUCT);

  // Retrieve field types
  std::vector<SymbolType> fieldTypes;
  fieldTypes.reserve(node->fieldLst()->fields().size());
  for (FieldNode *field : node->fieldLst()->fields()) {
    // Visit field type
    auto fieldType = std::any_cast<SymbolType>(visit(field));
    if (fieldType.is(TY_UNRESOLVED))
      continue;

    // Check for struct with infinite size.
    // This can happen if the struct A has a field with type A
    if (fieldType.is(TY_STRUCT) && fieldType.getBodyScope() == node->structScope)
      throw SemanticError(field, STRUCT_INFINITE_SIZE, "Struct with infinite size detected");

    // Add to field types
    fieldTypes.push_back(fieldType);

    // Update type of field entry
    SymbolTableEntry *fieldEntry = currentScope->lookupStrict(field->fieldName);
    assert(fieldEntry != nullptr);
    fieldEntry->updateType(fieldType, false);

    // Check if the template type list contains this type
    if (!fieldType.isCoveredByGenericTypeList(templateTypesGeneric))
      throw SemanticError(field->dataType(), GENERIC_TYPE_NOT_IN_TEMPLATE, "Generic field type not included in struct template");
  }

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == SCOPE_GLOBAL);

  // Build struct object
  Struct spiceStruct(node->structName, node->entry, node->structScope, fieldTypes, templateTypesGeneric, interfaceTypes, node);
  StructManager::insertStruct(currentScope, spiceStruct, &node->structManifestations);
  spiceStruct.structScope = node->structScope;

  return nullptr;
}

std::any TypeChecker::visitInterfaceDefPrepare(InterfaceDefNode *node) {
  std::vector<SymbolType> usedTemplateTypes;
  std::vector<GenericType> templateTypesGeneric;

  // Retrieve interface template types
  if (node->hasTemplateTypes) {
    usedTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    templateTypesGeneric.reserve(node->templateTypeLst()->dataTypes().size());
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      HANDLE_UNRESOLVED_TYPE_PTR(templateType)
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedTemplateTypes.push_back(*genericType);
      templateTypesGeneric.push_back(*genericType);
    }
  }

  // Update type of interface entry
  SymbolType interfaceType(TY_INTERFACE, node->interfaceName, {.bodyScope = node->interfaceScope}, usedTemplateTypes);
  interfaceType.specifiers = node->interfaceSpecifiers;
  assert(node->entry != nullptr);
  node->entry->updateType(interfaceType, false);

  // Change to interface scope
  currentScope = node->interfaceScope;
  assert(currentScope->type == SCOPE_INTERFACE);

  // Visit signatures
  std::vector<Function *> signatures;
  signatures.reserve(node->signatures().size());
  for (SignatureNode *signature : node->signatures()) {
    auto method = std::any_cast<std::vector<Function *> *>(visit(signature));
    if (!method)
      return nullptr;
    signatures.insert(signatures.end(), method->begin(), method->end());
  }

  // Change to root scope
  currentScope = rootScope;
  assert(currentScope->type == SCOPE_GLOBAL);

  // Build interface object
  Interface spiceInterface(node->interfaceName, node->entry, node->interfaceScope, signatures, templateTypesGeneric, node);
  InterfaceManager::insertInterface(currentScope, spiceInterface, &node->interfaceManifestations);
  spiceInterface.interfaceScope = node->interfaceScope;

  return nullptr;
}

std::any TypeChecker::visitEnumDefPrepare(EnumDefNode *node) {
  // Update type of enum entry
  SymbolType enumType(TY_ENUM, node->enumName);
  enumType.specifiers = node->enumSpecifiers;
  assert(node->entry != nullptr);
  node->entry->updateType(enumType, false);

  // Change to enum scope
  currentScope = node->enumScope;
  assert(currentScope->type == SCOPE_ENUM);

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

  // Change to root scope
  currentScope = rootScope;
  assert(currentScope->type == SCOPE_GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitGenericTypeDefPrepare(GenericTypeDefNode *node) {
  // Retrieve type conditions
  std::vector<SymbolType> typeConditions;
  typeConditions.reserve(node->typeAltsLst()->dataTypes().size());
  for (const auto &typeAlt : node->typeAltsLst()->dataTypes()) {
    auto typeCondition = std::any_cast<SymbolType>(visit(typeAlt));
    HANDLE_UNRESOLVED_TYPE_PTR(typeCondition)
    if (!typeCondition.is(TY_DYN))
      typeConditions.push_back(typeCondition);
  }

  // Add generic type to the scope
  const GenericType genericType(node->typeName, typeConditions);
  rootScope->insertGenericType(node->typeName, genericType);

  // Check if only one type condition is set
  if (typeConditions.size() == 1 && !typeConditions.front().is(TY_DYN))
    sourceFile->compilerOutput.warnings.emplace_back(node->typeAltsLst()->codeLoc, SINGLE_GENERIC_TYPE_CONDITION,
                                                     "Generic type is locked to one type");

  return nullptr;
}

std::any TypeChecker::visitAliasDefPrepare(AliasDefNode *node) {
  assert(node->entry != nullptr && node->aliasedTypeContainerEntry != nullptr);

  // Update type of alias entry
  const SymbolType symbolType(TY_ALIAS, node->dataTypeString);
  node->entry->updateType(symbolType, false);

  // Update type of the aliased type container entry
  auto aliasedType = std::any_cast<SymbolType>(visit(node->dataType()));
  HANDLE_UNRESOLVED_TYPE_PTR(aliasedType)
  node->aliasedTypeContainerEntry->updateType(aliasedType, false);
  node->aliasedTypeContainerEntry->used = true; // The container type is always used per default

  return nullptr;
}

std::any TypeChecker::visitGlobalVarDefPrepare(GlobalVarDefNode *node) {
  // Insert variable name to symbol table
  auto globalVarType = std::any_cast<SymbolType>(visit(node->dataType()));
  HANDLE_UNRESOLVED_TYPE_PTR(globalVarType)

  if (node->constant()) { // Variable is initialized here
    SymbolType rhsType = std::any_cast<ExprResult>(visit(node->constant())).type;
    HANDLE_UNRESOLVED_TYPE_PTR(rhsType)
    if (globalVarType.is(TY_DYN)) { // Perform type inference
      globalVarType = rhsType;
    } else if (!globalVarType.matches(rhsType, false, true, true)) { // Check if types are matching
      throw SemanticError(node->constant(), OPERATOR_WRONG_DATA_TYPE,
                          "Expected " + globalVarType.getName() + ", but got " + rhsType.getName());
    }

    // Update compile time value of entry with the value of the constant
    node->entry->compileTimeValue = &node->constant()->getCompileTimeValue();
  }

  // Check if the type is still missing
  if (globalVarType.is(TY_DYN))
    throw SemanticError(node->dataType(), GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope to initialize the variable
  if (!globalVarType.isPrimitive())
    throw SemanticError(node->dataType(), GLOBAL_OF_INVALID_TYPE, "Spice does only global variables of primitive type");

  // Update type of global var entry
  assert(node->entry != nullptr);
  node->entry->updateType(globalVarType, false);

  // Check if a value is attached
  if (!node->constant() && globalVarType.isConst())
    throw SemanticError(node, GLOBAL_CONST_WITHOUT_VALUE, "You must specify a value for constant global variables");

  return nullptr;
}

std::any TypeChecker::visitExtDeclPrepare(ExtDeclNode *node) {
  // Collect argument types
  std::vector<SymbolType> argTypes;
  ParamList argList;
  if (node->hasArgs) {
    argList.reserve(node->argTypeLst()->dataTypes().size());
    for (DataTypeNode *arg : node->argTypeLst()->dataTypes()) {
      // Visit argument
      auto argType = std::any_cast<SymbolType>(visit(arg));
      HANDLE_UNRESOLVED_TYPE_PTR(argType)
      // Check if the argument type is 'dyn'
      if (argType.is(TY_DYN))
        throw SemanticError(arg, UNEXPECTED_DYN_TYPE, "Dyn data type is not allowed as arg type for external functions");
      // Save argument
      argTypes.push_back(argType);
      argList.push_back({argType, false});
    }
  }

  // Retrieve return type
  SymbolType returnType(TY_DYN);
  const bool isFunction = node->returnType();
  if (isFunction) { // External function
    returnType = std::any_cast<SymbolType>(visit(node->returnType()));
    HANDLE_UNRESOLVED_TYPE_PTR(returnType)
    // Check if return type is dyn
    if (returnType.is(TY_DYN))
      throw SemanticError(node->returnType(), UNEXPECTED_DYN_TYPE, "dyn is not allowed as return type for external functions");
  }

  // Add function to current scope
  Function spiceFunc = Function(node->extFunctionName, node->entry, SymbolType(TY_DYN), returnType, argList, {}, node, true);
  node->extFunction = FunctionManager::insertFunction(currentScope, spiceFunc, &node->extFunctionManifestations);
  node->extFunction->mangleFunctionName = false;

  // Check procedure attributes
  if (node->attrs()) {
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLE); attr != nullptr)
      node->extFunction->mangleFunctionName = attr->value()->compileTimeValue.boolValue;
    if (AttrNode *attr = node->attrs()->attrLst()->getAttrByName(AttrNode::ATTR_CORE_COMPILER_MANGLED_NAME); attr != nullptr)
      node->extFunction->predefinedMangledName = attr->value()->compileTimeValue.stringValue;
  }

  // Prepare ext function type
  SymbolType extFunctionType(isFunction ? TY_FUNCTION : TY_PROCEDURE);
  if (isFunction)
    extFunctionType.setFunctionReturnType(returnType);
  extFunctionType.setFunctionParamTypes(argTypes);

  // Set type of external function
  node->entry->updateType(extFunctionType, false);

  return nullptr;
}

std::any TypeChecker::visitImportStmtPrepare(ImportStmtNode *node) {
  // Set entry to import type
  const SymbolType importType(TY_IMPORT, node->importName);
  assert(node->entry != nullptr);
  node->entry->updateType(importType, false);

  return nullptr;
}

} // namespace spice::compiler