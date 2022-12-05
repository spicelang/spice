// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), rootScope(sourceFile->globalScope.get()),
      warnings(sourceFile->compilerOutput.warnings) {}

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;
  reVisitRequested = false;

  // Visit children
  visitChildren(node);

  // Reset the AST
  node->reset();

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
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_THREAD_BODY);

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  // ThreadDef returns a tid as byte*
  return node->setEvaluatedSymbolType(SymbolType(TY_BYTE).toPointer(node));
}

std::any TypeChecker::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Change to unsafe block body scope
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_UNSAFE_BODY);

  // Visit body
  visit(node->body());

  // Leave unsafe block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any TypeChecker::visitForLoop(ForLoopNode *node) {
  // Change to for body scope
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_FOR_BODY);

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
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_FOREACH_BODY);

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
    node->itemDecl()->dataType()->setEvaluatedSymbolType(itemType);
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
  currentScope = node->bodyScope;
  assert(currentScope->type == SCOPE_WHILE_BODY);

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

std::any TypeChecker::visitIfStmt(IfStmtNode *node) {
  // Change to then body scope
  currentScope = node->thenBodyScope;
  assert(currentScope->type == SCOPE_IF_ELSE_BODY);

  // Visit condition
  SymbolType conditionType = std::any_cast<ExprResult>(visit(node->condition())).type;
  // Check if condition evaluates to bool
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

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
  currentScope = node->elseBodyScope;
  assert(currentScope->type == SCOPE_IF_ELSE_BODY);

  // Visit body
  visit(node->body());

  // Leave else body scope
  currentScope = node->elseBodyScope->parent;

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

std::any TypeChecker::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Change to anonymous scope body scope
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
  FunctionManager::insertFunction(currentScope, signature, node, &node->signatureManifestations);

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

    // Check if type has to be inferred or both types are fixed
    localVarType = OpRuleManager::getAssignResultType(node, localVarType, rhsTy);

    // Push symbolType to the declaration data type
    node->dataType()->setEvaluatedSymbolType(localVarType);
  } else {
    // Visit data type
    localVarType = std::any_cast<SymbolType>(visit(node->dataType()));
  }

  // Update the type of the variable
  SymbolTableEntry *localVarEntry = currentScope->lookupStrict(node->varName);
  assert(localVarEntry != nullptr);
  localVarEntry->updateType(localVarType, /*overwriteExistingType=*/true);

  return node->setEvaluatedSymbolType(localVarType);
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

  // Check if function without return value
  if (!node->hasReturnValue)
    throw SemanticError(node, RETURN_WITHOUT_VALUE_RESULT, "Return without value, but result variable is not initialized yet");

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

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = node->templatedString.find_first_of('%');
  while (index != std::string::npos && index != node->templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->assignExpr().size() <= placeholderCount)
      throw SemanticError(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments");

    // Get next assignment
    AssignExprNode *assignment = node->assignExpr()[placeholderCount];
    // Visit assignment
    SymbolType argType = std::any_cast<ExprResult>(visit(assignment)).type;

    switch (node->templatedString[index + 1]) {
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
      if (!argType.is(TY_STRING) && !argType.isPointerOf(TY_CHAR) && !argType.isArrayOf(TY_CHAR))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "The placeholder string expects string, char* or char[], but got " + argType.getName());
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!argType.isPointer() && !argType.isArray() && !argType.is(TY_STRING))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "The placeholder string expects pointer, array or string, but got " + argType.getName());
      placeholderCount++;
      break;
    }
    }
    index = node->templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->assignExpr().size())
    throw SemanticError(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments");

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_BOOL))};
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  SymbolType symbolType;
  if (node->isType) { // Size of type
    symbolType = std::any_cast<SymbolType>(visit(node->dataType()));
  } else { // Size of value
    symbolType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;
  }

  // Check if symbol type is dynamically sized array
  if (symbolType.is(TY_ARRAY) && symbolType.getArraySize() == ARRAY_SIZE_DYNAMIC)
    throw SemanticError(node, SIZEOF_DYNAMIC_SIZED_ARRAY, "Cannot get size of dynamically sized array at compile time");

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT))};
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  SymbolType argType = std::any_cast<ExprResult>(visit(node->assignExpr())).type;

  // Check if arg is of type array
  if (!argType.isArray())
    throw SemanticError(node->assignExpr(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays");

  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT))};
}

std::any TypeChecker::visitTidCall(TidCallNode *node) {
  // Nothing to check here. Tid builtin has no arguments
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT))};
}

std::any TypeChecker::visitJoinCall(JoinCallNode *node) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(node);

  for (AssignExprNode *assignExpr : node->assignExpressions()) {
    // Visit assign expression
    SymbolType assignExprType = std::any_cast<ExprResult>(visit(assignExpr)).type;
    // Check if type is byte* or byte*[]
    if (assignExprType == bytePtr && assignExprType.isArrayOf(bytePtr))
      throw SemanticError(assignExpr, JOIN_ARG_MUST_BE_TID,
                          "The join builtin expects a thread id (byte*) or an array of thread ids (byte*[])");
  }

  // Return the number of threads that were joined
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_INT))};
}

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if ternary
  if (node->ternaryExpr())
    return visit(node->ternaryExpr());

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
      rhsType = OpRuleManager::getAssignResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_PLUS_EQUAL) {
      rhsType = opRuleManager.getPlusEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_MINUS_EQUAL) {
      rhsType = opRuleManager.getMinusEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_MUL_EQUAL) {
      rhsType = OpRuleManager::getMulEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_DIV_EQUAL) {
      rhsType = OpRuleManager::getDivEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_REM_EQUAL) {
      rhsType = OpRuleManager::getRemEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_SHL_EQUAL) {
      rhsType = OpRuleManager::getSHLEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_SHR_EQUAL) {
      rhsType = OpRuleManager::getSHREqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_AND_EQUAL) {
      rhsType = OpRuleManager::getAndEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_OR_EQUAL) {
      rhsType = OpRuleManager::getOrEqualResultType(node, lhsType, rhsType);
    } else if (node->op == AssignExprNode::OP_XOR_EQUAL) {
      rhsType = OpRuleManager::getXorEqualResultType(node, lhsType, rhsType);
    }

    if (lhsVar) { // Variable is involved on the left side
      // Perform type inference
      if (lhsType.is(TY_DYN) || (lhsType.is(TY_STRING) && rhsType.is(TY_STRING)))
        lhsVar->updateType(rhsType, false);

      // In case the lhs variable is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsVar->name); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    }

    return ExprResult{node->setEvaluatedSymbolType(rhsType)};
  }

  throw std::runtime_error("Internal compiler error: AssignStmt fall-through"); // GCOV_EXCL_LINE
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

  return ExprResult{node->setEvaluatedSymbolType(trueType)};
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (int i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    currentType = OpRuleManager::getLogicalOrResultType(node, currentType, rhsTy);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
}

std::any TypeChecker::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (int i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    currentType = OpRuleManager::getLogicalAndResultType(node, currentType, rhsTy);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
}

std::any TypeChecker::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (int i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    currentType = OpRuleManager::getBitwiseOrResultType(node, currentType, rhsTy);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
}

std::any TypeChecker::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (int i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    currentType = OpRuleManager::getBitwiseXorResultType(node, currentType, rhsTy);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
}

std::any TypeChecker::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->children.size() == 1)
    return visit(node->operands().front());

  // Visit leftmost operand
  SymbolType currentType = std::any_cast<ExprResult>(visit(node->operands()[0])).type;
  // Loop through all remaining operands
  for (int i = 1; i < node->operands().size(); i++) {
    SymbolType rhsTy = std::any_cast<ExprResult>(visit(node->operands()[i])).type;
    currentType = OpRuleManager::getBitwiseAndResultType(node, currentType, rhsTy);
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
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
    resultType = OpRuleManager::getEqualResultType(node, lhsTy, rhsTy);
  else if (node->op == EqualityExprNode::OP_NOT_EQUAL) // Operator was not equal
    resultType = OpRuleManager::getNotEqualResultType(node, lhsTy, rhsTy);
  else
    throw std::runtime_error("Internal compiler error: EqualityExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType)};
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
    resultType = OpRuleManager::getLessResultType(node, lhsTy, rhsTy);
  else if (node->op == RelationalExprNode::OP_GREATER) // Operator was greater
    resultType = OpRuleManager::getGreaterResultType(node, lhsTy, rhsTy);
  else if (node->op == RelationalExprNode::OP_LESS_EQUAL) // Operator was less equal
    resultType = OpRuleManager::getLessEqualResultType(node, lhsTy, rhsTy);
  else if (node->op == RelationalExprNode::OP_GREATER_EQUAL) // Operator was greater equal
    resultType = OpRuleManager::getGreaterEqualResultType(node, lhsTy, rhsTy);
  else
    throw std::runtime_error("Internal compiler error: RelationalExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType)};
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
    resultType = OpRuleManager::getShiftLeftResultType(node, lhsTy, rhsTy);
  else if (node->op == ShiftExprNode::OP_SHIFT_RIGHT) // Operator was shr
    resultType = OpRuleManager::getShiftRightResultType(node, lhsTy, rhsTy);
  else
    throw std::runtime_error("Internal compiler error: ShiftExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType)};
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
      currentType = opRuleManager.getPlusResultType(operand, currentType, operandType);
    else if (op == AdditiveExprNode::OP_MINUS)
      currentType = opRuleManager.getMinusResultType(operand, currentType, operandType);
    else
      throw std::runtime_error("Additive expr fall-through");

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentType);
    node->opQueue.pop();
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
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
      currentType = OpRuleManager::getMulResultType(operand, currentType, operandType);
    else if (op == MultiplicativeExprNode::OP_DIV)
      currentType = OpRuleManager::getDivResultType(operand, currentType, operandType);
    else if (op == MultiplicativeExprNode::OP_REM)
      currentType = OpRuleManager::getRemResultType(operand, currentType, operandType);
    else
      throw std::runtime_error("Multiplicative expr fall-through");

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentType);
    node->opQueue.pop();
  }

  return ExprResult{node->setEvaluatedSymbolType(currentType)};
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
  SymbolType resultType = opRuleManager.getCastResultType(node, dstType, srcType);

  return ExprResult{node->setEvaluatedSymbolType(resultType)};
}

std::any TypeChecker::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  // Reset access scope
  accessScope = nullptr;

  // Visit the right side
  auto [operandType, operandEntry] = std::any_cast<ExprResult>(visit(node->postfixUnaryExpr()));

  // Loop through the op queue
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    // Check operator
    const PrefixUnaryExprNode::PrefixUnaryOp &op = node->opQueue.front().first;
    switch (op) {
    case PrefixUnaryExprNode::OP_MINUS:
      operandType = OpRuleManager::getPrefixMinusResultType(node->postfixUnaryExpr(), operandType);
      break;
    case PrefixUnaryExprNode::OP_PLUS_PLUS:
      operandType = OpRuleManager::getPrefixPlusPlusResultType(node->postfixUnaryExpr(), operandType);
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = operandEntry ? currentScope->symbolTable.lookupCapture(operandEntry->name) : nullptr; lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_MINUS_MINUS:
      operandType = OpRuleManager::getPrefixMinusMinusResultType(node->postfixUnaryExpr(), operandType);
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = operandEntry ? currentScope->symbolTable.lookupCapture(operandEntry->name) : nullptr; lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_NOT:
      operandType = OpRuleManager::getPrefixNotResultType(node->postfixUnaryExpr(), operandType);
      break;
    case PrefixUnaryExprNode::OP_BITWISE_NOT:
      operandType = OpRuleManager::getPrefixBitwiseNotResultType(node->postfixUnaryExpr(), operandType);
      break;
    case PrefixUnaryExprNode::OP_INDIRECTION:
      operandType = OpRuleManager::getPrefixMulResultType(node->postfixUnaryExpr(), operandType);
      break;
    case PrefixUnaryExprNode::OP_ADDRESS_OF:
      operandType = OpRuleManager::getPrefixBitwiseAndResultType(node->postfixUnaryExpr(), operandType);
      break;
    default:
      throw std::runtime_error("Prefix unary fall-through");
    }

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, operandType);
    node->opQueue.pop();
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType), operandEntry};
}

std::any TypeChecker::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  // Visit left side
  auto [lhsTy, lhsEntry] = std::any_cast<ExprResult>(visit(node->atomicExpr()));

  size_t subscriptCounter = 0;
  size_t identifierCounter = 0;

  // Loop through op queue
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    const PostfixUnaryExprNode::PostfixUnaryOp &op = node->opQueue.front().first;
    switch (op) {
    case PostfixUnaryExprNode::OP_SUBSCRIPT: {
      // Check if we can apply the subscript operator on the lhs type
      if (!lhsTy.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
        throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE,
                            "Can only apply subscript operator on array type, got " + lhsTy.getName(true));

      // Visit index assignment
      AssignExprNode *indexAssignExpr = node->assignExpr()[subscriptCounter++];
      SymbolType indexType = std::any_cast<ExprResult>(visit(indexAssignExpr)).type;
      // Check if the index is of the right type
      if (!indexType.isOneOf({TY_INT, TY_LONG}))
        throw SemanticError(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long");

      // Check if we have an unsafe operation
      if (lhsTy.is(TY_PTR) && !currentScope->doesAllowUnsafeOperations())
        throw SemanticError(
            node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
            "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.");

      // Check if we have a hardcoded array size
      if (lhsTy.is(TY_ARRAY) && lhsTy.getArraySize() > ARRAY_SIZE_UNKNOWN && indexAssignExpr->hasCompileTimeValue()) {
        std::int32_t constIndex = indexAssignExpr->getCompileTimeValue().intValue;
        size_t constSize = lhsTy.getArraySize();
        // Check if we are accessing out-of-bounds memory
        if (constIndex >= constSize) {
          const std::string idxStr = std::to_string(constIndex);
          const std::string sizeStr = std::to_string(constSize);
          throw SemanticError(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                              "You are trying to access element with index " + idxStr + " of an array with size " + sizeStr);
        }
      }

      // Get item type
      lhsTy = lhsTy.getContainedTy();
      break;
    }
    case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
      // Check if lhs is enum or strobj
      if (!lhsTy.isBaseType(TY_STRUCT))
        throw SemanticError(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + lhsTy.getName());

      // Retrieve registry entry
      const std::string structName = lhsEntry->getType().getBaseType().getSubType();
      const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(structName);
      assert(registryEntry != nullptr);
      Scope *structScope = registryEntry->targetScope;
      assert(structScope != nullptr);

      // Get accessed field
      SymbolTableEntry *memberEntry = structScope->lookupStrict(node->identifier[identifierCounter]);
      if (!memberEntry)
        throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE,
                            "Field '" + node->identifier[identifierCounter] + "' not found in struct " + structName);

      // Check for insufficient visibility
      if (structScope->isImportedBy(rootScope) && !memberEntry->specifiers.isPublic())
        throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                            "Cannot access field '" + memberEntry->name + "' due to its private visibility");

      // Set field to used
      memberEntry->used = true;

      // Overwrite type and entry of left side with member type and entry
      lhsTy = memberEntry->getType();
      lhsEntry = memberEntry;
      identifierCounter++;
      break;
    }
    case PostfixUnaryExprNode::OP_PLUS_PLUS: {
      lhsTy = OpRuleManager::getPostfixPlusPlusResultType(node->atomicExpr(), lhsTy);
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = lhsEntry ? currentScope->symbolTable.lookupCapture(lhsEntry->name) : nullptr; lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    case PostfixUnaryExprNode::OP_MINUS_MINUS: {
      lhsTy = OpRuleManager::getPostfixMinusMinusResultType(node->atomicExpr(), lhsTy);
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = lhsEntry ? currentScope->symbolTable.lookupCapture(lhsEntry->name) : nullptr; lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    default:
      throw std::runtime_error("PostfixUnary fall-through");
    }

    node->opQueue.emplace(op, lhsTy);
    node->opQueue.pop();
  }

  if (lhsTy.is(TY_INVALID)) {
    const std::string varName = lhsEntry ? lhsEntry->name : "";
    throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + varName + "' was referenced before declared");
  }

  return ExprResult{node->setEvaluatedSymbolType(lhsTy), lhsEntry};
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
    assert(varEntry != nullptr);
    accessScope = registryEntry->targetScope;
    assert(accessScope != nullptr);
  } else {
    // Load symbol table entry
    varEntry = accessScope->lookup(node->identifierFragments.back());
    if (!varEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found");
  }
  const SymbolType varType = varEntry->getType();

  if (varType.is(TY_INVALID))
    throw SemanticError(node, USED_BEFORE_DECLARED, "Symbol '" + varEntry->name + "' was used before declared.");

  // The base type should be a primitive or struct
  if (!varType.getBaseType().isPrimitive() && !varType.isBaseType(TY_STRUCT))
    throw SemanticError(node, INVALID_SYMBOL_ACCESS, "A symbol of type " + varType.getName() + " cannot be accessed here");

  // Check if is an imported variable
  if (accessScope->isImportedBy(currentScope)) {
    // Check if the entry is public
    if (!varEntry->specifiers.isPublic())
      throw SemanticError(node, INSUFFICIENT_VISIBILITY, "Cannot access '" + varEntry->name + "' due to its private visibility");
  }

  // Set symbol table entry to used
  varEntry->used = true;

  // Retrieve scope for the new scope path fragment
  if (varEntry->getType().isBaseType(TY_STRUCT)) { // Base type struct
    // Set access scope to struct scope
    const NameRegistryEntry *nameRegistryEntry = sourceFile->getNameRegistryEntry(varEntry->getType().getBaseType().getSubType());
    assert(nameRegistryEntry != nullptr);

    // Change the access scope to the struct scope
    accessScope = nameRegistryEntry->targetScope;
    assert(accessScope != nullptr);

    // Check if the entry is public if it is imported
    assert(nameRegistryEntry->targetEntry != nullptr);
    const SymbolTableEntry *structEntry = nameRegistryEntry->targetEntry;
    if (!structEntry->specifiers.isPublic() && accessScope->parent->isImportedBy(currentScope))
      throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                          "Cannot access struct '" + structEntry->name + "' due to its private visibility");
  }

  return ExprResult{node->setEvaluatedSymbolType(varEntry->getType()), varEntry};
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
      throw SemanticError(node->nilType(), UNEXPECTED_DYN_TYPE_SA, "Nil must have an explicit type");
    return ExprResult{node->setEvaluatedSymbolType(nilType)};
  }

  throw std::runtime_error("Value fall-through");
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
    throw std::runtime_error("Constant fall-through");
  }
  return ExprResult{node->setEvaluatedSymbolType(SymbolType(superType))};
}

std::any TypeChecker::visitFunctionCall(FunctionCallNode *node) {
  // Visit template type hints
  node->concreteTemplateTypes.clear();
  if (node->hasTemplateTypes) {
    node->concreteTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    for (DataTypeNode *templateTypeNode : node->templateTypeLst()->dataTypes()) {
      // Visit template type
      const auto templateType = any_cast<SymbolType>(visit(templateTypeNode));
      // Check if template type is valid
      if (templateType.isBaseType(TY_GENERIC))
        throw SemanticError(node, EXPECTED_NON_GENERIC_TYPE, "Template hints may contain non-generic types only");
      // Add the type to the concrete template types list
      node->concreteTemplateTypes.push_back(templateType);
    }
  }

  // Visit args
  node->argTypes.clear();
  if (node->hasArgs) {
    node->argTypes.reserve(node->argLst()->args().size());
    for (AssignExprNode *arg : node->argLst()->args()) {
      // Visit argument
      const SymbolType argType = any_cast<ExprResult>(visit(arg)).type;
      assert(!argType.isBaseType(TY_GENERIC));
      // Save arg type to arg types list
      node->argTypes.push_back(argType);
    }
  }

  // Check if this is a ctor call to the String type
  const bool isStringRt = rootScope->lookupStrict(STROBJ_NAME) != nullptr;
  if (node->functionNameFragments.size() == 1 && node->fqFunctionName == STROBJ_NAME && !isStringRt)
    sourceFile->requestRuntimeModule(STRING_RT);

  // Retrieve entry of the first fragment
  SymbolTableEntry *firstFragmentEntry = currentScope->lookup(node->functionNameFragments.front());

  // Check if this is a method call or a normal function call
  Scope *functionParentScope;
  SymbolType returnType(TY_DYN);
  SymbolType thisType(TY_DYN);
  if (firstFragmentEntry != nullptr && firstFragmentEntry->getType().is(TY_STRUCT) && firstFragmentEntry->symbolTable->parent) {
    // This is a method call
    node->isMethodCall = true;
    const SymbolType firstFragmentType = firstFragmentEntry->getType();
    Scope *structBodyScope = firstFragmentType.getStructBodyScope();
    assert(structBodyScope != nullptr);
    auto [scope, type] = visitMethodCall(node, firstFragmentType, structBodyScope);
    functionParentScope = scope;
    thisType = type;
  } else {
    // This is an ordinary function call
    auto [scope, type, knownStructName] = visitOrdinaryFctCall(node);
    functionParentScope = scope;
    thisType = type;

    // Only ordinary function calls can be constructors
    if (node->isConstructorCall) {
      // Set a name to the thisType that is known to the current source file
      thisType = thisType.replaceBaseSubType(knownStructName);
      assert(thisType.is(TY_STRUCT));
    }
  }
  assert(functionParentScope != nullptr);

  // Check if we were able to find a function
  if (!node->calledFunction) {
    // Build error message
    const std::string functionName = node->functionNameFragments.back();
    ParamList errArgTypes;
    for (const auto &argType : node->argTypes)
      errArgTypes.push_back({argType, false});
    const SymbolType dynType(TY_DYN);
    Function f(functionName, nullptr, thisType, returnType, errArgTypes, /*templateTypes=*/{}, node, /*external=*/false);
    // Throw error
    throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure '" + f.getSignature() + "' could not be found");
  }

  // Check if we need to request a re-visit, because the function body was not type-checked yet
  if (!node->calledFunction->alreadyTypeChecked)
    reVisitRequested = true;

  // Retrieve return type
  if (node->isConstructorCall) {
    // Add anonymous symbol to keep track of de-allocation
    currentScope->symbolTable.insertAnonymous(thisType, node);
    // Set return type to 'this' type
    returnType = thisType;
  } else {
    // Set return type to return type of function
    returnType = node->calledFunction->returnType;
  }

  // Get function entry from function object
  SymbolTableEntry *functionEntry = node->calledFunction->entry;

  // Check if the called function has sufficient visibility
  if (functionParentScope->isImportedBy(rootScope) && !functionEntry->specifiers.isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Function/procedure '" + node->calledFunction->getSignature() + "' has insufficient visibility");

  // Set the function to used
  node->calledFunction->used = true;
  functionEntry->used = true;

  // Procedures always have the return type 'bool'
  if (node->calledFunction->isProcedure() || returnType.is(TY_DYN))
    returnType = SymbolType(TY_BOOL);

  return ExprResult{node->setEvaluatedSymbolType(returnType)};
}

std::tuple<Scope *, SymbolType, std::string> TypeChecker::visitOrdinaryFctCall(FunctionCallNode *node) const {
  // Check if the exported name registry contains that function name
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqFunctionName);
  if (!registryEntry)
    throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "Function/procedure/struct could not be found");
  SymbolTableEntry *symbolEntry = registryEntry->targetEntry;

  // Check if the target symbol is a struct -> this must be a constructor call
  SymbolType thisType(TY_DYN);
  std::string knownStructName;
  if (symbolEntry != nullptr && symbolEntry->getType().is(TY_STRUCT)) {
    const NameRegistryEntry *structRegistryEntry = registryEntry;
    const std::string structName = structRegistryEntry->targetEntry->name;

    // Append the actual function name 'ctor' to the saved names
    node->functionNameFragments.emplace_back(CTOR_FUNCTION_NAME);
    node->fqFunctionName += ".";
    node->fqFunctionName += CTOR_FUNCTION_NAME;

    // Retrieve the name registry entry for the constructor
    registryEntry = sourceFile->getNameRegistryEntry(node->fqFunctionName);
    // Check if the constructor was found
    if (!registryEntry)
      throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "The struct '" + structName + "' does not provide a constructor");
    node->isConstructorCall = true;

    // Set the 'this' type of the function to the struct type
    thisType = symbolEntry->getType();

    // Set the struct to used
    structRegistryEntry->targetEntry->used = true;

    // Get the name, that can be used in the current source file to identify the struct
    knownStructName = structRegistryEntry->name;
  }

  // Retrieve function object
  const std::string functionName = node->functionNameFragments.back();
  Scope *functionParentScope = registryEntry->targetScope;
  node->calledFunction = FunctionManager::matchFunction(functionParentScope, functionName, thisType, node->concreteTemplateTypes,
                                                        node->argTypes, node);

  return std::make_tuple(functionParentScope, thisType, knownStructName);
}

std::pair<Scope *, SymbolType> TypeChecker::visitMethodCall(FunctionCallNode *node, SymbolType thisType, Scope *structScope) {
  // Traverse through structs - the first fragment is already looked up and the last one is the function name
  for (size_t i = 1; i < node->functionNameFragments.size() - 1; i++) {
    const std::string identifier = node->functionNameFragments.at(i);

    // Retrieve field entry
    SymbolTableEntry *fieldEntry = structScope->lookupStrict(identifier);
    if (!fieldEntry->getType().is(TY_STRUCT))
      throw SemanticError(node, INVALID_MEMBER_ACCESS, "Cannot call a method on '" + identifier + "', since it is no struct");
    fieldEntry->used = true;

    // Get struct type and scope
    thisType = fieldEntry->getType();
    structScope = thisType.getStructBodyScope();
    assert(structScope != nullptr);
  }

  // Retrieve function object
  const std::string functionName = node->functionNameFragments.back();
  Scope *functionParentScope = structScope;
  node->calledFunction = FunctionManager::matchFunction(functionParentScope, functionName, thisType, node->concreteTemplateTypes,
                                                        node->argTypes, node);

  return std::make_pair(functionParentScope, thisType);
}

std::any TypeChecker::visitArrayInitialization(ArrayInitializationNode *node) {
  SymbolType actualItemType(TY_DYN);
  // Check if all values have the same type
  long actualSize = 0;
  if (node->itemLst()) {
    actualSize = (long)node->itemLst()->args().size();
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

  const SymbolType arrayType = actualItemType.toArray(node, actualSize);
  return ExprResult{node->setEvaluatedSymbolType(arrayType)};
}

std::any TypeChecker::visitStructInstantiation(StructInstantiationNode *node) {
  // Check if access scope was altered
  if (accessScope != nullptr && accessScope != currentScope)
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + node->fqStructName + "'");

  // Retrieve struct
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqStructName);
  if (!registryEntry)
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Cannot find struct '" + node->fqStructName + "'");
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *structEntry = registryEntry->targetEntry;
  Scope *structScope = accessScope = registryEntry->targetScope;

  // Get struct type and change it to the fully qualified name for identifying without ambiguities
  SymbolType structType = structEntry->getType().replaceBaseSubType(registryEntry->name);

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (node->templateTypeLst()) {
    concreteTemplateTypes.reserve(node->templateTypeLst()->dataTypes().size());
    for (const auto &dataType : node->templateTypeLst()->dataTypes())
      concreteTemplateTypes.push_back(std::any_cast<SymbolType>(visit(dataType)));
  }

  // Get the struct instance
  Struct *spiceStruct = structScope->parent->matchStruct(currentScope, structEntry->name, concreteTemplateTypes, node);
  if (!spiceStruct) {
    const std::string structSignature = Struct::getSignature(structEntry->name, concreteTemplateTypes);
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
  }
  spiceStruct->used = true;

  // Set template types to the struct
  const std::vector<GenericType> templateTypesGeneric = spiceStruct->templateTypes;
  std::vector<SymbolType> templateTypes;
  for (const GenericType &genericType : templateTypesGeneric)
    templateTypes.emplace_back(genericType.typeChain);
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  std::vector<SymbolType> fieldTypes;
  if (node->fieldLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst()->args().size())
      throw SemanticError(node->fieldLst(), NUMBER_OF_FIELDS_NOT_MATCHING,
                          "You've passed too less/many field values. Pass either none or all of them");

    // Check if the field types are matching
    for (int i = 0; i < node->fieldLst()->args().size(); i++) {
      // Get actual type
      AssignExprNode *assignExpr = node->fieldLst()->args()[i];
      SymbolType actualType = std::any_cast<ExprResult>(visit(assignExpr)).type;
      // Get expected type
      const SymbolTableEntry *expectedField = structScope->symbolTable.lookupByIndex(i);
      assert(expectedField != nullptr);
      SymbolType expectedType = expectedField->getType();
      // Check if type matches declaration
      if (actualType != expectedType)
        throw SemanticError(assignExpr, FIELD_TYPE_NOT_MATCHING,
                            "Expected type " + expectedType.getName() + " for the field '" + expectedField->name + "', but got " +
                                actualType.getName());
    }
  }

  // Insert anonymous symbol to keep track of dtor calls for de-allocation
  SymbolTableEntry *anonymousEntry = currentScope->symbolTable.insertAnonymous(structType, node);

  return ExprResult{node->setEvaluatedSymbolType(structType), anonymousEntry};
}

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  // Visit base data type
  auto type = std::any_cast<SymbolType>(visit(node->baseDataType()));

  size_t assignExprCounter = 0;
  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    DataTypeNode::TypeModifier typeModifier = tmQueue.front();
    switch (typeModifier.modifierType) {
    case DataTypeNode::TYPE_PTR: {
      type = type.toPointer(node);
      break;
    }
    case DataTypeNode::TYPE_ARRAY: {
      if (typeModifier.hasSize) {
        if (typeModifier.isSizeHardcoded) {
          if (typeModifier.hardcodedSize <= 1)
            throw SemanticError(node, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated");
        } else {
          // Do not allow dynamic sized types in parameter lists
          if (node->isParamType())
            throw SemanticError(node, ARRAY_SIZE_INVALID, "Types in parameter lists may not be dynamically sized");
          // Visit size
          SymbolType sizeType = std::any_cast<ExprResult>(visit(node->arraySizeExpr()[assignExprCounter++])).type;
          if (!sizeType.isOneOf({TY_INT, TY_LONG, TY_SHORT}))
            throw SemanticError(node, ARRAY_SIZE_INVALID, "The array size must be of type int, long or short");
        }
      }
      type = type.toArray(node, typeModifier.hardcodedSize);
      break;
    }
    default:
      throw std::runtime_error("Modifier type fall-through");
    }
    tmQueue.pop();
  }

  return node->setEvaluatedSymbolType(type);
}

std::any TypeChecker::visitBaseDataType(BaseDataTypeNode *node) {
  switch (node->type) {
  case BaseDataTypeNode::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(SymbolType(TY_DOUBLE));
  case BaseDataTypeNode::TYPE_INT:
    return node->setEvaluatedSymbolType(SymbolType(TY_INT));
  case BaseDataTypeNode::TYPE_SHORT:
    return node->setEvaluatedSymbolType(SymbolType(TY_SHORT));
  case BaseDataTypeNode::TYPE_LONG:
    return node->setEvaluatedSymbolType(SymbolType(TY_LONG));
  case BaseDataTypeNode::TYPE_BYTE:
    return node->setEvaluatedSymbolType(SymbolType(TY_BYTE));
  case BaseDataTypeNode::TYPE_CHAR:
    return node->setEvaluatedSymbolType(SymbolType(TY_CHAR));
  case BaseDataTypeNode::TYPE_STRING:
    return node->setEvaluatedSymbolType(SymbolType(TY_STRING));
  case BaseDataTypeNode::TYPE_BOOL:
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
  case BaseDataTypeNode::TY_CUSTOM:
    return node->setEvaluatedSymbolType(std::any_cast<SymbolType>(visit(node->customDataType())));
  default:
    return node->setEvaluatedSymbolType(SymbolType(TY_DYN));
  }
}

std::any TypeChecker::visitCustomDataType(CustomDataTypeNode *node) {
  // It is a struct type -> get the access scope
  if (!accessScope)
    accessScope = currentScope;

  const bool isImported = node->typeNameFragments.size() > 1;
  const std::string firstFragment = node->typeNameFragments.front();

  // Check if it is a String type
  const bool isStringRT = rootScope->lookupStrict(STROBJ_NAME) != nullptr;
  if (!isImported && !isStringRT && firstFragment == STROBJ_NAME)
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check if it is a generic type
  if (!isImported && rootScope->lookupGenericType(firstFragment)) {
    const SymbolType *genericType = rootScope->lookupGenericType(firstFragment);
    assert(genericType != nullptr);
    return node->setEvaluatedSymbolType(*genericType);
  }

  // Check if the type exists in the exported names registry
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqTypeName);
  if (!registryEntry)
    throw SemanticError(node, UNKNOWN_DATATYPE, "Unknown datatype '" + node->fqTypeName + "'");
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *entry = registryEntry->targetEntry;
  accessScope = registryEntry->targetScope->parent;

  // Get struct type and change it to the fully qualified name for identifying without ambiguities
  SymbolType entryType = entry->getType().replaceBaseSubType(registryEntry->name);

  // Enums can early-return
  if (entryType.is(TY_ENUM))
    return SymbolType(TY_INT);

  if (entryType.is(TY_STRUCT)) {
    // Collect the concrete template types
    std::vector<SymbolType> concreteTemplateTypes;
    if (node->templateTypeLst())
      for (DataTypeNode *dataType : node->templateTypeLst()->dataTypes())
        concreteTemplateTypes.push_back(std::any_cast<SymbolType>(visit(dataType)));

    // Set the struct instance to used, if found
    // Here, it is allowed to accept, that the struct cannot be found, because there are self-referencing structs
    Struct *spiceStruct = accessScope->matchStruct(nullptr, node->typeNameFragments.back(), concreteTemplateTypes, node);
    if (spiceStruct)
      spiceStruct->used = true;

    return node->setEvaluatedSymbolType(entryType);
  }

  if (entryType.is(TY_INTERFACE)) {
    // Check if template types are given
    if (node->templateTypeLst())
      throw SemanticError(node->templateTypeLst(), INTERFACE_WITH_TEMPLATE_LIST,
                          "Referencing interfaces with template lists is not allowed");

    // Set the interface instance to used
    Interface *spiceInterface = accessScope->lookupInterface(node->typeNameFragments.back());
    assert(spiceInterface != nullptr);
    spiceInterface->used = true;

    return node->setEvaluatedSymbolType(entryType);
  }

  const std::string errorMessage =
      entryType.is(TY_INVALID) ? "Used type before declared" : "Expected type, but got " + entryType.getName();
  throw SemanticError(node, EXPECTED_TYPE, errorMessage);
}