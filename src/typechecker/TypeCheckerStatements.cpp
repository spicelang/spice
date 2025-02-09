// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

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
    if (!localVarType.isRef() && rhsEntry != nullptr && rhsEntry->anonymous)
      currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
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
      const Struct *spiceStruct = localVarType.getStruct(node);
      assert(spiceStruct != nullptr);
      auto structDeclNode = spice_pointer_cast<StructDefNode *>(spiceStruct->declNode);
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

  return localVarType;
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

  return node->returnType = returnType;
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

} // namespace spice::compiler
