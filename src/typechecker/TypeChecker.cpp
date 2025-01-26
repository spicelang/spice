// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
#include <ast/Attributes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/ScopeHandle.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/MacroDefs.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), warnings(sourceFile->compilerOutput.warnings) {
}

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Initialize AST nodes with size of 1
  const bool isPrepare = typeCheckerMode == TC_MODE_PRE;
  if (isPrepare)
    node->resizeToNumberOfManifestations(1);

  // Visit children
  visitChildren(node);

  // Check which implicit structures we need for each struct, defined in this source file
  if (isPrepare) {
    const std::vector<const Struct *> manifestations = rootScope->getAllStructManifestationsInDeclarationOrder();
    for (const Struct *manifestation : manifestations) {
      // Check if we need to create a default ctor, copy ctor or dtor
      createDefaultCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultCopyCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultDtorIfRequired(*manifestation, manifestation->scope);
    }
  }

  return nullptr;
}

std::any TypeChecker::visitMainFctDef(MainFctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitMainFctDefPrepare(node);
  else
    return visitMainFctDefCheck(node);
}

std::any TypeChecker::visitFctDef(FctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitFctDefPrepare(node);
  else
    return visitFctDefCheck(node);
}

std::any TypeChecker::visitProcDef(ProcDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitProcDefPrepare(node);
  else
    return visitProcDefCheck(node);
}

std::any TypeChecker::visitStructDef(StructDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitStructDefPrepare(node);
  else
    return visitStructDefCheck(node);
}

std::any TypeChecker::visitInterfaceDef(InterfaceDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitInterfaceDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitEnumDef(EnumDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitEnumDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGenericTypeDef(GenericTypeDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitGenericTypeDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitAliasDef(AliasDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitAliasDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitGlobalVarDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitExtDecl(ExtDeclNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitExtDeclPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitImportDef(ImportDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitImportDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitUnsafeBlock(UnsafeBlockNode *node) {
  // Change to unsafe block body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::UNSAFE_BODY);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitForLoop(ForLoopNode *node) {
  // Change to for body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::FOR_BODY);

  // Visit loop variable declaration
  visit(node->initDecl);

  // Visit condition
  const QualType conditionType = std::any_cast<ExprResult>(visit(node->condAssign)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condAssign, CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool")

  // Visit incrementer
  visit(node->incAssign);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitForeachLoop(ForeachLoopNode *node) {
  // Visit iterator assignment
  AssignExprNode *iteratorNode = node->iteratorAssign;
  QualType iteratorOrIterableType = std::any_cast<ExprResult>(visit(iteratorNode)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(iteratorOrIterableType)
  iteratorOrIterableType = iteratorOrIterableType.removeReferenceWrapper();

  // Retrieve iterator type
  QualType iteratorType = iteratorOrIterableType;

  if (iteratorOrIterableType.isIterable(node)) {
    const QualType &iterableType = iteratorOrIterableType;
    if (iteratorOrIterableType.isArray()) { // Array
      const NameRegistryEntry *nameRegistryEntry = sourceFile->getNameRegistryEntry(ARRAY_ITERATOR_NAME);
      if (!nameRegistryEntry) {
        softError(node, UNKNOWN_DATATYPE, "Forgot to import \"std/iterator/array-iterator\"?");
        return nullptr;
      }
      nameRegistryEntry->targetEntry->used = nameRegistryEntry->importEntry->used = true;
      Scope *matchScope = nameRegistryEntry->targetScope->parent;
      assert(matchScope->type == ScopeType::GLOBAL);
      QualType unsignedLongType(TY_LONG);
      unsignedLongType.makeUnsigned(true);
      const ArgList argTypes = {Arg(iterableType, false), Arg(unsignedLongType, false)};
      const QualType thisType(TY_DYN);
      node->getIteratorFct = FunctionManager::match(this, matchScope, "iterate", thisType, argTypes, {}, true, iteratorNode);
    } else { // Struct, implementing Iterator interface
      Scope *matchScope = iterableType.getBodyScope();
      node->getIteratorFct = FunctionManager::match(this, matchScope, "getIterator", iterableType, {}, {}, true, iteratorNode);
    }
    if (node->getIteratorFct == nullptr)
      throw SemanticError(iteratorNode, INVALID_ITERATOR, "No getIterator() function found for the given iterable type");

    iteratorType = QualType(node->getIteratorFct->returnType);
    // Create anonymous entry for the iterator
    currentScope->symbolTable.insertAnonymous(iteratorType, iteratorNode);
  }

  // Change to foreach body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::FOREACH_BODY);

  // Check iterator type
  if (!iteratorType.isIterator(node)) {
    const std::string errMsg =
        "Can only iterate over arrays or data structures, inheriting from IIterator or IIterable. You provided " +
        iteratorType.getName(false);
    softError(node->iteratorAssign, OPERATOR_WRONG_DATA_TYPE, errMsg);
    return nullptr;
  }
  const QualTypeList &iteratorTemplateTypes = iteratorType.getTemplateTypes();
  if (iteratorTemplateTypes.empty())
    SOFT_ERROR_ER(node->iteratorAssign, INVALID_ITERATOR,
                  "Iterator has no generic arguments so that the item type could not be inferred")

  const bool hasIdx = node->idxVarDecl;
  if (hasIdx) {
    // Visit index declaration or assignment
    auto indexType = std::any_cast<QualType>(visit(node->idxVarDecl));
    HANDLE_UNRESOLVED_TYPE_PTR(indexType)
    // Check if index type is int
    if (!indexType.is(TY_LONG))
      SOFT_ERROR_ER(node->idxVarDecl, FOREACH_IDX_NOT_LONG,
                    "Index in foreach loop must be of type long. You provided " + indexType.getName(false))
  }

  // Retrieve .get(), .getIdx(), .isValid() and .next() functions
  Scope *matchScope = iteratorType.getBodyScope();
  QualType iteratorItemType;
  if (hasIdx) {
    node->getIdxFct = FunctionManager::match(this, matchScope, "getIdx", iteratorType, {}, {}, false, node);
    assert(node->getIdxFct != nullptr);
    iteratorItemType = node->getIdxFct->returnType.getTemplateTypes().back();
  } else {
    node->getFct = FunctionManager::match(this, matchScope, "get", iteratorType, {}, {}, false, node);
    assert(node->getFct != nullptr);
    iteratorItemType = node->getFct->returnType;
  }
  node->isValidFct = FunctionManager::match(this, matchScope, "isValid", iteratorType, {}, {}, false, node);
  assert(node->isValidFct != nullptr);
  node->nextFct = FunctionManager::match(this, matchScope, "next", iteratorType, {}, {}, false, node);
  assert(node->nextFct != nullptr);

  // Retrieve item variable entry
  SymbolTableEntry *itemVarSymbol = currentScope->lookupStrict(node->itemVarDecl->varName);
  assert(itemVarSymbol != nullptr);

  // Check type of the item
  auto itemType = std::any_cast<QualType>(visit(node->itemVarDecl));
  HANDLE_UNRESOLVED_TYPE_PTR(itemType)
  if (itemType.is(TY_DYN)) { // Perform type inference
    // Update evaluated symbol type of the declaration data type
    node->itemVarDecl->dataType->setEvaluatedSymbolType(iteratorItemType, manIdx);
    // Update item type
    itemType = iteratorItemType;
  } else {
    // Check item type
    const ExprResult itemResult = {itemType, itemVarSymbol};
    const ExprResult iteratorItemResult = {iteratorItemType, nullptr /* always a temporary */};
    (void)opRuleManager.getAssignResultType(node->itemVarDecl, itemResult, iteratorItemResult, true, false, ERROR_FOREACH_ITEM);
  }

  // Update type of item
  itemVarSymbol->updateType(itemType, true);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitWhileLoop(WhileLoopNode *node) {
  // Change to while body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::WHILE_BODY);

  // Visit condition
  const QualType conditionType = std::any_cast<ExprResult>(visit(node->condition)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition, CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool")

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Change to while body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::WHILE_BODY);

  // Visit body
  visit(node->body);

  // Visit condition
  const QualType conditionType = std::any_cast<ExprResult>(visit(node->condition)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition, CONDITION_MUST_BE_BOOL, "Do-While loop condition must be of type bool")

  return nullptr;
}

std::any TypeChecker::visitIfStmt(IfStmtNode *node) {
  // Change to then body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::IF_ELSE_BODY);

  // Visit condition
  const QualType conditionType = std::any_cast<ExprResult>(visit(node->condition)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(conditionType)
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition, CONDITION_MUST_BE_BOOL, "If condition must be of type bool")

  // Warning for bool assignment
  if (node->condition->op == AssignExprNode::AssignOp::OP_ASSIGN)
    sourceFile->compilerOutput.warnings.emplace_back(node->condition->codeLoc, BOOL_ASSIGN_AS_CONDITION,
                                                     "If you want to compare the values, use '=='");

  // Visit body
  visit(node->thenBody);

  // Leave then body scope
  scopeHandle.leaveScopeEarly();

  // Visit else statement if existing
  if (node->elseStmt)
    visit(node->elseStmt);

  return nullptr;
}

std::any TypeChecker::visitElseStmt(ElseStmtNode *node) {
  // Visit if statement in the case of an else if branch
  if (node->isElseIf) {
    visit(node->ifStmt);
    return nullptr;
  }

  // Change to else body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::IF_ELSE_BODY);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitSwitchStmt(SwitchStmtNode *node) {
  // Check expression type
  const QualType exprType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_PTR(exprType)
  if (!exprType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}))
    SOFT_ERROR_ER(node->assignExpr, SWITCH_EXPR_MUST_BE_PRIMITIVE,
                  "Switch expression must be of int, short, long, byte, char or bool type")

  // Visit children
  visitChildren(node);

  // Check if case constant types match switch expression type
  for (const CaseBranchNode *caseBranchNode : node->caseBranches)
    for (CaseConstantNode *constantNode : caseBranchNode->caseConstants) {
      const QualType constantType = std::any_cast<ExprResult>(visit(constantNode)).type;
      if (!constantType.matches(exprType, false, true, true))
        SOFT_ERROR_ER(constantNode, SWITCH_CASE_TYPE_MISMATCH, "Case value type does not match the switch expression type")
    }

  return nullptr;
}

std::any TypeChecker::visitCaseBranch(CaseBranchNode *node) {
  // Change to case body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::CASE_BODY);

  // Visit constant list
  for (CaseConstantNode *constant : node->caseConstants)
    visit(constant);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitDefaultBranch(DefaultBranchNode *node) {
  // Change to default body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::DEFAULT_BODY);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Change to anonymous scope body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::ANONYMOUS_BLOCK_BODY);

  // Visit body
  visit(node->body);

  return nullptr;
}

std::any TypeChecker::visitStmtLst(StmtLstNode *node) {
  // Visit nodes in this scope
  for (StmtNode *stmt : node->statements) {
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

  for (DeclStmtNode *param : node->params) {
    // Visit param
    const auto paramType = std::any_cast<QualType>(visit(param));
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
    namedParams.push_back({param->varName.c_str(), paramType, metOptional});
  }

  return namedParams;
}

std::any TypeChecker::visitField(FieldNode *node) {
  auto fieldType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_QT(fieldType)

  if (TernaryExprNode *defaultValueNode = node->defaultValue) {
    const QualType defaultValueType = std::any_cast<ExprResult>(visit(defaultValueNode)).type;
    HANDLE_UNRESOLVED_TYPE_QT(defaultValueType)
    if (!fieldType.matches(defaultValueType, false, true, true))
      SOFT_ERROR_QT(node, FIELD_TYPE_NOT_MATCHING, "Type of the default values does not match the field type")
  }

  return fieldType;
}

std::any TypeChecker::visitSignature(SignatureNode *node) {
  const bool isFunction = node->signatureType == SignatureNode::SignatureType::TYPE_FUNCTION;

  // Retrieve function template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC)) {
        softError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        return static_cast<std::vector<Function *> *>(nullptr);
      }
      // Convert generic symbol type to generic type
      const GenericType *genericType = rootScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Visit return type
  QualType returnType(TY_DYN);
  if (isFunction) {
    returnType = std::any_cast<QualType>(visit(node->returnType));
    if (returnType.is(TY_UNRESOLVED))
      return static_cast<std::vector<Function *> *>(nullptr);

    if (!returnType.isCoveredByGenericTypeList(usedGenericTypes))
      softError(node->returnType, GENERIC_TYPE_NOT_IN_TEMPLATE,
                "Generic return type not included in the template type list of the function");
  }

  // Visit params
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    paramList.reserve(node->paramTypeLst->dataTypes.size());
    for (DataTypeNode *param : node->paramTypeLst->dataTypes) {
      auto paramType = std::any_cast<QualType>(visit(param));
      if (paramType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);

      // Check if the type is present in the template for generic types
      if (!paramType.isCoveredByGenericTypeList(usedGenericTypes)) {
        softError(node->paramTypeLst, GENERIC_TYPE_NOT_IN_TEMPLATE,
                  "Generic param type not included in the template type list of the function");
        continue;
      }

      paramTypes.push_back(paramType);
      paramList.push_back({paramType, false});
    }
  }

  // Build signature object
  const Function signature(node->methodName, nullptr, QualType(TY_DYN), returnType, paramList, usedGenericTypes, node);

  // Add signature to current scope
  Function *manifestation = FunctionManager::insert(currentScope, signature, &node->signatureManifestations);
  manifestation->entry = node->entry;
  manifestation->used = true;

  // Prepare signature type
  const SuperType superType = isFunction ? TY_FUNCTION : TY_PROCEDURE;
  QualType signatureType = QualType(superType).getWithFunctionParamAndReturnTypes(returnType, paramTypes);
  signatureType.setSpecifiers(node->signatureSpecifiers);

  // Set entry to signature type
  assert(node->entry != nullptr);
  node->entry->updateType(signatureType, false);
  node->entry->used = true;

  return &node->signatureManifestations;
}

std::any TypeChecker::visitDeclStmt(DeclStmtNode *node) {
  // Retrieve entry of the lhs variable
  SymbolTableEntry *localVarEntry = currentScope->lookupStrict(node->varName);
  assert(localVarEntry != nullptr);

  QualType localVarType;
  if (node->hasAssignment) {
    // Visit the right side
    auto rhs = std::any_cast<ExprResult>(visit(node->assignExpr));
    auto [rhsTy, rhsEntry] = rhs;

    // Visit data type
    localVarType = std::any_cast<QualType>(visit(node->dataType));

    // Check if type has to be inferred or both types are fixed
    if (!localVarType.is(TY_UNRESOLVED) && !rhsTy.is(TY_UNRESOLVED)) {
      const ExprResult lhsResult = {localVarType, localVarEntry};
      const auto [type, copyCtor] = opRuleManager.getAssignResultType(node, lhsResult, rhs, true);
      localVarType = type;
      node->calledCopyCtor = copyCtor;

      // If this is a struct type, check if the type is known. If not, error out
      if (localVarType.isBase(TY_STRUCT) && !sourceFile->getNameRegistryEntry(localVarType.getBase().getSubType())) {
        const std::string structName = localVarType.getBase().getSubType();
        softError(node->dataType, UNKNOWN_DATATYPE, "Unknown struct type '" + structName + "'. Forgot to import?");
        localVarType = QualType(TY_UNRESOLVED);
      }
    } else {
      localVarType = QualType(TY_UNRESOLVED);
    }

    // If there is an anonymous entry attached (e.g. for struct instantiation) and we take over ownership, delete it
    if (!localVarType.isRef() && rhsEntry != nullptr && rhsEntry->anonymous) {
      currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
      rhs.entry = rhsEntry = nullptr;
    }
  } else {
    // Visit data type
    localVarType = std::any_cast<QualType>(visit(node->dataType));

    // References with no initialization are illegal
    if (localVarType.isRef() && !node->isFctParam && !node->isForEachItem)
      softError(node, REFERENCE_WITHOUT_INITIALIZER, "References must always be initialized directly");

    // If this is a struct, check for the default ctor
    if (localVarType.is(TY_STRUCT) && !node->isFctParam && !node->isForEachItem) {
      Scope *matchScope = localVarType.getBodyScope();
      assert(matchScope != nullptr);
      // Check if we are required to call a ctor
      auto structDeclNode = spice_pointer_cast<StructDefNode *>(localVarType.getStruct(node)->declNode);
      node->isCtorCallRequired = matchScope->hasRefFields() || structDeclNode->emitVTable;
      // Check if we have a no-args ctor to call
      const std::string &structName = localVarType.getSubType();
      const QualType &thisType = localVarType;
      node->calledInitCtor = FunctionManager::match(this, matchScope, CTOR_FUNCTION_NAME, thisType, {}, {}, false, node);
      if (!node->calledInitCtor && node->isCtorCallRequired)
        SOFT_ERROR_QT(node, MISSING_NO_ARGS_CTOR, "Struct '" + structName + "' misses a no-args constructor")
    }
  }

  // Update the type of the variable
  localVarEntry->updateType(localVarType, true);
  node->entries.at(manIdx) = localVarEntry;

  // Update the state of the variable
  localVarEntry->updateState(INITIALIZED, node);

  return node->setEvaluatedSymbolType(localVarType, manIdx);
}

std::any TypeChecker::visitCaseConstant(CaseConstantNode *node) {
  // If we have a normal constant, we can take the symbol type from there
  if (node->constant)
    return visit(node->constant);

  // Check if a local or global variable can be found by searching for the name
  if (node->identifierFragments.size() == 1)
    node->enumItemEntry = currentScope->lookup(node->identifierFragments.back());

  // If no local or global was found, search in the name registry
  if (!node->enumItemEntry) {
    const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqIdentifier);
    if (!registryEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found")
    node->enumItemEntry = registryEntry->targetEntry;
  }
  assert(node->enumItemEntry != nullptr);

  // Check for the correct type
  if (node->enumItemEntry->scope->type != ScopeType::ENUM)
    SOFT_ERROR_ER(node, CASE_CONSTANT_NOT_ENUM, "Case constants must be of type enum")

  const QualType varType = node->enumItemEntry->getQualType();
  assert(varType.is(TY_INT));
  return ExprResult{node->setEvaluatedSymbolType(varType, manIdx)};
}

std::any TypeChecker::visitReturnStmt(ReturnStmtNode *node) {
  // Retrieve return variable entry
  SymbolTableEntry *returnVar = currentScope->lookup(RETURN_VARIABLE_NAME);
  const bool isFunction = returnVar != nullptr;
  const QualType returnType = isFunction ? returnVar->getQualType() : QualType(TY_DYN);

  // Check if procedure with return value
  if (!isFunction) {
    if (node->hasReturnValue)
      SOFT_ERROR_ER(node->assignExpr, RETURN_WITH_VALUE_IN_PROCEDURE, "Return with value in procedure is not allowed")
    return nullptr;
  }

  if (!node->hasReturnValue && !returnVar->getLifecycle().isInitialized())
    SOFT_ERROR_QT(node, RETURN_WITHOUT_VALUE_RESULT, "Return without value, but result variable is not initialized yet")

  if (!node->hasReturnValue)
    return nullptr;

  // Visit right side
  const auto rhs = std::any_cast<ExprResult>(visit(node->assignExpr));
  HANDLE_UNRESOLVED_TYPE_QT(rhs.type)

  // Check if types match
  const ExprResult returnResult = {returnType, returnVar};
  auto [_, copyCtor] = opRuleManager.getAssignResultType(node->assignExpr, returnResult, rhs, false, true, ERROR_MSG_RETURN);
  node->calledCopyCtor = copyCtor;

  // Check if the dtor call on the return value can be skipped
  if (rhs.entry != nullptr) {
    if (rhs.entry->anonymous) {
      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      currentScope->symbolTable.deleteAnonymous(rhs.entry->name);
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
  const unsigned int maxBreaks = currentScope->getLoopNestingDepth();
  if (static_cast<unsigned int>(node->breakTimes) > maxBreaks)
    SOFT_ERROR_ER(node, INVALID_BREAK_NUMBER, "We can only break " + std::to_string(maxBreaks) + " time(s) here")

  return nullptr;
}

std::any TypeChecker::visitContinueStmt(ContinueStmtNode *node) {
  // Check if the stated number is valid
  if (node->continueTimes < 1)
    SOFT_ERROR_ER(node, INVALID_CONTINUE_NUMBER,
                  "Continue count must be >= 1, you provided " + std::to_string(node->continueTimes))

  // Check if we can continue this often
  const unsigned int maxContinues = currentScope->getLoopNestingDepth();
  if (static_cast<unsigned int>(node->continueTimes) > maxContinues)
    SOFT_ERROR_ER(node, INVALID_CONTINUE_NUMBER, "We can only continue " + std::to_string(maxContinues) + " time(s) here")

  return nullptr;
}

std::any TypeChecker::visitFallthroughStmt(FallthroughStmtNode *node) {
  // Check if we can do a fallthrough here
  if (!currentScope->isInCaseBranch())
    SOFT_ERROR_ER(node, FALLTHROUGH_NOT_ALLOWED, "Fallthrough is only allowed in case branches")

  return nullptr;
}

std::any TypeChecker::visitAssertStmt(AssertStmtNode *node) {
  // Visit condition
  const QualType conditionType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(conditionType)

  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    SOFT_ERROR_ER(node->assignExpr, ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool")

  return nullptr;
}

std::any TypeChecker::visitBuiltinCall(BuiltinCallNode *node) {
  if (node->printfCall)
    return visitPrintfCall(node->printfCall);
  if (node->sizeofCall)
    return visitSizeofCall(node->sizeofCall);
  if (node->alignofCall)
    return visitAlignofCall(node->alignofCall);
  if (node->lenCall)
    return visitLenCall(node->lenCall);
  if (node->panicCall)
    return visitPanicCall(node->panicCall);
  if (node->sysCall)
    return visitSysCall(node->sysCall);
  assert_fail("Unknown builtin call");
  return nullptr;
}

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = node->templatedString.find_first_of('%');
  while (index != std::string::npos && index != node->templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->args.size() <= placeholderCount)
      SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments")

    // Get next assignment
    AssignExprNode *assignment = node->args.at(placeholderCount);
    // Visit assignment
    QualType argType = std::any_cast<ExprResult>(visit(assignment)).type;
    HANDLE_UNRESOLVED_TYPE_ER(argType)
    argType = argType.removeReferenceWrapper();

    switch (node->templatedString.at(index + 1)) {
    case 'c': {
      if (!argType.is(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects char, but got " + argType.getName(false))
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
                      "The placeholder string expects int, short, long, byte or bool, but got " + argType.getName(false))
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
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects double, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 's': {
      if (!argType.is(TY_STRING) && !argType.isStringObj() && !argType.isPtrTo(TY_CHAR) && !argType.isArrayOf(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects string, String, char* or char[], but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!argType.isPtr() && !argType.isArray() && !argType.is(TY_STRING))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects pointer, array or string, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    default:
      SOFT_ERROR_ER(node, PRINTF_TYPE_ERROR, "The placeholder string contains an invalid placeholder")
    }
    index = node->templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->args.size())
    SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_INT), manIdx)};
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  if (node->isType) { // Size of type
    visit(node->dataType);
  } else { // Size of value
    visit(node->assignExpr);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitAlignofCall(AlignofCallNode *node) {
  if (node->isType) { // Align of type
    visit(node->dataType);
  } else { // Align of value
    visit(node->assignExpr);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  QualType argType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isArray() && !argType.is(TY_STRING))
    SOFT_ERROR_ER(node->assignExpr, EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays or strings")

  // If we want to use the len builtin on a string, we need to import the string runtime module
  if (argType.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitPanicCall(PanicCallNode *node) {
  QualType argType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isErrorObj())
    SOFT_ERROR_ER(node->assignExpr, EXPECTED_ERROR_TYPE, "The panic builtin can only work with errors")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx)};
}

std::any TypeChecker::visitSysCall(SysCallNode *node) {
  // Check if the syscall number if of type short
  const QualType sysCallNumberType = std::any_cast<ExprResult>(visit(node->args.front())).type;
  if (!sysCallNumberType.is(TY_SHORT))
    SOFT_ERROR_ER(node->args.front(), INVALID_SYSCALL_NUMBER_TYPE, "Syscall number must be of type short")

  // Check if the syscall number is out of range
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->hasCompileTimeValue()) {
    const unsigned short sysCallNumber = node->getCompileTimeValue().shortValue;
    if (sysCallNumber < 0 || sysCallNumber > 439)
      SOFT_ERROR_ER(node, SYSCALL_NUMBER_OUT_OF_RANGE, "Only syscall numbers between 0 and 439 are supported")
  }

  // Check if too many syscall args are given
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->args.size() > 6)
    SOFT_ERROR_ER(node->args.front(), TOO_MANY_SYSCALL_ARGS, "There are no syscalls that support more than 6 arguments")

  // Visit children
  for (size_t i = 1; i < node->args.size(); i++)
    visit(node->args.at(i));

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if ternary
  if (node->ternaryExpr) {
    auto result = std::any_cast<ExprResult>(visit(node->ternaryExpr));
    node->setEvaluatedSymbolType(result.type, manIdx);
    return result;
  }

  // Check if assignment
  if (node->op != AssignExprNode::AssignOp::OP_NONE) {
    // Visit the right side first
    auto rhs = std::any_cast<ExprResult>(visit(node->rhs));
    auto [rhsType, rhsEntry] = rhs;
    HANDLE_UNRESOLVED_TYPE_ER(rhsType)
    // Then visit the left side
    auto lhs = std::any_cast<ExprResult>(visit(node->lhs));
    auto [lhsType, lhsVar] = lhs;
    HANDLE_UNRESOLVED_TYPE_ER(lhsType)

    // Take a look at the operator
    if (node->op == AssignExprNode::AssignOp::OP_ASSIGN) {
      const bool isDecl = lhs.entry->isField() && !lhs.entry->getLifecycle().isInitialized();
      rhsType = opRuleManager.getAssignResultType(node, lhs, rhs, isDecl).first;

      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      if (rhsEntry != nullptr && rhsEntry->anonymous) {
        currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
        rhsEntry = nullptr;
      }
    } else if (node->op == AssignExprNode::AssignOp::OP_PLUS_EQUAL) {
      rhsType = opRuleManager.getPlusEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_MINUS_EQUAL) {
      rhsType = opRuleManager.getMinusEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_MUL_EQUAL) {
      rhsType = opRuleManager.getMulEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_DIV_EQUAL) {
      rhsType = opRuleManager.getDivEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_REM_EQUAL) {
      rhsType = opRuleManager.getRemEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_SHL_EQUAL) {
      rhsType = opRuleManager.getSHLEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_SHR_EQUAL) {
      rhsType = opRuleManager.getSHREqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_AND_EQUAL) {
      rhsType = opRuleManager.getAndEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_OR_EQUAL) {
      rhsType = opRuleManager.getOrEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_XOR_EQUAL) {
      rhsType = opRuleManager.getXorEqualResultType(node, lhs, rhs);
    }

    if (lhsVar) { // Variable is involved on the left side
      // Perform type inference
      if (lhsType.is(TY_DYN))
        lhsVar->updateType(rhsType, false);

      // In case the lhs variable is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsVar->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      lhsVar->updateState(INITIALIZED, node);
    }

    return ExprResult{node->setEvaluatedSymbolType(rhsType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "AssignExpr fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitTernaryExpr(TernaryExprNode *node) {
  // Check if there is a ternary operator applied
  if (!node->falseExpr)
    return visit(node->condition);

  // Visit condition
  const auto condition = std::any_cast<ExprResult>(visit(node->condition));
  HANDLE_UNRESOLVED_TYPE_ER(condition.type)
  const auto trueExpr = node->isShortened ? condition : std::any_cast<ExprResult>(visit(node->trueExpr));
  const auto [trueType, trueEntry] = trueExpr;
  HANDLE_UNRESOLVED_TYPE_ER(trueType)
  const auto falseExpr = std::any_cast<ExprResult>(visit(node->falseExpr));
  const auto [falseType, falseEntry] = falseExpr;
  HANDLE_UNRESOLVED_TYPE_ER(falseType)

  // Check if the condition evaluates to bool
  if (!condition.type.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool")

  // Check if trueType and falseType are matching
  const QualType trueTypeModified = trueType.removeReferenceWrapper();
  const QualType falseTypeModified = falseType.removeReferenceWrapper();
  if (!trueTypeModified.matches(falseTypeModified, false, true, false))
    SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                  "True and false operands in ternary must be of same data type. Got " + trueType.getName(true) + " and " +
                      falseType.getName(true))

  // If there is an anonymous symbol attached to left or right, remove it,
  // since the result takes over the ownership of any destructible object.
  const bool removeAnonymousSymbolTrueSide = trueEntry && trueEntry->anonymous;
  if (removeAnonymousSymbolTrueSide) {
    currentScope->symbolTable.deleteAnonymous(trueEntry->name);
  } else if (trueEntry && !trueEntry->anonymous && !trueType.isRef() && !trueType.isTriviallyCopyable(node)) {
    node->trueSideCallsCopyCtor = true;
  }
  const bool removeAnonymousSymbolFalseSide = falseEntry && falseEntry->anonymous;
  if (removeAnonymousSymbolFalseSide) {
    currentScope->symbolTable.deleteAnonymous(falseEntry->name);
  } else if (falseEntry && !falseEntry->anonymous && !falseType.isRef() && !falseType.isTriviallyCopyable(node)) {
    node->falseSideCallsCopyCtor = true;
  }

  // Create a new anonymous symbol for the result if required
  const QualType &resultType = trueType;
  SymbolTableEntry *anonymousSymbol = nullptr;
  const bool removedAnonymousSymbols = removeAnonymousSymbolTrueSide || removeAnonymousSymbolFalseSide;
  const bool calledCopyCtor = node->trueSideCallsCopyCtor || node->falseSideCallsCopyCtor;
  if (removedAnonymousSymbols || calledCopyCtor || resultType.isRef())
    anonymousSymbol = currentScope->symbolTable.insertAnonymous(resultType, node);

  // Lookup copy ctor, if at least one side needs it
  if (node->trueSideCallsCopyCtor || node->falseSideCallsCopyCtor)
    node->calledCopyCtor = matchCopyCtor(trueTypeModified, node);

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx), anonymousSymbol};
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getLogicalOrResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getLogicalAndResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseOrResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseXorResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseAndResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitEqualityExpr(EqualityExprNode *node) {
  // Check if at least one equality operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit right side first, then left side
  const auto rhs = std::any_cast<ExprResult>(visit(node->operands[1]));
  HANDLE_UNRESOLVED_TYPE_ER(rhs.type)
  const auto lhs = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(lhs.type)

  // Check if we need the string runtime to perform a string comparison
  if (lhs.type.is(TY_STRING) && rhs.type.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check operator
  ExprResult result;
  if (node->op == EqualityExprNode::EqualityOp::OP_EQUAL) // Operator was equal
    result = opRuleManager.getEqualResultType(node, lhs, rhs, 0);
  else if (node->op == EqualityExprNode::EqualityOp::OP_NOT_EQUAL) // Operator was not equal
    result = opRuleManager.getNotEqualResultType(node, lhs, rhs, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE

  node->setEvaluatedSymbolType(result.type, manIdx);
  return result;
}

std::any TypeChecker::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit right side first, then left side
  const auto rhs = std::any_cast<ExprResult>(visit(node->operands[1]));
  HANDLE_UNRESOLVED_TYPE_ER(rhs.type)
  const auto lhs = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(lhs.type)

  // Check operator
  QualType resultType;
  if (node->op == RelationalExprNode::RelationalOp::OP_LESS) // Operator was less
    resultType = OpRuleManager::getLessResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_GREATER) // Operator was greater
    resultType = OpRuleManager::getGreaterResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_LESS_EQUAL) // Operator was less equal
    resultType = OpRuleManager::getLessEqualResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_GREATER_EQUAL) // Operator was greater equal
    resultType = OpRuleManager::getGreaterEqualResultType(node, lhs, rhs);
  else
    throw CompilerError(UNHANDLED_BRANCH, "RelationalExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitShiftExpr(ShiftExprNode *node) {
  // Check if at least one shift operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)

  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const ShiftExprNode::ShiftOp &op = node->opQueue.front().first;
    if (op == ShiftExprNode::ShiftOp::OP_SHIFT_LEFT)
      currentResult = opRuleManager.getShiftLeftResultType(node, currentResult, operandResult, i);
    else if (op == ShiftExprNode::ShiftOp::OP_SHIFT_RIGHT)
      currentResult = opRuleManager.getShiftRightResultType(node, currentResult, operandResult, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)

  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const AdditiveExprNode::AdditiveOp &op = node->opQueue.front().first;
    if (op == AdditiveExprNode::AdditiveOp::OP_PLUS)
      currentResult = opRuleManager.getPlusResultType(node, currentResult, operandResult, i);
    else if (op == AdditiveExprNode::AdditiveOp::OP_MINUS)
      currentResult = opRuleManager.getMinusResultType(node, currentResult, operandResult, i);
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
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)
  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const MultiplicativeExprNode::MultiplicativeOp &op = node->opQueue.front().first;
    if (op == MultiplicativeExprNode::MultiplicativeOp::OP_MUL)
      currentResult = opRuleManager.getMulResultType(node, currentResult, operandResult, i);
    else if (op == MultiplicativeExprNode::MultiplicativeOp::OP_DIV)
      currentResult = opRuleManager.getDivResultType(node, currentResult, operandResult, i);
    else if (op == MultiplicativeExprNode::MultiplicativeOp::OP_REM)
      currentResult = OpRuleManager::getRemResultType(node, currentResult, operandResult);
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
    return visit(node->prefixUnaryExpr);

  // Visit source type
  const auto src = std::any_cast<ExprResult>(visit(node->prefixUnaryExpr));
  HANDLE_UNRESOLVED_TYPE_ER(src.type)
  // Visit destination type
  const auto dstType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_ER(dstType)

  // Check for identity cast
  if (src.type == dstType) {
    const CompilerWarning warning(node->codeLoc, IDENTITY_CAST, "You cast from a type to itself. Thus, this can be simplified.");
    sourceFile->compilerOutput.warnings.push_back(warning);
  }

  // Get result type
  const QualType resultType = opRuleManager.getCastResultType(node, dstType, src);

  SymbolTableEntry *entry = src.type.isSameContainerTypeAs(dstType) ? src.entry : nullptr;
  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx), entry};
}

std::any TypeChecker::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  // Reset access scope
  accessScope = nullptr;

  // If no operator is applied, simply visit the postfix unary expression
  if (node->op == PrefixUnaryExprNode::PrefixUnaryOp::OP_NONE)
    return visit(node->postfixUnaryExpr);

  // Visit the right side
  PrefixUnaryExprNode *rhsNode = node->prefixUnaryExpr;
  auto operand = std::any_cast<ExprResult>(visit(rhsNode));
  auto [operandType, operandEntry] = operand;
  HANDLE_UNRESOLVED_TYPE_ER(operandType)
  // Determine action, based on the given operator
  switch (node->op) {
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS:
    operandType = OpRuleManager::getPrefixMinusResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_PLUS_PLUS:
    operandType = opRuleManager.getPrefixPlusPlusResultType(node, operand);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS_MINUS:
    operandType = opRuleManager.getPrefixMinusMinusResultType(node, operand);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_NOT:
    operandType = OpRuleManager::getPrefixNotResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_BITWISE_NOT:
    operandType = OpRuleManager::getPrefixBitwiseNotResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_DEREFERENCE:
    operandType = OpRuleManager::getPrefixMulResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_ADDRESS_OF:
    operandType = OpRuleManager::getPrefixBitwiseAndResultType(node, operand);
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType, manIdx), operandEntry};
}

std::any TypeChecker::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  // If no operator is applied, simply visit the atomic expression
  if (node->op == PostfixUnaryExprNode::PostfixUnaryOp::OP_NONE)
    return visit(node->atomicExpr);

  // Visit left side
  PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr;
  auto operand = std::any_cast<ExprResult>(visit(lhsNode));
  auto [operandType, operandEntry] = operand;
  HANDLE_UNRESOLVED_TYPE_ER(operandType)

  switch (node->op) {
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_SUBSCRIPT: {
    // Visit index assignment
    AssignExprNode *indexAssignExpr = node->subscriptIndexExpr;
    const auto index = std::any_cast<ExprResult>(visit(indexAssignExpr));
    HANDLE_UNRESOLVED_TYPE_ER(index.type)
    // Check if the index is of the right type
    if (!index.type.isOneOf({TY_INT, TY_LONG}))
      SOFT_ERROR_ER(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long")

    // Check is there is an overloaded operator function available, if yes accept it
    const auto [type, _] = opRuleManager.isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SUBSCRIPT, {operand, index}, 0);
    if (!type.is(TY_INVALID)) {
      operandType = type;
      break;
    }

    operandType = operandType.removeReferenceWrapper();

    // Check if we can apply the subscript operator on the lhs type
    if (!operandType.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
      SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                    "Can only apply subscript operator on array type, got " + operandType.getName(true))

    // Check if we have an unsafe operation
    if (operandType.isPtr() && !currentScope->doesAllowUnsafeOperations())
      SOFT_ERROR_ER(
          node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
          "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.")

    // Check if we have a hardcoded array index
    if (operandType.isArray() && operandType.getArraySize() != ARRAY_SIZE_UNKNOWN && indexAssignExpr->hasCompileTimeValue()) {
      const int32_t constIndex = indexAssignExpr->getCompileTimeValue().intValue;
      const unsigned int constSize = operandType.getArraySize();
      // Check if we are accessing out-of-bounds memory
      if (constIndex >= static_cast<int32_t>(constSize)) {
        const std::string idxStr = std::to_string(constIndex);
        const std::string sizeStr = std::to_string(constSize);
        SOFT_ERROR_ER(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                      "You are trying to access element with index " + idxStr + " of an array with size " + sizeStr)
      }
    }

    // Get item type
    operandType = operandType.getContained();

    // Remove heap specifier
    operandType.getSpecifiers().isHeap = false;

    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_MEMBER_ACCESS: {
    const std::string &fieldName = node->identifier;

    // Check if lhs is enum or strobj
    QualType lhsBaseTy = operandType;
    autoDeReference(lhsBaseTy);
    if (!lhsBaseTy.is(TY_STRUCT))
      SOFT_ERROR_ER(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + operandType.getName(false))

    // Retrieve registry entry
    const std::string &structName = lhsBaseTy.getSubType();
    Scope *structScope = lhsBaseTy.getBodyScope();

    // If we only have the generic struct scope, lookup the concrete manifestation scope
    if (structScope->isGenericScope) {
      Scope *matchScope = structScope->parent;
      Struct *spiceStruct = StructManager::match(matchScope, structName, lhsBaseTy.getTemplateTypes(), node);
      assert(spiceStruct != nullptr);
      structScope = spiceStruct->scope;
    }
    assert(!structScope->isGenericScope); // At this point we always expect a substantiation scope

    // Get accessed field
    std::vector<size_t> indexPath;
    SymbolTableEntry *memberEntry = structScope->symbolTable.lookupInComposedFields(fieldName, indexPath);
    if (!memberEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Field '" + node->identifier + "' not found in struct " + structName)
    const QualType memberType = memberEntry->getQualType();

    // Check for insufficient visibility
    if (structScope->isImportedBy(rootScope) && !memberEntry->getQualType().getBase().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access field '" + fieldName + "' due to its private visibility")

    // Set field to used
    memberEntry->used = true;

    // Overwrite type and entry of left side with member type and entry
    operandType = memberType;
    operandEntry = memberEntry;
    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_PLUS_PLUS: {
    operandType = opRuleManager.getPostfixPlusPlusResultType(node, operand, 0).type;

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_MINUS_MINUS: {
    operandType = opRuleManager.getPostfixMinusMinusResultType(node, operand, 0).type;

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  }
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  if (operandType.is(TY_INVALID)) {
    const std::string varName = operandEntry ? operandEntry->name : "";
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + varName + "' was referenced before declared")
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType, manIdx), operandEntry};
}

std::any TypeChecker::visitAtomicExpr(AtomicExprNode *node) {
  // Check if constant
  if (node->constant)
    return visit(node->constant);

  // Check if value
  if (node->value)
    return visit(node->value);

  // Check for builtin calls
  if (node->builtinCall)
    return visit(node->builtinCall);

  // Check for assign expression within parentheses
  if (node->assignExpr)
    return visit(node->assignExpr);

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
  AtomicExprNode::VarAccessData &data = node->data.at(manIdx);
  data = {varEntry, accessScope, accessScope->symbolTable.lookupCapture(varEntry->name)};
  const QualType varType = varEntry->getQualType();
  HANDLE_UNRESOLVED_TYPE_ER(varType)

  if (varType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && varEntry->global) {
    // Check if overloaded function was referenced
    const std::vector<Function *> *manifestations = varEntry->declNode->getFctManifestations(varEntry->name);
    if (manifestations->size() > 1)
      SOFT_ERROR_ER(node, REFERENCED_OVERLOADED_FCT, "Overloaded functions / functions with optional params cannot be referenced")
    if (!manifestations->front()->templateTypes.empty())
      SOFT_ERROR_ER(node, REFERENCED_OVERLOADED_FCT, "Generic functions cannot be referenced")
    // Set referenced function to used
    Function *referencedFunction = manifestations->front();
    referencedFunction->used = true;
    referencedFunction->entry->used = true;
  }

  if (varType.is(TY_INVALID))
    SOFT_ERROR_ER(node, USED_BEFORE_DECLARED, "Symbol '" + varEntry->name + "' was used before declared.")

  // The base type should be an extended primitive
  const QualType baseType = varType.getBase();
  if (!baseType.isExtendedPrimitive() && !baseType.is(TY_DYN))
    SOFT_ERROR_ER(node, INVALID_SYMBOL_ACCESS, "A symbol of type " + varType.getName(false) + " cannot be accessed here")

  // Check if is an imported variable
  if (accessScope->isImportedBy(rootScope)) {
    // Check if the entry is public
    if (varEntry->scope->type != ScopeType::ENUM && !varEntry->getQualType().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access '" + varEntry->name + "' due to its private visibility")
  }

  // Check if we have seen a 'this.' prefix, because the generator needs that
  if (varEntry->scope->type == ScopeType::STRUCT && node->identifierFragments.front() != THIS_VARIABLE_NAME)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE,
                  "The symbol '" + node->fqIdentifier + "' could not be found. Missing 'this.' prefix?")

  // Set symbol table entry to used
  varEntry->used = true;

  // Retrieve scope for the new scope path fragment
  if (baseType.is(TY_STRUCT)) {
    // Set access scope to struct scope
    const std::string &structName = baseType.getSubType();
    const NameRegistryEntry *nameRegistryEntry = sourceFile->getNameRegistryEntry(structName);
    assert(nameRegistryEntry != nullptr);

    // Change the access scope to the struct scope
    data.accessScope = accessScope = nameRegistryEntry->targetScope;

    // Check if the entry is public if it is imported
    assert(nameRegistryEntry->targetEntry != nullptr);
    if (!nameRegistryEntry->targetEntry->getQualType().isPublic() && accessScope->parent->isImportedBy(rootScope))
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access struct '" + structName + "' due to its private visibility")
  }

  return ExprResult{node->setEvaluatedSymbolType(varType, manIdx), varEntry};
}

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
  case ConstantNode::PrimitiveValueType::TYPE_BYTE:
    superType = TY_BYTE;
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

  // Create symbol type
  QualType symbolType(superType);
  symbolType.setSpecifiers(TypeSpecifiers::of(superType));

  return ExprResult{node->setEvaluatedSymbolType(symbolType, manIdx)};
}

std::any TypeChecker::visitFctCall(FctCallNode *node) {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Retrieve arg types
  data.argResults.clear();
  if (node->hasArgs) {
    data.argResults.reserve(node->argLst->args.size());
    for (AssignExprNode *arg : node->argLst->args) {
      // Visit argument
      const auto argResult = std::any_cast<ExprResult>(visit(arg));
      HANDLE_UNRESOLVED_TYPE_ER(argResult.type)
      assert(!argResult.type.hasAnyGenericParts());
      // Save arg type to arg types list
      data.argResults.push_back(argResult);
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
        data.callType = FctCallNode::FctCallType::TYPE_CTOR;
      else
        data.callType = FctCallNode::FctCallType::TYPE_METHOD;
    } else if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && firstFragEntry->scope->type != ScopeType::GLOBAL) {
      data.callType = FctCallNode::FctCallType::TYPE_FCT_PTR;
    }
  }

  // Get struct name. Retrieve it from alias if required
  std::string fqFunctionName = node->fqFunctionName;
  SymbolTableEntry *aliasEntry = rootScope->lookupStrict(fqFunctionName);
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
  const bool isAliasType = aliasEntry && aliasEntry->getQualType().is(TY_ALIAS);
  if (isAliasType) {
    aliasedTypeContainerEntry = rootScope->lookupStrict(aliasEntry->name + ALIAS_CONTAINER_SUFFIX);
    assert(aliasedTypeContainerEntry != nullptr);
    // Set alias entry used
    aliasEntry->used = true;
    fqFunctionName = aliasedTypeContainerEntry->getQualType().getSubType();
  }

  // Get the concrete template types
  QualTypeList concreteTemplateTypes;
  if (isAliasType) {
    // Retrieve concrete template types from type alias
    concreteTemplateTypes = aliasedTypeContainerEntry->getQualType().getTemplateTypes();
    // Check if the aliased type specified template types and the struct instantiation does
    if (!concreteTemplateTypes.empty() && node->hasTemplateTypes)
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

      concreteTemplateTypes.push_back(templateType);
    }
  }

  // Check if this is a method call or a normal function call
  if (data.isMethodCall()) {
    // This is a method call
    data.thisType = firstFragEntry->getQualType();
    Scope *structBodyScope = data.thisType.getBase().getBodyScope();
    assert(structBodyScope != nullptr);
    if (bool success = visitMethodCall(node, structBodyScope, concreteTemplateTypes); !success) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
    assert(data.calleeParentScope != nullptr);
  } else if (data.isFctPtrCall()) {
    // This is a function pointer call
    const QualType &functionType = firstFragEntry->getQualType().getBase();
    assert(functionType.isOneOf({TY_FUNCTION, TY_PROCEDURE}));
    bool success = visitFctPtrCall(node, functionType);
    if (!success) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
  } else {
    // This is an ordinary function call
    assert(data.isOrdinaryCall() || data.isCtorCall());
    bool success = visitOrdinaryFctCall(node, concreteTemplateTypes, fqFunctionName);
    if (!success) // Check if soft errors occurred
      return ExprResult{node->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};
    assert(data.calleeParentScope != nullptr);

    // If the call is no ordinary call, it must be a constructor, which takes a struct as this type.
    assert(data.isOrdinaryCall() || data.thisType.is(TY_STRUCT));
  }

  if (!data.isFctPtrCall()) {
    // Check if we were able to find a function
    if (!data.callee) {
      // Build error message
      const std::string functionName = data.isCtorCall() ? CTOR_FUNCTION_NAME : node->functionNameFragments.back();
      ParamList errArgTypes;
      errArgTypes.reserve(data.argResults.size());
      for (const auto &[type, _] : data.argResults)
        errArgTypes.push_back({type, false});
      const std::string signature = Function::getSignature(functionName, data.thisType, QualType(TY_DYN), errArgTypes, {}, false);
      // Throw error
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure '" + signature + "' could not be found")
    }

    // Check if we need to request a re-visit, because the function body was not type-checked yet
    requestRevisitIfRequired(data.callee);

    // Get function entry from function object
    SymbolTableEntry *functionEntry = data.callee->entry;

    // Check if the called function has sufficient visibility
    data.isImported = data.calleeParentScope->isImportedBy(rootScope);
    if (data.isImported && !functionEntry->getQualType().isPublic()) {
      const QualType functionEntryType = functionEntry->getQualType();
      const std::string signature = data.callee->getSignature();
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
      assert(data.callee != nullptr);
      paramTypes = data.callee->getParamTypes();
    }

    node->argLst->argInfos.clear();
    for (size_t argIdx = 0; argIdx < data.argResults.size(); argIdx++) {
      const QualType &expectedType = paramTypes.at(argIdx);
      const auto &[actualType, entry] = data.argResults.at(argIdx);

      Function *copyCtor = nullptr;
      if (expectedType.is(TY_STRUCT) && actualType.is(TY_STRUCT) && !actualType.isTriviallyCopyable(node)) {
        copyCtor = matchCopyCtor(actualType, node);
        currentScope->symbolTable.insertAnonymous(actualType, node, argIdx + 1); // +1 because 0 is reserved for return value
      }

      node->argLst->argInfos.push_back(ArgLstNode::ArgInfo{copyCtor});
    }
    assert(node->argLst->argInfos.size() == node->argLst->args.size());
  }

  // Retrieve return type
  const bool isFct = data.isFctPtrCall() ? firstFragEntry->getQualType().getBase().is(TY_FUNCTION) : data.callee->isFunction();
  QualType returnType;
  if (data.isFctPtrCall()) {
    returnType = isFct ? firstFragEntry->getQualType().getBase().getFunctionReturnType() : QualType(TY_BOOL);
  } else if (data.isCtorCall()) {
    // Set return type to 'this' type
    returnType = data.thisType;
  } else if (data.callee->isProcedure()) {
    // Procedures always have the return type 'dyn'
    returnType = QualType(TY_DYN);
  } else {
    returnType = data.callee->returnType;
  }

  // Initialize return type if required
  SymbolTableEntry *anonymousSymbol = nullptr;
  if (returnType.isBase(TY_STRUCT)) {
    QualType returnBaseType = returnType.getBase();
    const std::string &structName = returnBaseType.getSubType();
    Scope *matchScope = returnBaseType.getBodyScope()->parent;
    assert(matchScope != nullptr);
    Struct *spiceStruct = StructManager::match(matchScope, structName, returnBaseType.getTemplateTypes(), node);
    if (!spiceStruct) {
      const std::string signature = Struct::getSignature(structName, returnBaseType.getTemplateTypes());
      SOFT_ERROR_ER(node, UNKNOWN_DATATYPE,
                    "Could not find struct candidate for struct '" + signature + "'. Do the template types match?")
    }
    returnType = returnType.getWithBodyScope(spiceStruct->scope).replaceBaseType(returnBaseType);
    returnType = mapImportedScopeTypeToLocalType(returnType.getBase().getBodyScope(), returnType);

    // Add anonymous symbol to keep track of de-allocation
    if (returnType.is(TY_STRUCT))
      anonymousSymbol = currentScope->symbolTable.insertAnonymous(returnType, node);
  }

  // Remove public specifier to not have public local variables
  returnType.getSpecifiers().isPublic = false;

  // Check if the return value gets discarded
  if (isFct && !node->hasReturnValueReceiver()) {
    // Check if we want to ignore the discarded return value
    bool ignoreUnusedReturnValue = false;
    if (!data.isFctPtrCall()) {
      assert(data.callee != nullptr);
      auto fctDef = dynamic_cast<const FctDefNode *>(data.callee->declNode);
      ignoreUnusedReturnValue = fctDef && fctDef->attrs && fctDef->attrs->attrLst->hasAttr(ATTR_IGNORE_UNUSED_RETURN_VALUE);
    }

    if (!ignoreUnusedReturnValue)
      warnings.emplace_back(node->codeLoc, UNUSED_RETURN_VALUE, "The return value of the function call is unused");
  }

  return ExprResult{node->setEvaluatedSymbolType(returnType, manIdx), anonymousSymbol};
}

bool TypeChecker::visitOrdinaryFctCall(FctCallNode *node, QualTypeList &templateTypes, std::string fqFunctionName) {
  FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Check if this is a ctor call to the String type
  if (node->functionNameFragments.size() == 1) {
    for (const auto &[typeName, runtimeModule] : TYPE_NAME_TO_RT_MODULE_MAPPING)
      if (fqFunctionName == typeName && !sourceFile->isRT(runtimeModule))
        sourceFile->requestRuntimeModule(runtimeModule);
    for (const auto &[fctName, runtimeModule] : FCT_NAME_TO_RT_MODULE_MAPPING)
      if (fqFunctionName == fctName && !sourceFile->isRT(runtimeModule))
        sourceFile->requestRuntimeModule(runtimeModule);
  }

  // Check if the type is generic (possible in case of ctor call)
  const QualType *genericType = rootScope->lookupGenericType(fqFunctionName);
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
  data.calleeParentScope = functionRegistryEntry->targetScope;

  // Check if the target symbol is a struct -> this must be a constructor call
  if (functionEntry != nullptr && functionEntry->getQualType().is(TY_STRUCT))
    data.callType = FctCallNode::FctCallType::TYPE_CTOR;

  // For constructor calls, do some preparation
  std::string functionName = node->functionNameFragments.back();
  if (data.isCtorCall()) {
    const NameRegistryEntry *structRegistryEntry = functionRegistryEntry;
    const SymbolTableEntry *structEntry = functionEntry;
    const std::string structName = structRegistryEntry->targetEntry->name;

    // Substantiate potentially generic this struct
    const Struct *thisStruct = StructManager::match(structEntry->scope, structName, templateTypes, node);
    if (!thisStruct) {
      const std::string signature = Struct::getSignature(structName, templateTypes);
      SOFT_ERROR_BOOL(node, UNKNOWN_DATATYPE,
                      "Could not find struct candidate for struct '" + signature + "'. Do the template types match?")
    }

    // Override function name
    functionName = CTOR_FUNCTION_NAME;

    // Set the 'this' type of the function to the struct type
    data.thisType = structEntry->getQualType().getWithBodyScope(thisStruct->scope);
    data.calleeParentScope = thisStruct->scope;
  }

  // Attach the concrete template types to the 'this' type
  if (!data.thisType.is(TY_DYN) && !templateTypes.empty())
    data.thisType = data.thisType.getWithTemplateTypes(templateTypes);

  // Map local arg types to imported types
  ArgList localArgs;
  localArgs.reserve(data.argResults.size());
  for (const ExprResult &argResult : data.argResults)
    localArgs.emplace_back(mapLocalTypeToImportedScopeType(data.calleeParentScope, argResult.type), argResult.isTemporary());

  // Map local template types to imported types
  for (QualType &templateType : templateTypes)
    templateType = mapLocalTypeToImportedScopeType(data.calleeParentScope, templateType);

  // Retrieve function object
  Scope *matchScope = data.calleeParentScope;
  data.callee = FunctionManager::match(this, matchScope, functionName, data.thisType, localArgs, templateTypes, false, node);

  return true;
}

bool TypeChecker::visitFctPtrCall(const FctCallNode *node, const QualType &functionType) const {
  const auto &[callType, isImported, thisType, argResults, callee, calleeParentScope] = node->data.at(manIdx);

  // Check if the given argument types match the type
  const QualTypeList expectedArgTypes = functionType.getFunctionParamTypes();
  if (argResults.size() != expectedArgTypes.size())
    SOFT_ERROR_BOOL(node, REFERENCED_UNDEFINED_FUNCTION, "Expected and actual number of arguments do not match")

  // Create resolver function, that always returns a nullptr
  TypeMatcher::ResolverFct resolverFct = [](const std::string &) { return nullptr; };

  for (size_t i = 0; i < argResults.size(); i++) {
    const QualType &actualType = argResults.at(i).type;
    const QualType &expectedType = expectedArgTypes.at(i);
    if (TypeMapping tm; !TypeMatcher::matchRequestedToCandidateType(expectedType, actualType, tm, resolverFct, false))
      SOFT_ERROR_BOOL(node->argLst->args.at(i), REFERENCED_UNDEFINED_FUNCTION,
                      "Expected " + expectedType.getName(false) + " but got " + actualType.getName(false))
  }
  return true;
}

bool TypeChecker::visitMethodCall(FctCallNode *node, Scope *structScope, QualTypeList &templateTypes) {
  auto &[callType, isImported, thisType, argResults, callee, calleeParentScope] = node->data.at(manIdx);

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
  ArgList localArgs;
  for (const ExprResult &argResult : argResults)
    localArgs.emplace_back(mapLocalTypeToImportedScopeType(calleeParentScope, argResult.type), argResult.isTemporary());

  // Map local template types to imported types
  for (QualType &templateType : templateTypes)
    templateType = mapLocalTypeToImportedScopeType(calleeParentScope, templateType);

  // 'this' type
  QualType localThisType = thisType;
  autoDeReference(localThisType);
  localThisType = mapLocalTypeToImportedScopeType(calleeParentScope, localThisType);

  // Retrieve function object
  const std::string &functionName = node->functionNameFragments.back();
  callee = FunctionManager::match(this, matchScope, functionName, localThisType, localArgs, templateTypes, false, node);

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

  const QualType arrayType = actualItemType.toArray(node, node->actualSize, true);
  return ExprResult{node->setEvaluatedSymbolType(arrayType, manIdx)};
}

std::any TypeChecker::visitStructInstantiation(StructInstantiationNode *node) {
  // Get struct name. Retrieve it from alias if required
  std::string structName = node->fqStructName;
  SymbolTableEntry *aliasEntry = rootScope->lookupStrict(structName);
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
  const bool isAliasType = aliasEntry && aliasEntry->getQualType().is(TY_ALIAS);
  if (isAliasType) {
    aliasedTypeContainerEntry = rootScope->lookupStrict(aliasEntry->name + ALIAS_CONTAINER_SUFFIX);
    assert(aliasedTypeContainerEntry != nullptr);
    // Set alias entry used
    aliasEntry->used = true;
    structName = aliasedTypeContainerEntry->getQualType().getSubType();
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

  // Get struct type
  QualType structType = structEntry->getQualType();

  // Get the concrete template types
  QualTypeList concreteTemplateTypes;
  if (isAliasType) {
    // Retrieve concrete template types from type alias
    concreteTemplateTypes = aliasedTypeContainerEntry->getQualType().getTemplateTypes();
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
  structName = structEntry->name;
  Struct *spiceStruct = StructManager::match(structScope->parent, structName, concreteTemplateTypes, node);
  if (!spiceStruct) {
    const std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found")
  }
  node->instantiatedStructs.at(manIdx) = spiceStruct;

  // Struct instantiation for an inheriting struct is forbidden, because the vtable needs to be initialized and this is done in
  // the ctor of the struct, which is never called in case of struct instantiation
  if (!spiceStruct->interfaceTypes.empty())
    SOFT_ERROR_ER(node, INVALID_STRUCT_INSTANTIATION, "Struct instantiations for inheriting structs are forbidden")

  // Use scope of concrete substantiation and not the scope of the generic type
  structScope = spiceStruct->scope;
  structType = structType.getWithBodyScope(structScope);

  // Set template types to the struct
  QualTypeList templateTypes;
  for (const GenericType &genericType : spiceStruct->templateTypes)
    templateTypes.emplace_back(genericType);
  structType = structType.getWithTemplateTypes(templateTypes);

  // Check if the number of fields matches
  if (node->fieldLst) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst->args.size())
      SOFT_ERROR_ER(node->fieldLst, NUMBER_OF_FIELDS_NOT_MATCHING,
                    "You've passed too less/many field values. Pass either none or all of them")

    // Check if the field types are matching
    const size_t fieldCount = spiceStruct->fieldTypes.size();
    const size_t explicitFieldsStartIdx = structScope->getFieldCount() - fieldCount;
    for (size_t i = 0; i < node->fieldLst->args.size(); i++) {
      // Get actual type
      AssignExprNode *assignExpr = node->fieldLst->args.at(i);
      auto fieldResult = std::any_cast<ExprResult>(visit(assignExpr));
      HANDLE_UNRESOLVED_TYPE_ER(fieldResult.type)
      // Get expected type
      SymbolTableEntry *expectedField = structScope->lookupField(explicitFieldsStartIdx + i);
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
    for (QualType &fieldType : spiceStruct->fieldTypes) {
      if (fieldType.isRef())
        SOFT_ERROR_ER(node, REFERENCE_WITHOUT_INITIALIZER,
                      "The struct takes at least one reference field. You need to instantiate it with all fields.")
    }
  }

  // Update type of struct entry
  structEntry->updateType(structType, true);

  // If not all values are constant, insert anonymous symbol to keep track of dtor calls for de-allocation
  SymbolTableEntry *anonymousEntry = nullptr;
  if (node->fieldLst != nullptr)
    if (std::ranges::any_of(node->fieldLst->args, [](const AssignExprNode *field) { return !field->hasCompileTimeValue(); }))
      anonymousEntry = currentScope->symbolTable.insertAnonymous(structType, node);

  // Remove public specifier to not have public local variables
  structType.getSpecifiers().isPublic = false;

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

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  // Visit base data type
  auto type = std::any_cast<QualType>(visit(node->baseDataType));
  HANDLE_UNRESOLVED_TYPE_QT(type)

  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    auto [modifierType, hasSize, hardcodedSize, sizeVarName] = tmQueue.front();

    // Only the outermost array can have an unknown size
    if (type.isArray() && type.getArraySize() == ARRAY_SIZE_UNKNOWN)
      SOFT_ERROR_QT(node, ARRAY_SIZE_INVALID,
                    "Usage of incomplete array type. Only the outermost array type may have unknown size")

    switch (modifierType) {
    case DataTypeNode::TypeModifierType::TYPE_PTR: {
      type = type.toPtr(node);
      break;
    }
    case DataTypeNode::TypeModifierType::TYPE_REF: {
      type = type.toRef(node);
      break;
    }
    case DataTypeNode::TypeModifierType::TYPE_ARRAY: {
      const std::string &varName = sizeVarName;
      if (!varName.empty()) {
        const SymbolTableEntry *globalVar = rootScope->lookupStrict(varName);
        if (!globalVar)
          SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + varName + "' ")
        if (!globalVar->getQualType().isConst())
          SOFT_ERROR_QT(node, EXPECTED_CONST_VARIABLE, "The size of the array must be known at compile time")
        if (!globalVar->getQualType().is(TY_INT))
          SOFT_ERROR_QT(node, OPERATOR_WRONG_DATA_TYPE, "Expected variable of type int")
        hardcodedSize = globalVar->declNode->getCompileTimeValue().intValue;
      }

      if (hasSize && hardcodedSize <= 1)
        SOFT_ERROR_QT(node, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated")
      type = type.toArray(node, hardcodedSize);
      break;
    }
    default:                                                               // GCOV_EXCL_LINE
      throw CompilerError(UNHANDLED_BRANCH, "Modifier type fall-through"); // GCOV_EXCL_LINE
    }
    tmQueue.pop();
  }

  // Attach the specifiers to the type
  if (node->specifierLst) {
    const QualType baseType = type.getBase();
    for (const SpecifierNode *specifier : node->specifierLst->specifiers) {
      if (specifier->type == SpecifierNode::SpecifierType::TY_CONST) {
        type.getSpecifiers().isConst = true;
      } else if (specifier->type == SpecifierNode::SpecifierType::TY_SIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_QT(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on type " + baseType.getName(false))
        type.getSpecifiers().isSigned = true;
        type.getSpecifiers().isUnsigned = false;
      } else if (specifier->type == SpecifierNode::SpecifierType::TY_UNSIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_QT(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on type " + baseType.getName(false))
        type.getSpecifiers().isSigned = false;
        type.getSpecifiers().isUnsigned = true;
      } else if (specifier->type == SpecifierNode::SpecifierType::TY_HEAP) {
        // Heap variables can only be pointers
        if (!type.removeReferenceWrapper().isOneOf({TY_PTR, TY_ARRAY, TY_STRING}))
          SOFT_ERROR_QT(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT,
                        "The heap specifier can only be applied to symbols of pointer type, you provided " +
                            baseType.getName(false))

        type.getSpecifiers().isHeap = true;
      } else if (specifier->type == SpecifierNode::SpecifierType::TY_COMPOSITION && node->isFieldType) {
        if (!type.is(TY_STRUCT))
          SOFT_ERROR_QT(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "The compose specifier can only be used on plain struct fields")
        type.getSpecifiers().isComposition = true;
      } else if (specifier->type == SpecifierNode::SpecifierType::TY_PUBLIC && (node->isFieldType || node->isGlobalType)) {
        type.getSpecifiers().isPublic = true;
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
        SOFT_ERROR_QT(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT,
                      "Cannot use this specifier on a " + std::string(entryName) + " definition")
      }
    }
  }

  return node->setEvaluatedSymbolType(type, manIdx);
}

std::any TypeChecker::visitBaseDataType(BaseDataTypeNode *node) {
  switch (node->type) {
  case BaseDataTypeNode::Type::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(QualType(TY_DOUBLE), manIdx);
  case BaseDataTypeNode::Type::TYPE_INT:
    return node->setEvaluatedSymbolType(QualType(TY_INT), manIdx);
  case BaseDataTypeNode::Type::TYPE_SHORT:
    return node->setEvaluatedSymbolType(QualType(TY_SHORT), manIdx);
  case BaseDataTypeNode::Type::TYPE_LONG:
    return node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx);
  case BaseDataTypeNode::Type::TYPE_BYTE:
    return node->setEvaluatedSymbolType(QualType(TY_BYTE), manIdx);
  case BaseDataTypeNode::Type::TYPE_CHAR:
    return node->setEvaluatedSymbolType(QualType(TY_CHAR), manIdx);
  case BaseDataTypeNode::Type::TYPE_STRING:
    return node->setEvaluatedSymbolType(QualType(TY_STRING), manIdx);
  case BaseDataTypeNode::Type::TYPE_BOOL:
    return node->setEvaluatedSymbolType(QualType(TY_BOOL), manIdx);
  case BaseDataTypeNode::Type::TYPE_CUSTOM: {
    const auto customType = std::any_cast<QualType>(visit(node->customDataType));
    HANDLE_UNRESOLVED_TYPE_QT(customType)
    return node->setEvaluatedSymbolType(customType, manIdx);
  }
  case BaseDataTypeNode::Type::TYPE_FUNCTION: {
    const auto functionType = std::any_cast<QualType>(visit(node->functionDataType));
    HANDLE_UNRESOLVED_TYPE_QT(functionType)
    return node->setEvaluatedSymbolType(functionType, manIdx);
  }
  default:
    return node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx);
  }
}

std::any TypeChecker::visitCustomDataType(CustomDataTypeNode *node) {
  // It is a struct type -> get the access scope
  Scope *localAccessScope = accessScope ? accessScope : currentScope;

  const std::string firstFragment = node->typeNameFragments.front();

  // Check this type requires a runtime module
  if (node->typeNameFragments.size() == 1)
    for (const auto &[typeName, runtimeModule] : TYPE_NAME_TO_RT_MODULE_MAPPING)
      if (firstFragment == typeName && !sourceFile->isRT(runtimeModule))
        sourceFile->requestRuntimeModule(runtimeModule);

  // Check if it is a generic type
  bool isImported = node->typeNameFragments.size() > 1;
  const QualType *genericType = rootScope->lookupGenericType(firstFragment);
  if (!isImported && genericType) {
    // Take the concrete replacement type for the name of this generic type if available
    QualType symbolType = *genericType;
    if (typeMapping.contains(firstFragment))
      symbolType = typeMapping.at(firstFragment);

    // Check if the replacement is a String type
    if (!isImported && symbolType.isStringObj() && !sourceFile->isStringRT())
      sourceFile->requestRuntimeModule(STRING_RT);

    return node->setEvaluatedSymbolType(symbolType, manIdx);
  }

  // Check if the type exists in the exported names registry
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqTypeName);
  if (!registryEntry)
    SOFT_ERROR_QT(node, UNKNOWN_DATATYPE, "Unknown datatype '" + node->fqTypeName + "'")
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *entry = registryEntry->targetEntry;
  assert(entry != nullptr);
  entry->used = true;
  localAccessScope = registryEntry->targetScope->parent;

  // Get struct type
  QualType entryType = entry->getQualType();

  // Enums can early-return
  if (entryType.is(TY_ENUM))
    return QualType(TY_INT);

  if (entryType.isOneOf({TY_STRUCT, TY_INTERFACE})) {
    assert(dynamic_cast<DataTypeNode *>(node->parent->parent) != nullptr);

    // Collect the concrete template types
    bool allTemplateTypesConcrete = true;
    QualTypeList templateTypes;
    if (node->templateTypeLst) {
      assert(localAccessScope != nullptr);
      isImported = localAccessScope->isImportedBy(rootScope);

      templateTypes.reserve(node->templateTypeLst->dataTypes.size());
      for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
        auto templateType = std::any_cast<QualType>(visit(dataType));
        HANDLE_UNRESOLVED_TYPE_QT(templateType)
        if (entryType.is(TY_GENERIC)) {
          allTemplateTypesConcrete = false;
        } else if (isImported) {
          // Introduce the local type to the imported source file
          [[maybe_unused]] QualType importedType = mapLocalTypeToImportedScopeType(localAccessScope, templateType);
          assert(importedType.is(templateType.getSuperType()));
        }
        templateTypes.push_back(templateType);
      }
      entryType = entryType.getWithTemplateTypes(templateTypes);
    }

    // Check if struct is defined before the current code location, if defined in the same source file
    const CodeLoc &declCodeLoc = entry->declNode->codeLoc;
    const CodeLoc &codeLoc = node->codeLoc;
    if (declCodeLoc.sourceFile->filePath == codeLoc.sourceFile->filePath && declCodeLoc > codeLoc) {
      if (entryType.is(TY_STRUCT)) {
        SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_STRUCT, "Structs must be defined before usage")
      } else {
        assert(entryType.is(TY_INTERFACE));
        SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_INTERFACE, "Interfaces must be defined before usage")
      }
    }

    if (allTemplateTypesConcrete) { // Only do the next step, if we have concrete template types
      // Set the struct/interface instance to used, if found
      // Here, it is allowed to accept, that the struct/interface cannot be found, because there are self-referencing ones
      if (entryType.is(TY_STRUCT)) {
        const std::string structName = node->typeNameFragments.back();
        if (Struct *spiceStruct = StructManager::match(localAccessScope, structName, templateTypes, node))
          entryType = entryType.getWithBodyScope(spiceStruct->scope);
      } else {
        assert(entryType.is(TY_INTERFACE));
        const std::string interfaceName = node->typeNameFragments.back();
        if (const Interface *spiceInterface = InterfaceManager::match(localAccessScope, interfaceName, templateTypes, node))
          entryType = entryType.getWithBodyScope(spiceInterface->scope);
      }
    }

    return node->setEvaluatedSymbolType(entryType, manIdx);
  }

  if (entryType.is(TY_ALIAS)) {
    // Get type of aliased type container entry
    const std::string aliasedContainerEntryName = entry->name + ALIAS_CONTAINER_SUFFIX;
    SymbolTableEntry *aliasedTypeContainerEntry = entry->scope->lookupStrict(aliasedContainerEntryName);
    assert(aliasedTypeContainerEntry != nullptr);
    return node->setEvaluatedSymbolType(aliasedTypeContainerEntry->getQualType(), manIdx);
  }

  const bool isInvalid = entryType.is(TY_INVALID);
  SOFT_ERROR_QT(node, EXPECTED_TYPE, isInvalid ? "Used type before declared" : "Expected type, but got " + entryType.getName())
}

std::any TypeChecker::visitFunctionDataType(FunctionDataTypeNode *node) {
  // Visit return type
  QualType returnType(TY_DYN);
  if (node->isFunction) {
    returnType = std::any_cast<QualType>(visit(node->returnType));
    HANDLE_UNRESOLVED_TYPE_QT(returnType)
    if (returnType.is(TY_DYN))
      SOFT_ERROR_ER(node->returnType, UNEXPECTED_DYN_TYPE, "Function types cannot have return type dyn")
  }

  // Visit param types
  QualTypeList paramTypes;
  if (const TypeLstNode *paramTypeListNode = node->paramTypeLst; paramTypeListNode != nullptr) {
    for (DataTypeNode *paramTypeNode : paramTypeListNode->dataTypes) {
      auto paramType = std::any_cast<QualType>(visit(paramTypeNode));
      HANDLE_UNRESOLVED_TYPE_QT(returnType)
      paramTypes.push_back(paramType);
    }
  }

  // Build function type
  const SuperType superType = node->isFunction ? TY_FUNCTION : TY_PROCEDURE;
  const QualType functionType = QualType(superType).getWithFunctionParamAndReturnTypes(returnType, paramTypes);

  return node->setEvaluatedSymbolType(functionType, manIdx);
}

/**
 * Check if the capture rules for async lambdas are enforced if the async attribute is set
 *
 * Only one capture with pointer type, pass-by-val is allowed, since only then we can store it in the second field of the
 * fat pointer and can ensure, that no stack variable is referenced inside the lambda.
 *
 * @param node Lambda base node
 * @param attrs Lambda attributes
 * @return False if the rules are violated, true otherwise
 */
bool TypeChecker::checkAsyncLambdaCaptureRules(const LambdaBaseNode *node, const LambdaAttrNode *attrs) const {
  // If the async attribute is not set, we can return early
  if (!attrs || !attrs->attrLst->hasAttr(ATTR_ASYNC) || !attrs->attrLst->getAttrValueByName(ATTR_ASYNC)->boolValue)
    return true; // Not violated

  // If we don't have any captures, we can return early
  const CaptureMap &captures = node->bodyScope->symbolTable.captures;
  if (captures.empty())
    return true; // Not violated

  // Check for the capture rules
  if (const Capture &capture = captures.begin()->second;
      captures.size() > 1 || !capture.capturedSymbol->getQualType().isPtr() || capture.getMode() != BY_VALUE) {
    const auto warningMessage =
        "Async lambdas can only capture one pointer by value without storing captures in the caller stack frame, which can lead "
        "to bugs due to references, outliving the validity scope of the referenced variable.";
    const CompilerWarning warning(node->codeLoc, ASYNC_LAMBDA_CAPTURE_RULE_VIOLATION, warningMessage);
    currentScope->sourceFile->compilerOutput.warnings.push_back(warning);
  }

  return false; // Violated
}

Function *TypeChecker::matchCopyCtor(const QualType &thisType, const ASTNode *node) {
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope != nullptr);
  const ArgList args = {{thisType.toConstRef(node), false}};
  return FunctionManager::match(this, matchScope, CTOR_FUNCTION_NAME, thisType, args, {}, true, node);
}

QualType TypeChecker::mapLocalTypeToImportedScopeType(const Scope *targetScope, const QualType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBase(TY_STRUCT))
    return symbolType;

  // If the target scope is in the current source file, we can return the symbol type as is
  SourceFile *targetSourceFile = targetScope->sourceFile;
  if (targetSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of the target source file
  for (const auto &entry : targetSourceFile->exportedNameRegistry | std::views::values)
    if (entry.targetEntry != nullptr && entry.targetEntry->getQualType().isBase(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == symbolType.getBase().getBodyScope())
          return symbolType;

  // The target source file does not know about the struct at all
  // -> show it how to find the struct
  const std::string structName = symbolType.getBase().getSubType();
  const NameRegistryEntry *origRegistryEntry = sourceFile->getNameRegistryEntry(structName);
  assert(origRegistryEntry != nullptr);
  const uint64_t targetTypeId = origRegistryEntry->typeId;
  SymbolTableEntry *targetEntry = origRegistryEntry->targetEntry;
  targetSourceFile->addNameRegistryEntry(structName, targetTypeId, targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

QualType TypeChecker::mapImportedScopeTypeToLocalType(const Scope *sourceScope, const QualType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBase(TY_STRUCT))
    return symbolType;

  // If the source scope is in the current source file, we can return the symbol type as is
  const SourceFile *sourceSourceFile = sourceScope->sourceFile;
  if (sourceSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of this source file
  const QualType baseType = symbolType.getBase();
  for (const auto &entry : sourceFile->exportedNameRegistry | std::views::values)
    if (entry.targetEntry != nullptr && entry.targetEntry->getQualType().isBase(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == baseType.getBodyScope())
          return symbolType;

  // This source file does not know about the struct at all
  // -> show it how to find the struct
  const NameRegistryEntry *origRegistryEntry = sourceSourceFile->getNameRegistryEntry(baseType.getSubType());
  assert(origRegistryEntry != nullptr);
  const uint64_t typeId = origRegistryEntry->typeId;
  SymbolTableEntry *targetEntry = origRegistryEntry->targetEntry;
  sourceFile->addNameRegistryEntry(baseType.getSubType(), typeId, targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

/**
 * Auto-dereference the given symbol type.
 * This process is NOT equivalent with getBase() because getBase() also removes e.g. array wrappers
 *
 * @param symbolType Input symbol type
 */
void TypeChecker::autoDeReference(QualType &symbolType) {
  while (symbolType.isPtr() || symbolType.isRef())
    symbolType = symbolType.getContained();
}

/**
 * Returns the operator function list for the current manifestation and the given node
 *
 * @param node Node to retrieve the op fct pointer list from
 * @return Op fct pointer list
 */
std::vector<const Function *> &TypeChecker::getOpFctPointers(ASTNode *node) const {
  assert(node->getOpFctPointers()->size() > manIdx);
  return node->getOpFctPointers()->at(manIdx);
}

/**
 * Check if a function has been type-checked already. If not, request a revisit
 *
 * @param fct Function to check
 */
void TypeChecker::requestRevisitIfRequired(const Function *fct) {
  if (fct && !fct->alreadyTypeChecked)
    fct->entry->scope->sourceFile->reVisitRequested = true;
}

/**
 * Add a soft error to the error list
 */
void TypeChecker::softError(const ASTNode *node, const SemanticErrorType errorType, const std::string &message) const {
  resourceManager.errorManager.addSoftError(node, errorType, message);
}

} // namespace spice::compiler