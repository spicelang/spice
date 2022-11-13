// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>

TCResult TypeChecker::visitMainFctDefLookup(MainFctDefNode *node) {
  std::string mainSignature = std::string(MAIN_FUNCTION_NAME) + "()";

  // Check if the function is already defined
  if (rootScope->lookup(mainSignature))
    throw SemanticError(node, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert function name into the root symbol table
  SymbolTableEntry *mainFctEntry = rootScope->insert(mainSignature, SymbolSpecifiers::of(TY_FUNCTION), node);
  mainFctEntry->isUsed = true;

  // Create the function scope
  node->fctScope = currentScope = rootScope->createChildScope(mainSignature, SCOPE_FUNC_PROC_BODY);

  // Declare variable for the return value in the function scope
  SymbolType returnType = SymbolType(TY_INT);
  SymbolTableEntry *returnVarEntry = node->fctScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), node);
  returnVarEntry->isUsed = true;

  // Visit arguments in new scope
  if (node->hasArgs)
    visit(node->paramLst());

  // Return to root scope
  currentScope = rootScope;

  return {};
}

TCResult TypeChecker::visitFctDefLookup(FctDefNode *node) {
  // Check if name is dtor
  if (node->functionName == "dtor")
    throw SemanticError(node, DTOR_MUST_BE_PROCEDURE, "Destructors are not allowed to be of type function");

  // Check if this is a global function or a method
  if (node->isMethod) {
    // Change to the struct scope
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!node->structScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create a new scope
  node->fctScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

  // Get 'this' type
  std::vector<GenericType> templateTypes;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    SymbolTableEntry *structEntry = node->structScope->lookup(node->structName);
    assert(structEntry != nullptr);
    thisType = structEntry->type;
    thisPtrType = thisType.toPointer(node);
    // Collect template types from this type
    for (const auto &templateType : thisType.getTemplateTypes())
      templateTypes.emplace_back(templateType);
  }

  // Get template types
  if (node->hasTemplateTypes) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
      auto templateType = visit(dataType);
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      GenericType *genericType = node->fctScope->symbolTable.lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      templateTypes.push_back(*genericType);
    }
  }

  // Visit parameters in new scope
  std::vector<std::string> paramNames;
  ParamList paramTypes;
  if (node->hasParams) {
    auto namedParamList = any_cast<NamedParamList>(visit(node->paramLst()));
    for (const auto &param : namedParamList) {
      // Check if the type is present in the template for generic types
      if (param.type.is(TY_GENERIC)) {
        if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == param.type; }))
          throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                              "Generic arg type not included in function template");
      }

      paramNames.push_back(param.name);
      paramTypes.push_back({param.type, param.isOptional});
    }
  }

  // Declare 'this' variable in new scope
  if (node->isMethod) {
    SymbolSpecifiers thisTypeSpecifiers(thisPtrType);
    thisTypeSpecifiers.setConst(true);
    node->fctScope->insert(THIS_VARIABLE_NAME, thisPtrType, thisTypeSpecifiers, node);
  }

  // Declare variable for the return value in the function scope
  auto returnType = visit(node->returnType());
  if (returnType.is(TY_DYN))
    throw SemanticError(node, UNEXPECTED_DYN_TYPE_SA, "Dyn return types are not allowed");
  node->fctScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), node);

  // Return to old scope
  currentScope = node->fctScope->parent;

  // Build function specifiers
  auto fctSymbolSpecifiers = SymbolSpecifiers(TY_FUNCTION);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        fctSymbolSpecifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        fctSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
    }
  }

  // Insert function into the symbol table
  Function spiceFunc(node->functionName, fctSymbolSpecifiers, thisType, returnType, paramTypes, templateTypes, node);
  currentScope->insertFunction(spiceFunc);

  // Rename / duplicate the original child block to reflect the substantiated versions of the function
  std::vector<Function> substantiatedFunctions = spiceFunc.substantiateOptionalParams();
  currentScope->renameChildScope(node->getScopeId(), substantiatedFunctions.front().getSignature());
  for (int i = 1; i < substantiatedFunctions.size(); i++)
    currentScope->copyChildScope(substantiatedFunctions.front().getSignature(), substantiatedFunctions[i].getSignature());

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return {};
}

TCResult TypeChecker::visitProcDefLookup(ProcDefNode *node) {
  if (node->hasParams && node->procedureName == "dtor")
    throw SemanticError(node, DTOR_WITH_PARAMS, "It is not allowed to specify parameters for destructors");

  // Change to the struct scope
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create a new scope
  node->procScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

  // Get 'this' type
  std::vector<GenericType> templateTypes;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType thisPtrType = thisType;
  if (node->isMethod) {
    SymbolTableEntry *structEntry = node->structScope->lookup(node->structName);
    thisType = structEntry->type;
    thisPtrType = thisType.toPointer(node);
    // Collect template types from this type
    for (const auto &templateType : thisType.getTemplateTypes())
      templateTypes.emplace_back(templateType);
  }

  // Get template types
  if (node->isGeneric) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
      auto templateType = visit(dataType);
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      GenericType *genericType = node->procScope->symbolTable.lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      templateTypes.push_back(*genericType);
    }
  }

  // Visit parameters in new scope
  std::vector<std::string> paramNames;
  ParamList paramTypes;
  if (node->hasParams) {
    auto namedParamList = any_cast<NamedParamList>(visit(node->paramLst()));
    for (const auto &param : namedParamList) {
      // Check if the type is present in the template for generic types
      if (param.type.is(TY_GENERIC)) {
        if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == param.type; }))
          throw SemanticError(node->paramLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                              "Generic arg type not included in procedure template");
      }

      paramNames.push_back(param.name);
      paramTypes.push_back({param.type, param.isOptional});
    }
  }

  // Declare 'this' variable in new scope
  if (node->isMethod) {
    auto thisSymbolSpecifiers = SymbolSpecifiers(thisPtrType);
    thisSymbolSpecifiers.setConst(true);
    node->procScope->insert(THIS_VARIABLE_NAME, thisPtrType, thisSymbolSpecifiers, node);
  }

  // Return to old scope
  currentScope = currentScope->parent;

  // Build procedure specifiers
  auto procSymbolSpecifiers = SymbolSpecifiers(TY_FUNCTION);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        procSymbolSpecifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        procSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a procedure definition");
    }
  }

  // Insert function into the symbol table
  Function spiceProc(node->procedureName, procSymbolSpecifiers, thisType, SymbolType(TY_DYN), paramTypes, templateTypes, node);
  currentScope->insertFunction(spiceProc);

  // Rename / duplicate the original child block to reflect the substantiated versions of the function
  std::vector<Function> substantiatedProcedures = spiceProc.substantiateOptionalParams();
  currentScope->renameChildScope(node->getScopeId(), substantiatedProcedures.front().getSignature());
  for (int i = 1; i < substantiatedProcedures.size(); i++)
    currentScope->copyChildScope(substantiatedProcedures.front().getSignature(), substantiatedProcedures[i].getSignature());

  // Leave the struct scope
  if (node->isMethod)
    currentScope = currentScope->parent;

  return {};
}

TCResult TypeChecker::visitStructDefLookup(StructDefNode *node) {
  // Check if struct already exists
  if (rootScope->lookup(node->structName))
    throw SemanticError(node, STRUCT_DECLARED_TWICE, "Duplicate struct '" + node->structName + "'");

  // Get template types
  std::vector<GenericType> genericTemplateTypes;
  std::vector<SymbolType> templateTypes;
  if (node->isGeneric) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
      auto templateType = visit(dataType);
      if (!templateType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      GenericType *genericType = currentScope->symbolTable.lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      genericTemplateTypes.push_back(*genericType);
      templateTypes.push_back(*genericType);
    }
  }

  // Get implemented interfaces
  std::vector<SymbolType> interfaceTypes;
  if (node->hasInterfaces) {
    for (const auto &dataType : node->interfaceTypeLst()->dataTypes()) {
      auto interfaceType = visit(dataType);
      if (!interfaceType.is(TY_INTERFACE))
        throw SemanticError(dataType, EXPECTED_INTERFACE_TYPE, "Expected interface type, got " + interfaceType.getName());
      interfaceTypes.push_back(interfaceType);
    }
  }

  // Build struct specifiers
  SymbolType symbolType = SymbolType(TY_STRUCT, node->structName, {}, templateTypes);
  auto structSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        structSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a struct definition");
    }
  }

  // Add the struct to the symbol table
  currentScope->insert(node->structName, symbolType, structSymbolSpecifiers, node);

  // Create scope for struct
  Scope *structScope = currentScope = currentScope->createChildScope(STRUCT_SCOPE_PREFIX + node->structName, SCOPE_STRUCT);

  // Insert a field for each field list entry
  std::vector<SymbolType> fieldTypes;
  for (const auto &field : node->fields()) {
    auto fieldType = visit(field->dataType());

    if (fieldType.isBaseType(TY_GENERIC)) { // Check if the type is present in the template for generic types
      if (std::none_of(genericTemplateTypes.begin(), genericTemplateTypes.end(),
                       [&](const GenericType &t) { return t == fieldType.getBaseType(); }))
        throw SemanticError(field->dataType(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                            "Generic field type not included in struct template");
    }

    auto fieldSymbolSpecifiers = SymbolSpecifiers(symbolType);
    if (SpecifierLstNode *specifierLst = field->specifierLst(); specifierLst) {
      for (const auto &specifier : specifierLst->specifiers()) {
        if (specifier->type == SpecifierNode::TY_CONST)
          throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Struct fields cannot have the const specifier attached");
        else if (specifier->type == SpecifierNode::TY_SIGNED)
          fieldSymbolSpecifiers.setSigned(true);
        else if (specifier->type == SpecifierNode::TY_UNSIGNED)
          fieldSymbolSpecifiers.setSigned(false);
        else if (specifier->type == SpecifierNode::TY_PUBLIC)
          fieldSymbolSpecifiers.setPublic(true);
        else
          throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a field definition");
      }
    }

    // Add the field to the symbol table
    currentScope->insert(field->name, fieldType, fieldSymbolSpecifiers, DECLARED, field);

    fieldTypes.push_back(fieldType);
  }

  // Return to the old scope
  currentScope = currentScope->parent;

  // Add struct
  Struct s(node->structName, structSymbolSpecifiers, fieldTypes, genericTemplateTypes, interfaceTypes, node);
  node->spiceStruct = currentScope->insertStruct(s);
  s.structScope = structScope;

  return {};
}