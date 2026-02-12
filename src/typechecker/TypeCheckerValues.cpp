// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <global/GlobalResourceManager.h>
#include <model/GenericType.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/ScopeHandle.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/MacroDefs.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

std::any TypeChecker::visitValue(ValueNode *node) {
  // Function call
  if (node->fctCall)
    return visit(node->fctCall);

  // Array initialization
  if (node->arrayInitialization)
    return visit(node->arrayInitialization);

  // Struct instantiation
  if (node->structInstantiation)
    return visit(node->structInstantiation);

  // Lambda function
  if (node->lambdaFunc)
    return visit(node->lambdaFunc);

  // Lambda procedure
  if (node->lambdaProc)
    return visit(node->lambdaProc);

  // Lambda expression
  if (node->lambdaExpr)
    return visit(node->lambdaExpr);

  // Typed nil
  if (node->isNil) {
    const auto nilType = std::any_cast<QualType>(visit(node->nilType));
    HANDLE_UNRESOLVED_TYPE_ER(nilType)
    if (nilType.is(TY_DYN))
      SOFT_ERROR_ER(node->nilType, UNEXPECTED_DYN_TYPE, "Nil must have an explicit type")
    return ExprResult{node->setEvaluatedSymbolType(nilType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "Value fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitConstant(ConstantNode *node) {
  SuperType superType;
  switch (node->type) {
  case ConstantNode::PrimitiveValueType::TYPE_DOUBLE:
    superType = TY_DOUBLE;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_INT:
    superType = TY_INT;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_SHORT:
    superType = TY_SHORT;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_LONG:
    superType = TY_LONG;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_CHAR:
    superType = TY_CHAR;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_STRING:
    superType = TY_STRING;
    break;
  case ConstantNode::PrimitiveValueType::TYPE_BOOL:
    superType = TY_BOOL;
    break;
  default:                                                          // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Constant fall-through"); // GCOV_EXCL_LINE
  }
  return ExprResult{node->setEvaluatedSymbolType(QualType(superType), manIdx)};
}

std::any TypeChecker::visitFctCall(FctCallNode *node) {
  FctCallNode::FctCallData &data = node->data.at(manIdx);
  auto &[callType, isImported, templateTypes, thisType, args, callee, calleeParentScope] = data;

  // Retrieve arg types
  args.clear();
  if (node->hasArgs) {
    args.reserve(node->argLst->args.size());
    for (AssignExprNode *arg : node->argLst->args) {
      // Visit argument
      const auto argResult = std::any_cast<ExprResult>(visit(arg));
      HANDLE_UNRESOLVED_TYPE_ER(argResult.type)
      assert(!argResult.type.hasAnyGenericParts());
      // Save arg type to arg types list
      args.emplace_back(argResult.type, argResult.isTemporary());
    }
  }

  // Retrieve entry of the first fragment
  const std::string &firstFrag = node->functionNameFragments.front();
  SymbolTableEntry *firstFragEntry = currentScope->lookup(firstFrag);
  if (firstFragEntry) {
    // Check if we have seen a 'this.' prefix, because the generator needs that
    if (firstFragEntry->scope->type == ScopeType::STRUCT && firstFrag != THIS_VARIABLE_NAME)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE,
                    "The symbol '" + firstFrag + "' could not be found. Missing 'this.' prefix?")

    firstFragEntry->used = true;
    // Decide of which type the function call is
    const QualType &baseType = firstFragEntry->getQualType().getBase();
    HANDLE_UNRESOLVED_TYPE_ER(baseType)
    if (baseType.isOneOf({TY_STRUCT, TY_INTERFACE})) {
      if (firstFragEntry->scope->type == ScopeType::GLOBAL)
        callType = FctCallNode::FctCallType::TYPE_CTOR;
      else
        callType = FctCallNode::FctCallType::TYPE_METHOD;
    } else if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && firstFragEntry->scope->type != ScopeType::GLOBAL) {
      callType = FctCallNode::FctCallType::TYPE_FCT_PTR;
    }
  }

  // Get struct name. Retrieve it from alias if required
  const auto &[structEntry, isAlias] = rootScope->symbolTable.lookupWithAliasResolution(node->fqFunctionName);
  const std::string &fqFunctionName = isAlias ? structEntry->getQualType().getSubType() : node->fqFunctionName;

  // Get the concrete template types
  templateTypes.clear();
  if (isAlias) {
    // Retrieve concrete template types from type alias
    templateTypes = structEntry->getQualType().getTemplateTypes();
    // Check if the aliased type specified template types and the struct instantiation does
    if (!templateTypes.empty() && node->hasTemplateTypes)
      SOFT_ERROR_ER(node->templateTypeLst, ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list")
  }

  // Get concrete template types
  if (node->hasTemplateTypes) {
    for (DataTypeNode *templateTypeNode : node->templateTypeLst->dataTypes) {
      auto templateType = std::any_cast<QualType>(visit(templateTypeNode));
      assert(!templateType.isOneOf({TY_DYN, TY_INVALID}));

      // Abort if the type is unresolved
      if (templateType.is(TY_UNRESOLVED))
        HANDLE_UNRESOLVED_TYPE_ER(templateType)

      // Check if the given type is generic
      if (templateType.is(TY_GENERIC))
        SOFT_ERROR_ER(templateTypeNode, EXPECTED_NON_GENERIC_TYPE, "You must specify a concrete type here")

      templateTypes.push_back(templateType);
    }
  }

  // Check if this is a method call or a normal function call
  if (data.isMethodCall()) {
    // This is a method call
    thisType = firstFragEntry->getQualType();
    Scope *structBodyScope = thisType.getBase().getBodyScope();
    assert(structBodyScope != nullptr);
    if (!visitMethodCall(node, structBodyScope)) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
    assert(calleeParentScope != nullptr);
  } else if (data.isFctPtrCall()) {
    // This is a function pointer call
    const QualType &functionType = firstFragEntry->getQualType().getBase();
    assert(functionType.isOneOf({TY_FUNCTION, TY_PROCEDURE}));
    if (!visitFctPtrCall(node, functionType)) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
  } else {
    // This is an ordinary function call
    assert(data.isOrdinaryCall() || data.isCtorCall());
    if (!visitOrdinaryFctCall(node, fqFunctionName)) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
    assert(calleeParentScope != nullptr);

    // If the call is no ordinary call, it must be a constructor, which takes a struct as this type.
    assert(data.isOrdinaryCall() || data.thisType.is(TY_STRUCT));
  }

  if (!data.isFctPtrCall()) {
    // Check if we were able to find a function
    if (!callee) {
      // Build error message
      const std::string functionName = data.isCtorCall() ? CTOR_FUNCTION_NAME : node->functionNameFragments.back();
      ParamList errArgTypes;
      errArgTypes.reserve(args.size());
      for (const auto &type : args | std::views::keys)
        errArgTypes.push_back({type, false});
      const std::string signature = Function::getSignature(functionName, thisType, QualType(TY_DYN), errArgTypes, {}, false);
      // Throw error
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure '" + signature + "' could not be found")
    }

    // Check if we need to request a re-visit, because the function body was not type-checked yet
    requestRevisitIfRequired(callee);

    // Get function entry from function object
    SymbolTableEntry *functionEntry = callee->entry;

    // Check if the called function has sufficient visibility
    isImported = calleeParentScope->isImportedBy(rootScope);
    if (isImported && !functionEntry->getQualType().isPublic()) {
      const QualType functionEntryType = functionEntry->getQualType();
      const std::string signature = callee->getSignature();
      if (functionEntryType.is(TY_FUNCTION))
        SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Function '" + signature + "' has insufficient visibility")
      else
        SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Procedure '" + signature + "' has insufficient visibility")
    }
  }

  // Generate arg infos
  if (node->hasArgs) {
    QualTypeList paramTypes;
    if (data.isFctPtrCall()) {
      const QualType &functionType = firstFragEntry->getQualType().getBase();
      paramTypes = functionType.getFunctionParamTypes();
    } else {
      assert(callee != nullptr);
      paramTypes = callee->getParamTypes();
    }

    node->argLst->argInfos.clear();
    for (size_t argIdx = 0; argIdx < args.size(); argIdx++) {
      const QualType &expectedType = paramTypes.at(argIdx);
      const auto &[actualType, _] = args.at(argIdx);

      Function *copyCtor = nullptr;
      if (expectedType.is(TY_STRUCT) && actualType.is(TY_STRUCT) && !actualType.isTriviallyCopyable(node)) {
        copyCtor = matchCopyCtor(actualType, node);
        // Insert anonymous symbol to track the dtor call of the copy
        AssignExprNode *argNode = node->argLst->args.at(argIdx);
        currentScope->symbolTable.insertAnonymous(actualType, argNode, SIZE_MAX);
      }

      node->argLst->argInfos.push_back(ArgLstNode::ArgInfo{copyCtor});
    }
    assert(node->argLst->argInfos.size() == node->argLst->args.size());
  }

  // Retrieve return type
  const bool isFct = data.isFctPtrCall() ? firstFragEntry->getQualType().getBase().is(TY_FUNCTION) : callee->isFunction();
  QualType returnType;
  if (data.isFctPtrCall()) {
    returnType = isFct ? firstFragEntry->getQualType().getBase().getFunctionReturnType() : QualType(TY_BOOL);
  } else if (data.isCtorCall()) {
    returnType = thisType;
  } else if (callee->isProcedure()) {
    returnType = QualType(TY_DYN);
  } else {
    returnType = callee->returnType;
  }
  const QualType returnBaseType = returnType.getBase();

  // Make sure this source file knows about the return type
  if (returnBaseType.is(TY_STRUCT))
    returnType = mapImportedScopeTypeToLocalType(returnBaseType.getBodyScope(), returnType);

  // Add anonymous symbol to keep track of dtor call, if non-trivially destructible
  SymbolTableEntry *anonymousSymbol = nullptr;
  if (returnType.is(TY_STRUCT) && !returnType.isTriviallyDestructible(node))
    anonymousSymbol = currentScope->symbolTable.insertAnonymous(returnType, node);

  // Remove public qualifier to not have public local variables
  returnType.getQualifiers().isPublic = false;

  // Check if the return value gets discarded
  if (isFct && !node->hasReturnValueReceiver()) {
    // Check if we want to ignore the discarded return value
    bool ignoreUnusedReturnValue = false;
    if (!data.isFctPtrCall()) {
      assert(callee != nullptr);
      auto fctDef = dynamic_cast<const FctDefNode *>(callee->declNode);
      ignoreUnusedReturnValue = fctDef && fctDef->attrs && fctDef->attrs->attrLst->hasAttr(ATTR_IGNORE_UNUSED_RETURN_VALUE);
    }

    if (!ignoreUnusedReturnValue)
      warnings.emplace_back(node->codeLoc, UNUSED_RETURN_VALUE, "The return value of the function call is unused");
  }

  return ExprResult{node->setEvaluatedSymbolType(returnType, manIdx), anonymousSymbol};
}

bool TypeChecker::visitOrdinaryFctCall(FctCallNode *node, std::string fqFunctionName) const {
  FctCallNode::FctCallData &data = node->data.at(manIdx);
  auto &[callType, isImported, templateTypes, thisType, args, callee, calleeParentScope] = data;

  // Check if this is a well-known ctor/fct call
  if (node->functionNameFragments.size() == 1) {
    ensureLoadedRuntimeForTypeName(fqFunctionName);
    ensureLoadedRuntimeForFunctionName(fqFunctionName);
  }

  // Check if the type is generic (possible in case of ctor call)
  const QualType *genericType = rootScope->lookupGenericTypeStrict(fqFunctionName);
  if (genericType && typeMapping.contains(fqFunctionName)) {
    const QualType &replacementType = typeMapping.at(fqFunctionName);
    if (replacementType.is(TY_STRUCT))
      fqFunctionName = replacementType.getSubType();
  }

  // Check if the exported name registry contains that function name
  const NameRegistryEntry *functionRegistryEntry = sourceFile->getNameRegistryEntry(fqFunctionName);
  if (!functionRegistryEntry) {
    const std::string msg = "Function/procedure/struct '" + node->functionNameFragments.back() + "' could not be found";
    SOFT_ERROR_BOOL(node, REFERENCED_UNDEFINED_FUNCTION, msg)
  }
  const SymbolTableEntry *functionEntry = functionRegistryEntry->targetEntry;
  calleeParentScope = functionRegistryEntry->targetScope;

  // Check if the target symbol is a struct -> this must be a constructor call
  std::string functionName = node->functionNameFragments.back();
  if (functionEntry != nullptr && functionEntry->getQualType().is(TY_STRUCT)) {
    callType = FctCallNode::FctCallType::TYPE_CTOR;
    functionName = CTOR_FUNCTION_NAME;

    const NameRegistryEntry *structRegistryEntry = functionRegistryEntry;
    const SymbolTableEntry *structEntry = functionEntry;

    // Substantiate potentially generic this struct
    const Struct *thisStruct = structEntry->getQualType().getStruct(node, templateTypes);
    if (!thisStruct) {
      const std::string signature = Struct::getSignature(structRegistryEntry->targetEntry->name, templateTypes);
      const std::string errorMsg = "Could not find struct candidate for struct '" + signature + "'. Do the template types match?";
      SOFT_ERROR_BOOL(node, UNKNOWN_DATATYPE, errorMsg)
    }

    // Set the 'this' type of the function to the struct type
    thisType = structEntry->getQualType().getWithBodyScope(thisStruct->scope);
    calleeParentScope = thisStruct->scope;
  }

  // Attach the concrete template types to the 'this' type
  if (!thisType.is(TY_DYN) && !templateTypes.empty())
    thisType = thisType.getWithTemplateTypes(templateTypes);

  // Map local arg types to imported types
  for (QualType &argType : args | std::views::keys)
    argType = mapLocalTypeToImportedScopeType(calleeParentScope, argType);

  // Map local template types to imported types
  for (QualType &templateType : templateTypes)
    templateType = mapLocalTypeToImportedScopeType(calleeParentScope, templateType);

  // Retrieve function object
  Scope *matchScope = calleeParentScope;
  callee = FunctionManager::match(matchScope, functionName, data.thisType, data.args, templateTypes, false, node);

  return true;
}

bool TypeChecker::visitFctPtrCall(const FctCallNode *node, const QualType &functionType) const {
  const FctCallNode::FctCallData &data = node->data.at(manIdx);
  const auto &[callType, isImported, templateTypes, thisType, args, callee, calleeParentScope] = data;

  // Check if the given argument types match the type
  const QualTypeList expectedArgTypes = functionType.getFunctionParamTypes();
  if (args.size() != expectedArgTypes.size())
    SOFT_ERROR_BOOL(node, REFERENCED_UNDEFINED_FUNCTION, "Expected and actual number of arguments do not match")

  // Create resolver function, that always returns a nullptr
  TypeMatcher::ResolverFct resolverFct = [](const std::string &) { return nullptr; };

  for (size_t i = 0; i < args.size(); i++) {
    const QualType &actualType = args.at(i).first;
    const QualType &expectedType = expectedArgTypes.at(i);
    if (TypeMapping tm; !TypeMatcher::matchRequestedToCandidateType(expectedType, actualType, tm, resolverFct, false))
      SOFT_ERROR_BOOL(node->argLst->args.at(i), REFERENCED_UNDEFINED_FUNCTION,
                      "Expected " + expectedType.getName(false) + " but got " + actualType.getName(false))
  }
  return true;
}

bool TypeChecker::visitMethodCall(FctCallNode *node, Scope *structScope) const {
  FctCallNode::FctCallData &data = node->data.at(manIdx);
  auto &[callType, isImported, templateTypes, thisType, args, callee, calleeParentScope] = data;

  // Traverse through structs - the first fragment is already looked up and the last one is the method name
  for (size_t i = 1; i < node->functionNameFragments.size() - 1; i++) {
    const std::string &identifier = node->functionNameFragments.at(i);

    // Retrieve field entry
    SymbolTableEntry *fieldEntry = structScope->lookupStrict(identifier);
    if (!fieldEntry) {
      std::stringstream errorMsg;
      errorMsg << "The type '";
      errorMsg << thisType.getBase().getName(false, true);
      errorMsg << "' does not have a member with the name '" << identifier << "'";
      SOFT_ERROR_BOOL(node, ACCESS_TO_NON_EXISTING_MEMBER, errorMsg.str())
    }
    if (!fieldEntry->getQualType().getBase().isOneOf({TY_STRUCT, TY_INTERFACE}))
      SOFT_ERROR_BOOL(node, INVALID_MEMBER_ACCESS,
                      "Cannot call a method on '" + identifier + "', since it is no struct or interface")
    fieldEntry->used = true;

    // Get struct type and scope
    thisType = fieldEntry->getQualType();
    structScope = thisType.getBase().getBodyScope();
    assert(structScope != nullptr);
  }

  if (thisType.is(TY_INTERFACE))
    SOFT_ERROR_BOOL(node, INVALID_MEMBER_ACCESS, "Cannot call a method on an interface")

  // Map local arg types to imported types
  Scope *matchScope = calleeParentScope = structScope;
  for (QualType &argType : args | std::views::keys)
    argType = mapLocalTypeToImportedScopeType(calleeParentScope, argType);

  // Map local template types to imported types
  for (QualType &templateType : templateTypes)
    templateType = mapLocalTypeToImportedScopeType(calleeParentScope, templateType);

  // 'this' type
  thisType = thisType.autoDeReference();
  thisType = mapLocalTypeToImportedScopeType(calleeParentScope, thisType);

  // Retrieve function object
  const std::string &functionName = node->functionNameFragments.back();
  callee = FunctionManager::match(matchScope, functionName, thisType, args, templateTypes, false, node);

  return true;
}

std::any TypeChecker::visitArrayInitialization(ArrayInitializationNode *node) {
  if (!node->itemLst || node->itemLst->args.empty())
    SOFT_ERROR_ER(node, ARRAY_SIZE_INVALID, "Array initializers must at least contain one value");
  node->actualSize = node->itemLst->args.size();

  QualType actualItemType(TY_DYN);
  // Check if all values have the same type
  for (AssignExprNode *arg : node->itemLst->args) {
    const QualType itemType = std::any_cast<ExprResult>(visit(arg)).type;
    HANDLE_UNRESOLVED_TYPE_ER(itemType)
    if (actualItemType.is(TY_DYN)) // Perform type inference
      actualItemType = itemType;
    else if (itemType != actualItemType) // Check if types are matching
      SOFT_ERROR_ER(arg, ARRAY_ITEM_TYPE_NOT_MATCHING,
                    "All provided values have to be of the same data type. You provided " + actualItemType.getName(false) +
                        " and " + itemType.getName(false))
  }
  assert(!actualItemType.is(TY_DYN));

  const QualType arrayType = actualItemType.toArr(node, node->actualSize, true);
  return ExprResult{node->setEvaluatedSymbolType(arrayType, manIdx)};
}

std::any TypeChecker::visitStructInstantiation(StructInstantiationNode *node) {
  // Retrieve struct name
  const auto [aliasedEntry, isAlias] = rootScope->symbolTable.lookupWithAliasResolution(node->fqStructName);
  const std::string &structName = isAlias ? aliasedEntry->getQualType().getSubType() : node->fqStructName;

  // Retrieve struct
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(structName);
  if (!registryEntry)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + structName + "'")
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *structEntry = registryEntry->targetEntry;

  // Check visibility
  if (!structEntry->getQualType().isPublic() && structEntry->scope->isImportedBy(currentScope))
    SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Struct '" + structName + "' has insufficient visibility")

  // Get struct type
  QualType structType = structEntry->getQualType();

  // Get the concrete template types
  QualTypeList concreteTemplateTypes;
  if (isAlias) {
    // Retrieve concrete template types from type alias
    concreteTemplateTypes = aliasedEntry->getQualType().getTemplateTypes();
    // Check if the aliased type specified template types and the struct instantiation does
    if (!concreteTemplateTypes.empty() && node->templateTypeLst)
      SOFT_ERROR_ER(node->templateTypeLst, ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list")
  }

  if (node->templateTypeLst) {
    concreteTemplateTypes.reserve(node->templateTypeLst->dataTypes.size());
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      auto concreteType = std::any_cast<QualType>(visit(dataType));
      HANDLE_UNRESOLVED_TYPE_ER(concreteType)
      // Check if generic type
      if (concreteType.is(TY_GENERIC))
        SOFT_ERROR_ER(dataType, EXPECTED_NON_GENERIC_TYPE, "Struct instantiations may only take concrete template types")
      concreteTemplateTypes.push_back(concreteType);
    }
  }

  // Get the struct instance
  Struct *spiceStruct = node->instantiatedStructs.at(manIdx) = structType.getStructAndAdjustType(node, concreteTemplateTypes);
  if (!spiceStruct)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT,
                  "Struct '" + Struct::getSignature(structName, concreteTemplateTypes) + "' could not be found")

  // Struct instantiation for an inheriting struct is forbidden, because the vtable needs to be initialized and this is done in
  // the ctor of the struct, which is never called in case of struct instantiation
  if (!spiceStruct->interfaceTypes.empty())
    SOFT_ERROR_ER(node, INVALID_STRUCT_INSTANTIATION, "Struct instantiations for inheriting structs are forbidden")

  // Check if the number of fields matches
  if (node->fieldLst) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst->args.size())
      SOFT_ERROR_ER(node->fieldLst, NUMBER_OF_FIELDS_NOT_MATCHING,
                    "You've passed too less/many field values. Pass either none or all of them")

    // Check if the field types are matching
    const size_t fieldCount = spiceStruct->fieldTypes.size();
    const size_t explicitFieldsStartIdx = spiceStruct->scope->getFieldCount() - fieldCount;
    for (size_t i = 0; i < node->fieldLst->args.size(); i++) {
      // Get actual type
      AssignExprNode *assignExpr = node->fieldLst->args.at(i);
      auto fieldResult = std::any_cast<ExprResult>(visit(assignExpr));
      HANDLE_UNRESOLVED_TYPE_ER(fieldResult.type)
      // Get expected type
      SymbolTableEntry *expectedField = spiceStruct->scope->lookupField(explicitFieldsStartIdx + i);
      assert(expectedField != nullptr);
      const ExprResult expected = {expectedField->getQualType(), expectedField};
      const bool rhsIsImmediate = assignExpr->hasCompileTimeValue();

      // Check if actual type matches expected type
      (void)opRuleManager.getFieldAssignResultType(assignExpr, expected, fieldResult, rhsIsImmediate, true);

      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      if (fieldResult.entry != nullptr && fieldResult.entry->anonymous) {
        currentScope->symbolTable.deleteAnonymous(fieldResult.entry->name);
        fieldResult.entry = nullptr;
      }
    }
  } else {
    if (std::ranges::any_of(spiceStruct->fieldTypes, [](const QualType &fieldType) { return fieldType.isRef(); }))
      SOFT_ERROR_ER(node, REFERENCE_WITHOUT_INITIALIZER,
                    "The struct takes at least one reference field. You need to instantiate it with all fields.")
  }

  // Update type of struct entry
  structEntry->updateType(structType, true);

  // Add anonymous symbol to keep track of dtor call, if non-trivially destructible
  SymbolTableEntry *anonymousEntry = nullptr;
  if (!structType.isTriviallyDestructible(node))
    anonymousEntry = currentScope->symbolTable.insertAnonymous(structType, node);

  // Remove public qualifier to not have public local variables
  structType.getQualifiers().isPublic = false;

  return ExprResult{node->setEvaluatedSymbolType(structType, manIdx), anonymousEntry};
}

std::any TypeChecker::visitLambdaFunc(LambdaFuncNode *node) {
  // Check if all control paths in the lambda body return
  bool returnsOnAllControlPaths = true;
  if (!node->returnsOnAllControlPaths(&returnsOnAllControlPaths))
    SOFT_ERROR_ER(node, MISSING_RETURN_STMT, "Not all control paths of this lambda function have a return statement")

  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit return type
  const auto returnType = std::any_cast<QualType>(visit(node->returnType));
  HANDLE_UNRESOLVED_TYPE_ER(returnType)
  if (returnType.is(TY_DYN))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed")

  // Set the type of the result variable
  SymbolTableEntry *resultVarEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultVarEntry != nullptr);
  resultVarEntry->updateType(returnType, true);
  resultVarEntry->used = true;

  // Visit parameters
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const auto &[name, qualType, isOptional] : namedParamList) {
      if (isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(qualType);
      paramList.push_back({qualType, isOptional});
    }
  }

  // Visit lambda body
  visit(node->body);

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  const QualType functionType = QualType(TY_FUNCTION)
                                    .getWithFunctionParamAndReturnTypes(returnType, paramTypes)
                                    .getWithLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, QualType(TY_DYN), returnType, paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  // Check special requirements if this is an async lambda
  (void)checkAsyncLambdaCaptureRules(node, node->lambdaAttr);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

std::any TypeChecker::visitLambdaProc(LambdaProcNode *node) {
  // Mark unreachable statements
  bool doSetPredecessorsUnreachable = true;
  node->returnsOnAllControlPaths(&doSetPredecessorsUnreachable);

  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit parameters
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const auto &[_, qualType, isOptional] : namedParamList) {
      if (isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(qualType);
      paramList.push_back({qualType, isOptional});
    }
  }

  // Visit lambda body
  visit(node->body);

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  const QualType functionType = QualType(TY_PROCEDURE)
                                    .getWithFunctionParamAndReturnTypes(QualType(TY_DYN), paramTypes)
                                    .getWithLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, QualType(TY_DYN), QualType(TY_DYN), paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  // Check special requirements if this is an async lambda
  (void)checkAsyncLambdaCaptureRules(node, node->lambdaAttr);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

std::any TypeChecker::visitLambdaExpr(LambdaExprNode *node) {
  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit parameters
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst));
    for (const NamedParam &param : namedParamList) {
      if (param.isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(param.qualType);
      paramList.push_back({param.qualType, param.isOptional});
    }
  }

  // Visit lambda expression
  const QualType returnType = std::any_cast<ExprResult>(visit(node->lambdaExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(returnType)
  if (returnType.is(TY_DYN))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed")

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  const SuperType superType = returnType.is(TY_DYN) ? TY_PROCEDURE : TY_FUNCTION;
  const QualType functionType = QualType(superType)
                                    .getWithFunctionParamAndReturnTypes(returnType, paramTypes)
                                    .getWithLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, QualType(TY_DYN), returnType, paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

} // namespace spice::compiler
