// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <symboltablebuilder/ScopeHandle.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), warnings(sourceFile->compilerOutput.warnings) {
}

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;
  reVisitRequested = false;

  // Initialize AST nodes with size of 1
  const bool isPrepare = typeCheckerMode == TC_MODE_PREPARE;
  if (isPrepare)
    node->resizeToNumberOfManifestations(1);

  // Visit children
  visitChildren(node);

  // Check which implicit structures we need for each struct, defined in this source file
  if (isPrepare) {
    for (const auto &[structName, manifestations] : rootScope->getStructs()) {
      for (const auto &[manifestationName, manifestation] : manifestations) {
        createDefaultCtorIfRequired(manifestation, manifestation.scope);
        createDefaultCopyCtorIfRequired(manifestation, manifestation.scope);
        createDefaultDtorIfRequired(manifestation, manifestation.scope);
      }
    }
  }

  return nullptr;
}

std::any TypeChecker::visitMainFctDef(MainFctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitMainFctDefPrepare(node);
  else
    return visitMainFctDefCheck(node);
}

std::any TypeChecker::visitFctDef(FctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitFctDefPrepare(node);
  else
    return visitFctDefCheck(node);
}

std::any TypeChecker::visitProcDef(ProcDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitProcDefPrepare(node);
  else
    return visitProcDefCheck(node);
}

std::any TypeChecker::visitStructDef(StructDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitStructDefPrepare(node);
  else
    return visitStructDefCheck(node);
}

std::any TypeChecker::visitInterfaceDef(InterfaceDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitInterfaceDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitEnumDef(EnumDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitEnumDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGenericTypeDef(GenericTypeDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitGenericTypeDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitAliasDef(AliasDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitAliasDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitGlobalVarDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitExtDecl(ExtDeclNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitExtDeclPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitUnsafeBlock(UnsafeBlockNode *node) {
  // Change to unsafe block body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::UNSAFE_BODY);

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitForLoop(ForLoopNode *node) {
  // Change to for body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::FOR_BODY);

  // Visit loop variable declaration
  visit(node->initDecl());

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condAssign())).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condAssign(), CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool")

  // Visit incrementer
  visit(node->incAssign());

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitForeachLoop(ForeachLoopNode *node) {
  // Change to foreach body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::FOREACH_BODY);

  // Check iterator type
  SymbolType iteratorType = std::any_cast<ExprResult>(visit(node->iteratorAssign())).type;
  HANDLE_UNRESOLVED_TYPE_PTR(iteratorType)
  if (!iteratorType.isIterator(node)) {
    softError(node->iteratorAssign(), OPERATOR_WRONG_DATA_TYPE,
              "Can only apply foreach loop on an iterator type. You provided " + iteratorType.getName());
    return nullptr;
  }
  const std::vector<SymbolType> &iteratorTemplateTypes = iteratorType.getTemplateTypes();
  if (iteratorTemplateTypes.empty())
    SOFT_ERROR_ER(node->iteratorAssign(), INVALID_ITERATOR,
                  "Iterator has no generic arguments so that the item type could not be inferred")

  const bool hasIdx = node->idxVarDecl();
  if (hasIdx) {
    // Visit index declaration or assignment
    auto indexType = std::any_cast<SymbolType>(visit(node->idxVarDecl()));
    HANDLE_UNRESOLVED_TYPE_PTR(indexType)
    // Check if index type is int
    if (!indexType.is(TY_LONG))
      SOFT_ERROR_ER(node->idxVarDecl(), FOREACH_IDX_NOT_LONG,
                    "Index in foreach loop must be of type long. You provided " + indexType.getName())
  }

  // Retrieve .get(), .getIdx(), .isValid() and .next() functions
  Scope *matchScope = iteratorType.getBodyScope();
  SymbolType iteratorItemType;
  if (hasIdx) {
    node->getIdxFct = FunctionManager::matchFunction(matchScope, "getIdx", iteratorType, {}, false, node);
    assert(node->getIdxFct != nullptr);
    iteratorItemType = node->getIdxFct->returnType.getTemplateTypes().back();
  } else {
    node->getFct = FunctionManager::matchFunction(matchScope, "get", iteratorType, {}, false, node);
    assert(node->getFct != nullptr);
    iteratorItemType = node->getFct->returnType;
  }
  node->isValidFct = FunctionManager::matchFunction(matchScope, "isValid", iteratorType, {}, false, node);
  assert(node->isValidFct != nullptr);
  node->nextFct = FunctionManager::matchFunction(matchScope, "next", iteratorType, {}, false, node);
  assert(node->nextFct != nullptr);

  // Check type of the item
  auto itemType = std::any_cast<SymbolType>(visit(node->itemVarDecl()));
  HANDLE_UNRESOLVED_TYPE_PTR(itemType)
  if (itemType.is(TY_DYN)) { // Perform type inference
    // Update evaluated symbol type of the declaration data type
    node->itemVarDecl()->dataType()->setEvaluatedSymbolType(iteratorItemType, manIdx);
    // Update item type
    itemType = iteratorItemType;
  } else {
    // Check item type
    opRuleManager.getAssignResultType(node->itemVarDecl(), itemType, iteratorItemType, manIdx, true, ERROR_FOREACH_ITEM);
  }

  // Update type of item
  SymbolTableEntry *itemVarSymbol = currentScope->lookupStrict(node->itemVarDecl()->varName);
  assert(itemVarSymbol != nullptr);
  itemVarSymbol->updateType(itemType, true);

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitWhileLoop(WhileLoopNode *node) {
  // Change to while body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::WHILE_BODY);

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condition())).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition(), CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool")

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Change to while body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::WHILE_BODY);

  // Visit body
  visit(node->body());

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condition())).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition(), CONDITION_MUST_BE_BOOL, "Do-While loop condition must be of type bool")

  return nullptr;
}

std::any TypeChecker::visitIfStmt(IfStmtNode *node) {
  // Change to then body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::IF_ELSE_BODY);

  // Visit condition
  AssignExprNode *condition = node->condition();
  SymbolType conditionType = std::any_cast<ExprResult>(visit(condition)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition(), CONDITION_MUST_BE_BOOL, "If condition must be of type bool")

  // Warning for bool assignment
  if (condition->hasOperator && condition->op == AssignExprNode::OP_ASSIGN)
    sourceFile->compilerOutput.warnings.emplace_back(condition->codeLoc, BOOL_ASSIGN_AS_CONDITION,
                                                     "If you want to compare the values, use '=='");

  // Visit body
  visit(node->thenBody());

  // Leave then body scope
  scopeHandle.leaveScopeEarly();

  // Visit else statement if existing
  if (node->elseStmt())
    visit(node->elseStmt());

  return nullptr;
}

std::any TypeChecker::visitElseStmt(ElseStmtNode *node) {
  // Visit if statement in the case of an else if branch
  if (node->isElseIf) {
    visit(node->ifStmt());
    return nullptr;
  }

  // Change to else body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::IF_ELSE_BODY);

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Change to anonymous scope body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::ANONYMOUS_BLOCK_BODY);

  // Visit body
  visit(node->body());

  return nullptr;
}

std::any TypeChecker::visitStmtLst(StmtLstNode *node) {
  // Visit nodes in this scope
  for (ASTNode *stmt : node->children) {
    if (!stmt)
      continue;
    // Print warning if statement is unreachable
    if (stmt->unreachable) {
      warnings.emplace_back(stmt->codeLoc, UNREACHABLE_CODE, "This statement is unreachable");
      continue;
    }
    // Visit the statement
    visit(stmt);
  }

  // Do cleanup of this scope, e.g. dtor calls for struct instances
  doScopeCleanup(node);

  return nullptr;
}

std::any TypeChecker::visitParamLst(ParamLstNode *node) {
  NamedParamList namedParams;
  bool metOptional = false;

  for (DeclStmtNode *param : node->params()) {
    // Visit param
    const auto paramType = std::any_cast<SymbolType>(visit(param));
    if (paramType.is(TY_UNRESOLVED))
      continue;

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (paramType.is(TY_DYN)) {
      softError(node, FCT_PARAM_IS_TYPE_DYN, "Type of parameter '" + param->varName + "' is invalid");
      continue;
    }

    // Ensure that no optional param comes after a mandatory param
    if (param->hasAssignment) {
      metOptional = true;
    } else if (metOptional) {
      softError(param, INVALID_PARAM_ORDER, "Mandatory parameters must go before any optional parameters");
      continue;
    }

    // Add parameter to named param list
    namedParams.push_back({param->varName, paramType, metOptional});
  }

  return namedParams;
}

std::any TypeChecker::visitField(FieldNode *node) {
  auto fieldType = std::any_cast<SymbolType>(visit(node->dataType()));
  HANDLE_UNRESOLVED_TYPE_ST(fieldType)

  if (TernaryExprNode *defaultValueNode = node->defaultValue(); defaultValueNode != nullptr) {
    if (!defaultValueNode->hasCompileTimeValue())
      SOFT_ERROR_ST(node, FIELD_DEFAULT_VALUE_NO_COMPILETIME_CONST, "Default values must be computable at compile time")

    const SymbolType defaultValueType = std::any_cast<ExprResult>(visit(defaultValueNode)).type;
    HANDLE_UNRESOLVED_TYPE_ST(fieldType)
    if (!fieldType.matches(defaultValueType, false, true, true))
      SOFT_ERROR_ST(node, FIELD_TYPE_NOT_MATCHING, "Type of the default values does not match the field type")
  }

  return fieldType;
}

std::any TypeChecker::visitSignature(SignatureNode *node) {
  const bool isFunction = node->signatureType == SignatureNode::TYPE_FUNCTION;

  // Visit return type
  SymbolType returnType(TY_DYN);
  if (isFunction)
    returnType = std::any_cast<SymbolType>(visit(node->returnType()));
  if (returnType.is(TY_UNRESOLVED))
    return static_cast<std::vector<Function *> *>(nullptr);

  // Retrieve function template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      auto templateType = std::any_cast<SymbolType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC)) {
        softError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        return static_cast<std::vector<Function *> *>(nullptr);
      }
      // Convert generic symbol type to generic type
      GenericType *genericType = rootScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Visit params
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    paramList.reserve(node->paramTypeLst()->dataTypes().size());
    for (DataTypeNode *param : node->paramTypeLst()->dataTypes()) {
      auto paramType = std::any_cast<SymbolType>(visit(param));
      if (paramType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);

      // Check if the type is present in the template for generic types
      if (!paramType.isCoveredByGenericTypeList(usedGenericTypes)) {
        softError(node->paramTypeLst(), GENERIC_TYPE_NOT_IN_TEMPLATE,
                  "Generic param type not included in the template type list of the function");
        continue;
      }

      paramTypes.push_back(paramType);
      paramList.push_back({paramType, false});
    }
  }

  // Check if all template types were used in the function parameters
  if (std::ranges::any_of(usedGenericTypes, [](const GenericType &genericType) { return !genericType.used; })) {
    softError(node->templateTypeLst(), GENERIC_TYPE_NOT_USED, "Generic type was not used by the function parameters");
    return static_cast<std::vector<Function *> *>(nullptr);
  }

  // Build signature object
  Function signature(node->methodName, nullptr, SymbolType(TY_DYN), returnType, paramList, usedGenericTypes, node);

  // Add signature to current scope
  Function *manifestation = FunctionManager::insertFunction(currentScope, signature, &node->signatureManifestations);
  manifestation->entry = node->entry;
  manifestation->used = true;

  // Prepare signature type
  SymbolType signatureType(isFunction ? TY_FUNCTION : TY_PROCEDURE);
  signatureType.specifiers = node->signatureSpecifiers;
  if (isFunction)
    signatureType.setFunctionReturnType(returnType);
  signatureType.setFunctionParamTypes(paramTypes);

  // Set entry to signature type
  assert(node->entry != nullptr);
  node->entry->updateType(signatureType, false);
  node->entry->used = true;

  return &node->signatureManifestations;
}

std::any TypeChecker::visitDeclStmt(DeclStmtNode *node) {
  SymbolType localVarType;
  if (node->hasAssignment) {
    // Visit the right side
    auto rhs = std::any_cast<ExprResult>(visit(node->assignExpr()));
    auto [rhsTy, rhsEntry] = rhs;

    // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
    if (rhsEntry != nullptr && rhsEntry->anonymous) {
      currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
      rhs.entry = rhsEntry = nullptr;
    }

    // Visit data type
    localVarType = std::any_cast<SymbolType>(visit(node->dataType()));

    // Infer the type left to right if the right side is an empty array initialization
    if (rhsTy.isArrayOf(TY_DYN))
      rhsTy = localVarType;

    // Check if type has to be inferred or both types are fixed
    if (!localVarType.is(TY_UNRESOLVED) && !rhsTy.is(TY_UNRESOLVED)) {
      localVarType = opRuleManager.getAssignResultType(node, localVarType, rhsTy, 0, true);

      // Call copy ctor if required
      if (localVarType.is(TY_STRUCT) && !node->isParam && !rhs.isTemporary()) {
        Scope *matchScope = localVarType.getBodyScope();
        assert(matchScope != nullptr);
        // Check if we have a no-args ctor to call
        const std::string structName = localVarType.getOriginalSubType();
        const SymbolType &thisType = localVarType;
        const std::vector<SymbolType> paramTypes = {thisType.toConstReference(node)};
        node->calledCopyCtor = FunctionManager::matchFunction(matchScope, CTOR_FUNCTION_NAME, thisType, paramTypes, true, node);
      }

      // If this is a struct type, check if the type is known. If not, error out
      if (localVarType.isBaseType(TY_STRUCT) && !sourceFile->getNameRegistryEntry(localVarType.getBaseType().getSubType())) {
        const std::string structName = localVarType.getBaseType().getOriginalSubType();
        softError(node->dataType(), UNKNOWN_DATATYPE, "Unknown struct type '" + structName + "'. Forgot to import?");
        localVarType = SymbolType(TY_UNRESOLVED);
      }
    } else {
      localVarType = SymbolType(TY_UNRESOLVED);
    }
  } else {
    // Visit data type
    localVarType = std::any_cast<SymbolType>(visit(node->dataType()));

    // References with no initialization are illegal
    if (localVarType.isRef() && !node->isParam && !node->isForEachItem)
      softError(node, REFERENCE_WITHOUT_INITIALIZER, "References must always be initialized directly");

    // If this is a struct, check for the default ctor
    if (localVarType.is(TY_STRUCT) && !node->isParam && !node->isForEachItem) {
      Scope *matchScope = localVarType.getBodyScope();
      assert(matchScope != nullptr);
      // Check if we need to call a ctor
      node->isCtorCallRequired = matchScope->hasRefFields();
      // Check if we have a no-args ctor to call
      const std::string structName = localVarType.getOriginalSubType();
      const SymbolType &thisType = localVarType;
      node->calledInitCtor = FunctionManager::matchFunction(matchScope, CTOR_FUNCTION_NAME, thisType, {}, false, node);
      if (!node->calledInitCtor && node->isCtorCallRequired)
        SOFT_ERROR_ST(node, MISSING_NO_ARGS_CTOR, "Struct '" + structName + "' misses a no-args constructor")
    }
  }

  // Update the type of the variable
  SymbolTableEntry *localVarEntry = currentScope->lookupStrict(node->varName);
  assert(localVarEntry != nullptr);
  localVarEntry->updateType(localVarType, true);
  node->entries.at(manIdx) = localVarEntry;

  // Update the state of the variable
  localVarEntry->updateState(INITIALIZED, node, true);

  return node->setEvaluatedSymbolType(localVarType, manIdx);
}

std::any TypeChecker::visitImportStmt(ImportStmtNode *node) {
  if (typeCheckerMode == TC_MODE_PREPARE)
    return visitImportStmtPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitReturnStmt(ReturnStmtNode *node) {
  // Retrieve return variable entry
  SymbolTableEntry *returnVar = currentScope->lookup(RETURN_VARIABLE_NAME);
  const bool isFunction = returnVar != nullptr;
  SymbolType returnType = isFunction ? returnVar->getType() : SymbolType(TY_DYN);

  // Check if procedure with return value
  if (!isFunction) {
    if (node->hasReturnValue)
      SOFT_ERROR_ER(node->assignExpr(), RETURN_WITH_VALUE_IN_PROCEDURE, "Return with value in procedure is not allowed")
    return nullptr;
  }

  if (!node->hasReturnValue && !returnVar->isInitialized())
    SOFT_ERROR_ST(node, RETURN_WITHOUT_VALUE_RESULT, "Return without value, but result variable is not initialized yet")

  if (!node->hasReturnValue)
    return nullptr;

  // Visit right side
  auto rhs = std::any_cast<ExprResult>(visit(node->assignExpr()));
  HANDLE_UNRESOLVED_TYPE_ST(rhs.type)

  // Check if types match
  opRuleManager.getAssignResultType(node->assignExpr(), returnType, rhs.type, 0, false, ERROR_MSG_RETURN);

  // Manager dtor call
  if (rhs.entry != nullptr) {
    if (rhs.entry->anonymous) {
      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      currentScope->symbolTable.deleteAnonymous(rhs.entry->name);
      rhs.entry = nullptr;
    } else {
      // Otherwise omit the destructor call, because the caller destructs the value
      rhs.entry->omitDtorCall = true;
    }
  }

  return node->setEvaluatedSymbolType(returnType, manIdx);
}

std::any TypeChecker::visitBreakStmt(BreakStmtNode *node) {
  // Check if the stated number is valid
  if (node->breakTimes < 1)
    SOFT_ERROR_ER(node, INVALID_BREAK_NUMBER, "Break count must be >= 1, you provided " + std::to_string(node->breakTimes))

  // Check if we can break this often
  const size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->breakTimes > maxBreaks)
    SOFT_ERROR_ER(node, INVALID_BREAK_NUMBER, "We can only break " + std::to_string(maxBreaks) + " time(s) here")

  return nullptr;
}

std::any TypeChecker::visitContinueStmt(ContinueStmtNode *node) {
  // Check if the stated number is valid
  if (node->continueTimes < 1)
    SOFT_ERROR_ER(node, INVALID_CONTINUE_NUMBER,
                  "Continue count must be >= 1, you provided " + std::to_string(node->continueTimes))

  // Check if we can continue this often
  const size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->continueTimes > maxBreaks)
    SOFT_ERROR_ER(node, INVALID_CONTINUE_NUMBER, "We can only continue " + std::to_string(maxBreaks) + " time(s) here")

  return nullptr;
}

std::any TypeChecker::visitAssertStmt(AssertStmtNode *node) {
  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  HANDLE_UNRESOLVED_TYPE_ER(conditionType)

  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->assignExpr(), ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool")

  return nullptr;
}

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = node->templatedString.find_first_of('%');
  while (index != std::string::npos && index != node->templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->args().size() <= placeholderCount)
      SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments")

    // Get next assignment
    AssignExprNode *assignment = node->args().at(placeholderCount);
    // Visit assignment
    SymbolType argType = std::any_cast<ExprResult>(visit(assignment)).type;
    HANDLE_UNRESOLVED_TYPE_ER(argType)
    argType = argType.removeReferenceWrapper();

    switch (node->templatedString.at(index + 1)) {
    case 'c': {
      if (!argType.is(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects char, but got " + argType.getName())
      placeholderCount++;
      break;
    }
    case 'd':
    case 'i':
    case 'l':
    case 'o':
    case 'u':
    case 'x':
    case 'X': {
      if (!argType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_BOOL}))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects int, short, long, byte or bool, but got " + argType.getName())
      placeholderCount++;
      break;
    }
    case 'a':
    case 'A':
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G': {
      if (!argType.is(TY_DOUBLE))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects double, but got " + argType.getName())
      placeholderCount++;
      break;
    }
    case 's': {
      const std::string strobjTypeName = STRING_RT_IMPORT_NAME + std::string(SCOPE_ACCESS_TOKEN) + STROBJ_NAME;
      const bool isString = argType.is(TY_STRING) || argType.is(TY_STRUCT, strobjTypeName);
      if (!isString && !argType.isPtrOf(TY_CHAR) && !argType.isArrayOf(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects string, String, char* or char[], but got " + argType.getName())
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!argType.isPtr() && !argType.isArray() && !argType.is(TY_STRING))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects pointer, array or string, but got " + argType.getName())
      placeholderCount++;
      break;
    }
    }
    index = node->templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->args().size())
    SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments")

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_BOOL), manIdx)};
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  if (node->isType) { // Size of type
    visit(node->dataType());
  } else { // Size of value
    visit(node->assignExpr());
  }

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitAlignofCall(AlignofCallNode *node) {
  if (node->isType) { // Align of type
    visit(node->dataType());
  } else { // Align of value
    visit(node->assignExpr());
  }

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  SymbolType argType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isArray() && !argType.is(TY_STRING))
    SOFT_ERROR_ER(node->assignExpr(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays or strings")

  // If we want to use the len builtin on a string, we need to import the string runtime module
  if (argType.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitPanicCall(PanicCallNode *node) {
  SymbolType argType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isErrorObj())
    SOFT_ERROR_ER(node->assignExpr(), EXPECTED_ERROR_TYPE, "The panic builtin can only work with errors")

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_DYN), manIdx)};
}

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if ternary
  if (node->ternaryExpr()) {
    auto result = std::any_cast<ExprResult>(visit(node->ternaryExpr()));
    node->setEvaluatedSymbolType(result.type, manIdx);
    return result;
  }

  // Check if assignment
  if (node->hasOperator) {
    // Visit the right side first
    auto [rhsType, rhsEntry] = std::any_cast<ExprResult>(visit(node->rhs()));
    HANDLE_UNRESOLVED_TYPE_ER(rhsType)
    // Then visit the left side
    auto [lhsType, lhsVar] = std::any_cast<ExprResult>(visit(node->lhs()));
    HANDLE_UNRESOLVED_TYPE_ER(lhsType)

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) {
      rhsType = opRuleManager.getAssignResultType(node, lhsType, rhsType, 0);

      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      if (rhsEntry != nullptr && rhsEntry->anonymous) {
        currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
        rhsEntry = nullptr;
      }
    } else if (node->op == AssignExprNode::OP_PLUS_EQUAL) {
      rhsType = opRuleManager.getPlusEqualResultType(node, lhsType, rhsType, 0).type;
    } else if (node->op == AssignExprNode::OP_MINUS_EQUAL) {
      rhsType = opRuleManager.getMinusEqualResultType(node, lhsType, rhsType, 0).type;
    } else if (node->op == AssignExprNode::OP_MUL_EQUAL) {
      rhsType = opRuleManager.getMulEqualResultType(node, lhsType, rhsType, 0).type;
    } else if (node->op == AssignExprNode::OP_DIV_EQUAL) {
      rhsType = opRuleManager.getDivEqualResultType(node, lhsType, rhsType, 0).type;
    } else if (node->op == AssignExprNode::OP_REM_EQUAL) {
      rhsType = opRuleManager.getRemEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_SHL_EQUAL) {
      rhsType = opRuleManager.getSHLEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_SHR_EQUAL) {
      rhsType = opRuleManager.getSHREqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_AND_EQUAL) {
      rhsType = opRuleManager.getAndEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_OR_EQUAL) {
      rhsType = opRuleManager.getOrEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_XOR_EQUAL) {
      rhsType = opRuleManager.getXorEqualResultType(node, lhsType, rhsType, 0);
    }

    if (lhsVar) { // Variable is involved on the left side
      // Perform type inference
      if (lhsType.is(TY_DYN))
        lhsVar->updateType(rhsType, false);

      // In case the lhs variable is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsVar->name); lhsCapture)
        lhsCapture->setCaptureType(READ_WRITE);

      // Update the state of the variable
      lhsVar->updateState(INITIALIZED, node, false);
    }

    return ExprResult{node->setEvaluatedSymbolType(rhsType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "AssignStmt fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitTernaryExpr(TernaryExprNode *node) {
  // Check if there is a ternary operator applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit condition
  LogicalOrExprNode *condition = node->operands()[0];
  SymbolType conditionType = std::any_cast<ExprResult>(visit(condition)).type;
  HANDLE_UNRESOLVED_TYPE_ER(conditionType)

  SymbolType trueType;
  SymbolType falseType;
  if (node->isShortened) {
    trueType = conditionType;
    falseType = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  } else {
    trueType = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
    HANDLE_UNRESOLVED_TYPE_ER(trueType)
    falseType = std::any_cast<ExprResult>(visit(node->operands()[2])).type;
  }
  HANDLE_UNRESOLVED_TYPE_ER(falseType)

  // Check if the condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(condition, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool")

  // Check if trueType and falseType are matching
  const SymbolType trueTypeModified = trueType.removeReferenceWrapper();
  const SymbolType falseTypeModified = falseType.removeReferenceWrapper();
  if (!trueTypeModified.matches(falseTypeModified, false, true, false))
    SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                  "True and false operands in ternary must be of same data type. Got " + trueType.getName(true) + " and " +
                      falseType.getName(true))

  return ExprResult{node->setEvaluatedSymbolType(trueType, manIdx)};
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(currentType)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
    currentType = OpRuleManager::getLogicalOrResultType(node, currentType, rhsTy, i - 1);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(currentType)
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
    currentType = OpRuleManager::getLogicalAndResultType(node, currentType, rhsTy, i - 1);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(currentType)
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
    currentType = OpRuleManager::getBitwiseOrResultType(node, currentType, rhsTy, i - 1);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(currentType)
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
    currentType = OpRuleManager::getBitwiseXorResultType(node, currentType, rhsTy, i - 1);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(currentType)
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
    currentType = OpRuleManager::getBitwiseAndResultType(node, currentType, rhsTy, i - 2);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitEqualityExpr(EqualityExprNode *node) {
  // Check if at least one equality operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit right side first, then left side
  SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(lhsTy)

  // Check if we need the string runtime to perform a string comparison
  if (lhsTy.is(TY_STRING) && rhsTy.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check operator
  ExprResult result;
  if (node->op == EqualityExprNode::OP_EQUAL) // Operator was equal
    result = opRuleManager.getEqualResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == EqualityExprNode::OP_NOT_EQUAL) // Operator was not equal
    result = opRuleManager.getNotEqualResultType(node, lhsTy, rhsTy, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE

  node->setEvaluatedSymbolType(result.type, manIdx);
  return result;
}

std::any TypeChecker::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit right side first, then left side
  SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(lhsTy)

  // Check operator
  SymbolType resultType;
  if (node->op == RelationalExprNode::OP_LESS) // Operator was less
    resultType = OpRuleManager::getLessResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == RelationalExprNode::OP_GREATER) // Operator was greater
    resultType = OpRuleManager::getGreaterResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == RelationalExprNode::OP_LESS_EQUAL) // Operator was less equal
    resultType = OpRuleManager::getLessEqualResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == RelationalExprNode::OP_GREATER_EQUAL) // Operator was greater equal
    resultType = OpRuleManager::getGreaterEqualResultType(node, lhsTy, rhsTy, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "RelationalExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitShiftExpr(ShiftExprNode *node) {
  // Check if at least one shift operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit right side first, then left
  SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  HANDLE_UNRESOLVED_TYPE_ER(rhsTy)
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  HANDLE_UNRESOLVED_TYPE_ER(lhsTy)

  // Check operator
  ExprResult currentResult;
  if (node->op == ShiftExprNode::OP_SHIFT_LEFT) // Operator was shl
    currentResult = opRuleManager.getShiftLeftResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == ShiftExprNode::OP_SHIFT_RIGHT) // Operator was shr
    currentResult = opRuleManager.getShiftRightResultType(node, lhsTy, rhsTy, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands()[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)

  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    // Visit next operand
    MultiplicativeExprNode *operand = node->operands()[i + 1];
    SymbolType operandType = std::any_cast<ExprResult>(visit(operand)).type;
    HANDLE_UNRESOLVED_TYPE_ER(operandType)

    // Check operator
    const AdditiveExprNode::AdditiveOp &op = node->opQueue.front().first;
    if (op == AdditiveExprNode::OP_PLUS)
      currentResult = opRuleManager.getPlusResultType(node, currentResult.type, operandType, i);
    else if (op == AdditiveExprNode::OP_MINUS)
      currentResult = opRuleManager.getMinusResultType(node, currentResult.type, operandType, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExpr fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  // Check if at least one multiplicative operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands()[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)
  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    // Visit next operand
    CastExprNode *operand = node->operands()[i + 1];
    SymbolType operandType = std::any_cast<ExprResult>(visit(operand)).type;
    HANDLE_UNRESOLVED_TYPE_ER(operandType)

    // Check operator
    const MultiplicativeExprNode::MultiplicativeOp &op = node->opQueue.front().first;
    if (op == MultiplicativeExprNode::OP_MUL)
      currentResult = opRuleManager.getMulResultType(node, currentResult.type, operandType, i);
    else if (op == MultiplicativeExprNode::OP_DIV)
      currentResult = opRuleManager.getDivResultType(node, currentResult.type, operandType, i);
    else if (op == MultiplicativeExprNode::OP_REM)
      currentResult = OpRuleManager::getRemResultType(node, currentResult.type, operandType, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "Multiplicative fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitCastExpr(CastExprNode *node) {
  // Check if cast is applied
  if (!node->isCast)
    return visit(node->prefixUnaryExpr());

  // Visit source type
  SymbolType srcType = std::any_cast<ExprResult>(visit(node->prefixUnaryExpr())).type;
  HANDLE_UNRESOLVED_TYPE_ER(srcType)
  // Visit destination type
  auto dstType = std::any_cast<SymbolType>(visit(node->dataType()));
  HANDLE_UNRESOLVED_TYPE_ER(dstType)

  // Check for identity cast
  if (srcType == dstType) {
    CompilerWarning warning(node->codeLoc, IDENTITY_CAST, "You cast from a type to itself. Thus, this can be simplified.");
    sourceFile->compilerOutput.warnings.push_back(warning);
  }

  // Get result type
  SymbolType resultType = opRuleManager.getCastResultType(node, dstType, srcType, 0);

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  // Reset access scope
  accessScope = nullptr;

  // If no operator is applied, simply visit the postfix unary expression
  if (node->op == PrefixUnaryExprNode::OP_NONE)
    return visit(node->postfixUnaryExpr());

  // Visit the right side
  PrefixUnaryExprNode *rhsNode = node->prefixUnary();
  auto [operandType, operandEntry] = std::any_cast<ExprResult>(visit(rhsNode));
  HANDLE_UNRESOLVED_TYPE_ER(operandType)
  // Determine action, based on the given operator
  switch (node->op) {
  case PrefixUnaryExprNode::OP_MINUS:
    operandType = OpRuleManager::getPrefixMinusResultType(node, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_PLUS_PLUS:
    operandType = opRuleManager.getPrefixPlusPlusResultType(node, operandType, 0);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setCaptureType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node, false);
    }

    break;
  case PrefixUnaryExprNode::OP_MINUS_MINUS:
    operandType = opRuleManager.getPrefixMinusMinusResultType(node, operandType, 0);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setCaptureType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node, false);
    }

    break;
  case PrefixUnaryExprNode::OP_NOT:
    operandType = OpRuleManager::getPrefixNotResultType(node, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_BITWISE_NOT:
    operandType = OpRuleManager::getPrefixBitwiseNotResultType(node, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_DEREFERENCE:
    operandType = OpRuleManager::getPrefixMulResultType(node, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_ADDRESS_OF:
    operandType = OpRuleManager::getPrefixBitwiseAndResultType(node, operandType, 0);
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType, manIdx), operandEntry};
}

std::any TypeChecker::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  // If no operator is applied, simply visit the atomic expression
  if (node->op == PostfixUnaryExprNode::OP_NONE)
    return visit(node->atomicExpr());

  // Visit left side
  PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr();
  auto [lhsType, lhsEntry] = std::any_cast<ExprResult>(visit(lhsNode));
  HANDLE_UNRESOLVED_TYPE_ER(lhsType)

  switch (node->op) {
  case PostfixUnaryExprNode::OP_SUBSCRIPT: {
    lhsType = lhsType.removeReferenceWrapper();

    // Check if we can apply the subscript operator on the lhs type
    if (!lhsType.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
      SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                    "Can only apply subscript operator on array type, got " + lhsType.getName(true))

    // Visit index assignment
    AssignExprNode *indexAssignExpr = node->assignExpr();
    SymbolType indexType = std::any_cast<ExprResult>(visit(indexAssignExpr)).type;
    HANDLE_UNRESOLVED_TYPE_ER(indexType)
    // Check if the index is of the right type
    if (!indexType.isOneOf({TY_INT, TY_LONG}))
      SOFT_ERROR_ER(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long")

    // Check if we have an unsafe operation
    if (lhsType.is(TY_PTR) && !currentScope->doesAllowUnsafeOperations())
      SOFT_ERROR_ER(
          node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
          "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.")

    // Check if we have a hardcoded array index
    if (lhsType.is(TY_ARRAY) && lhsType.getArraySize() != ARRAY_SIZE_UNKNOWN && indexAssignExpr->hasCompileTimeValue()) {
      std::int32_t constIndex = indexAssignExpr->getCompileTimeValue().intValue;
      size_t constSize = lhsType.getArraySize();
      // Check if we are accessing out-of-bounds memory
      if (constIndex >= constSize) {
        const std::string idxStr = std::to_string(constIndex);
        const std::string sizeStr = std::to_string(constSize);
        SOFT_ERROR_ER(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                      "You are trying to access element with index " + idxStr + " of an array with size " + sizeStr)
      }
    }

    // Get item type
    lhsType = lhsType.getContainedTy();

    // Remove heap specifier
    lhsType.specifiers.isHeap = false;

    break;
  }
  case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
    const std::string &fieldName = node->identifier;

    // Check if lhs is enum or strobj
    SymbolType lhsBaseTy = lhsType;
    autoDeReference(lhsBaseTy);
    if (!lhsBaseTy.is(TY_STRUCT))
      SOFT_ERROR_ER(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + lhsType.getName())

    // Retrieve registry entry
    const std::string &structName = lhsBaseTy.getSubType();
    Scope *structScope = lhsBaseTy.getBodyScope();

    // If we only have the generic struct scope, lookup the concrete manifestation scope
    if (structScope->isGenericScope) {
      const std::string structName = lhsBaseTy.getOriginalSubType();
      Scope *matchScope = lhsBaseTy.getBodyScope()->parent;
      Struct *spiceStruct = StructManager::matchStruct(matchScope, structName, lhsBaseTy.getTemplateTypes(), node);
      assert(spiceStruct != nullptr);
      structScope = spiceStruct->scope;
    }
    assert(!structScope->isGenericScope); // At this point we always expect a substantiation scope

    // Get accessed field
    std::vector<size_t> indexPath;
    SymbolTableEntry *memberEntry = structScope->symbolTable.lookupInComposedFields(fieldName, indexPath);
    if (!memberEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Field '" + node->identifier + "' not found in struct " + structName)
    SymbolType memberType = memberEntry->getType();

    // Check for insufficient visibility
    if (structScope->isImportedBy(rootScope) && !memberEntry->getType().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access field '" + fieldName + "' due to its private visibility")

    // Set field to used
    memberEntry->used = true;

    // Overwrite type and entry of left side with member type and entry
    lhsType = memberType;
    lhsEntry = memberEntry;
    break;
  }
  case PostfixUnaryExprNode::OP_PLUS_PLUS: {
    ExprResult result = opRuleManager.getPostfixPlusPlusResultType(node, lhsType, 0);
    lhsType = result.type;
    lhsEntry = result.entry;

    if (lhsEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsEntry->name); lhsCapture)
        lhsCapture->setCaptureType(READ_WRITE);

      // Update the state of the variable
      lhsEntry->updateState(INITIALIZED, node, false);
    }

    break;
  }
  case PostfixUnaryExprNode::OP_MINUS_MINUS: {
    ExprResult result = opRuleManager.getPostfixMinusMinusResultType(node, lhsType, 0);
    lhsType = result.type;
    lhsEntry = result.entry;

    if (lhsEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsEntry->name); lhsCapture)
        lhsCapture->setCaptureType(READ_WRITE);

      // Update the state of the variable
      lhsEntry->updateState(INITIALIZED, node, false);
    }

    break;
  }
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  if (lhsType.is(TY_INVALID)) {
    const std::string varName = lhsEntry ? lhsEntry->name : "";
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + varName + "' was referenced before declared")
  }

  return ExprResult{node->setEvaluatedSymbolType(lhsType, manIdx), lhsEntry};
}

std::any TypeChecker::visitAtomicExpr(AtomicExprNode *node) {
  // Check if constant
  if (node->constant())
    return visit(node->constant());

  // Check if value
  if (node->value())
    return visit(node->value());

  // Check for builtin calls
  if (node->printfCall())
    return visit(node->printfCall());
  if (node->sizeofCall())
    return visit(node->sizeofCall());
  if (node->alignofCall())
    return visit(node->alignofCall());
  if (node->lenCall())
    return visit(node->lenCall());
  if (node->panicCall())
    return visit(node->panicCall());

  // Check for assign expression within parentheses
  if (node->assignExpr())
    return visit(node->assignExpr());

  // Identifier (local or global variable access)
  assert(!node->fqIdentifier.empty());
  if (!accessScope)
    accessScope = currentScope;

  // Check if a local or global variable can be found by searching for the name
  SymbolTableEntry *varEntry = nullptr;
  if (node->identifierFragments.size() == 1)
    varEntry = accessScope->lookup(node->identifierFragments.back());

  // If no local or global was found, search in the name registry
  if (!varEntry) {
    const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqIdentifier);
    if (!registryEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found")
    varEntry = registryEntry->targetEntry;
    accessScope = registryEntry->targetScope;
  }
  assert(varEntry != nullptr);
  assert(accessScope != nullptr);
  node->entries.at(manIdx) = varEntry;
  node->accessScopes.at(manIdx) = accessScope;
  SymbolType varType = varEntry->getType();
  HANDLE_UNRESOLVED_TYPE_ER(varType)

  if (varType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && varEntry->global) {
    // Check if overloaded function was referenced
    const std::vector<Function *> *manifestations = varEntry->declNode->getFctManifestations(varEntry->name);
    if (manifestations->size() > 1)
      SOFT_ERROR_ER(node, REFERENCED_OVERLOADED_FCT,
                    "Overloaded functions or functions with optional parameters cannot be referenced")
    // Set referenced function to used
    Function *referencedFunction = manifestations->front();
    referencedFunction->used = true;
    referencedFunction->entry->used = true;
  }

  if (varType.is(TY_INVALID))
    SOFT_ERROR_ER(node, USED_BEFORE_DECLARED, "Symbol '" + varEntry->name + "' was used before declared.")

  // The base type should be a primitive, struct, function or procedure
  if (!varType.getBaseType().isPrimitive() && !varType.getBaseType().isOneOf({TY_STRUCT, TY_FUNCTION, TY_PROCEDURE, TY_DYN}))
    SOFT_ERROR_ER(node, INVALID_SYMBOL_ACCESS, "A symbol of type " + varType.getName() + " cannot be accessed here")

  // Check if is an imported variable
  if (accessScope->isImportedBy(rootScope)) {
    // Check if the entry is public
    if (varEntry->scope->type != ScopeType::ENUM && !varEntry->getType().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access '" + varEntry->name + "' due to its private visibility")
  }

  // Check if we have seen a 'this.' prefix, because the generator needs that
  if (varEntry->scope->type == ScopeType::STRUCT && node->identifierFragments.front() != THIS_VARIABLE_NAME)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE,
                  "The symbol '" + node->fqIdentifier + "' could not be found. Missing 'this.' prefix?")

  // Set symbol table entry to used
  varEntry->used = true;

  // Retrieve scope for the new scope path fragment
  if (varType.isBaseType(TY_STRUCT)) {
    // Set access scope to struct scope
    const NameRegistryEntry *nameRegistryEntry = sourceFile->getNameRegistryEntry(varType.getBaseType().getSubType());
    assert(nameRegistryEntry != nullptr);

    // Change the access scope to the struct scope
    accessScope = nameRegistryEntry->targetScope;
    assert(accessScope != nullptr);
    node->accessScopes.at(manIdx) = accessScope;

    // Check if the entry is public if it is imported
    assert(nameRegistryEntry->targetEntry != nullptr);
    if (!nameRegistryEntry->targetEntry->getType().isPublic() && accessScope->parent->isImportedBy(rootScope))
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY,
                    "Cannot access struct '" + nameRegistryEntry->targetEntry->name + "' due to its private visibility")
  }

  return ExprResult{node->setEvaluatedSymbolType(varType, manIdx), varEntry};
}

std::any TypeChecker::visitValue(ValueNode *node) {
  // Function call
  if (node->fctCall())
    return visit(node->fctCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  // Lambda function
  if (node->lambdaFunc())
    return visit(node->lambdaFunc());

  // Lambda procedure
  if (node->lambdaProc())
    return visit(node->lambdaProc());

  // Lambda expression
  if (node->lambdaExpr())
    return visit(node->lambdaExpr());

  // Typed nil
  if (node->isNil) {
    auto nilType = std::any_cast<SymbolType>(visit(node->nilType()));
    HANDLE_UNRESOLVED_TYPE_ER(nilType)
    if (nilType.is(TY_DYN))
      SOFT_ERROR_ER(node->nilType(), UNEXPECTED_DYN_TYPE, "Nil must have an explicit type")
    return ExprResult{node->setEvaluatedSymbolType(nilType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "Value fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitConstant(ConstantNode *node) {
  SymbolSuperType superType;
  switch (node->type) {
  case ConstantNode::TYPE_DOUBLE:
    superType = TY_DOUBLE;
    break;
  case ConstantNode::TYPE_INT:
    superType = TY_INT;
    break;
  case ConstantNode::TYPE_SHORT:
    superType = TY_SHORT;
    break;
  case ConstantNode::TYPE_LONG:
    superType = TY_LONG;
    break;
  case ConstantNode::TYPE_BYTE:
    superType = TY_BYTE;
    break;
  case ConstantNode::TYPE_CHAR:
    superType = TY_CHAR;
    break;
  case ConstantNode::TYPE_STRING:
    superType = TY_STRING;
    break;
  case ConstantNode::TYPE_BOOL:
    superType = TY_BOOL;
    break;
  default:                                                          // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Constant fall-through"); // GCOV_EXCL_LINE
  }

  // Create symbol type
  SymbolType symbolType(superType);
  symbolType.specifiers = TypeSpecifiers::of(superType);

  return ExprResult{node->setEvaluatedSymbolType(symbolType, manIdx)};
}

std::any TypeChecker::visitFctCall(FctCallNode *node) {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Retrieve arg types
  data.argTypes.clear();
  if (node->hasArgs) {
    const std::vector<AssignExprNode *> &args = node->argLst()->args();
    data.argTypes.reserve(args.size());
    for (AssignExprNode *arg : args) {
      // Visit argument
      const SymbolType argType = std::any_cast<ExprResult>(visit(arg)).type;
      HANDLE_UNRESOLVED_TYPE_ER(argType)
      assert(!argType.hasAnyGenericParts());
      // Save arg type to arg types list
      data.argTypes.push_back(argType);
    }
  }

  // Retrieve entry of the first fragment
  SymbolTableEntry *firstFragEntry = currentScope->lookup(node->functionNameFragments.front());
  if (firstFragEntry) {
    firstFragEntry->used = true;
    // Decide of which type the function call is
    const SymbolType &baseType = firstFragEntry->getType().getBaseType();
    HANDLE_UNRESOLVED_TYPE_ER(baseType)
    if (baseType.isOneOf({TY_STRUCT, TY_INTERFACE})) {
      data.callType = firstFragEntry->scope->type == ScopeType::GLOBAL ? FctCallNode::TYPE_CTOR : FctCallNode::TYPE_METHOD;
    } else if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && firstFragEntry->scope->type != ScopeType::GLOBAL) {
      data.callType = FctCallNode::TYPE_FCT_PTR;
    }
  }

  // Check if this is a method call or a normal function call
  if (data.isMethodCall()) {
    // This is a method call
    data.thisType = firstFragEntry->getType();
    Scope *structBodyScope = data.thisType.getBaseType().getBodyScope();
    assert(structBodyScope != nullptr);
    bool success = visitMethodCall(node, structBodyScope);
    if (!success) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_UNRESOLVED), manIdx)};
    assert(data.calleeParentScope != nullptr);
  } else if (data.isFctPtrCall()) {
    // This is a function pointer call
    const SymbolType &functionType = firstFragEntry->getType().getBaseType();
    assert(functionType.isOneOf({TY_FUNCTION, TY_PROCEDURE}));
    bool success = visitFctPtrCall(node, functionType);
    if (!success) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_UNRESOLVED), manIdx)};
  } else {
    // This is an ordinary function call
    assert(data.isOrdinaryCall() || data.isCtorCall());
    const std::string knownStructName = visitOrdinaryFctCall(node);
    if (knownStructName == UNRESOLVED_TYPE_NAME) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_UNRESOLVED), manIdx)};
    assert(data.calleeParentScope != nullptr);

    // Only ordinary function calls can be constructors
    if (data.isCtorCall()) {
      // Set a name to the thisType that is known to the current source file
      data.thisType = data.thisType.replaceBaseSubType(knownStructName);
      assert(data.thisType.is(TY_STRUCT));
    }
  }

  if (!data.isFctPtrCall()) {
    // Check if we were able to find a function
    if (!data.callee) {
      // Build error message
      const std::string functionName = data.isCtorCall() ? CTOR_FUNCTION_NAME : node->functionNameFragments.back();
      ParamList errArgTypes;
      for (const SymbolType &argType : data.argTypes)
        errArgTypes.push_back({argType, false});
      const std::string signature = Function::getSignature(functionName, data.thisType, SymbolType(TY_DYN), errArgTypes, {});
      // Throw error
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure '" + signature + "' could not be found")
    }

    // Check if we need to request a re-visit, because the function body was not type-checked yet
    requestRevisitIfRequired(data.callee);

    // Get function entry from function object
    SymbolTableEntry *functionEntry = data.callee->entry;

    // Check if the called function has sufficient visibility
    data.isImported = data.calleeParentScope->isImportedBy(rootScope);
    if (data.isImported && !functionEntry->getType().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY,
                    "Function/procedure '" + data.callee->getSignature() + "' has insufficient visibility")
  }

  // Retrieve return type
  const bool isFct = data.isFctPtrCall() ? firstFragEntry->getType().getBaseType().is(TY_FUNCTION) : data.callee->isFunction();
  SymbolType returnType;
  if (data.isFctPtrCall()) {
    returnType = isFct ? firstFragEntry->getType().getBaseType().getFunctionReturnType() : SymbolType(TY_BOOL);
  } else if (data.isCtorCall()) {
    // Set return type to 'this' type
    returnType = data.thisType;
  } else if (data.callee->isProcedure()) {
    // Procedures always have the return type 'bool'
    returnType = SymbolType(TY_BOOL);
  } else {
    returnType = data.callee->returnType;
  }

  // Initialize return type if required
  SymbolTableEntry *anonymousSymbol = nullptr;
  if (returnType.isBaseType(TY_STRUCT)) {
    SymbolType returnBaseType = returnType.getBaseType();
    const std::string structName = returnBaseType.getOriginalSubType();
    Scope *matchScope = returnBaseType.getBodyScope()->parent;
    assert(matchScope != nullptr);
    Struct *spiceStruct = StructManager::matchStruct(matchScope, structName, returnBaseType.getTemplateTypes(), node);
    assert(spiceStruct != nullptr);
    returnBaseType.setBodyScope(spiceStruct->scope);
    returnType = returnType.replaceBaseType(returnBaseType);

    returnType = mapImportedScopeTypeToLocalType(returnType.getBaseType().getBodyScope(), returnType);

    // Add anonymous symbol to keep track of deallocation
    if (returnType.is(TY_STRUCT))
      anonymousSymbol = currentScope->symbolTable.insertAnonymous(returnType, node);
  }

  // Remove public specifier to not have public local variables
  returnType.specifiers.isPublic = false;

  // Check if the return value gets used
  if (isFct && !node->hasReturnValueReceiver())
    warnings.emplace_back(node->codeLoc, UNUSED_RETURN_VALUE, "The return value of the function call is unused");

  return ExprResult{node->setEvaluatedSymbolType(returnType, manIdx), anonymousSymbol};
}

std::string TypeChecker::visitOrdinaryFctCall(FctCallNode *node) {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Check if this is a ctor call to the String type
  if (node->functionNameFragments.size() == 1 && node->fqFunctionName == STROBJ_NAME && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  // Get struct name. Retrieve it from alias if required
  std::string fqFunctionName = node->fqFunctionName;
  SymbolTableEntry *aliasEntry = rootScope->lookupStrict(fqFunctionName);
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
  const bool isAliasType = aliasEntry && aliasEntry->getType().is(TY_ALIAS);
  if (isAliasType) {
    aliasedTypeContainerEntry = rootScope->lookupStrict(aliasEntry->name + ALIAS_CONTAINER_SUFFIX);
    assert(aliasedTypeContainerEntry != nullptr);
    // Set alias entry used
    aliasEntry->used = true;
    fqFunctionName = aliasedTypeContainerEntry->getType().getSubType();
  }

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (isAliasType) {
    // Retrieve concrete template types from type alias
    concreteTemplateTypes = aliasedTypeContainerEntry->getType().getTemplateTypes();
    // Check if the aliased type specified template types and the struct instantiation does
    if (!concreteTemplateTypes.empty() && node->hasTemplateTypes)
      SOFT_ERROR_STR(node->templateTypeLst(), ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list")
  }

  // Check if the exported name registry contains that function name
  const NameRegistryEntry *functionRegistryEntry = sourceFile->getNameRegistryEntry(fqFunctionName);
  if (!functionRegistryEntry)
    SOFT_ERROR_STR(node, REFERENCED_UNDEFINED_FUNCTION,
                   "Function/procedure/struct '" + node->functionNameFragments.back() + "' could not be found")
  SymbolTableEntry *functionEntry = functionRegistryEntry->targetEntry;

  // Check if the target symbol is a struct -> this must be a constructor call
  if (functionEntry != nullptr && functionEntry->getType().is(TY_STRUCT))
    data.callType = FctCallNode::TYPE_CTOR;

  // Get concrete template types
  if (node->hasTemplateTypes) {
    // Only constructors may have template types
    if (!data.isCtorCall())
      SOFT_ERROR_STR(node->templateTypeLst(), INVALID_TEMPLATE_TYPES, "Template types are only allowed for constructor calls")

    for (DataTypeNode *templateTypeNode : node->templateTypeLst()->dataTypes()) {
      auto templateType = std::any_cast<SymbolType>(visit(templateTypeNode));
      assert(!templateType.isOneOf({TY_DYN, TY_INVALID}));

      // Abort if the type is unresolved
      if (templateType.is(TY_UNRESOLVED))
        return UNRESOLVED_TYPE_NAME;

      // Check if the given type is generic
      if (templateType.is(TY_GENERIC))
        SOFT_ERROR_STR(templateTypeNode, EXPECTED_NON_GENERIC_TYPE, "You must specify a concrete type here")

      concreteTemplateTypes.push_back(templateType);
    }
  }

  // For constructor calls, do some preparation
  std::string knownStructName;
  std::string functionName = node->functionNameFragments.back();
  if (data.isCtorCall()) {
    const NameRegistryEntry *structRegistryEntry = functionRegistryEntry;
    const SymbolTableEntry *structEntry = functionEntry;
    const std::string structName = structRegistryEntry->targetEntry->name;

    // Substantiate potentially generic this struct
    Struct *thisStruct = StructManager::matchStruct(structEntry->scope, structName, concreteTemplateTypes, node);
    if (!thisStruct) {
      const std::string signature = Struct::getSignature(structName, concreteTemplateTypes);
      SOFT_ERROR_STR(node, UNKNOWN_DATATYPE,
                     "Could not find struct candidate for struct '" + signature + "'. Do the template types match?")
    }

    // Override function name
    functionName = CTOR_FUNCTION_NAME;

    // Retrieve the name registry entry for the constructor
    functionRegistryEntry = sourceFile->getNameRegistryEntry(fqFunctionName + MEMBER_ACCESS_TOKEN + functionName);
    // Check if the constructor was found
    if (!functionRegistryEntry)
      SOFT_ERROR_STR(node, REFERENCED_UNDEFINED_FUNCTION, "The struct '" + structName + "' does not provide a constructor")

    // Set the 'this' type of the function to the struct type
    data.thisType = structEntry->getType();
    data.thisType.setBodyScope(thisStruct->scope);

    // Get the fully qualified name, that can be used in the current source file to identify the struct
    knownStructName = structRegistryEntry->name;
  }

  // Attach the concrete template types to the 'this' type
  if (!data.thisType.is(TY_DYN) && !concreteTemplateTypes.empty())
    data.thisType.setTemplateTypes(concreteTemplateTypes);

  // Map local types to imported types
  data.calleeParentScope = functionRegistryEntry->targetScope;
  std::vector<SymbolType> localArgTypes = data.argTypes;
  for (SymbolType &localArgType : localArgTypes)
    localArgType = mapLocalTypeToImportedScopeType(data.calleeParentScope, localArgType);

  // Retrieve function object
  data.callee = FunctionManager::matchFunction(data.calleeParentScope, functionName, data.thisType, localArgTypes, false, node);

  return knownStructName;
}

bool TypeChecker::visitFctPtrCall(FctCallNode *node, const SymbolType &functionType) const {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Check if the given argument types match the type
  const std::vector<SymbolType> &actualArgTypes = data.argTypes;
  const std::vector<SymbolType> expectedArgTypes = functionType.getFunctionParamTypes();
  if (actualArgTypes.size() != expectedArgTypes.size())
    SOFT_ERROR_BOOL(node, REFERENCED_UNDEFINED_FUNCTION, "Expected and actual number of arguments do not match")

  // Create resolver function, that always returns a nullptr
  TypeMatcher::ResolverFct resolverFct = [](const std::string &genericTypeName) { return nullptr; };

  for (size_t i = 0; i < actualArgTypes.size(); i++) {
    const SymbolType &actualType = actualArgTypes.at(i);
    const SymbolType &expectedType = expectedArgTypes.at(i);
    TypeMapping tm;
    if (!TypeMatcher::matchRequestedToCandidateType(expectedType, actualType, tm, resolverFct, false))
      SOFT_ERROR_BOOL(node->argLst()->args().at(i), REFERENCED_UNDEFINED_FUNCTION,
                      "Expected " + expectedType.getName() + " but got " + actualType.getName())
  }
  return true;
}

bool TypeChecker::visitMethodCall(FctCallNode *node, Scope *structScope) const {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Methods cannot have template types
  if (node->hasTemplateTypes)
    SOFT_ERROR_BOOL(node->templateTypeLst(), INVALID_TEMPLATE_TYPES, "Template types are only allowed for constructor calls")

  // Traverse through structs - the first fragment is already looked up and the last one is the method name
  for (size_t i = 1; i < node->functionNameFragments.size() - 1; i++) {
    const std::string &identifier = node->functionNameFragments.at(i);

    // Retrieve field entry
    SymbolTableEntry *fieldEntry = structScope->lookupStrict(identifier);
    if (!fieldEntry)
      SOFT_ERROR_BOOL(node, ACCESS_TO_NON_EXISTING_MEMBER,
                      "The type " + data.thisType.getName() + " does not have a member with the name '" + identifier + "'")
    if (!fieldEntry->getType().isBaseType(TY_STRUCT))
      SOFT_ERROR_BOOL(node, INVALID_MEMBER_ACCESS, "Cannot call a method on '" + identifier + "', since it is no struct")
    fieldEntry->used = true;

    // Get struct type and scope
    data.thisType = fieldEntry->getType().getBaseType();
    structScope = data.thisType.getBodyScope();
    assert(structScope != nullptr);
  }

  // Map local types to imported types
  data.calleeParentScope = structScope;
  // Arg types
  std::vector<SymbolType> localArgTypes = data.argTypes;
  for (SymbolType &localArgType : localArgTypes)
    localArgType = mapLocalTypeToImportedScopeType(data.calleeParentScope, localArgType);
  // 'this' type
  SymbolType localThisType = data.thisType;
  autoDeReference(localThisType);
  localThisType = mapLocalTypeToImportedScopeType(data.calleeParentScope, localThisType);

  if (data.thisType.is(TY_INTERFACE))
    SOFT_ERROR_BOOL(node, INVALID_MEMBER_ACCESS, "Cannot call a method on an interface")

  // Retrieve function object
  const std::string &functionName = node->functionNameFragments.back();
  data.callee = FunctionManager::matchFunction(data.calleeParentScope, functionName, localThisType, localArgTypes, false, node);

  return true;
}

std::any TypeChecker::visitArrayInitialization(ArrayInitializationNode *node) {
  SymbolType actualItemType(TY_DYN);
  // Check if all values have the same type
  if (node->itemLst()) {
    node->actualSize = static_cast<long>(node->itemLst()->args().size());
    for (AssignExprNode *arg : node->itemLst()->args()) {
      const SymbolType itemType = std::any_cast<ExprResult>(visit(arg)).type;
      HANDLE_UNRESOLVED_TYPE_ER(itemType)
      if (actualItemType.is(TY_DYN)) // Perform type inference
        actualItemType = itemType;
      else if (itemType != actualItemType) // Check if types are matching
        SOFT_ERROR_ER(arg, ARRAY_ITEM_TYPE_NOT_MATCHING,
                      "All provided values have to be of the same data type. You provided " + actualItemType.getName() + " and " +
                          itemType.getName())
    }
  }

  const SymbolType arrayType = actualItemType.toArray(node, node->actualSize, true);
  return ExprResult{node->setEvaluatedSymbolType(arrayType, manIdx)};
}

std::any TypeChecker::visitStructInstantiation(StructInstantiationNode *node) {
  // Get struct name. Retrieve it from alias if required
  std::string structName = node->fqStructName;
  SymbolTableEntry *aliasEntry = rootScope->lookupStrict(structName);
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
  const bool isAliasType = aliasEntry && aliasEntry->getType().is(TY_ALIAS);
  if (isAliasType) {
    aliasedTypeContainerEntry = rootScope->lookupStrict(aliasEntry->name + ALIAS_CONTAINER_SUFFIX);
    assert(aliasedTypeContainerEntry != nullptr);
    // Set alias entry used
    aliasEntry->used = true;
    structName = aliasedTypeContainerEntry->getType().getSubType();
  }

  // Check if access scope was altered
  if (accessScope != nullptr && accessScope != currentScope)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + structName + "'")

  // Retrieve struct
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(structName);
  if (!registryEntry)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + structName + "'")
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *structEntry = registryEntry->targetEntry;
  Scope *structScope = accessScope = registryEntry->targetScope;

  // Get struct type and change it to the fully qualified name for identifying without ambiguities
  SymbolType structType = structEntry->getType().replaceBaseSubType(registryEntry->name);

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (isAliasType) {
    // Retrieve concrete template types from type alias
    concreteTemplateTypes = aliasedTypeContainerEntry->getType().getTemplateTypes();
    // Check if the aliased type specified template types and the struct instantiation does
    if (!concreteTemplateTypes.empty() && node->templateTypeLst())
      SOFT_ERROR_ER(node->templateTypeLst(), ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list")
  }

  if (node->templateTypeLst()) {
    concreteTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      auto concreteType = std::any_cast<SymbolType>(visit(dataType));
      HANDLE_UNRESOLVED_TYPE_ER(concreteType)
      // Check if generic type
      if (concreteType.is(TY_GENERIC))
        SOFT_ERROR_ER(dataType, EXPECTED_NON_GENERIC_TYPE, "Struct instantiations may only take concrete template types")
      concreteTemplateTypes.push_back(concreteType);
    }
  }

  // Get the struct instance
  structName = structEntry->name;
  Struct *spiceStruct = StructManager::matchStruct(structScope->parent, structName, concreteTemplateTypes, node);
  if (!spiceStruct) {
    const std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found")
  }
  node->instantiatedStructs.at(manIdx) = spiceStruct;

  // Use scope of concrete substantiation and not the scope of the generic type
  structScope = spiceStruct->scope;
  structType.setBodyScope(structScope);

  // Set template types to the struct
  std::vector<SymbolType> templateTypes;
  for (const GenericType &genericType : spiceStruct->templateTypes)
    templateTypes.emplace_back(genericType.typeChain);
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  if (node->fieldLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst()->args().size())
      SOFT_ERROR_ER(node->fieldLst(), NUMBER_OF_FIELDS_NOT_MATCHING,
                    "You've passed too less/many field values. Pass either none or all of them")

    // Check if the field types are matching
    const size_t fieldCount = spiceStruct->fieldTypes.size();
    const size_t explicitFieldsStartIdx = structScope->getFieldCount() - fieldCount;
    for (size_t i = 0; i < node->fieldLst()->args().size(); i++) {
      // Get actual type
      AssignExprNode *assignExpr = node->fieldLst()->args().at(i);
      auto fieldResult = std::any_cast<ExprResult>(visit(assignExpr));
      HANDLE_UNRESOLVED_TYPE_ER(fieldResult.type)
      // Get expected type
      const SymbolTableEntry *expectedField = structScope->symbolTable.lookupStrictByIndex(explicitFieldsStartIdx + i);
      assert(expectedField != nullptr);
      SymbolType expectedType = expectedField->getType();
      const bool rhsIsImmediate = assignExpr->hasCompileTimeValue();

      // Check if actual type matches expected type
      opRuleManager.getFieldAssignResultType(assignExpr, expectedType, fieldResult.type, 0, rhsIsImmediate);

      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      if (fieldResult.entry != nullptr && fieldResult.entry->anonymous) {
        currentScope->symbolTable.deleteAnonymous(fieldResult.entry->name);
        fieldResult.entry = nullptr;
      }
    }
  } else {
    for (SymbolType &fieldType : spiceStruct->fieldTypes) {
      if (fieldType.isRef())
        SOFT_ERROR_ER(node, REFERENCE_WITHOUT_INITIALIZER,
                      "The struct takes at least one reference field. You need to instantiate it with all fields.")
    }
  }

  // Update type of struct entry
  structEntry->updateType(structType, true);

  // If not all values are constant, insert anonymous symbol to keep track of dtor calls for de-allocation
  SymbolTableEntry *anonymousEntry = nullptr;
  if (node->fieldLst() != nullptr)
    if (std::ranges::any_of(node->fieldLst()->args(), [](AssignExprNode *field) { return !field->hasCompileTimeValue(); }))
      anonymousEntry = currentScope->symbolTable.insertAnonymous(structType, node);

  // Remove public specifier to not have public local variables
  structType.specifiers.isPublic = false;

  return ExprResult{node->setEvaluatedSymbolType(structType, manIdx), anonymousEntry};
}

std::any TypeChecker::visitLambdaFunc(LambdaFuncNode *node) {
  // Check if all control paths in the lambda body return
  if (!node->returnsOnAllControlPaths(nullptr))
    SOFT_ERROR_ER(node, MISSING_RETURN_STMT, "Not all control paths of this lambda function have a return statement")

  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit return type
  auto returnType = std::any_cast<SymbolType>(visit(node->returnType()));
  HANDLE_UNRESOLVED_TYPE_ST(returnType)
  if (returnType.is(TY_DYN))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed")

  // Set the type of the result variable
  SymbolTableEntry *resultVarEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultVarEntry != nullptr);
  resultVarEntry->updateType(returnType, true);
  resultVarEntry->used = true;

  // Visit parameters
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      if (param.isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(param.type);
      paramList.push_back({param.type, param.isOptional});
    }
  }

  // Visit lambda body
  visit(node->body());

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  SymbolType functionType(TY_FUNCTION);
  functionType.setFunctionReturnType(returnType);
  functionType.setFunctionParamTypes(paramTypes);
  functionType.setHasLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, SymbolType(TY_DYN), returnType, paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

std::any TypeChecker::visitLambdaProc(LambdaProcNode *node) {
  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit parameters
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      if (param.isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(param.type);
      paramList.push_back({param.type, param.isOptional});
    }
  }

  // Visit lambda body
  visit(node->body());

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  SymbolType functionType(TY_PROCEDURE);
  functionType.setFunctionParamTypes(paramTypes);
  functionType.setHasLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, SymbolType(TY_DYN), SymbolType(TY_DYN), paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

std::any TypeChecker::visitLambdaExpr(LambdaExprNode *node) {
  // Change to function scope
  Scope *bodyScope = currentScope->getChildScope(node->getScopeId());
  ScopeHandle scopeHandle(this, bodyScope, ScopeType::LAMBDA_BODY);

  // Visit parameters
  std::vector<SymbolType> paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    // Visit param list to retrieve the param names
    auto namedParamList = std::any_cast<NamedParamList>(visit(node->paramLst()));
    for (const NamedParam &param : namedParamList) {
      if (param.isOptional)
        softError(node, LAMBDA_WITH_OPTIONAL_PARAMS, "Lambdas cannot have optional parameters");

      paramTypes.push_back(param.type);
      paramList.push_back({param.type, param.isOptional});
    }
  }

  // Visit lambda expression
  SymbolType returnType = std::any_cast<ExprResult>(visit(node->lambdaExpr())).type;
  HANDLE_UNRESOLVED_TYPE_ER(returnType)
  if (returnType.is(TY_DYN))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Dyn return types are not allowed")

  // Leave function body scope
  scopeHandle.leaveScopeEarly();

  // Prepare type of function
  const bool isFunction = !returnType.is(TY_DYN);
  SymbolType functionType(isFunction ? TY_FUNCTION : TY_PROCEDURE);
  if (isFunction)
    functionType.setFunctionReturnType(returnType);
  functionType.setFunctionParamTypes(paramTypes);
  functionType.setHasLambdaCaptures(!bodyScope->symbolTable.captures.empty());

  // Create function object
  const std::string fctName = "lambda." + node->codeLoc.toPrettyLineAndColumn();
  node->manifestations.at(manIdx) = Function(fctName, nullptr, SymbolType(TY_DYN), returnType, paramList, {}, node);
  node->manifestations.at(manIdx).bodyScope = bodyScope;
  node->manifestations.at(manIdx).mangleSuffix = "." + std::to_string(manIdx);

  return ExprResult{node->setEvaluatedSymbolType(functionType, manIdx)};
}

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  // Visit base data type
  auto type = std::any_cast<SymbolType>(visit(node->baseDataType()));
  HANDLE_UNRESOLVED_TYPE_ST(type)

  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    DataTypeNode::TypeModifier typeModifier = tmQueue.front();

    // Only the outermost array can have an unknown size
    if (type.isArray() && type.getArraySize() == ARRAY_SIZE_UNKNOWN)
      SOFT_ERROR_ST(node, ARRAY_SIZE_INVALID,
                    "Usage of incomplete array type. Only the outermost array type may have unknown size")

    switch (typeModifier.modifierType) {
    case DataTypeNode::TYPE_PTR: {
      type = type.toPointer(node);
      break;
    }
    case DataTypeNode::TYPE_REF: {
      type = type.toReference(node);
      break;
    }
    case DataTypeNode::TYPE_ARRAY: {
      const std::string &varName = typeModifier.sizeVarName;
      if (!varName.empty()) {
        SymbolTableEntry *globalVar = rootScope->lookupStrict(varName);
        if (!globalVar)
          SOFT_ERROR_ST(node, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + varName + "' ")
        if (!globalVar->getType().isConst())
          SOFT_ERROR_ST(node, EXPECTED_CONST_VARIABLE, "The size of the array must be known at compile time")
        if (!globalVar->getType().is(TY_INT))
          SOFT_ERROR_ST(node, OPERATOR_WRONG_DATA_TYPE, "Expected variable of type int")
        typeModifier.hardcodedSize = globalVar->declNode->getCompileTimeValue().intValue;
      }

      if (typeModifier.hasSize && typeModifier.hardcodedSize <= 1)
        SOFT_ERROR_ST(node, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated")
      type = type.toArray(node, typeModifier.hardcodedSize);
      break;
    }
    default:                                                               // GCOV_EXCL_LINE
      throw CompilerError(UNHANDLED_BRANCH, "Modifier type fall-through"); // GCOV_EXCL_LINE
    }
    tmQueue.pop();
  }

  // Attach the specifiers to the type
  if (node->specifierLst()) {
    const SymbolType baseType = type.getBaseType();
    for (const SpecifierNode *specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST) {
        type.specifiers.isConst = true;
      } else if (specifier->type == SpecifierNode::TY_SIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_ST(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on type " + baseType.getName())
        type.specifiers.isSigned = true;
        type.specifiers.isUnsigned = false;
      } else if (specifier->type == SpecifierNode::TY_UNSIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_ST(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on type " + baseType.getName())
        type.specifiers.isSigned = false;
        type.specifiers.isUnsigned = true;
      } else if (specifier->type == SpecifierNode::TY_HEAP) {
        // Heap variables can only be pointers
        if (!type.removeReferenceWrapper().isOneOf({TY_PTR, TY_STRING}))
          SOFT_ERROR_ST(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT,
                        "The heap specifier can only be applied to symbols of pointer type, you provided " + baseType.getName())

        type.specifiers.isHeap = true;
      } else if (specifier->type == SpecifierNode::TY_COMPOSITION && node->isFieldType) {
        if (!type.is(TY_STRUCT))
          SOFT_ERROR_ST(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "The compose specifier can only be used on plain struct fields")
        type.specifiers.isComposition = true;
      } else if (specifier->type == SpecifierNode::TY_PUBLIC && (node->isFieldType || node->isGlobalType)) {
        type.specifiers.isPublic = true;
      } else {
        const char *entryName = "local variable";
        if (node->isGlobalType)
          entryName = "global variable";
        else if (node->isFieldType)
          entryName = "field";
        else if (node->isParamType)
          entryName = "param";
        else if (node->isReturnType)
          entryName = "return variable";
        SOFT_ERROR_ST(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT,
                      "Cannot use this specifier on a " + std::string(entryName) + " definition")
      }
    }
  }

  return node->setEvaluatedSymbolType(type, manIdx);
}

std::any TypeChecker::visitBaseDataType(BaseDataTypeNode *node) {
  switch (node->type) {
  case BaseDataTypeNode::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(SymbolType(TY_DOUBLE), manIdx);
  case BaseDataTypeNode::TYPE_INT:
    return node->setEvaluatedSymbolType(SymbolType(TY_INT), manIdx);
  case BaseDataTypeNode::TYPE_SHORT:
    return node->setEvaluatedSymbolType(SymbolType(TY_SHORT), manIdx);
  case BaseDataTypeNode::TYPE_LONG:
    return node->setEvaluatedSymbolType(SymbolType(TY_LONG), manIdx);
  case BaseDataTypeNode::TYPE_BYTE:
    return node->setEvaluatedSymbolType(SymbolType(TY_BYTE), manIdx);
  case BaseDataTypeNode::TYPE_CHAR:
    return node->setEvaluatedSymbolType(SymbolType(TY_CHAR), manIdx);
  case BaseDataTypeNode::TYPE_STRING:
    return node->setEvaluatedSymbolType(SymbolType(TY_STRING), manIdx);
  case BaseDataTypeNode::TYPE_BOOL:
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL), manIdx);
  case BaseDataTypeNode::TYPE_CUSTOM: {
    auto customType = std::any_cast<SymbolType>(visit(node->customDataType()));
    HANDLE_UNRESOLVED_TYPE_ST(customType)
    return node->setEvaluatedSymbolType(customType, manIdx);
  }
  case BaseDataTypeNode::TYPE_FUNCTION: {
    auto functionType = std::any_cast<SymbolType>(visit(node->functionDataType()));
    HANDLE_UNRESOLVED_TYPE_ST(functionType)
    return node->setEvaluatedSymbolType(functionType, manIdx);
  }
  default:
    return node->setEvaluatedSymbolType(SymbolType(TY_DYN), manIdx);
  }
}

std::any TypeChecker::visitCustomDataType(CustomDataTypeNode *node) {
  // It is a struct type -> get the access scope
  Scope *localAccessScope = accessScope ?: currentScope;

  const bool isImported = node->typeNameFragments.size() > 1;
  const std::string firstFragment = node->typeNameFragments.front();

  // Check if it is a String type
  if (!isImported && firstFragment == STROBJ_NAME && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check if it is a generic type
  const SymbolType *genericType = rootScope->lookupGenericType(firstFragment);
  if (!isImported && genericType) {
    // Take the concrete replacement type for the name of this generic type if available
    SymbolType symbolType = *genericType;
    if (typeMapping.contains(firstFragment))
      symbolType = typeMapping.at(firstFragment);
    return node->setEvaluatedSymbolType(symbolType, manIdx);
  }

  // Check if it is a type alias
  SymbolTableEntry *aliasEntry = rootScope->lookupStrict(firstFragment);
  if (!isImported && aliasEntry && aliasEntry->getType().is(TY_ALIAS)) {
    // Set alias entry used
    aliasEntry->used = true;
    // Get type of aliased type container entry
    const std::string aliasedContainerEntryName = aliasEntry->name + ALIAS_CONTAINER_SUFFIX;
    SymbolTableEntry *aliasedTypeContainerEntry = rootScope->lookupStrict(aliasedContainerEntryName);
    assert(aliasedTypeContainerEntry != nullptr);
    return node->setEvaluatedSymbolType(aliasedTypeContainerEntry->getType(), manIdx);
  }

  // Check if the type exists in the exported names registry
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqTypeName);
  if (!registryEntry)
    SOFT_ERROR_ST(node, UNKNOWN_DATATYPE, "Unknown datatype '" + node->fqTypeName + "'")
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *entry = registryEntry->targetEntry;
  assert(entry != nullptr);
  localAccessScope = registryEntry->targetScope->parent;

  // Get struct type and change it to the fully qualified name for identifying without ambiguities
  SymbolType entryType = entry->getType().replaceBaseSubType(registryEntry->name);

  // Enums can early-return
  if (entryType.is(TY_ENUM))
    return SymbolType(TY_INT);

  if (entryType.isOneOf({TY_STRUCT, TY_INTERFACE})) {
    const DataTypeNode *dataTypeNode = dynamic_cast<DataTypeNode *>(node->parent->parent);
    assert(dataTypeNode != nullptr);
    const bool isParamOrFieldOrReturnType = dataTypeNode->isParamType || dataTypeNode->isFieldType || dataTypeNode->isReturnType;

    // Collect the concrete template types
    bool allTemplateTypesConcrete = true;
    std::vector<SymbolType> templateTypes;
    if (node->templateTypeLst()) {
      templateTypes.reserve(node->templateTypeLst()->dataTypes().size());
      for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
        auto templateType = std::any_cast<SymbolType>(visit(dataType));
        HANDLE_UNRESOLVED_TYPE_ST(templateType)
        // Generic types are only allowed for parameters and fields at this point
        if (entryType.is(TY_STRUCT) && templateType.is(TY_GENERIC) && !isParamOrFieldOrReturnType)
          SOFT_ERROR_ST(dataType, EXPECTED_NON_GENERIC_TYPE, "Only concrete template types are allowed here")
        if (entryType.is(TY_GENERIC))
          allTemplateTypesConcrete = false;
        templateTypes.push_back(templateType);
      }
      entryType.setTemplateTypes(templateTypes);
    }

    if (entryType.is(TY_STRUCT)) {
      // Check if struct is defined before the current code location, if defined in the same source file
      if (entry->declNode->codeLoc.sourceFilePath == node->codeLoc.sourceFilePath && entry->declNode->codeLoc > node->codeLoc)
        SOFT_ERROR_ST(node, REFERENCED_UNDEFINED_STRUCT, "Structs must be defined before usage")

      if (allTemplateTypesConcrete || !isParamOrFieldOrReturnType) { // Only do the next step, if we have concrete template types
        // Set the struct instance to used, if found
        // Here, it is allowed to accept, that the struct cannot be found, because there are self-referencing structs
        const std::string structName = node->typeNameFragments.back();
        Struct *spiceStruct = StructManager::matchStruct(localAccessScope, structName, templateTypes, node);
        if (spiceStruct)
          entryType.setBodyScope(spiceStruct->scope);
      }
    }

    if (entryType.is(TY_INTERFACE)) {
      // Set the interface instance to used, if found
      const std::string interfaceName = node->typeNameFragments.back();
      Interface *spiceInterface = InterfaceManager::matchInterface(localAccessScope, interfaceName, templateTypes, node);
      if (!spiceInterface)
        SOFT_ERROR_ST(node, UNKNOWN_DATATYPE, "Unknown interface " + Interface::getSignature(interfaceName, templateTypes))
      entryType.setBodyScope(spiceInterface->scope);
    }

    // Remove public specifier
    entryType.specifiers.isPublic = false;

    return node->setEvaluatedSymbolType(entryType, manIdx);
  }

  const std::string errorMessage =
      entryType.is(TY_INVALID) ? "Used type before declared" : "Expected type, but got " + entryType.getName();
  SOFT_ERROR_ST(node, EXPECTED_TYPE, errorMessage)
}

std::any TypeChecker::visitFunctionDataType(FunctionDataTypeNode *node) {
  // Visit return type
  SymbolType returnType(TY_DYN);
  if (node->isFunction) {
    returnType = std::any_cast<SymbolType>(visit(node->returnType()));
    HANDLE_UNRESOLVED_TYPE_ST(returnType)
    if (returnType.is(TY_DYN))
      SOFT_ERROR_ER(node->returnType(), UNEXPECTED_DYN_TYPE, "Function types cannot have return type dyn")
  }

  // Visit param types
  std::vector<SymbolType> paramTypes;
  if (const TypeLstNode *paramTypeListNode = node->paramTypeLst(); paramTypeListNode != nullptr) {
    for (DataTypeNode *paramTypeNode : paramTypeListNode->dataTypes()) {
      auto paramType = std::any_cast<SymbolType>(visit(paramTypeNode));
      HANDLE_UNRESOLVED_TYPE_ST(returnType)
      paramTypes.push_back(paramType);
    }
  }

  // Build function type
  SymbolType functionType(node->isFunction ? TY_FUNCTION : TY_PROCEDURE);
  if (node->isFunction)
    functionType.setFunctionReturnType(returnType);
  functionType.setFunctionParamTypes(paramTypes);

  return node->setEvaluatedSymbolType(functionType, manIdx);
}

SymbolType TypeChecker::mapLocalTypeToImportedScopeType(const Scope *targetScope, const SymbolType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBaseType(TY_STRUCT))
    return symbolType;

  // If the target scope is in the current source file, we can return the symbol type as is
  SourceFile *targetSourceFile = targetScope->sourceFile;
  if (targetSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of the target source file
  for (const auto &[_, entry] : targetSourceFile->exportedNameRegistry)
    if (entry.targetEntry != nullptr && entry.targetEntry->getType().isBaseType(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == symbolType.getBaseType().getBodyScope())
          return symbolType.replaceBaseSubType(manifestation->name);

  // The target source file does not know about the struct at all
  // -> show it how to find the struct
  const std::string structName = symbolType.getBaseType().getSubType();
  const NameRegistryEntry *origRegistryEntry = sourceFile->getNameRegistryEntry(structName);
  assert(origRegistryEntry != nullptr);
  targetSourceFile->addNameRegistryEntry(structName, origRegistryEntry->targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

SymbolType TypeChecker::mapImportedScopeTypeToLocalType(const Scope *sourceScope, const SymbolType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBaseType(TY_STRUCT))
    return symbolType;

  // If the source scope is in the current source file, we can return the symbol type as is
  SourceFile *sourceSourceFile = sourceScope->sourceFile;
  if (sourceSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of this source file
  const SymbolType baseType = symbolType.getBaseType();
  for (const auto &[_, entry] : sourceFile->exportedNameRegistry)
    if (entry.targetEntry != nullptr && entry.targetEntry->getType().isBaseType(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == baseType.getBodyScope()) {
          // Get the 'fullest-qualified' registry entry
          const NameRegistryEntry *mostQualifiedEntry = sourceFile->getNameRegistryEntry(entry.name);
          assert(mostQualifiedEntry != nullptr);
          return symbolType.replaceBaseSubType(mostQualifiedEntry->name);
        }

  // This source file does not know about the struct at all
  // -> show it how to find the struct
  const NameRegistryEntry *origRegistryEntry = sourceSourceFile->getNameRegistryEntry(baseType.getOriginalSubType());
  assert(origRegistryEntry != nullptr);
  sourceFile->addNameRegistryEntry(baseType.getSubType(), origRegistryEntry->targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

/**
 * Auto-dereference the given symbol type.
 * This process is NOT equivalent with getBaseType() because getBaseType() also removes e.g. array wrappers
 *
 * @param symbolType Input symbol type
 */
void TypeChecker::autoDeReference(SymbolType &symbolType) {
  while (symbolType.isPtr() || symbolType.isRef())
    symbolType = symbolType.getContainedTy();
}

/**
 * Returns the operator function list for the current manifestation and the given node
 *
 * @param node Node to retrieve the op fct pointer list from
 * @return Op fct pointer list
 */
std::vector<const Function *> &TypeChecker::getOpFctPointers(ASTNode *node) const {
  assert(node->opFct.size() > manIdx);
  return node->opFct.at(manIdx);
}

/**
 * Check if a function has been type-checked already. If not, request a revisit
 *
 * @param function Function to check
 */
void TypeChecker::requestRevisitIfRequired(const Function *fct) {
  if (fct && !fct->alreadyTypeChecked && !fct->entry->scope->isImportedBy(rootScope))
    reVisitRequested = true;
}

/**
 * Add a soft error to the error list
 */
void TypeChecker::softError(const ASTNode *node, SemanticErrorType errorType, const std::string &message) const {
  resourceManager.errorManager.addSoftError(node, errorType, message);
}

} // namespace spice::compiler