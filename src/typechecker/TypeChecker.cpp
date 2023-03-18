// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), rootScope(sourceFile->globalScope.get()),
      warnings(sourceFile->compilerOutput.warnings) {}

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;
  reVisitRequested = false;

  // Initialize AST nodes with size of 1
  if (typeCheckerMode == TC_MODE_PREPARE)
    node->resizeToNumberOfManifestations(1);

  // Visit children
  visitChildren(node);

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

std::any TypeChecker::visitThreadDef(ThreadDefNode *node) {
  // Change to thread body scope
  changeToScope(node->bodyScope, SCOPE_THREAD_BODY);

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  // ThreadDef returns a tid as byte*
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_BYTE).toPointer(node), manIdx)};
}

std::any TypeChecker::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Change to unsafe block body scope
  changeToScope(node->bodyScope, SCOPE_UNSAFE_BODY);

  // Visit body
  visit(node->body());

  // Leave unsafe block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitForLoop(ForLoopNode *node) {
  // Change to for body scope
  changeToScope(node->bodyScope, SCOPE_FOR_BODY);

  // Visit loop variable declaration
  visit(node->initDecl());

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condAssign())).type;
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condAssign(), CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");

  // Visit incrementer
  visit(node->incAssign());

  // Visit body
  visit(node->body());

  // Leave body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitForeachLoop(ForeachLoopNode *node) {
  // Change to foreach body scope
  changeToScope(node->bodyScope, SCOPE_FOREACH_BODY);

  // Check type of the array
  SymbolType arrayType = std::any_cast<ExprResult>(visit(node->arrayAssign())).type;
  if (!arrayType.isArray())
    throw SemanticError(node->arrayAssign(), OPERATOR_WRONG_DATA_TYPE,
                        "Can only apply foreach loop on an array type. You provided " + arrayType.getName());

  // Check size of the array
  if (arrayType.getArraySize() == ARRAY_SIZE_UNKNOWN)
    throw SemanticError(node->arrayAssign(), ARRAY_SIZE_INVALID,
                        "Can only apply foreach loop on an array type of which the size is known at compile time");

  if (node->idxVarDecl()) {
    // Visit index declaration or assignment
    auto indexType = std::any_cast<SymbolType>(visit(node->idxVarDecl()));
    // Check if index type is int
    if (!indexType.is(TY_INT))
      throw SemanticError(node->idxVarDecl(), ARRAY_INDEX_NOT_INT,
                          "Index in foreach loop must be of type int. You provided " + indexType.getName());
  } else {
    // Update type of default idx variable
    SymbolTableEntry *idxVarEntry = currentScope->lookupStrict(FOREACH_DEFAULT_IDX_VARIABLE_NAME);
    assert(idxVarEntry != nullptr);
    idxVarEntry->updateType(SymbolType(TY_INT), false);
  }

  // Check type of the item
  auto itemType = std::any_cast<SymbolType>(visit(node->itemDecl()));
  if (itemType.is(TY_DYN)) { // Perform type inference
    itemType = arrayType.getContainedTy();
    // Update evaluated symbol type of the declaration data type
    node->itemDecl()->dataType()->setEvaluatedSymbolType(itemType, manIdx);
  } else if (itemType != arrayType.getContainedTy()) { // Check types
    throw SemanticError(node->itemDecl(), OPERATOR_WRONG_DATA_TYPE,
                        "Foreach loop item type does not match array type. Expected " + arrayType.getName() + ", provided " +
                            itemType.getName());
  }

  // Update type of item
  SymbolTableEntry *itemVarSymbol = currentScope->lookupStrict(node->itemDecl()->varName);
  assert(itemVarSymbol != nullptr);
  itemVarSymbol->updateType(itemType, false);

  // Visit body
  visit(node->body());

  // Leave foreach body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitWhileLoop(WhileLoopNode *node) {
  // Change to while body scope
  changeToScope(node->bodyScope, SCOPE_WHILE_BODY);

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condition())).type;
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");

  // Visit body
  visit(node->body());

  // Leave while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Change to while body scope
  changeToScope(node->bodyScope, SCOPE_WHILE_BODY);

  // Visit body
  visit(node->body());

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condition())).type;
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "Do-While loop condition must be of type bool");

  // Leave while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitIfStmt(IfStmtNode *node) {
  // Change to then body scope
  changeToScope(node->thenBodyScope, SCOPE_IF_ELSE_BODY);

  // Visit condition
  AssignExprNode *condition = node->condition();
  SymbolType conditionType = std::any_cast<ExprResult>(visit(condition)).type;
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

  // Warning for bool assignment
  if (condition->hasOperator && condition->op == AssignExprNode::OP_ASSIGN)
    sourceFile->compilerOutput.warnings.emplace_back(condition->codeLoc, BOOL_ASSIGN_AS_CONDITION,
                                                     "If you want to compare the values, use '=='");

  // Visit body
  visit(node->thenBody());

  // Leave then body scope
  currentScope = node->thenBodyScope->parent;

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
  changeToScope(node->elseBodyScope, SCOPE_IF_ELSE_BODY);

  // Visit body
  visit(node->body());

  // Leave else body scope
  currentScope = node->elseBodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Change to anonymous scope body scope
  node->bodyScope->parent = currentScope;                           // Needed for nested scopes in generic functions
  node->bodyScope->symbolTable.parent = &currentScope->symbolTable; // Needed for nested scopes in generic functions
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_ANONYMOUS_BLOCK_BODY);

  // Visit body
  visit(node->body());

  // Leave anonymous scope body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitStmtLst(StmtLstNode *node) {
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
  return nullptr;
}

std::any TypeChecker::visitParamLst(ParamLstNode *node) {
  NamedParamList namedParams;
  bool metOptional = false;

  for (DeclStmtNode *param : node->params()) {
    // Visit param
    const auto paramType = std::any_cast<SymbolType>(visit(param));

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (paramType.is(TY_DYN))
      throw SemanticError(node, FCT_PARAM_IS_TYPE_DYN, "Type of parameter '" + param->varName + "' is invalid");

    // Ensure that no optional param comes after a mandatory param
    if (param->hasAssignment) {
      metOptional = true;
    } else if (metOptional) {
      throw SemanticError(param, INVALID_PARAM_ORDER, "Mandatory parameters must go before any optional parameters");
    }

    // Add parameter to named param list
    namedParams.push_back({param->varName, paramType, metOptional});
  }

  return namedParams;
}

std::any TypeChecker::visitField(FieldNode *node) {
  // Visit field type
  const auto fieldType = std::any_cast<SymbolType>(node->dataType());

  // Update type of field entry
  SymbolTableEntry *fieldEntry = currentScope->lookupStrict(node->fieldName);
  assert(fieldEntry != nullptr);
  fieldEntry->updateType(fieldType, false);

  return fieldType;
}

std::any TypeChecker::visitSignature(SignatureNode *node) {
  // Visit return type
  auto returnType = std::any_cast<SymbolType>(visit(node->dataType()));

  // Visit params
  ParamList paramTypes;
  if (node->hasParams) {
    paramTypes.reserve(node->paramTypeLst()->dataTypes().size());
    for (DataTypeNode *param : node->paramTypeLst()->dataTypes()) {
      auto paramType = std::any_cast<SymbolType>(visit(param));
      paramTypes.push_back({paramType, false});
    }
  }

  // Build signature object
  Function signature(node->methodName, /*entry=*/nullptr, /*thisType=*/SymbolType(TY_DYN), returnType, paramTypes, {}, node,
                     /*external=*/false);

  // Add signature to current scope
  FunctionManager::insertFunction(currentScope, signature, &node->signatureManifestations);

  // Set entry to signature type
  SymbolType signatureType(node->signatureType == SignatureNode::TYPE_FUNCTION ? TY_FUNCTION : TY_PROCEDURE);
  assert(node->entry != nullptr);
  node->entry->updateType(signatureType, false);

  return &node->signatureManifestations;
}

std::any TypeChecker::visitDeclStmt(DeclStmtNode *node) {
  SymbolType localVarType;
  if (node->hasAssignment) {
    // Visit the right side
    auto [rhsTy, rhsEntry] = std::any_cast<ExprResult>(visit(node->assignExpr()));

    // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
    if (rhsEntry != nullptr && rhsEntry->anonymous)
      currentScope->symbolTable.deleteAnonymous(rhsEntry->name);

    // Visit data type
    localVarType = std::any_cast<SymbolType>(visit(node->dataType()));

    // Infer the type left to right if the right side is an empty array initialization
    if (rhsTy.isArrayOf(TY_DYN))
      rhsTy = localVarType;

    // Check if type has to be inferred or both types are fixed
    localVarType = OpRuleManager::getAssignResultType(node, localVarType, rhsTy, 0);

    // Push symbolType to the declaration data type
    node->dataType()->setEvaluatedSymbolType(localVarType, manIdx);
  } else {
    // Visit data type
    localVarType = std::any_cast<SymbolType>(visit(node->dataType()));

    // References with no initialization are illegal
    if (localVarType.isRef() && !node->isParam)
      throw SemanticError(node, REFERENCE_WITHOUT_INITIALIZER, "References must always be initialized directly");
  }

  // Update the type of the variable
  SymbolTableEntry *localVarEntry = currentScope->lookupStrict(node->varName);
  assert(localVarEntry != nullptr);
  localVarEntry->updateType(localVarType, /*overwriteExistingType=*/true);
  node->entries.at(manIdx) = localVarEntry;

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
      throw SemanticError(node->assignExpr(), RETURN_WITH_VALUE_IN_PROCEDURE, "Return with value in procedure is not allowed");
    return nullptr;
  }

  // ToDo: Check if function without return value
  // if (!node->hasReturnValue)
  //  throw SemanticError(node, RETURN_WITHOUT_VALUE_RESULT, "Return without value, but result variable is not initialized yet");

  if (!node->hasReturnValue)
    return nullptr;

  // Visit right side
  SymbolType returnValueType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  // Check if types match
  if (returnValueType != returnType)
    throw SemanticError(node->assignExpr(), OPERATOR_WRONG_DATA_TYPE,
                        "Passed wrong data type to return statement. Expected " + returnType.getName() + " but got " +
                            returnValueType.getName());

  return nullptr;
}

std::any TypeChecker::visitBreakStmt(BreakStmtNode *node) {
  // Check if the stated number is valid
  if (node->breakTimes < 1)
    throw SemanticError(node, INVALID_BREAK_NUMBER, "Break count must be >= 1, you provided " + std::to_string(node->breakTimes));

  // Check if we can break this often
  const size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->breakTimes > maxBreaks)
    throw SemanticError(node, INVALID_BREAK_NUMBER, "We can only break " + std::to_string(maxBreaks) + " time(s) here");

  return nullptr;
}

std::any TypeChecker::visitContinueStmt(ContinueStmtNode *node) {
  // Check if the stated number is valid
  if (node->continueTimes < 1)
    throw SemanticError(node, INVALID_CONTINUE_NUMBER,
                        "Continue count must be >= 1, you provided " + std::to_string(node->continueTimes));

  // Check if we can continue this often
  const size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->continueTimes > maxBreaks)
    throw SemanticError(node, INVALID_CONTINUE_NUMBER, "We can only continue " + std::to_string(maxBreaks) + " time(s) here");

  return nullptr;
}

std::any TypeChecker::visitAssertStmt(AssertStmtNode *node) {
  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;

  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->assignExpr(), ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool");

  return nullptr;
}

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = node->templatedString.find_first_of('%');
  while (index != std::string::npos && index != node->templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->args().size() <= placeholderCount)
      throw SemanticError(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments");

    // Get next assignment
    AssignExprNode *assignment = node->args().at(placeholderCount);
    // Visit assignment
    SymbolType argType = std::any_cast<ExprResult>(visit(assignment)).type;
    argType = argType.removeReferenceWrappers();

    switch (node->templatedString.at(index + 1)) {
    case 'c': {
      if (!argType.is(TY_CHAR))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects char, but got " + argType.getName());
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
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "The placeholder string expects int, short, long, byte or bool, but got " + argType.getName());
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
        throw SemanticError(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects double, but got " + argType.getName());
      placeholderCount++;
      break;
    }
    case 's': {
      const std::string strobjTypeName = STRING_RT_IMPORT_NAME + std::string(SCOPE_ACCESS_TOKEN) + STROBJ_NAME;
      const bool isString = argType.is(TY_STRING) || argType.is(TY_STRUCT, strobjTypeName);
      if (!isString && !argType.isPtrOf(TY_CHAR) && !argType.isArrayOf(TY_CHAR))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "The placeholder string expects string, String, char* or char[], but got " + argType.getName());
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!argType.isPtr() && !argType.isArray() && !argType.is(TY_STRING))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "The placeholder string expects pointer, array or string, but got " + argType.getName());
      placeholderCount++;
      break;
    }
    }
    index = node->templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->args().size())
    throw SemanticError(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments");

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_BOOL), manIdx)};
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  if (node->isType) { // Size of type
    visit(node->dataType());
  } else { // Size of value
    visit(node->assignExpr());
  }

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT), manIdx)};
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  SymbolType argType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  argType = argType.removeReferenceWrappers();

  // Check if arg is of type array
  if (!argType.isArray())
    throw SemanticError(node->assignExpr(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays");

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT), manIdx)};
}

std::any TypeChecker::visitTidCall(TidCallNode *node) {
  // Nothing to check here. Tid builtin has no arguments
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT), manIdx)};
}

std::any TypeChecker::visitJoinCall(JoinCallNode *node) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(node);

  for (AssignExprNode *assignExpr : node->assignExpressions()) {
    // Visit assign expression
    SymbolType assignExprType = std::any_cast<ExprResult>(visit(assignExpr)).type;
    assignExprType = assignExprType.removeReferenceWrappers();
    // Check if type is byte* or byte*[]
    if (assignExprType == bytePtr && assignExprType.isArrayOf(bytePtr))
      throw SemanticError(assignExpr, JOIN_ARG_MUST_BE_TID,
                          "The join builtin expects a thread id (byte*) or an array of thread ids (byte*[])");
  }

  // Return the number of threads that were joined
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT), manIdx)};
}

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if ternary
  if (node->ternaryExpr()) {
    const auto result = std::any_cast<ExprResult>(visit(node->ternaryExpr()));
    node->setEvaluatedSymbolType(result.type, manIdx);
    return result;
  }

  // Check if thread def
  if (node->threadDef())
    return visit(node->threadDef());

  // Check if assignment
  if (node->hasOperator) {
    // Visit the right side first
    auto [rhsType, _] = std::any_cast<ExprResult>(visit(node->rhs()));
    // Then visit the left side
    auto [lhsType, lhsVar] = std::any_cast<ExprResult>(visit(node->lhs()));

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) {
      rhsType = OpRuleManager::getAssignResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_PLUS_EQUAL) {
      rhsType = opRuleManager.getPlusEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_MINUS_EQUAL) {
      rhsType = opRuleManager.getMinusEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_MUL_EQUAL) {
      rhsType = opRuleManager.getMulEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_DIV_EQUAL) {
      rhsType = opRuleManager.getDivEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_REM_EQUAL) {
      rhsType = OpRuleManager::getRemEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_SHL_EQUAL) {
      rhsType = OpRuleManager::getSHLEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_SHR_EQUAL) {
      rhsType = OpRuleManager::getSHREqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_AND_EQUAL) {
      rhsType = OpRuleManager::getAndEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_OR_EQUAL) {
      rhsType = OpRuleManager::getOrEqualResultType(node, lhsType, rhsType, 0);
    } else if (node->op == AssignExprNode::OP_XOR_EQUAL) {
      rhsType = OpRuleManager::getXorEqualResultType(node, lhsType, rhsType, 0);
    }

    if (lhsVar) { // Variable is involved on the left side
      // Perform type inference
      if (lhsType.is(TY_DYN))
        lhsVar->updateType(rhsType, false);

      // In case the lhs variable is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsVar->name); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
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

  SymbolType trueType;
  SymbolType falseType;
  if (node->isShortened) {
    trueType = conditionType;
    falseType = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  } else {
    trueType = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
    falseType = std::any_cast<ExprResult>(visit(node->operands()[2])).type;
  }

  // Check if the condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(condition, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");

  // Check if trueType and falseType are matching
  if (trueType != falseType)
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "True and false operands in ternary must be of same data type");

  return ExprResult{node->setEvaluatedSymbolType(trueType, manIdx)};
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
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
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
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
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
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
  // Loop through all remaining operands
  for (size_t i = 2; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
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
  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
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
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;

  // Check if we need the string runtime to perform a string comparison
  if (lhsTy.is(TY_STRING) && rhsTy.is(TY_STRING))
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check operator
  SymbolType resultType;
  if (node->op == EqualityExprNode::OP_EQUAL) // Operator was equal
    resultType = opRuleManager.getEqualResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == EqualityExprNode::OP_NOT_EQUAL) // Operator was not equal
    resultType = opRuleManager.getNotEqualResultType(node, lhsTy, rhsTy, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit right side first, then left side
  SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[1])).type;
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;

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
  SymbolType lhsTy = std::any_cast<ExprResult>(visit(node->operands()[0])).type;

  // Check operator
  SymbolType resultType;
  if (node->op == ShiftExprNode::OP_SHIFT_LEFT) // Operator was shl
    resultType = OpRuleManager::getShiftLeftResultType(node, lhsTy, rhsTy, 0);
  else if (node->op == ShiftExprNode::OP_SHIFT_RIGHT) // Operator was shr
    resultType = OpRuleManager::getShiftRightResultType(node, lhsTy, rhsTy, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    // Visit next operand
    MultiplicativeExprNode *operand = node->operands()[i + 1];
    SymbolType operandType = std::any_cast<ExprResult>(visit(operand)).type;

    // Check operator
    const AdditiveExprNode::AdditiveOp &op = node->opQueue.front().first;
    if (op == AdditiveExprNode::OP_PLUS)
      currentType = opRuleManager.getPlusResultType(node, currentType, operandType, i);
    else if (op == AdditiveExprNode::OP_MINUS)
      currentType = opRuleManager.getMinusResultType(node, currentType, operandType, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExpr fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentType);
    node->opQueue.pop();
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  // Check if at least one multiplicative operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    // Visit next operand
    CastExprNode *operand = node->operands()[i + 1];
    SymbolType operandType = std::any_cast<ExprResult>(visit(operand)).type;

    // Check operator
    const MultiplicativeExprNode::MultiplicativeOp &op = node->opQueue.front().first;
    if (op == MultiplicativeExprNode::OP_MUL)
      currentType = opRuleManager.getMulResultType(node, currentType, operandType, i);
    else if (op == MultiplicativeExprNode::OP_DIV)
      currentType = opRuleManager.getDivResultType(node, currentType, operandType, i);
    else if (op == MultiplicativeExprNode::OP_REM)
      currentType = OpRuleManager::getRemResultType(node, currentType, operandType, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "Multiplicative fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentType);
    node->opQueue.pop();
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType, manIdx)};
}

std::any TypeChecker::visitCastExpr(CastExprNode *node) {
  // Check if cast is applied
  if (!node->isCasted)
    return visit(node->prefixUnaryExpr());

  // Visit source type
  SymbolType srcType = std::any_cast<ExprResult>(visit(node->prefixUnaryExpr())).type;
  // Visit destination type
  auto dstType = std::any_cast<SymbolType>(visit(node->dataType()));

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

  // Determine action, based on the given operator
  switch (node->op) {
  case PrefixUnaryExprNode::OP_MINUS:
    operandType = OpRuleManager::getPrefixMinusResultType(rhsNode, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_PLUS_PLUS:
    operandType = OpRuleManager::getPrefixPlusPlusResultType(rhsNode, operandType, 0);

    // In case the lhs is captured, notify the capture about the write access
    if (Capture *lhsCapture = operandEntry ? currentScope->symbolTable.lookupCapture(operandEntry->name) : nullptr; lhsCapture)
      lhsCapture->setCaptureMode(READ_WRITE);

    break;
  case PrefixUnaryExprNode::OP_MINUS_MINUS:
    operandType = OpRuleManager::getPrefixMinusMinusResultType(rhsNode, operandType, 0);

    // In case the lhs is captured, notify the capture about the write access
    if (Capture *lhsCapture = operandEntry ? currentScope->symbolTable.lookupCapture(operandEntry->name) : nullptr; lhsCapture)
      lhsCapture->setCaptureMode(READ_WRITE);

    break;
  case PrefixUnaryExprNode::OP_NOT:
    operandType = OpRuleManager::getPrefixNotResultType(rhsNode, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_BITWISE_NOT:
    operandType = OpRuleManager::getPrefixBitwiseNotResultType(rhsNode, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_INDIRECTION:
    operandType = OpRuleManager::getPrefixMulResultType(rhsNode, operandType, 0);
    break;
  case PrefixUnaryExprNode::OP_ADDRESS_OF:
    operandType = OpRuleManager::getPrefixBitwiseAndResultType(rhsNode, operandType, 0);
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

  switch (node->op) {
  case PostfixUnaryExprNode::OP_SUBSCRIPT: {
    // Check if we can apply the subscript operator on the lhs type
    if (!lhsType.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
      throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE,
                          "Can only apply subscript operator on array type, got " + lhsType.getName(true));

    // Visit index assignment
    AssignExprNode *indexAssignExpr = node->assignExpr();
    SymbolType indexType = std::any_cast<ExprResult>(visit(indexAssignExpr)).type;
    // Check if the index is of the right type
    if (!indexType.isOneOf({TY_INT, TY_LONG}))
      throw SemanticError(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long");

    // Check if we have an unsafe operation
    if (lhsType.is(TY_PTR) && !currentScope->doesAllowUnsafeOperations())
      throw SemanticError(
          node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
          "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.");

    // Check if we have a hardcoded array index
    if (lhsType.is(TY_ARRAY) && lhsType.getArraySize() != ARRAY_SIZE_UNKNOWN && indexAssignExpr->hasCompileTimeValue()) {
      std::int32_t constIndex = indexAssignExpr->getCompileTimeValue().intValue;
      size_t constSize = lhsType.getArraySize();
      // Check if we are accessing out-of-bounds memory
      if (constIndex >= constSize) {
        const std::string idxStr = std::to_string(constIndex);
        const std::string sizeStr = std::to_string(constSize);
        throw SemanticError(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                            "You are trying to access element with index " + idxStr + " of an array with size " + sizeStr);
      }
    }

    // Get item type
    lhsType = lhsType.getContainedTy();
    break;
  }
  case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
    const std::string &fieldName = node->identifier;

    // Check if lhs is enum or strobj
    SymbolType lhsBaseTy = lhsType;
    TypeChecker::autoDeReference(lhsBaseTy);
    if (!lhsBaseTy.is(TY_STRUCT))
      throw SemanticError(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + lhsType.getName());

    // Retrieve registry entry
    const std::string &structName = lhsBaseTy.getSubType();
    Scope *structScope = lhsBaseTy.getStructBodyScope();

    // Get accessed field
    SymbolTableEntry *memberEntry = structScope->lookupStrict(fieldName);
    if (!memberEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE,
                          "Field '" + node->identifier + "' not found in struct " + structName);
    SymbolType memberType = memberEntry->getType();

    // Check for insufficient visibility
    if (structScope->isImportedBy(rootScope) && !memberEntry->getType().isPublic())
      throw SemanticError(node, INSUFFICIENT_VISIBILITY, "Cannot access field '" + fieldName + "' due to its private visibility");

    // Set field to used
    memberEntry->used = true;

    // Remove reference wrappers
    memberType = memberType.removeReferenceWrappers();

    // Overwrite type and entry of left side with member type and entry
    lhsType = memberType;
    lhsEntry = memberEntry;
    break;
  }
  case PostfixUnaryExprNode::OP_PLUS_PLUS: {
    lhsType = OpRuleManager::getPostfixPlusPlusResultType(lhsNode, lhsType, 0);

    // In case the lhs is captured, notify the capture about the write access
    if (Capture *lhsCapture = lhsEntry ? currentScope->symbolTable.lookupCapture(lhsEntry->name) : nullptr; lhsCapture)
      lhsCapture->setCaptureMode(READ_WRITE);

    break;
  }
  case PostfixUnaryExprNode::OP_MINUS_MINUS: {
    lhsType = OpRuleManager::getPostfixMinusMinusResultType(lhsNode, lhsType, 0);

    // In case the lhs is captured, notify the capture about the write access
    if (Capture *lhsCapture = lhsEntry ? currentScope->symbolTable.lookupCapture(lhsEntry->name) : nullptr; lhsCapture)
      lhsCapture->setCaptureMode(READ_WRITE);

    break;
  }
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE;
  }

  if (lhsType.is(TY_INVALID)) {
    const std::string varName = lhsEntry ? lhsEntry->name : "";
    throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + varName + "' was referenced before declared");
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
  if (node->lenCall())
    return visit(node->lenCall());
  if (node->tidCall())
    return visit(node->tidCall());
  if (node->joinCall())
    return visit(node->joinCall());

  // Check for assign expression within parentheses
  if (node->assignExpr())
    return visit(node->assignExpr());

  // Identifier (local or global variable access)
  assert(!node->fqIdentifier.empty());
  if (!accessScope)
    accessScope = currentScope;

  // Retrieve var entry
  SymbolTableEntry *varEntry;
  if (node->identifierFragments.size() > 1) {
    const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqIdentifier);
    if (!registryEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found");
    varEntry = registryEntry->targetEntry;
    accessScope = registryEntry->targetScope;
  } else {
    // Load symbol table entry
    varEntry = accessScope->lookup(node->identifierFragments.back());
    if (!varEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found");
  }
  assert(varEntry != nullptr);
  assert(accessScope != nullptr);
  node->entries.at(manIdx) = varEntry;
  node->accessScopes.at(manIdx) = accessScope;
  SymbolType varType = varEntry->getType();

  if (varType.is(TY_INVALID))
    throw SemanticError(node, USED_BEFORE_DECLARED, "Symbol '" + varEntry->name + "' was used before declared.");

  // The base type should be a primitive or struct
  if (!varType.getBaseType().isPrimitive() && !varType.getBaseType().isOneOf({TY_STRUCT, TY_DYN}))
    throw SemanticError(node, INVALID_SYMBOL_ACCESS, "A symbol of type " + varType.getName() + " cannot be accessed here");

  // Check if is an imported variable
  if (accessScope->isImportedBy(currentScope)) {
    // Check if the entry is public
    if (!varEntry->getType().isPublic())
      throw SemanticError(node, INSUFFICIENT_VISIBILITY, "Cannot access '" + varEntry->name + "' due to its private visibility");
  }

  // Check if we have seen a 'this.' prefix, because the generator needs that
  if (varEntry->scope->type == SCOPE_STRUCT && node->identifierFragments.front() != THIS_VARIABLE_NAME)
    throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE,
                        "The symbol '" + node->fqIdentifier + "' could not be found. Missing 'this.' prefix?");

  // Set symbol table entry to used
  varEntry->used = true;

  // De-reference references
  while (varType.isRef())
    varType = varType.getContainedTy();

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
    const SymbolTableEntry *structEntry = nameRegistryEntry->targetEntry;
    if (!structEntry->getType().isPublic() && accessScope->parent->isImportedBy(currentScope))
      throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                          "Cannot access struct '" + structEntry->name + "' due to its private visibility");
  }

  return ExprResult{node->setEvaluatedSymbolType(varType, manIdx), varEntry};
}

std::any TypeChecker::visitValue(ValueNode *node) {
  // Function call
  if (node->functionCall())
    return visit(node->functionCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  // Typed nil
  if (node->isNil) {
    auto nilType = std::any_cast<SymbolType>(visit(node->nilType()));
    if (nilType.is(TY_DYN))
      throw SemanticError(node->nilType(), UNEXPECTED_DYN_TYPE, "Nil must have an explicit type");
    return ExprResult{node->setEvaluatedSymbolType(nilType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "Value fall-through");
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
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Constant fall-through");
  }
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(superType), manIdx)};
}

std::any TypeChecker::visitFunctionCall(FunctionCallNode *node) {
  FunctionCallNode::FunctionCallData &data = node->data.at(manIdx);

  // Visit args
  data.argTypes.clear();
  if (node->hasArgs) {
    data.argTypes.reserve(node->argLst()->args().size());
    for (AssignExprNode *arg : node->argLst()->args()) {
      // Visit argument
      const SymbolType argType = any_cast<ExprResult>(visit(arg)).type;
      assert(!argType.hasAnyGenericParts());
      // Save arg type to arg types list
      data.argTypes.push_back(argType);
    }
  }

  // Check if this is a ctor call to the String type
  const bool isStringRt = rootScope->lookupStrict(STROBJ_NAME) != nullptr;
  if (node->functionNameFragments.size() == 1 && node->fqFunctionName == STROBJ_NAME && !isStringRt)
    sourceFile->requestRuntimeModule(STRING_RT);

  // Retrieve entry of the first fragment
  SymbolTableEntry *firstFragmentEntry = currentScope->lookup(node->functionNameFragments.front());
  if (firstFragmentEntry) {
    firstFragmentEntry->used = true;
    data.isMethodCall = firstFragmentEntry->getType().isBaseType(TY_STRUCT) && firstFragmentEntry->scope->symbolTable.parent;
  }

  // Check if this is a method call or a normal function call
  SymbolType returnType(TY_DYN);
  SymbolType thisType(TY_DYN);
  if (data.isMethodCall) {
    // This is a method call
    data.thisType = firstFragmentEntry->getType();
    Scope *structBodyScope = data.thisType.getBaseType().getStructBodyScope();
    assert(structBodyScope != nullptr);
    auto [scope, type] = visitMethodCall(node, structBodyScope);
    data.calleeParentScope = scope;
    thisType = type;
  } else {
    // This is an ordinary function call
    auto [scope, type, knownStructName] = visitOrdinaryFctCall(node);
    data.calleeParentScope = scope;
    thisType = type;

    // Only ordinary function calls can be constructors
    if (data.isConstructorCall) {
      // Set a name to the thisType that is known to the current source file
      thisType = thisType.replaceBaseSubType(knownStructName);
      assert(thisType.is(TY_STRUCT));
    }
  }
  assert(data.calleeParentScope != nullptr);

  // Check if we were able to find a function
  if (!data.callee) {
    // Build error message
    const std::string functionName = data.isConstructorCall ? CTOR_FUNCTION_NAME : node->functionNameFragments.back();
    ParamList errArgTypes;
    for (const SymbolType &argType : data.argTypes)
      errArgTypes.push_back({argType, false});
    const std::string signature = Function::getSignature(functionName, thisType, returnType, errArgTypes, {});
    // Throw error
    throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure '" + signature + "' could not be found");
  }

  // Check if we need to request a re-visit, because the function body was not type-checked yet
  if (!data.callee->alreadyTypeChecked && !data.callee->external)
    reVisitRequested = true;

  // Retrieve return type
  if (data.isConstructorCall) {
    // Add anonymous symbol to keep track of de-allocation
    currentScope->symbolTable.insertAnonymous(thisType, node);
    // Set return type to 'this' type
    returnType = thisType;
  } else {
    // Set return type to return type of function
    returnType = data.callee->returnType;
  }

  // Get function entry from function object
  SymbolTableEntry *functionEntry = data.callee->entry;

  // Check if the called function has sufficient visibility
  data.isImported = data.calleeParentScope->isImportedBy(rootScope);
  if (data.isImported && !functionEntry->getType().isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Function/procedure '" + data.callee->getSignature() + "' has insufficient visibility");

  // Check if down-call
  if (!data.isImported) {
    const CodeLoc &callLoc = node->codeLoc;
    const CodeLoc &defLoc = functionEntry->getDeclCodeLoc();
    data.isDownCall = defLoc.line > callLoc.line || (defLoc.line == callLoc.line && defLoc.col > callLoc.col);
  }

  // Procedures always have the return type 'bool'
  if (data.callee->isProcedure() || (data.callee->isMethodProcedure() && !data.isConstructorCall) || returnType.is(TY_DYN))
    returnType = SymbolType(TY_BOOL);

  // Check if the return value gets used
  if ((data.callee->isFunction() || data.callee->isMethodFunction()) && !node->hasReturnValueReceiver())
    warnings.emplace_back(node->codeLoc, UNUSED_RETURN_VALUE, "The return value of the function call is unused");

  return ExprResult{node->setEvaluatedSymbolType(returnType, manIdx)};
}

std::tuple<Scope *, SymbolType, std::string> TypeChecker::visitOrdinaryFctCall(FunctionCallNode *node) {
  FunctionCallNode::FunctionCallData &data = node->data.at(manIdx);

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
      throw SemanticError(node->templateTypeLst(), ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list");
  }

  // Check if the exported name registry contains that function name
  const NameRegistryEntry *functionRegistryEntry = sourceFile->getNameRegistryEntry(fqFunctionName);
  if (!functionRegistryEntry)
    throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION,
                        "Function/procedure/struct '" + node->functionNameFragments.back() + "' could not be found");
  SymbolTableEntry *functionEntry = functionRegistryEntry->targetEntry;

  // Check if the target symbol is a struct -> this must be a constructor call
  data.isConstructorCall = functionEntry != nullptr && functionEntry->getType().is(TY_STRUCT);

  // Get concrete template types
  if (node->hasTemplateTypes) {
    // Only constructors may have template types
    if (!data.isConstructorCall)
      throw SemanticError(node->templateTypeLst(), INVALID_TEMPLATE_TYPES,
                          "Template types are only allowed for constructor calls");

    for (DataTypeNode *templateTypeNode : node->templateTypeLst()->dataTypes()) {
      auto templateType = std::any_cast<SymbolType>(visit(templateTypeNode));
      assert(!templateType.isOneOf({TY_DYN, TY_INVALID}));

      // Check if the given type is generic
      if (templateType.is(TY_GENERIC))
        throw SemanticError(templateTypeNode, EXPECTED_NON_GENERIC_TYPE, "You must specify a concrete type here");

      concreteTemplateTypes.push_back(templateType);
    }
  }

  // For constructor calls, do some preparation
  std::string knownStructName;
  std::string functionName = node->functionNameFragments.back();
  if (data.isConstructorCall) {
    const NameRegistryEntry *structRegistryEntry = functionRegistryEntry;
    const SymbolTableEntry *structEntry = functionEntry;
    const std::string structName = structRegistryEntry->targetEntry->name;

    // Substantiate potentially generic this struct
    Struct *thisStruct = StructManager::matchStruct(structEntry->scope, structName, concreteTemplateTypes, node);
    if (!thisStruct) {
      const std::string signature = Struct::getSignature(structName, concreteTemplateTypes);
      throw SemanticError(node, UNKNOWN_DATATYPE,
                          "Could not find struct candidate for struct '" + signature + "'. Do the template types match?");
    }

    // Override function name
    functionName = CTOR_FUNCTION_NAME;

    // Retrieve the name registry entry for the constructor
    functionRegistryEntry = sourceFile->getNameRegistryEntry(fqFunctionName + MEMBER_ACCESS_TOKEN + functionName);
    // Check if the constructor was found
    if (!functionRegistryEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "The struct '" + structName + "' does not provide a constructor");

    // Set the 'this' type of the function to the struct type
    data.thisType = structEntry->getType();
    data.thisType.setStructBodyScope(thisStruct->structScope);

    // Get the fully qualified name, that can be used in the current source file to identify the struct
    knownStructName = structRegistryEntry->name;
  }

  // Attach the concrete template types to the 'this' type
  if (!data.thisType.is(TY_DYN) && !concreteTemplateTypes.empty())
    data.thisType.setTemplateTypes(concreteTemplateTypes);

  // Map local types to imported types
  Scope *functionParentScope = functionRegistryEntry->targetScope;
  std::vector<SymbolType> importedArgTypes = data.argTypes;
  for (SymbolType &symbolType : importedArgTypes)
    symbolType = mapLocalTypeToImportedScopeType(functionParentScope, symbolType);

  // Retrieve function object
  data.callee = FunctionManager::matchFunction(functionParentScope, functionName, data.thisType, importedArgTypes, node);

  return std::make_tuple(functionParentScope, data.thisType, knownStructName);
}

std::pair<Scope *, SymbolType> TypeChecker::visitMethodCall(FunctionCallNode *node, Scope *structScope) const {
  FunctionCallNode::FunctionCallData &data = node->data.at(manIdx);

  // Methods cannot have template types
  if (node->hasTemplateTypes)
    throw SemanticError(node->templateTypeLst(), INVALID_TEMPLATE_TYPES, "Template types are only allowed for constructor calls");

  // Traverse through structs - the first fragment is already looked up and the last one is the function name
  for (size_t i = 1; i < node->functionNameFragments.size() - 1; i++) {
    const std::string identifier = node->functionNameFragments.at(i);

    // Retrieve field entry
    SymbolTableEntry *fieldEntry = structScope->lookupStrict(identifier);
    if (!fieldEntry->getType().is(TY_STRUCT))
      throw SemanticError(node, INVALID_MEMBER_ACCESS, "Cannot call a method on '" + identifier + "', since it is no struct");
    fieldEntry->used = true;

    // Get struct type and scope
    data.thisType = fieldEntry->getType();
    structScope = data.thisType.getStructBodyScope();
    assert(structScope != nullptr);
  }

  // Map local types to imported types
  Scope *functionParentScope = structScope;
  // Arg types
  std::vector<SymbolType> importedArgTypes = data.argTypes;
  for (SymbolType &symbolType : importedArgTypes)
    symbolType = mapLocalTypeToImportedScopeType(functionParentScope, symbolType);
  // 'this' type
  SymbolType thisType = data.thisType;
  TypeChecker::autoDeReference(thisType);
  SymbolType importedThisType = mapLocalTypeToImportedScopeType(functionParentScope, thisType);

  // Retrieve function object
  const std::string functionName = node->functionNameFragments.back();
  data.callee = FunctionManager::matchFunction(functionParentScope, functionName, importedThisType, importedArgTypes, node);

  return std::make_pair(functionParentScope, data.thisType);
}

std::any TypeChecker::visitArrayInitialization(ArrayInitializationNode *node) {
  SymbolType actualItemType(TY_DYN);
  // Check if all values have the same type
  if (node->itemLst()) {
    node->actualSize = static_cast<long>(node->itemLst()->args().size());
    for (AssignExprNode *arg : node->itemLst()->args()) {
      const SymbolType itemType = std::any_cast<ExprResult>(visit(arg)).type;
      if (actualItemType.is(TY_DYN)) // Perform type inference
        actualItemType = itemType;
      else if (itemType != actualItemType) // Check if types are matching
        throw SemanticError(arg, ARRAY_ITEM_TYPE_NOT_MATCHING,
                            "All provided values have to be of the same data type. You provided " + actualItemType.getName() +
                                " and " + itemType.getName());
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
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + structName + "'");

  // Retrieve struct
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(structName);
  if (!registryEntry)
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + structName + "'");
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
      throw SemanticError(node->templateTypeLst(), ALIAS_WITH_TEMPLATE_LIST, "The aliased type already has a template list");
  }

  if (node->templateTypeLst()) {
    concreteTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
      auto concreteType = std::any_cast<SymbolType>(visit(dataType));
      // Check if generic type
      if (concreteType.is(TY_GENERIC))
        throw SemanticError(dataType, EXPECTED_NON_GENERIC_TYPE, "Struct instantiations may only take concrete template types");
      concreteTemplateTypes.push_back(concreteType);
    }
  }

  // Get the struct instance
  structName = structEntry->name;
  Struct *spiceStruct = StructManager::matchStruct(structScope->parent, structName, concreteTemplateTypes, node);
  if (!spiceStruct) {
    const std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
  }
  node->instantiatedStructs.at(manIdx) = spiceStruct;

  // Use scope of concrete substantiation and not the scope of the generic type
  structScope = spiceStruct->structScope;
  structType.setStructBodyScope(structScope);

  // Set template types to the struct
  std::vector<SymbolType> templateTypes;
  for (const GenericType &genericType : spiceStruct->templateTypes)
    templateTypes.emplace_back(genericType.typeChain);
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  if (node->fieldLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst()->args().size())
      throw SemanticError(node->fieldLst(), NUMBER_OF_FIELDS_NOT_MATCHING,
                          "You've passed too less/many field values. Pass either none or all of them");

    // Check if the field types are matching
    for (size_t i = 0; i < node->fieldLst()->args().size(); i++) {
      // Get actual type
      AssignExprNode *assignExpr = node->fieldLst()->args().at(i);
      SymbolType actualType = std::any_cast<ExprResult>(visit(assignExpr)).type;
      // Get expected type
      const SymbolTableEntry *expectedField = structScope->symbolTable.lookupByIndex(i);
      assert(expectedField != nullptr);
      SymbolType expectedType = expectedField->getType();
      // Also accept, if expected type is reference of actual type
      if (expectedType.isRef() && expectedType.getContainedTy() == actualType)
        expectedType = expectedType.getContainedTy();
      // Check if type matches declaration
      if (actualType != expectedType)
        throw SemanticError(assignExpr, FIELD_TYPE_NOT_MATCHING,
                            "Expected type " + expectedType.getName() + " for the field '" + expectedField->name + "', but got " +
                                actualType.getName());
      // Check if immediate assigned to non-const reference
      if (expectedType.isRef() && !expectedField->getType().isConst() && assignExpr->hasCompileTimeValue())
        throw SemanticError(
            assignExpr, FIELD_TYPE_NOT_MATCHING,
            "Cannot assign immediate to non-const reference field. Either make the reference const or reference a variable");
    }
  } else {
    for (SymbolType &fieldType : spiceStruct->fieldTypes) {
      if (fieldType.isRef())
        throw SemanticError(node, REFERENCE_WITHOUT_INITIALIZER,
                            "The struct takes at least one reference field. You need to instantiate it with all fields.");
    }
  }

  // Update type of struct entry
  structEntry->updateType(structType, true);

  // Insert anonymous symbol to keep track of dtor calls for de-allocation
  SymbolTableEntry *anonymousEntry = currentScope->symbolTable.insertAnonymous(structType, node);

  return ExprResult{node->setEvaluatedSymbolType(structType, manIdx), anonymousEntry};
}

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  // Visit base data type
  auto type = std::any_cast<SymbolType>(visit(node->baseDataType()));

  size_t assignExprCounter = 0;
  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    DataTypeNode::TypeModifier typeModifier = tmQueue.front();

    // Only the outermost array can have an unknown size
    if (type.isArray() && type.getArraySize() == ARRAY_SIZE_UNKNOWN)
      throw SemanticError(node, ARRAY_SIZE_INVALID,
                          "Usage of incomplete array type. Only the outermost array type may have unknown size");

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
          throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + varName + "' ");
        if (!globalVar->getType().isConst())
          throw SemanticError(node, EXPECTED_CONST_VARIABLE, "The size of the array must be known at compile time");
        if (!globalVar->getType().is(TY_INT))
          throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Expected variable of type int");
        assert(globalVar->compileTimeValue != nullptr);
        typeModifier.hardcodedSize = globalVar->compileTimeValue->intValue;
      }

      if (typeModifier.hasSize && typeModifier.hardcodedSize <= 1)
        throw SemanticError(node, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated");
      type = type.toArray(node, typeModifier.hardcodedSize);
      break;
    }
    default:
      throw CompilerError(UNHANDLED_BRANCH, "Modifier type fall-through"); // GCOV_EXCL_LINE
    }
    tmQueue.pop();
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
  case BaseDataTypeNode::TY_CUSTOM:
    return node->setEvaluatedSymbolType(std::any_cast<SymbolType>(visit(node->customDataType())), manIdx);
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
  const bool isStringRT = rootScope->lookupStrict(STROBJ_NAME) != nullptr;
  if (!isImported && !isStringRT && firstFragment == STROBJ_NAME)
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
    throw SemanticError(node, UNKNOWN_DATATYPE, "Unknown datatype '" + node->fqTypeName + "'");
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *entry = registryEntry->targetEntry;
  assert(entry != nullptr);
  localAccessScope = registryEntry->targetScope->parent;

  // Get struct type and change it to the fully qualified name for identifying without ambiguities
  SymbolType entryType = entry->getType().replaceBaseSubType(registryEntry->name);

  // Enums can early-return
  if (entryType.is(TY_ENUM))
    return SymbolType(TY_INT);

  if (entryType.is(TY_STRUCT)) {
    // Check if struct is defined before the current code location, if defined in the same source file
    if (entry->declNode->codeLoc.sourceFilePath == node->codeLoc.sourceFilePath && entry->declNode->codeLoc > node->codeLoc)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Structs must be defined before usage");

    const bool isParamOrFieldOrReturnType = node->isParamType || node->isFieldType || node->isReturnType;

    // Collect the concrete template types
    std::vector<SymbolType> templateTypes;
    if (node->templateTypeLst()) {
      templateTypes.reserve(node->templateTypeLst()->dataTypes().size());
      for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes()) {
        auto templateType = std::any_cast<SymbolType>(visit(dataType));
        // Generic types are only allowed for parameters and fields at this point
        if (templateType.is(TY_GENERIC) && !isParamOrFieldOrReturnType)
          throw SemanticError(dataType, EXPECTED_NON_GENERIC_TYPE, "Only concrete template types are allowed here");
        templateTypes.push_back(templateType);
      }
      entryType.setTemplateTypes(templateTypes);
    }

    if (!node->templateTypeLst() || !isParamOrFieldOrReturnType) { // Only do the next step, if we have concrete template types
      // Set the struct instance to used, if found
      // Here, it is allowed to accept, that the struct cannot be found, because there are self-referencing structs
      const std::string structName = node->typeNameFragments.back();
      Struct *spiceStruct = StructManager::matchStruct(localAccessScope, structName, templateTypes, node);
      if (spiceStruct)
        entryType.setStructBodyScope(spiceStruct->structScope);
    }

    return node->setEvaluatedSymbolType(entryType, manIdx);
  }

  if (entryType.is(TY_INTERFACE)) {
    // Check if template types are given
    if (node->templateTypeLst())
      throw SemanticError(node->templateTypeLst(), INTERFACE_WITH_TEMPLATE_LIST,
                          "Referencing interfaces with template lists is not allowed");

    // Set the interface instance to used
    Interface *spiceInterface = localAccessScope->lookupInterface(node->typeNameFragments.back());
    assert(spiceInterface != nullptr);
    spiceInterface->used = true;

    return node->setEvaluatedSymbolType(entryType, manIdx);
  }

  const std::string errorMessage =
      entryType.is(TY_INVALID) ? "Used type before declared" : "Expected type, but got " + entryType.getName();
  throw SemanticError(node, EXPECTED_TYPE, errorMessage);
}

SymbolType TypeChecker::mapLocalTypeToImportedScopeType(const Scope *targetScope, const SymbolType &symbolType) const {
  // Skip all types, exception structs, interfaces and enums
  if (!symbolType.getBaseType().is(TY_STRUCT))
    return symbolType;

  // If the target scope is in the current source file, we can return the symbol type as is
  SourceFile *targetSourceFile = targetScope->sourceFile;
  if (targetSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of the target source file
  for (const auto &[_, entry] : targetSourceFile->exportedNameRegistry)
    if (entry.targetEntry != nullptr && entry.targetEntry->getType().isBaseType(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->structScope == symbolType.getBaseType().getStructBodyScope())
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
  // Skip all types, exception structs, interfaces and enums
  if (!symbolType.getBaseType().is(TY_STRUCT))
    return symbolType;

  // If the target scope is in the current source file, we can return the symbol type as is
  SourceFile *sourceSourceFile = sourceScope->sourceFile;
  if (sourceSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of the target source file
  for (const auto &[_, entry] : sourceFile->exportedNameRegistry)
    if (entry.targetEntry != nullptr && entry.targetEntry->getType().isBaseType(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->structScope == symbolType.getBaseType().getStructBodyScope()) {
          // Get the 'fullest-qualified' registry entry
          const NameRegistryEntry *mostQualifiedEntry = sourceFile->getNameRegistryEntry(entry.name);
          assert(mostQualifiedEntry != nullptr);
          return symbolType.replaceBaseSubType(mostQualifiedEntry->name);
        }

  // This source file does not know about the struct at all
  // -> show it how to find the struct
  const std::string structName = symbolType.getBaseType().getSubType();
  const NameRegistryEntry *origRegistryEntry = sourceSourceFile->getNameRegistryEntry(structName);
  assert(origRegistryEntry != nullptr);
  sourceFile->addNameRegistryEntry(structName, origRegistryEntry->targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

/**
 * Change to the passed scope.
 * For nested scopes in generic functions/procedures it is important to have the right parent for symbol lookups
 * Therefore, changeToScope sets the children's parent to the old scope to always have the right parent
 *
 * @param scope Scope to change to
 * @param scopeType Expected type of the given scope
 */
void TypeChecker::changeToScope(Scope *scope, const ScopeType scopeType) {
  assert(scope != nullptr);
  assert(scope->type == scopeType);
  // Adjust members of the new scope
  scope->parent = currentScope;
  scope->symbolTable.parent = &currentScope->symbolTable;
  // Set the scope
  currentScope = scope;
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

} // namespace spice::compiler