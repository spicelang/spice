// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/Attributes.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitMainFctDefPrepare(MainFctDefNode *node) {
  // Mark unreachable statements
  bool returnsOnAllControlPaths = true;
  node->returnsOnAllControlPaths(&returnsOnAllControlPaths);

  // Retrieve return type
  const QualType returnType(TY_INT);

  // Change to function body scope
  currentScope = node->bodyScope;

  // Set type of 'result' variable to int
  SymbolTableEntry *resultEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultEntry != nullptr);
  resultEntry->updateType(returnType, false);
  resultEntry->used = true;

  // Retrieve param types
  QualTypeList paramTypes;
  if (node->takesArgs) {
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const auto &[name, qualType, isOptional] : namedParamList)
      paramTypes.push_back(qualType);
  }

  // Prepare type of function
  const QualType functionType = QualType(TY_FUNCTION).getWithFunctionParamAndReturnTypes(returnType, paramTypes);

  // Update main function symbol type
  SymbolTableEntry *functionEntry = rootScope->lookupStrict(MAIN_FUNCTION_NAME);
  assert(functionEntry != nullptr);
  functionEntry->updateType(functionType, false);
  functionEntry->used = true;

  // Leave main function body scope
  currentScope = rootScope;

  return nullptr;
}

std::any TypeChecker::visitFctDefPrepare(FctDefNode *node) {
  // Check if name is dtor
  if (node->name->name == DTOR_FUNCTION_NAME)
    SOFT_ERROR_BOOL(node, DTOR_MUST_BE_PROCEDURE, "Destructors are not allowed to be of type function")

  // Check if all control paths in the function return
  bool doSetPredecessorsUnreachable = true;
  if (!node->returnsOnAllControlPaths(&doSetPredecessorsUnreachable))
    SOFT_ERROR_BOOL(node, MISSING_RETURN_STMT, "Not all control paths of this function have a return statement")

  // Change to function scope
  currentScope = node->scope;
  assert(currentScope->type == ScopeType::FUNC_PROC_BODY);

  // Retrieve function template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = rootScope->lookupGenericTypeStrict(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  QualType thisType(TY_DYN); // If the function is not a method, the default this type is TY_DYN
  if (node->isMethod) {
    Scope *structParentScope = node->structScope->parent;
    SymbolTableEntry *structEntry = structParentScope->lookupStrict(node->name->structName);
    assert(structEntry != nullptr);
    // Set struct to used
    structEntry->used = true;
    // Get type and ptr type
    thisType = structEntry->getQualType();
    const QualType thisPtrType = thisType.toPtr(node);
    // Collect template types of 'this' type
    for (const QualType &templateType : thisType.getTemplateTypes()) {
      const auto lambda = [&](const GenericType &genericType) { return genericType == templateType; };
      if (std::ranges::none_of(usedGenericTypes, lambda))
        usedGenericTypes.emplace_back(templateType);
      usedGenericTypes.back().used = true;
    }

    // Set type of 'this' variable
    SymbolTableEntry *thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    thisEntry->updateType(thisPtrType, false);
  }

  // Visit parameters
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    std::vector<const char *> paramNames;
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const auto &[name, qualType, isOptional] : namedParamList) {
      paramNames.push_back(name);
      HANDLE_UNRESOLVED_TYPE_PTR(qualType);
      paramTypes.push_back(qualType);
      paramList.push_back({qualType, isOptional});
      // Check if the type is present in the template for generic types
      if (!qualType.isCoveredByGenericTypeList(usedGenericTypes))
        throw SemanticError(node->paramLst, GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic param type not included in the template type list of the function");
    }
  }

  // Retrieve return type
  auto returnType = std::any_cast<QualType>(visit(node->returnType));
  HANDLE_UNRESOLVED_TYPE_PTR(returnType)
  if (returnType.is(TY_DYN))
    SOFT_ERROR_BOOL(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed")
  if (!returnType.isCoveredByGenericTypeList(usedGenericTypes))
    SOFT_ERROR_BOOL(node->returnType, GENERIC_TYPE_NOT_IN_TEMPLATE,
                    "Generic return type not included in the template type list of the function")

  // Leave function body scope
  currentScope = node->scope->parent;
  assert(currentScope->type == ScopeType::GLOBAL || currentScope->type == ScopeType::STRUCT);

  // Prepare type of function
  QualType functionType = QualType(TY_FUNCTION).getWithFunctionParamAndReturnTypes(returnType, paramTypes);
  functionType.setQualifiers(node->qualifiers);

  // Update type of function entry
  SymbolTableEntry *functionEntry = currentScope->lookupStrict(node->getSymbolTableEntryName());
  assert(functionEntry != nullptr);
  functionEntry->updateType(functionType, false);

  // Build function object
  Function spiceFunc(node->name->name, functionEntry, thisType, returnType, paramList, usedGenericTypes, node);
  spiceFunc.bodyScope = node->scope;
  FunctionManager::insert(currentScope, spiceFunc, &node->manifestations);

  // Check function attributes
  if (node->attrs) {
    const AttrLstNode *attrLst = node->attrs->attrLst;
    Function *firstManifestation = node->manifestations.front();
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLE))
      firstManifestation->mangleFunctionName = value->boolValue;
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLED_NAME)) {
      const std::string &stringValue = resourceManager.compileTimeStringValues.at(value->stringValueOffset);
      firstManifestation->predefinedMangledName = stringValue;
    }
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_TEST); value && value->boolValue) {
      // Make sure that the function has the correct signature
      if (node->hasParams)
        throw SemanticError(node->paramLst, TEST_FUNCTION_WITH_PARAMS, "Test function may not have parameters");
      if (!returnType.is(TY_BOOL))
        throw SemanticError(node->returnType, TEST_FUNCTION_WRONG_RETURN_TYPE, "Test function must return a bool");
      // Add to test function list
      firstManifestation->entry->used = true; // Avoid printing unused warnings
      firstManifestation->used = true;        // Always keep test functions, because they are called implicitly by the test main
      sourceFile->testFunctions.push_back(node->manifestations.front());
    }
  }

  // Duplicate / rename the original child scope to reflect the substantiated versions of the function
  for (size_t i = 1; i < node->manifestations.size(); i++) {
    Scope *scope = currentScope->copyChildScope(node->getScopeId(), node->manifestations.at(i)->getScopeName());
    node->manifestations.at(i)->bodyScope = scope;
  }
  currentScope->renameChildScope(node->getScopeId(), node->manifestations.front()->getScopeName());

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == ScopeType::GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitProcDefPrepare(ProcDefNode *node) {
  // Mark unreachable statements
  bool doSetPredecessorsUnreachable = true;
  node->returnsOnAllControlPaths(&doSetPredecessorsUnreachable);

  // Check if dtor and has params
  if (node->hasParams && node->name->name == DTOR_FUNCTION_NAME)
    throw SemanticError(node, DTOR_WITH_PARAMS, "It is not allowed to specify parameters for destructors");

  // Change to procedure scope
  currentScope = node->scope;
  assert(currentScope->type == ScopeType::FUNC_PROC_BODY);

  // Retrieve procedure template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      const GenericType *genericType = rootScope->lookupGenericTypeStrict(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Retrieve 'this' type
  QualType thisType(TY_DYN); // If the procedure is not a method, the default this type is TY_DYN
  if (node->isMethod) {
    Scope *structParentScope = node->structScope->parent;
    SymbolTableEntry *structEntry = structParentScope->lookupStrict(node->name->structName);
    assert(structEntry != nullptr);
    // Set struct to used
    structEntry->used = true;
    // Get type and ptr type
    thisType = structEntry->getQualType();
    const QualType thisPtrType = thisType.toPtr(node);
    // Collect template types of 'this' type
    for (const QualType &templateType : thisType.getTemplateTypes()) {
      const auto lambda = [&](const GenericType &genericType) { return genericType == templateType; };
      if (std::ranges::none_of(usedGenericTypes, lambda))
        usedGenericTypes.emplace_back(templateType);
      usedGenericTypes.back().used = true;
    }

    // Set type of 'this' variable
    SymbolTableEntry *thisEntry = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisEntry != nullptr);
    thisEntry->updateType(thisPtrType, false);
  }

  // Visit parameters
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    std::vector<const char *> paramNames;
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const auto &[name, qualType, isOptional] : namedParamList) {
      paramNames.push_back(name);
      paramTypes.push_back(qualType);
      HANDLE_UNRESOLVED_TYPE_PTR(qualType);
      paramList.push_back({qualType, isOptional});
      // Check if the type is present in the template for generic types
      if (!qualType.isCoveredByGenericTypeList(usedGenericTypes))
        throw SemanticError(node->paramLst, GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic param type not included in the template type list of the procedure");
    }
  }

  // Leave procedure body scope
  currentScope = node->scope->parent;
  assert(currentScope->type == ScopeType::GLOBAL || currentScope->type == ScopeType::STRUCT);

  // Prepare type of procedure
  QualType procedureType = QualType(TY_PROCEDURE).getWithFunctionParamAndReturnTypes(QualType(TY_DYN), paramTypes);
  procedureType.setQualifiers(node->qualifiers);

  // Update type of procedure entry
  SymbolTableEntry *procedureEntry = currentScope->lookupStrict(node->getSymbolTableEntryName());
  assert(procedureEntry != nullptr);
  procedureEntry->updateType(procedureType, false);

  // Build procedure object
  Function spiceProc(node->name->name, procedureEntry, thisType, QualType(TY_DYN), paramList, usedGenericTypes, node);
  spiceProc.bodyScope = node->scope;
  FunctionManager::insert(currentScope, spiceProc, &node->manifestations);

  // Check procedure attributes
  if (node->attrs) {
    const AttrLstNode *attrLst = node->attrs->attrLst;
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLE))
      node->manifestations.front()->mangleFunctionName = value->boolValue;
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLED_NAME)) {
      const std::string &stringValue = resourceManager.compileTimeStringValues.at(value->stringValueOffset);
      node->manifestations.front()->predefinedMangledName = stringValue;
    }
  }

  // Duplicate / rename the original child scope to reflect the substantiated versions of the procedure
  for (size_t i = 1; i < node->manifestations.size(); i++) {
    Scope *scope = currentScope->copyChildScope(node->getScopeId(), node->manifestations.at(i)->getScopeName());
    node->manifestations.at(i)->bodyScope = scope;
  }
  currentScope->renameChildScope(node->getScopeId(), node->manifestations.front()->getScopeName());

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == ScopeType::GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitStructDefPrepare(StructDefNode *node) {
  QualTypeList usedTemplateTypes;
  std::vector<GenericType> templateTypesGeneric;

  // Retrieve struct template types
  if (node->hasTemplateTypes) {
    usedTemplateTypes.reserve(node->templateTypeLst->dataTypes.size());
    templateTypesGeneric.reserve(node->templateTypeLst->dataTypes.size());
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        continue;
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      // Convert generic symbol type to generic type
      GenericType *genericType = rootScope->lookupGenericTypeStrict(templateType.getSubType());
      assert(genericType != nullptr);
      usedTemplateTypes.push_back(*genericType);
      templateTypesGeneric.push_back(*genericType);
    }
  }

  // Retrieve implemented interfaces
  QualTypeList interfaceTypes;
  if (node->hasInterfaces) {
    interfaceTypes.reserve(node->interfaceTypeLst->dataTypes.size());
    for (DataTypeNode *interfaceNode : node->interfaceTypeLst->dataTypes) {
      // Visit interface type
      auto interfaceType = std::any_cast<QualType>(visit(interfaceNode));
      if (interfaceType.is(TY_UNRESOLVED))
        continue;
      // Check if it is an interface type
      if (!interfaceType.is(TY_INTERFACE))
        throw SemanticError(interfaceNode, EXPECTED_INTERFACE_TYPE,
                            "Expected interface type, got " + interfaceType.getName(false));
      // Check for visibility
      if (interfaceType.getBodyScope()->isImportedBy(rootScope) && !interfaceType.isPublic())
        throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                            "Cannot access interface '" + interfaceType.getSubType() + "' due to its private visibility");
      // Add to interface types
      interfaceTypes.push_back(interfaceType);
      // Update the type of the entry for that interface field
      const std::string &interfaceName = interfaceNode->baseDataType->customDataType->typeNameFragments.back();
      SymbolTableEntry *interfaceEntry = node->structScope->lookupStrict("this." + interfaceName);
      assert(interfaceEntry != nullptr);
      interfaceEntry->updateType(interfaceType, false);
    }
  }

  // Update type of struct entry
  assert(node->entry != nullptr);
  const TypeChainElementData data = {.bodyScope = node->structScope};
  const Type *type = TypeRegistry::getOrInsert(TY_STRUCT, node->structName, node->typeId, data, usedTemplateTypes);
  node->entry->updateType(QualType(type, node->qualifiers), false);

  // Change to struct scope
  currentScope = node->structScope;
  assert(currentScope->type == ScopeType::STRUCT);

  // Retrieve field types
  QualTypeList fieldTypes;
  fieldTypes.reserve(node->fields.size());
  for (FieldNode *field : node->fields) {
    // Visit field type
    auto fieldType = std::any_cast<QualType>(visit(field));
    if (fieldType.is(TY_UNRESOLVED))
      sourceFile->checkForSoftErrors(); // We get into trouble if we continue without the field type -> abort

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
      throw SemanticError(field->dataType, GENERIC_TYPE_NOT_IN_TEMPLATE, "Generic field type not included in struct template");
  }

  // Change to the root scope
  currentScope = rootScope;
  assert(currentScope->type == ScopeType::GLOBAL);

  // Build struct object
  Struct spiceStruct(node->structName, node->entry, node->structScope, fieldTypes, templateTypesGeneric, interfaceTypes, node);
  StructManager::insert(currentScope, spiceStruct, &node->structManifestations);
  spiceStruct.scope = node->structScope;

  // Request RTTI runtime if the struct is polymorphic
  node->emitVTable |= node->hasInterfaces;
  if (node->attrs && node->attrs->attrLst->hasAttr(ATTR_CORE_COMPILER_EMIT_VTABLE))
    node->emitVTable |= node->attrs->attrLst->getAttrValueByName(ATTR_CORE_COMPILER_EMIT_VTABLE)->boolValue;
  if (node->emitVTable && !sourceFile->isRttiRT())
    sourceFile->requestRuntimeModule(RTTI_RT);

  return nullptr;
}

std::any TypeChecker::visitInterfaceDefPrepare(InterfaceDefNode *node) {
  QualTypeList usedTemplateTypes;
  std::vector<GenericType> templateTypesGeneric;

  // Retrieve interface template types
  if (node->hasTemplateTypes) {
    usedTemplateTypes.reserve(node->templateTypeLst->dataTypes.size());
    templateTypesGeneric.reserve(node->templateTypeLst->dataTypes.size());
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      HANDLE_UNRESOLVED_TYPE_PTR(templateType)
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC)) {
        softError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        continue;
      }
      // Convert generic symbol type to generic type
      const GenericType *genericType = rootScope->lookupGenericTypeStrict(templateType.getSubType());
      assert(genericType != nullptr);
      usedTemplateTypes.push_back(*genericType);
      templateTypesGeneric.push_back(*genericType);
    }
  }

  // Update type of interface entry
  const TypeChainElementData data = {.bodyScope = node->interfaceScope};
  const Type *type = TypeRegistry::getOrInsert(TY_INTERFACE, node->interfaceName, node->typeId, data, usedTemplateTypes);
  const QualType interfaceType(type, node->qualifiers);
  assert(node->entry != nullptr);
  node->entry->updateType(interfaceType, false);

  // Change to interface scope
  currentScope = node->interfaceScope;
  assert(currentScope->type == ScopeType::INTERFACE);

  // Visit methods
  size_t vtableIndex = 0;
  std::vector<Function *> methods;
  methods.reserve(node->signatures.size());
  for (SignatureNode *signature : node->signatures) {
    const auto method = std::any_cast<std::vector<Function *> *>(visit(signature));
    if (!method)
      return nullptr;

    // Set 'this' type
    for (Function *m : *method) {
      m->isVirtual = true; // Interface methods are always virtual
      m->vtableIndex = vtableIndex;
      m->thisType = interfaceType;
    }

    methods.insert(methods.end(), method->begin(), method->end());
    vtableIndex++;
  }

  // Change to root scope
  currentScope = rootScope;
  assert(currentScope->type == ScopeType::GLOBAL);

  // Build interface object
  Interface spiceInterface(node->interfaceName, node->entry, node->interfaceScope, methods, templateTypesGeneric, node);
  InterfaceManager::insert(currentScope, spiceInterface, &node->interfaceManifestations);
  spiceInterface.scope = node->interfaceScope;

  // Request RTTI runtime, that is always required when dealing with interfaces due to polymorphism
  if (!sourceFile->isRttiRT())
    sourceFile->requestRuntimeModule(RTTI_RT);

  return nullptr;
}

std::any TypeChecker::visitEnumDefPrepare(EnumDefNode *node) {
  // Update type of enum entry
  const TypeChainElementData data = {.bodyScope = node->enumScope};
  const Type *type = TypeRegistry::getOrInsert(TY_ENUM, node->enumName, node->typeId, data, {});
  assert(node->entry != nullptr);
  node->entry->updateType(QualType(type, node->qualifiers), false);

  // Change to enum scope
  currentScope = node->enumScope;
  assert(currentScope->type == ScopeType::ENUM);

  // Loop through all items with values
  std::vector<std::string> names;
  std::vector<uint32_t> values;
  for (const EnumItemNode *enumItem : node->itemLst->items) {
    // Save name
    names.push_back(enumItem->itemName);
    // Check for duplicate value
    if (enumItem->hasValue) {
      if (std::ranges::find(values, enumItem->itemValue) != values.end()) {
        softError(enumItem, DUPLICATE_ENUM_ITEM_VALUE, "Duplicate enum item value, please use another");
        continue;
      }
      values.push_back(enumItem->itemValue);
    }
  }

  // Loop through all items without values
  uint32_t nextValue = 0;
  const QualType intSymbolType(TY_INT);
  for (EnumItemNode *enumItem : node->itemLst->items) {
    // Update type of enum item entry
    SymbolTableEntry *itemEntry = currentScope->lookupStrict(enumItem->itemName);
    assert(itemEntry != nullptr);
    itemEntry->updateType(intSymbolType, false);
    // Fill in value if not filled yet
    if (!enumItem->hasValue) {
      while (std::ranges::find(values, nextValue) != values.end())
        nextValue++;
      enumItem->itemValue = nextValue;
      values.push_back(nextValue);
    }
  }

  // Change to root scope
  currentScope = rootScope;
  assert(currentScope->type == ScopeType::GLOBAL);

  return nullptr;
}

std::any TypeChecker::visitGenericTypeDefPrepare(GenericTypeDefNode *node) {
  // Retrieve type conditions
  QualTypeList typeConditions;
  typeConditions.reserve(node->typeAltsLst->dataTypes.size());
  for (const auto &typeAlt : node->typeAltsLst->dataTypes) {
    auto typeCondition = std::any_cast<QualType>(visit(typeAlt));
    HANDLE_UNRESOLVED_TYPE_PTR(typeCondition)
    typeConditions.push_back(typeCondition);
  }

  // Add generic type to the scope
  const GenericType genericType(node->typeName, typeConditions);
  rootScope->insertGenericType(node->typeName, genericType);

  // Check if only one type condition is set
  if (typeConditions.size() == 1 && !typeConditions.front().is(TY_DYN))
    sourceFile->compilerOutput.warnings.emplace_back(node->typeAltsLst->codeLoc, SINGLE_GENERIC_TYPE_CONDITION,
                                                     "Generic type is locked to one type");

  // Check if the list contains the dyn type, along with other types
  const bool containsDynType = std::ranges::any_of(typeConditions, [&](const QualType &type) { return type.is(TY_DYN); });
  if (containsDynType && typeConditions.size() > 1)
    sourceFile->compilerOutput.warnings.emplace_back(
        node->typeAltsLst->codeLoc, INEFFECTIVE_GENERIC_TYPE_CONDITION,
        "One or several type conditions are superfluous, because the dyn type condition is given");

  return nullptr;
}

std::any TypeChecker::visitAliasDefPrepare(AliasDefNode *node) {
  assert(node->entry != nullptr && node->aliasedTypeContainerEntry != nullptr);

  // Update type of alias entry
  const Type *type = TypeRegistry::getOrInsert(TY_ALIAS, node->aliasName, node->typeId, {}, {});
  node->entry->updateType(QualType(type, node->qualifiers), false);

  // Update type of the aliased type container entry
  const auto aliasedType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_PTR(aliasedType)
  node->aliasedTypeContainerEntry->updateType(aliasedType, false);
  node->aliasedTypeContainerEntry->used = true; // The container type is always used per default

  return nullptr;
}

std::any TypeChecker::visitGlobalVarDefPrepare(GlobalVarDefNode *node) {
  // Insert variable name to symbol table
  auto globalVarType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_PTR(globalVarType)

  if (node->constant) { // Variable is initialized here
    const QualType rhsType = std::any_cast<ExprResult>(visit(node->constant)).type;
    HANDLE_UNRESOLVED_TYPE_PTR(rhsType)
    if (globalVarType.is(TY_DYN)) { // Perform type inference
      globalVarType = rhsType;
    } else if (!globalVarType.matches(rhsType, false, true, true)) { // Check if types are matching
      SOFT_ERROR_BOOL(node->constant, OPERATOR_WRONG_DATA_TYPE,
                      "Expected " + globalVarType.getName(false) + ", but got " + rhsType.getName(false))
    }
  }

  // Check if the type is still missing
  if (globalVarType.is(TY_DYN))
    SOFT_ERROR_BOOL(node->dataType, GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type")

  // Check if we would need to insert instructions in the global scope to initialize the variable
  if (!globalVarType.isPrimitive())
    SOFT_ERROR_BOOL(node->dataType, GLOBAL_OF_INVALID_TYPE, "Spice does only support global variables of primitive type")

  // Update type of global var entry
  assert(node->entry != nullptr);
  node->entry->updateType(globalVarType, false);

  // Check if a value is attached
  if (!node->constant && globalVarType.isConst())
    SOFT_ERROR_BOOL(node, GLOBAL_CONST_WITHOUT_VALUE, "You must specify a value for constant global variables")

  return nullptr;
}

std::any TypeChecker::visitExtDeclPrepare(ExtDeclNode *node) {
  // Collect argument types
  QualTypeList argTypes;
  ParamList argList;
  if (node->hasArgs) {
    argList.reserve(node->argTypeLst->typeLst->dataTypes.size());
    for (DataTypeNode *arg : node->argTypeLst->typeLst->dataTypes) {
      // Visit argument
      auto argType = std::any_cast<QualType>(visit(arg));
      HANDLE_UNRESOLVED_TYPE_PTR(argType)
      // Check if the argument type is 'dyn'
      if (argType.is(TY_DYN)) {
        softError(arg, UNEXPECTED_DYN_TYPE, "Dyn data type is not allowed as arg type for external functions");
        continue;
      }
      // Save argument
      argTypes.push_back(argType);
      argList.push_back({argType, false});
    }
  }

  // Retrieve return type
  QualType returnType(TY_DYN);
  const bool isFunction = node->returnType;
  if (isFunction) { // External function
    returnType = std::any_cast<QualType>(visit(node->returnType));
    HANDLE_UNRESOLVED_TYPE_PTR(returnType)
    // Check if return type is dyn
    if (returnType.is(TY_DYN))
      SOFT_ERROR_BOOL(node->returnType, UNEXPECTED_DYN_TYPE, "dyn is not allowed as return type for external functions")
  }

  // Add function to current scope
  const Function spiceFunc(node->extFunctionName, node->entry, QualType(TY_DYN), returnType, argList, {}, node);
  node->extFunction = FunctionManager::insert(currentScope, spiceFunc, &node->extFunctionManifestations);
  node->extFunction->mangleFunctionName = false;
  node->extFunction->alreadyTypeChecked = true;
  node->extFunction->isVararg = node->argTypeLst && node->argTypeLst->hasEllipsis;

  // Check procedure attributes
  if (node->attrs) {
    const AttrLstNode *attrLst = node->attrs->attrLst;
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLE))
      node->extFunction->mangleFunctionName = value->boolValue;
    if (const CompileTimeValue *value = attrLst->getAttrValueByName(ATTR_CORE_COMPILER_MANGLED_NAME)) {
      const std::string &stringValue = resourceManager.compileTimeStringValues.at(value->stringValueOffset);
      node->extFunction->predefinedMangledName = stringValue;
    }
  }

  // Prepare ext function type
  const SuperType superType = isFunction ? TY_FUNCTION : TY_PROCEDURE;
  const QualType extFunctionType = QualType(superType).getWithFunctionParamAndReturnTypes(returnType, argTypes);

  // Set type of external function
  node->entry->updateType(extFunctionType, false);

  // Rename the original child scope to reflect the substantiated versions of the external function
  currentScope->renameChildScope(node->getScopeId(), spiceFunc.getScopeName());

  return nullptr;
}

std::any TypeChecker::visitImportDefPrepare(ImportDefNode *node) {
  // Set entry to import type
  const QualType importType(TY_IMPORT, node->importName);
  assert(node->entry != nullptr);
  node->entry->updateType(importType, false);

  return nullptr;
}

} // namespace spice::compiler