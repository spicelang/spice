// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/ScopeHandle.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

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
      node->getIteratorFct = FunctionManager::match(matchScope, "iterate", thisType, argTypes, {}, true, iteratorNode);
    } else { // Struct, implementing Iterator interface
      Scope *matchScope = iterableType.getBodyScope();
      node->getIteratorFct = FunctionManager::match(matchScope, "getIterator", iterableType, {}, {}, true, iteratorNode);
    }
    if (node->getIteratorFct == nullptr)
      throw SemanticError(iteratorNode, INVALID_ITERATOR, "No getIterator() function found for the given iterable type");

    iteratorType = QualType(node->getIteratorFct->returnType);
    // Add anonymous symbol to keep track of dtor call, if non-trivially destructible
    if (!iteratorType.isTriviallyDestructible(iteratorNode))
      currentScope->symbolTable.insertAnonymous(iteratorType, iteratorNode);
  }

  // Change to foreach body scope
  ScopeHandle scopeHandle(this, node->getScopeId(), ScopeType::FOREACH_BODY);

  // Check iterator type
  if (!iteratorType.isIterator(node)) {
    const std::string errMsg =
        "Can only iterate over arrays or data structures, inheriting from IIterator or IIterable. You provided " +
        iteratorType.getName(false, true);
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
    node->getIdxFct = FunctionManager::match(matchScope, "getIdx", iteratorType, {}, {}, false, node);
    RETURN_NULLPTR_IF_NULLPTR(node->getIdxFct);
    iteratorItemType = node->getIdxFct->returnType.getTemplateTypes().back();
  } else {
    node->getFct = FunctionManager::match(matchScope, "get", iteratorType, {}, {}, false, node);
    RETURN_NULLPTR_IF_NULLPTR(node->getFct);
    iteratorItemType = node->getFct->returnType;
  }
  node->isValidFct = FunctionManager::match(matchScope, "isValid", iteratorType, {}, {}, false, node);
  RETURN_NULLPTR_IF_NULLPTR(node->isValidFct);
  node->nextFct = FunctionManager::match(matchScope, "next", iteratorType, {}, {}, false, node);
  RETURN_NULLPTR_IF_NULLPTR(node->nextFct);

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
  }

  // Check result type. This is important, also for copy ctor calls, etc.
  const ExprResult itemResult = {itemType, itemVarSymbol};
  const ExprResult iteratorItemResult = {iteratorItemType, nullptr /* always a temporary */};
  const auto [_, copyCtor] =
      opRuleManager.getAssignResultType(node->itemVarDecl, itemResult, iteratorItemResult, true, false, ERROR_FOREACH_ITEM);
  node->calledItemCopyCtor = copyCtor;

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

std::any TypeChecker::visitCaseConstant(CaseConstantNode *node) {
  // If we have a normal constant, we can take the symbol type from there
  if (node->constant)
    return visit(node->constant);

  // Check if a local or global variable can be found by searching for the name
  if (node->identifierFragments.size() == 1)
    node->entry = currentScope->lookup(node->identifierFragments.back());

  // If no local or global was found, search in the name registry
  if (!node->entry) {
    const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqIdentifier);
    if (!registryEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found")
    node->entry = registryEntry->targetEntry;
  }
  assert(node->entry != nullptr);

  // Check for the correct type
  const QualType &qualType = node->entry->getQualType();
  if (!qualType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}))
    SOFT_ERROR_ER(node, CASE_CONSTANT_INVALID_TYPE, "Case constants must be of type int, short, long, byte, char, bool or enum")

  return ExprResult{node->setEvaluatedSymbolType(qualType, manIdx)};
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

} // namespace spice::compiler
