// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"
#include "util/CommonUtil.h"

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;
  reVisitRequested = false;

  // Visit children
  ASTVisitor::visitChildren(node);

  // Reset the AST
  node->reset();

  // Check if the main function exists
  if (sourceFile->mainFile && !rootScope->lookup(std::string(MAIN_FUNCTION_NAME) + "()"))
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found");

  return reVisitRequested;
}

std::any TypeChecker::visitMainFctDef(MainFctDefNode *node) {
  if (runNumber == 1) // First run
    return visitMainFctDefLookup(node);
  else // Other runs
    return visitMainFctDefAnalyze(node);
  assert(false && "MainFctDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitFctDef(FctDefNode *node) {
  if (runNumber == 1) // First run
    return visitFctDefLookup(node);
  else // Other runs
    return visitFctDefAnalyze(node);
  assert(false && "FctDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitProcDef(ProcDefNode *node) {
  if (runNumber == 1) // First run
    return visitProcDefLookup(node);
  else // Other runs
    return visitProcDefAnalyze(node);
  assert(false && "ProcDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitStructDef(StructDefNode *node) {
  if (runNumber == 1) // First run
    return visitStructDefLookup(node);
  else if (runNumber == 2) // Second run
    return visitStructDefAnalyze(node);
  return nullptr;
}

std::any TypeChecker::visitInterfaceDef(InterfaceDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if interface already exists in this scope
  if (currentScope->lookup(node->interfaceName))
    throw SemanticError(node, INTERFACE_DECLARED_TWICE, "Duplicate interface '" + node->interfaceName + "'");

  // Build interface specifiers
  SymbolType symbolType = SymbolType(TY_INTERFACE, node->interfaceName, {}, {});
  auto interfaceSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        interfaceSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an interface definition");
    }
  }

  // Add the interface to the symbol table
  currentScope->insert(node->interfaceName, symbolType, interfaceSymbolSpecifiers, DECLARED, node);

  // Create scope for interface
  Scope *interfaceScope = currentScope =
      currentScope->createChildScope(INTERFACE_SCOPE_PREFIX + node->interfaceName, SCOPE_INTERFACE);

  // Warning when no signatures are defined
  if (node->signatures().empty())
    sourceFile->compilerOutput.warnings.emplace_back(INTERFACE_WITHOUT_SIGNATURE, "The interface '" + node->interfaceName +
                                                                                      "' does not contain method signatures");

  // Visit contained signatures
  std::vector<Function *> methods;
  methods.reserve(node->signatures().size());
  for (SignatureNode *signature : node->signatures()) {
    auto method = any_cast<Function *>(visit(signature));
    assert(method != nullptr);
    methods.push_back(method);
  }

  // Return to the old scope
  currentScope = currentScope->parent;

  // Add interface
  Interface i(node->interfaceName, interfaceSymbolSpecifiers, methods, node);
  currentScope->insertInterface(i);
  i.interfaceScope = interfaceScope;

  return nullptr;
}

std::any TypeChecker::visitEnumDef(EnumDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if enum already exists in this scope
  if (currentScope->lookup(node->enumName))
    throw SemanticError(node, ENUM_DECLARED_TWICE, "Duplicate symbol name '" + node->enumName + "'");

  // Build symbol specifiers
  auto enumSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_ENUM, node->enumName));
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        enumSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an enum definition");
    }
  }

  // Add symbol table entry and child scope
  currentScope->insert(node->enumName, SymbolType(TY_ENUM, node->enumName), enumSymbolSpecifiers, node);
  node->enumScope = currentScope->createChildScope(ENUM_SCOPE_PREFIX + node->enumName, SCOPE_ENUM);

  // Loop through all items with values
  std::vector<std::string> names;
  std::vector<uint32_t> values;
  for (const EnumItemNode *enumItem : node->itemLst()->items()) {
    // Check if the name does exist already
    if (std::find(names.begin(), names.end(), enumItem->name) != names.end())
      throw SemanticError(enumItem, DUPLICATE_ENUM_ITEM_NAME, "Duplicate enum item name, please use another");
    names.push_back(enumItem->name);

    if (enumItem->hasValue) {
      if (std::find(values.begin(), values.end(), enumItem->itemValue) != values.end())
        throw SemanticError(enumItem, DUPLICATE_ENUM_ITEM_VALUE, "Duplicate enum item value, please use another");
      values.push_back(enumItem->itemValue);
    }
  }

  // Loop through all items without values
  uint32_t nextValue = 0;
  SymbolType intSymbolType = SymbolType(TY_INT);
  for (EnumItemNode *enumItem : node->itemLst()->items()) {
    if (!enumItem->hasValue) {
      while (std::find(values.begin(), values.end(), nextValue) != values.end())
        nextValue++;
      enumItem->itemValue = nextValue;
      values.push_back(nextValue);
    }

    node->enumScope->insert(enumItem->name, intSymbolType, SymbolSpecifiers(intSymbolType), enumItem);
  }

  return nullptr;
}

std::any TypeChecker::visitGenericTypeDef(GenericTypeDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if type already exists in this scope
  if (currentScope->lookup(node->typeName))
    throw SemanticError(node, GENERIC_TYPE_DECLARED_TWICE, "Duplicate symbol name '" + node->typeName + "'");

  // Get type conditions
  std::vector<SymbolType> typeConditions;
  for (const auto &typeAlt : node->typeAltsLst()->dataTypes()) {
    auto typeCondition = any_cast<SymbolType>(visit(typeAlt));
    typeConditions.push_back(typeCondition);
  }

  // Check if only one type condition is set
  if (typeConditions.size() == 1 && !typeConditions.at(0).is(TY_DYN))
    sourceFile->compilerOutput.warnings.emplace_back(node->typeAltsLst()->codeLoc, SINGLE_GENERIC_TYPE_CONDITION,
                                                     "Generic type is locked to one type");

  // Build symbol specifiers
  GenericType genericType = GenericType(node->typeName, typeConditions);
  auto structSymbolSpecifiers = SymbolSpecifiers(genericType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        structSymbolSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a struct definition");
    }
  }

  // Add it to the symbol table
  currentScope->symbolTable.insertGenericType(node->typeName, genericType);

  return nullptr;
}

std::any TypeChecker::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if symbol already exists in the symbol table
  if (currentScope->lookup(node->varName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The global variable '" + node->varName + "' was declared more than once");

  // Check if symbol already exists in any imported module scope
  if (currentScope->symbolTable.lookupGlobal(node->varName, true))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE,
                        "A global variable named '" + node->varName +
                            "' is already declared in another module. Please use a different name.");

  // Insert variable name to symbol table
  auto symbolType = any_cast<SymbolType>(visit(node->dataType()));

  if (node->value()) { // Variable is initialized here
    auto valueType = any_cast<SymbolType>(visit(node->value()));
    // Infer type
    if (symbolType.is(TY_DYN)) {
      symbolType = valueType;
    } else if (symbolType != valueType) {
      throw SemanticError(node->value(), OPERATOR_WRONG_DATA_TYPE,
                          "Cannot apply the assign operator on different data types. You provided " + symbolType.getName(false) +
                              " and " + valueType.getName(false));
    }
  }

  // Check if the type is missing
  if (symbolType.is(TY_DYN))
    throw SemanticError(node->dataType(), GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope
  if (!symbolType.isPrimitive())
    throw SemanticError(node->dataType(), GLOBAL_OF_INVALID_TYPE, "Spice does not allow global variables of this type");

  // Create symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST) {
        // Check if a value is attached
        if (!node->value())
          throw SemanticError(node, GLOBAL_CONST_WITHOUT_VALUE, "You must specify a value for constant global variables");
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->type == SpecifierNode::TY_SIGNED) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->type == SpecifierNode::TY_UNSIGNED) {
        symbolTypeSpecifiers.setSigned(false);
      } else if (specifier->type == SpecifierNode::TY_PUBLIC) {
        symbolTypeSpecifiers.setPublic(true);
      } else {
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a global variable definition");
      }
    }
  }

  // Insert into symbol table
  currentScope->insert(node->varName, symbolType, symbolTypeSpecifiers, node);

  return nullptr;
}

std::any TypeChecker::visitExtDecl(ExtDeclNode *node) {
  if (runNumber > 1)
    return nullptr;

  ParamList argTypes;
  if (node->hasArgs) {
    // Check if an argument is dyn
    for (const auto &arg : node->argTypeLst()->dataTypes()) {
      auto argType = any_cast<SymbolType>(visit(arg));
      if (argType.is(TY_DYN))
        throw SemanticError(arg, UNEXPECTED_DYN_TYPE_SA, "Dyn data type is not allowed as arg type for external functions");
      argTypes.push_back({argType, false});
    }
  }

  if (node->returnType()) { // Function
    // Check if return type is dyn
    auto returnType = any_cast<SymbolType>(visit(node->returnType()));
    if (returnType.is(TY_DYN))
      throw SemanticError(node->returnType(), UNEXPECTED_DYN_TYPE_SA,
                          "Dyn data type is not allowed as return type for external functions");

    // Insert function into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    Function spiceFunc(node->extFunctionName, symbolSpecifiers, SymbolType(TY_DYN), returnType, argTypes, {}, node);
    currentScope->insertFunction(spiceFunc);

    // Add return symbol for function
    node->fctScope = currentScope->createChildScope(spiceFunc.getSignature(), SCOPE_FUNC_PROC_BODY);
    node->fctScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, node);
    node->fctScope->lookup(RETURN_VARIABLE_NAME)->isUsed = true;
  } else { // Procedure
    // Insert procedure into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
    Function spiceProc(node->extFunctionName, symbolSpecifiers, SymbolType(TY_DYN), SymbolType(TY_DYN), argTypes, {}, node);
    currentScope->insertFunction(spiceProc);

    // Add empty scope for function body
    node->fctScope = currentScope->createChildScope(spiceProc.getSignature(), SCOPE_FUNC_PROC_BODY);
  }

  return nullptr;
}

std::any TypeChecker::visitThreadDef(ThreadDefNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_THREAD_BODY);
  currentScope->capturingRequired = true; // Requires capturing because the LLVM IR will end up in a separate function

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  return node->setEvaluatedSymbolType(SymbolType(TY_BYTE).toPointer(node));
}

std::any TypeChecker::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_UNSAFE_BODY);

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  return nullptr;
}

std::any TypeChecker::visitForLoop(ForLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOR_BODY);

  // Visit loop variable declaration in new scope
  visit(node->initDecl());

  // Visit condition in new scope
  auto conditionType = any_cast<SymbolType>(visit(node->condAssign()));
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condAssign(), CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");

  // Visit incrementer in new scope
  visit(node->incAssign());

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;
  return nullptr;
}

std::any TypeChecker::visitForeachLoop(ForeachLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOREACH_BODY);

  // Check type of the array
  auto arrayType = any_cast<SymbolType>(visit(node->arrayAssign()));
  if (!arrayType.isArray())
    throw SemanticError(node->arrayAssign(), OPERATOR_WRONG_DATA_TYPE,
                        "Can only apply foreach loop on an array type. You provided " + arrayType.getName(false));

  if (arrayType.getArraySize() == 0)
    throw SemanticError(node->arrayAssign(), OPERATOR_WRONG_DATA_TYPE,
                        "Can only apply foreach loop on an array type of which the size is known at compile time");

  // Check index assignment or declaration
  SymbolType indexType;
  if (node->idxVarDecl()) {
    indexType = any_cast<SymbolType>(visit(node->idxVarDecl()));

    // Set declared variable to initialized, because we increment it internally in the loop
    if (!node->idxVarDecl()->assignExpr()) {
      std::string varName = node->idxVarDecl()->varName;
      SymbolTableEntry *entry = currentScope->lookup(varName);
      assert(entry != nullptr);
      entry->updateState(INITIALIZED, node->idxVarDecl());
    }

    // Check if index type is int
    if (!indexType.is(TY_INT))
      throw SemanticError(node->idxVarDecl(), ARRAY_INDEX_NOT_INT_OR_LONG,
                          "Index in foreach loop must be of type int. You provided " + indexType.getName(false));
  } else {
    // Declare the variable with the default index variable name
    SymbolType symbolType = SymbolType(TY_INT);
    auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    symbolTypeSpecifiers.setConst(true);
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, symbolType, symbolTypeSpecifiers, node);
  }

  // Check type of the item
  auto itemType = any_cast<SymbolType>(visit(node->itemDecl()));
  SymbolTableEntry *itemVarSymbol = currentScope->lookup(node->itemDecl()->varName);
  assert(itemVarSymbol != nullptr);
  if (itemType.is(TY_DYN)) {
    itemType = arrayType.getContainedTy();
    itemVarSymbol->updateType(itemType, false);

    // Update symbolType of the declaration data type
    node->itemDecl()->dataType()->setEvaluatedSymbolType(itemType);
  } else {
    if (itemType != arrayType.getContainedTy())
      throw SemanticError(node->itemDecl(), OPERATOR_WRONG_DATA_TYPE,
                          "Foreach loop item type does not match array type. Expected " + arrayType.getName(false) +
                              ", provided " + itemType.getName(false));
  }
  itemVarSymbol->updateState(INITIALIZED, node->itemDecl());

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  return nullptr;
}

std::any TypeChecker::visitWhileLoop(WhileLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_WHILE_BODY);

  // Visit condition
  auto conditionType = any_cast<SymbolType>(visit(node->condition()));
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  return nullptr;
}

std::any TypeChecker::visitIfStmt(IfStmtNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY);

  // Visit condition
  AssignExprNode *condition = node->condition();
  auto conditionType = any_cast<SymbolType>(visit(condition));
  if (!conditionType.is(TY_BOOL))
    throw SemanticError(node->condition(), CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

  // Warning for bool assignment
  if (condition->hasOperator && condition->op == AssignExprNode::OP_ASSIGN)
    sourceFile->compilerOutput.warnings.emplace_back(condition->codeLoc, BOOL_ASSIGN_AS_CONDITION,
                                                     "If you want to compare the values, use '=='");

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  // Visit else statement if it exists
  if (node->elseStmt())
    visit(node->elseStmt());

  return nullptr;
}

std::any TypeChecker::visitElseStmt(ElseStmtNode *node) {
  if (node->isElseIf) { // Visit if statement in the case of an else if branch
    visit(node->ifStmt());
  } else { // Make a new scope in case of an else branch
    // Create a new scope
    currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY);

    // Visit statement list in new scope
    visit(node->stmtLst());

    // Return to old scope
    currentScope = currentScope->parent;
  }
  return nullptr;
}

std::any TypeChecker::visitAssertStmt(AssertStmtNode *node) {
  auto assertConditionType = any_cast<SymbolType>(visit(node->assignExpr()));

  // Check if assertStmt evaluates to bool
  if (!assertConditionType.is(TY_BOOL))
    throw SemanticError(node->assignExpr(), ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool");

  return nullptr;
}

std::any TypeChecker::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Create child scope
  currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_ANONYMOUS);

  // Visit statement list
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->parent;

  return nullptr;
}

std::any TypeChecker::visitParamLst(ParamLstNode *node) {
  NamedParamList namedParamList;
  bool metOptional = false;
  for (const auto &param : node->params()) {
    visit(param);
    SymbolType paramType = param->dataType()->getEvaluatedSymbolType();

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (paramType.is(TY_DYN))
      throw SemanticError(node, FCT_PARAM_IS_TYPE_DYN, "Type of parameter '" + param->varName + "' is invalid");

    // Ensure that no optional param comes after a mandatory param
    if (param->hasAssignment) {
      metOptional = true;
    } else if (metOptional) {
      throw SemanticError(param, INVALID_PARAM_ORDER, "Mandatory parameters must go before any optional parameters");
    }

    namedParamList.push_back({param->varName, paramType, metOptional});
  }
  return namedParamList;
}

std::any TypeChecker::visitSignature(SignatureNode *node) {
  // Build method specifiers
  auto methodSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        methodSpecifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a signature definition");
    }
  }

  // Visit return type
  node->returnType = any_cast<SymbolType>(visit(node->dataType()));

  // Visit param types
  if (node->hasParams) {
    for (DataTypeNode *param : node->paramTypeLst()->dataTypes()) {
      auto paramType = any_cast<SymbolType>(visit(param));
      node->paramTypes.push_back({paramType, false});
    }
  }

  Function f(node->methodName, methodSpecifiers, SymbolType(TY_DYN), node->returnType, node->paramTypes, node->templateTypes,
             node);
  return currentScope->insertFunction(f);
}

std::any TypeChecker::visitDeclStmt(DeclStmtNode *node) {
  // Check if symbol already exists in the symbol table
  if (currentScope->symbolTable.lookupStrict(node->varName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The variable '" + node->varName + "' was declared more than once");

  // Get the type of the symbol
  SymbolType symbolType = expectedType = any_cast<SymbolType>(visit(node->dataType()));

  // Visit the right side
  SymbolState initialState = DECLARED;
  if (node->hasAssignment) {
    auto rhsTy = any_cast<SymbolType>(visit(node->assignExpr()));
    // Check if type has to be inferred or both types are fixed
    symbolType = OpRuleManager::getAssignResultType(node, symbolType, rhsTy);
    initialState = INITIALIZED;

    // Push symbolType to the declaration data type
    node->dataType()->setEvaluatedSymbolType(symbolType);

    // If the rhs is of type array and was the array initialization, there must be a size attached
    if (symbolType.isArray() && symbolType.getArraySize() == 0 && currentVarName.empty())
      throw SemanticError(node->dataType(), ARRAY_SIZE_INVALID, "The declaration of an array type must have a size attached");
  }

  // Build symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->type == SpecifierNode::TY_SIGNED) {
        symbolTypeSpecifiers.setSigned(true);
        symbolType.isBaseTypeSigned = true;
      } else if (specifier->type == SpecifierNode::TY_UNSIGNED) {
        symbolTypeSpecifiers.setSigned(false);
        symbolType.isBaseTypeSigned = false;
      } else {
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a local variable declaration");
      }
    }
  }

  // Insert variable into symbol table
  currentScope->insert(node->varName, symbolType, symbolTypeSpecifiers, initialState, node);

  // Insert call to empty constructor, if no value was assigned to a struct
  if (!node->isParam() && !node->hasAssignment && symbolType.isOneOf({TY_STRUCT, TY_STROBJ})) {
    SymbolTableEntry *entry = currentScope->symbolTable.lookupStrict(node->varName);
    assert(entry != nullptr);
    insertEmptyConstructorCall(node, entry);
  }

  return symbolType;
}

std::any TypeChecker::visitReturnStmt(ReturnStmtNode *node) {
  SymbolType returnType = SymbolType(TY_DYN);
  SymbolTableEntry *returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
  if (returnVariable) { // Return variable => function
    expectedType = returnVariable->type;

    // Check if there is a value attached to the return statement
    if (node->hasReturnValue) {
      // Visit the value
      returnType = any_cast<SymbolType>(visit(node->assignExpr()));

      // Check data type of return statement
      if (returnVariable->type.is(TY_DYN)) {
        // Set explicit return type to the return variable
        returnVariable->updateType(returnType, false);
      } else {
        // Check if return type matches with function definition
        if (returnType != returnVariable->type)
          throw SemanticError(node->assignExpr(), OPERATOR_WRONG_DATA_TYPE,
                              "Passed wrong data type to return statement. Expected " + returnVariable->type.getName(false) +
                                  " but got " + returnType.getName(false));
      }

      // Set the return variable to initialized
      returnVariable->updateState(INITIALIZED, node);
    } else {
      returnType = returnVariable->type;
    }

    // Check if result variable is initialized
    if (returnVariable->state != INITIALIZED)
      throw SemanticError(node, RETURN_WITHOUT_VALUE_RESULT, "Return without value, but result variable is not initialized yet");
    returnVariable->isUsed = true;
  } else {
    // No return variable => procedure
    if (node->assignExpr())
      throw SemanticError(node->assignExpr(), RETURN_WITH_VALUE_IN_PROCEDURE,
                          "Return statements in procedures may not have a value attached");
  }

  // Call destructors for variables, that are going out of scope
  std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
  for (SymbolTableEntry *varEntry : varsToDestruct)
    insertDestructorCall(varEntry->declNode, varEntry);

  return nullptr;
}

std::any TypeChecker::visitBreakStmt(BreakStmtNode *node) {
  if (node->breakTimes != 1) {
    // Check if the stated number is valid
    if (node->breakTimes < 1)
      throw SemanticError(node, INVALID_BREAK_NUMBER,
                          "Break count must be >= 1, you provided " + std::to_string(node->breakTimes));
  }
  // Check if we can break this often
  size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->breakTimes > maxBreaks)
    throw SemanticError(node, INVALID_BREAK_NUMBER, "We can only break " + std::to_string(maxBreaks) + " time(s) here");
  return nullptr;
}

std::any TypeChecker::visitContinueStmt(ContinueStmtNode *node) {
  if (node->continueTimes != 1) {
    // Check if the stated number is valid
    if (node->continueTimes < 1)
      throw SemanticError(node, INVALID_CONTINUE_NUMBER,
                          "Continue count must be >= 1, you provided " + std::to_string(node->continueTimes));
  }
  // Check if we can continue this often
  size_t maxBreaks = currentScope->getLoopNestingDepth();
  if (node->continueTimes > maxBreaks)
    throw SemanticError(node, INVALID_CONTINUE_NUMBER, "We can only continue " + std::to_string(maxBreaks) + " time(s) here");
  return nullptr;
}

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  std::size_t index = node->templatedString.find_first_of('%');
  int placeholderCount = 0;
  while (index != std::string::npos) {
    // Check if there is another assignExpr
    if (node->assignExpr().size() <= placeholderCount)
      throw SemanticError(node, PRINTF_ARG_COUNT_ERROR,
                          "The placeholder string contains more placeholders that arguments were passed");

    auto assignment = node->assignExpr()[placeholderCount];
    auto assignmentType = any_cast<SymbolType>(visit(assignment));
    switch (node->templatedString[index + 1]) {
    case 'c': {
      if (!assignmentType.is(TY_CHAR))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "Template string expects char, but got " + assignmentType.getName(false));
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
      if (!assignmentType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_BOOL}))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "Template string expects int, short, long, byte or bool, but got " + assignmentType.getName(false));
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
      if (!assignmentType.is(TY_DOUBLE))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "Template string expects double, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 's': {
      if (!assignmentType.isOneOf({TY_STRING, TY_STROBJ}) && !assignmentType.isPointerOf(TY_CHAR) &&
          !assignmentType.isArrayOf(TY_CHAR))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "Template string expects string, char* or char[], but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!assignmentType.isPointer() && !assignmentType.isArray() && !assignmentType.is(TY_STRING))
        throw SemanticError(assignment, PRINTF_TYPE_ERROR,
                            "Template string expects pointer, array or string, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    }
    index = node->templatedString.find_first_of('%', index + 1);
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->assignExpr().size())
    throw SemanticError(node, PRINTF_ARG_COUNT_ERROR,
                        "The placeholder string contains less placeholders that arguments were passed");

  return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  SymbolType symbolType;
  if (node->isType) { // Size of type
    symbolType = any_cast<SymbolType>(visit(node->dataType()));
  } else { // Size of value
    symbolType = any_cast<SymbolType>(visit(node->assignExpr()));
  }

  // Check if symbol type is dynamically sized array
  if (symbolType.is(TY_ARRAY) && symbolType.getArraySize() == -1)
    throw SemanticError(node, SIZEOF_DYNAMIC_SIZED_ARRAY, "Cannot get sizeof dynamically sized array at compile time");

  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  auto argType = any_cast<SymbolType>(visit(node->assignExpr()));

  // Check if arg is of type array
  if (!argType.isArray())
    throw SemanticError(node->assignExpr(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays");

  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any TypeChecker::visitTidCall(TidCallNode *node) {
  // Nothing to check here. Tid builtin has no arguments
  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any TypeChecker::visitJoinCall(JoinCallNode *node) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(node);
  for (const auto &assignExpr : node->assignExpressions()) {
    auto argSymbolType = any_cast<SymbolType>(visit(assignExpr));
    if (argSymbolType == bytePtr && argSymbolType.isArrayOf(bytePtr))
      throw SemanticError(assignExpr, JOIN_ARG_MUST_BE_TID,
                          "You have to pass a thread id (byte*) or a array of thread ids (byte*[]) to to join builtin");
  }

  // Return the number of threads that were joined
  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if there is an assign operator applied
  if (node->hasOperator) { // This is an assignment
    // Get symbol type of right side
    auto rhsTy = any_cast<SymbolType>(visit(node->rhs()));

    // Visit the left side
    currentVarName = "";    // Reset the current variable name
    currentEntry = nullptr; // Reset the current entry
    auto lhsTy = any_cast<SymbolType>(visit(node->lhs()));
    std::string variableName = currentVarName;

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) {
      rhsTy = OpRuleManager::getAssignResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_PLUS_EQUAL) {
      rhsTy = opRuleManager.getPlusEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_MINUS_EQUAL) {
      rhsTy = opRuleManager.getMinusEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_MUL_EQUAL) {
      rhsTy = opRuleManager.getMulEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_DIV_EQUAL) {
      rhsTy = opRuleManager.getDivEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_REM_EQUAL) {
      rhsTy = opRuleManager.getRemEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_SHL_EQUAL) {
      rhsTy = opRuleManager.getSHLEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_SHR_EQUAL) {
      rhsTy = opRuleManager.getSHREqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_AND_EQUAL) {
      rhsTy = opRuleManager.getAndEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_OR_EQUAL) {
      rhsTy = opRuleManager.getOrEqualResultType(node, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_XOR_EQUAL) {
      rhsTy = opRuleManager.getXorEqualResultType(node, lhsTy, rhsTy);
    }

    if (!variableName.empty()) { // Variable is involved on the left side
      // Check if the symbol exists
      if (!currentEntry)
        throw SemanticError(node->lhs(), REFERENCED_UNDEFINED_VARIABLE,
                            "The variable '" + variableName + "' was referenced before defined");

      // Perform type inference
      if (lhsTy.is(TY_DYN) || (lhsTy.is(TY_STRING) && rhsTy.is(TY_STRING)))
        currentEntry->updateType(rhsTy, false);

      // Update state in symbol table
      if (!currentEntry->type.isOneOf({TY_FUNCTION, TY_PROCEDURE}))
        currentEntry->updateState(INITIALIZED, node->lhs());

      // In case the lhs variable is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->symbolTable.lookupCapture(variableName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);

      // If we overwrite the value of a variable, destruct the old value
      if (node->op == AssignExprNode::OP_ASSIGN && lhsTy == currentEntry->type && lhsTy.isOneOf({TY_STRUCT, TY_STROBJ}))
        insertDestructorCall(node->rhs(), currentEntry);
    }

    return node->setEvaluatedSymbolType(rhsTy);
  } else if (node->ternaryExpr()) {
    return visit(node->ternaryExpr());
  } else if (node->threadDef()) {
    return visit(node->threadDef());
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitTernaryExpr(TernaryExprNode *node) {
  // Check if there is a ternary operator applied
  if (node->children.size() > 1) {
    LogicalOrExprNode *condition = node->operands()[0];
    auto conditionType = any_cast<SymbolType>(visit(condition));
    SymbolType trueType;
    SymbolType falseType;
    if (node->isShortened) {
      trueType = conditionType;
      falseType = any_cast<SymbolType>(visit(node->operands()[1]));
    } else {
      trueType = any_cast<SymbolType>(visit(node->operands()[1]));
      falseType = any_cast<SymbolType>(visit(node->operands()[2]));
    }
    // Check if the condition evaluates to boolean
    if (!conditionType.is(TY_BOOL))
      throw SemanticError(condition, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");
    // Check if trueType and falseType are matching
    if (trueType != falseType)
      throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "True and false operands in ternary must be of same data type");
    return node->setEvaluatedSymbolType(trueType);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = OpRuleManager::getLogicalOrResultType(node, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = OpRuleManager::getLogicalAndResultType(node, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = OpRuleManager::getBitwiseOrResultType(node, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = OpRuleManager::getBitwiseXorResultType(node, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = OpRuleManager::getBitwiseAndResultType(node, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitEqualityExpr(EqualityExprNode *node) {
  // Check if at least one equality operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (lhsTy.is(TY_STRING) && rhsTy.is(TY_STRING))
      sourceFile.requestRuntimeModule(STRING_RT);

    if (node->op == EqualityExprNode::OP_EQUAL) // Operator was equal
      return node->setEvaluatedSymbolType(OpRuleManager::getEqualResultType(node, lhsTy, rhsTy));
    else if (node->op == EqualityExprNode::OP_NOT_EQUAL) // Operator was not equal
      return node->setEvaluatedSymbolType(OpRuleManager::getNotEqualResultType(node, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (node->op == RelationalExprNode::OP_LESS) // Operator was less
      return node->setEvaluatedSymbolType(OpRuleManager::getLessResultType(node, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_GREATER) // Operator was greater
      return node->setEvaluatedSymbolType(OpRuleManager::getGreaterResultType(node, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_LESS_EQUAL) // Operator was less equal
      return node->setEvaluatedSymbolType(OpRuleManager::getLessEqualResultType(node, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_GREATER_EQUAL) // Operator was greater equal
      return node->setEvaluatedSymbolType(OpRuleManager::getGreaterEqualResultType(node, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitShiftExpr(ShiftExprNode *node) {
  // Check if at least one shift operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (node->op == ShiftExprNode::OP_SHIFT_LEFT) // Operator was shl
      return node->setEvaluatedSymbolType(OpRuleManager::getShiftLeftResultType(node, lhsTy, rhsTy));
    else if (node->op == ShiftExprNode::OP_SHIFT_RIGHT) // Operator was shr
      return node->setEvaluatedSymbolType(OpRuleManager::getShiftRightResultType(node, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->operands().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(node->operands()[0]));

    auto opQueue = node->opQueue;
    std::queue<std::pair<AdditiveExprNode::AdditiveOp, SymbolType>> newOpQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      MultiplicativeExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      auto operandType = any_cast<SymbolType>(visit(operand));

      switch (opQueue.front().first) {
      case AdditiveExprNode::OP_PLUS:
        currentType = opRuleManager.getPlusResultType(operand, currentType, operandType);
        break;
      case AdditiveExprNode::OP_MINUS:
        currentType = opRuleManager.getMinusResultType(operand, currentType, operandType);
        break;
      default:
        throw std::runtime_error("Additive expr fall-through");
      }

      newOpQueue.emplace(opQueue.front().first, currentType);
      opQueue.pop();
    }
    node->opQueue = newOpQueue;

    return node->setEvaluatedSymbolType(currentType);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  // Check if at least one multiplicative operator is applied
  if (node->operands().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(node->operands()[0]));

    auto opQueue = node->opQueue;
    std::queue<std::pair<MultiplicativeExprNode::MultiplicativeOp, SymbolType>> newOpQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      CastExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      auto operandType = any_cast<SymbolType>(visit(operand));

      switch (opQueue.front().first) {
      case MultiplicativeExprNode::OP_MUL:
        currentType = opRuleManager.getMulResultType(operand, currentType, operandType);
        break;
      case MultiplicativeExprNode::OP_DIV:
        currentType = OpRuleManager::getDivResultType(operand, currentType, operandType);
        break;
      case MultiplicativeExprNode::OP_REM:
        currentType = OpRuleManager::getRemResultType(operand, currentType, operandType);
        break;
      default:
        throw std::runtime_error("Multiplicative expr fall-through");
      }

      newOpQueue.emplace(opQueue.front().first, currentType);
      opQueue.pop();
    }
    node->opQueue = newOpQueue;

    return node->setEvaluatedSymbolType(currentType);
  }
  return visit(node->operands().front());
}

std::any TypeChecker::visitCastExpr(CastExprNode *node) {
  std::any rhs = visit(node->prefixUnaryExpr());

  if (node->isCasted) { // Cast is applied
    auto srcType = any_cast<SymbolType>(rhs);
    auto dstType = any_cast<SymbolType>(visit(node->dataType()));
    SymbolType symbolType = opRuleManager.getCastResultType(node, dstType, any_cast<SymbolType>(rhs));
    return node->setEvaluatedSymbolType(symbolType);
  }

  return rhs;
}

std::any TypeChecker::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  currentVarName = "";                  // Reset the current variable name
  scopePath.clear();                    // Clear the scope path
  currentThisType = SymbolType(TY_DYN); // Reset this type

  auto rhs = any_cast<SymbolType>(visit(node->postfixUnaryExpr()));

  auto opStack = node->opStack;
  std::stack<std::pair<PrefixUnaryExprNode::PrefixUnaryOp, SymbolType>> newOpStack;
  while (!opStack.empty()) {
    switch (opStack.top().first) {
    case PrefixUnaryExprNode::OP_MINUS:
      rhs = OpRuleManager::getPrefixMinusResultType(node->postfixUnaryExpr(), rhs);
      break;
    case PrefixUnaryExprNode::OP_PLUS_PLUS:
      rhs = OpRuleManager::getPrefixPlusPlusResultType(node->postfixUnaryExpr(), rhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, node);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_MINUS_MINUS:
      rhs = OpRuleManager::getPrefixMinusMinusResultType(node->postfixUnaryExpr(), rhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, node);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_NOT:
      rhs = OpRuleManager::getPrefixNotResultType(node->postfixUnaryExpr(), rhs);
      break;
    case PrefixUnaryExprNode::OP_BITWISE_NOT:
      rhs = OpRuleManager::getPrefixBitwiseNotResultType(node->postfixUnaryExpr(), rhs);
      break;
    case PrefixUnaryExprNode::OP_INDIRECTION:
      rhs = OpRuleManager::getPrefixMulResultType(node->postfixUnaryExpr(), rhs);
      break;
    case PrefixUnaryExprNode::OP_ADDRESS_OF:
      rhs = OpRuleManager::getPrefixBitwiseAndResultType(node->postfixUnaryExpr(), rhs);
      break;
    default:
      throw std::runtime_error("Prefix unary fall-through");
    }

    newOpStack.emplace(opStack.top().first, rhs);
    opStack.pop();
  }
  node->opStack = newOpStack;

  return node->setEvaluatedSymbolType(rhs);
}

std::any TypeChecker::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  auto lhs = any_cast<SymbolType>(visit(node->atomicExpr()));
  if (lhs.is(TY_INVALID))
    throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE,
                        "Variable '" + node->atomicExpr()->identifier + "' was referenced before declared");

  size_t subscriptCounter = 0;
  size_t memberAccessCounter = 0;

  auto opQueue = node->opQueue;
  std::queue<std::pair<PostfixUnaryExprNode::PostfixUnaryOp, SymbolType>> newOpQueue;
  while (!opQueue.empty()) {
    switch (opQueue.front().first) {
    case PostfixUnaryExprNode::OP_SUBSCRIPT: {
      std::string arrayName = currentVarName;              // Save array name
      SymbolTableEntry *currentEntryBackup = currentEntry; // Save current entry
      ScopePath scopePathBackup = scopePath;               // Save scope path

      AssignExprNode *indexExpr = node->assignExpr()[subscriptCounter++];
      auto indexType = any_cast<SymbolType>(visit(indexExpr));

      if (!indexType.isOneOf({TY_INT, TY_LONG}))
        throw SemanticError(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long");
      if (!lhs.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
        throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE,
                            "Can only apply subscript operator on array type, got " + lhs.getName(true));

      if (lhs.is(TY_PTR) && !allowUnsafeOperations) {
        throw SemanticError(
            node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
            "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.");
      } else if (lhs.is(TY_ARRAY) && lhs.getArraySize() > 0 && indexExpr->hasCompileTimeValue()) {
        std::int32_t constIndex = indexExpr->getCompileTimeValue().intValue;
        size_t constSize = lhs.getArraySize();
        if (constIndex >= constSize)
          throw SemanticError(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                              "You are trying to access element with index " + std::to_string(constIndex) +
                                  " of an array with size " + std::to_string(constSize));
      }

      // Get array item type
      lhs = currentThisType = lhs.getContainedTy();

      currentVarName = arrayName;        // Restore array name
      scopePath = scopePathBackup;       // Restore scope path
      currentEntry = currentEntryBackup; // Restore current entry

      // Retrieve scope for the new scope path fragment
      if (lhs.isBaseType(TY_STRUCT)) {
        Scope *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        assert(accessScope != nullptr);

        std::string structSignature = Struct::getSignature(lhs.getBaseType().getSubType(), lhs.getBaseType().getTemplateTypes());
        Scope *newAccessScope = accessScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
        assert(newAccessScope != nullptr);
        // Push the retrieved scope to the scope path
        scopePath.pushFragment("[idx]", newAccessScope);
      }

      break;
    }
    case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
      // Check if lhs is enum or strobj
      if (!lhs.isBaseType(TY_STRUCT) && !lhs.isOneOf({TY_ENUM, TY_STROBJ}))
        throw SemanticError(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + lhs.getName());

      currentThisType = lhs;

      PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
      lhs = any_cast<SymbolType>(visit(rhs)); // Visit rhs
      break;
    }
    case PostfixUnaryExprNode::OP_SCOPE_ACCESS: {
      // Check if lhs is import
      if (!lhs.is(TY_IMPORT))
        throw SemanticError(node, SCOPE_ACCESS_ONLY_IMPORTS, "Cannot apply scope access operator on " + lhs.getName());

      PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
      lhs = any_cast<SymbolType>(visit(rhs)); // Visit rhs
      break;
    }
    case PostfixUnaryExprNode::OP_PLUS_PLUS: {
      lhs = OpRuleManager::getPostfixPlusPlusResultType(node->atomicExpr(), lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, node);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    case PostfixUnaryExprNode::OP_MINUS_MINUS: {
      lhs = opRuleManager->getPostfixMinusMinusResultType(node->atomicExpr(), lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, node);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    default:
      throw std::runtime_error("PostfixUnary fall-through");
    }

    newOpQueue.emplace(opQueue.front().first, lhs);
    opQueue.pop();
  }
  node->opQueue = newOpQueue;

  if (lhs.is(TY_INVALID))
    throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + currentVarName + "' was referenced before declared");

  return node->setEvaluatedSymbolType(lhs);
}

std::any TypeChecker::visitAtomicExpr(AtomicExprNode *node) {
  if (node->value())
    return visit(node->value());

  if (!node->identifier.empty()) {
    checkForReservedKeyword(node, node->identifier);
    currentVarName = node->identifier;

    // Retrieve access scope
    Scope *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
    assert(accessScope);

    // Load symbol table entry
    SymbolTableEntry *entry = currentEntry = accessScope->lookup(currentVarName);

    // Check if symbol exists. If it does not exist, just return because it could be the function name of a function call
    // The existence of the variable is checked in the visitPostfixUnaryExpr method.
    if (!entry)
      return SymbolType(TY_INVALID);

    if (accessScope->isImported(currentScope)) {
      // Check if the entry is public if it is imported
      if (!entry->specifiers.isPublic())
        throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                            "Cannot access '" + currentVarName + "' due to its private visibility");

      // Check if the entry is an external global variable and needs to be imported
      if (entry->isGlobal && !entry->type.isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
        initExtGlobal(accessScope, scopePath.getScopePrefix(true), entry->name, node);
    }

    // Set symbol to used
    entry->isUsed = true;

    // Retrieve scope for the new scope path fragment
    if (entry->type.is(TY_IMPORT)) { // Import
      accessScope = accessScope->getChildScope(entry->name);
    } else if (entry->type.isBaseType(TY_STRUCT)) { // Struct
      std::string structSignature =
          Struct::getSignature(entry->type.getBaseType().getSubType(), entry->type.getBaseType().getTemplateTypes());

      // Get struct table
      accessScope = accessScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);

      // Retrieve the original type if the struct was imported
      Capture *structCapture = currentScope->symbolTable.lookupCapture(structSignature);
      currentThisType = structCapture ? structCapture->capturedEntry->type : entry->type;
      currentThisType.setTemplateTypes(entry->type.getTemplateTypes());

      // Check if the entry is public if it is imported
      if (structCapture && !structCapture->capturedEntry->specifiers.isPublic() && accessScope->parent->isImported(currentScope))
        throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                            "Cannot access '" + structSignature + "' due to its private visibility");

      // If the return type is an external struct, initialize it
      if (!scopePath.isEmpty() && scopePath.getCurrentScope()->isImported(currentScope)) {
        SymbolTableEntry *parentStruct = currentScope->lookup(scopePath.getLastScopeName());
        assert(parentStruct != nullptr);
        std::string scopePrefix = CommonUtil::getPrefix(parentStruct->type.getSubType(), ".");
        SymbolType symbolType = initExtStruct(accessScope, scopePrefix, entry->type.getBaseType().getSubType(),
                                              currentThisType.getTemplateTypes(), node);
        return node->setEvaluatedSymbolType(symbolType);
      }
    } else if (entry->type.isBaseType(TY_ENUM)) { // Enum
      // Get enum table
      accessScope = accessScope->getChildScope(ENUM_SCOPE_PREFIX + node->identifier);
    } else {
      // Check if we have seen a 'this.' prefix, because the generator needs that
      if (entry->scope->scopeType == SCOPE_STRUCT && currentThisType.is(TY_DYN))
        throw SemanticError(node, REFERENCED_UNDEFINED_VARIABLE,
                            "The symbol '" + currentVarName + "' could not be found. Missing 'this.' prefix?");
    }
    assert(accessScope != nullptr);

    // Otherwise, push the retrieved scope to the scope path
    scopePath.pushFragment(currentVarName, accessScope);

    return node->setEvaluatedSymbolType(entry->type);
  }

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

  return visit(node->assignExpr());
}

std::any TypeChecker::visitValue(ValueNode *node) {
  // Primitive value
  if (node->primitiveValue())
    return visit(node->primitiveValue());

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
    auto nilType = any_cast<SymbolType>(visit(node->nilType()));
    if (nilType.is(TY_DYN))
      throw SemanticError(node->nilType(), UNEXPECTED_DYN_TYPE_SA, "Nil must have an explicit type");
    return node->setEvaluatedSymbolType(nilType);
  }

  throw std::runtime_error("Value fall-through");
}

std::any TypeChecker::visitPrimitiveValue(PrimitiveValueNode *node) {
  switch (node->type) {
  case PrimitiveValueNode::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(SymbolType(TY_DOUBLE));
  case PrimitiveValueNode::TYPE_INT:
    return node->setEvaluatedSymbolType(SymbolType(TY_INT));
  case PrimitiveValueNode::TYPE_SHORT:
    return node->setEvaluatedSymbolType(SymbolType(TY_SHORT));
  case PrimitiveValueNode::TYPE_LONG:
    return node->setEvaluatedSymbolType(SymbolType(TY_LONG));
  case PrimitiveValueNode::TYPE_BYTE:
    return node->setEvaluatedSymbolType(SymbolType(TY_BYTE));
  case PrimitiveValueNode::TYPE_CHAR:
    return node->setEvaluatedSymbolType(SymbolType(TY_CHAR));
  case PrimitiveValueNode::TYPE_STRING:
    return node->setEvaluatedSymbolType(SymbolType(TY_STRING));
  case PrimitiveValueNode::TYPE_BOOL:
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
  }
  throw std::runtime_error("Primitive value fall-through");
}

std::any TypeChecker::visitFunctionCall(FunctionCallNode *node) {
  // Get the access scope
  Scope *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Initialize loop variable
  std::vector<SymbolType> concreteTemplateTypes;
  std::string functionName;
  SymbolType thisType = currentThisType;
  bool constructorCall = false;

  // Check if it is a reference to the String type
  bool isStringRuntime = rootScope->lookupStrict(STROBJ_NAME) != nullptr && !rootScope->lookupCaptureStrict(STROBJ_NAME);
  if (thisType.is(TY_DYN) && node->functionNameFragments.size() == 1 && node->functionNameFragments.front() == STROBJ_NAME &&
      !isStringRuntime) {
    sourceFile->requestRuntimeModule(STRING_RT);
    accessScope = resourceManager.runtimeModuleManager.getModuleScope(STRING_RT);
  }

  for (size_t i = 0; i < node->functionNameFragments.size(); i++) {
    std::string identifier = node->functionNameFragments[i];
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    SymbolType symbolBaseType;
    if (symbolEntry != nullptr) {
      symbolBaseType = symbolEntry->type.getBaseType();
      symbolEntry->isUsed = true;
    }

    if (i < node->functionNameFragments.size() - 1) { // not last fragment
      if (!symbolEntry)
        throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION,
                            "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisType = symbolBaseType;
    } else { // last fragment
      // Get the concrete template types
      if (node->hasTemplateTypes) {
        for (const auto &dataType : node->templateTypeLst()->dataTypes())
          concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
      }

      if (symbolEntry != nullptr && symbolBaseType.is(TY_STRUCT)) {
        std::string structSignature = Struct::getSignature(identifier, concreteTemplateTypes);

        // Get the struct instance
        Struct *spiceStruct = accessScope->matchStruct(currentScope, identifier, concreteTemplateTypes, node);
        if (!spiceStruct)
          throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
        spiceStruct->isUsed = true;

        symbolEntry = accessScope->lookup(structSignature);
        assert(symbolEntry != nullptr);
        symbolEntry->isUsed = true;

        // Import struct if necessary
        if (accessScope->isImported(currentScope))
          thisType = initExtStruct(accessScope, scopePath.getScopePrefix(true), identifier, concreteTemplateTypes, node);
        else
          thisType = symbolBaseType;

        functionName = CTOR_FUNCTION_NAME;
        constructorCall = true;
      } else { // last fragment is no struct
        functionName = identifier;
        continue;
      }
    }

    std::string tableName = symbolEntry->type.is(TY_IMPORT) ? identifier : STRUCT_SCOPE_PREFIX + thisType.getName();
    accessScope = accessScope->lookupTable(tableName);

    if (!accessScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "Cannot call a function on '" + identifier + "'");

    scopePath.pushFragment(identifier, accessScope);
  }
  assert(accessScope != nullptr);

  ScopePath scopePathBackup = scopePath;

  // Visit args
  std::vector<SymbolType> argTypes;
  if (node->hasArgs) {
    for (const auto &arg : node->argLst()->args())
      argTypes.push_back(any_cast<SymbolType>(visit(arg)));
  }

  scopePath = scopePathBackup;

  // Set to root scope if it did not change
  if (accessScope == currentScope)
    accessScope = rootScope;

  // Map this type
  if (thisType.is(TY_IMPORT))
    thisType = SymbolType(TY_DYN); // Avoid this type import
  else if (thisType.is(TY_STROBJ))
    thisType = SymbolType(TY_STRUCT, STROBJ_NAME); // Change this type from strobj to struct(String)

  // Get the function/procedure instance
  SymbolType origThisType = thisType.replaceBaseSubType(CommonUtil::getLastFragment(thisType.getBaseType().getSubType(), "."));
  Function *spiceFunc =
      accessScope->matchFunction(currentScope, functionName, origThisType, concreteTemplateTypes, argTypes, node);
  if (!spiceFunc) {
    // Build dummy function to get a better error message
    SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));

    ParamList errArgTypes;
    for (const auto &argType : argTypes)
      errArgTypes.push_back({argType, false});

    Function f(functionName, specifiers, thisType, SymbolType(TY_DYN), errArgTypes, {}, node);
    throw SemanticError(node, REFERENCED_UNDEFINED_FUNCTION, "Function/Procedure '" + f.getSignature() + "' could not be found");
  }
  spiceFunc->isUsed = true;

  // Get function entry
  SymbolTableEntry *functionEntry = accessScope->lookup(spiceFunc->getSignature());
  assert(functionEntry != nullptr);
  functionEntry->isUsed = true; // Set the function to used

  // Check if the function entry has sufficient visibility
  if (accessScope->isImported(currentScope) && !functionEntry->specifiers.isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Cannot access function/procedure '" + spiceFunc->getSignature() + "' due to its private visibility");

  // Analyze the function if not done yet. This is only necessary if we call a function in the same source file, which was
  // declared above.
  if (!accessScope->isImported(currentScope) && spiceFunc->getDeclCodeLoc().line < node->codeLoc.line)
    reAnalyzeRequired = true;

  if (constructorCall) {
    // Add anonymous symbol to keep track of de-allocation
    currentScope->insertAnonymous(thisType, node, INITIALIZED);

    // Map this type back
    if (thisType.is(TY_STRUCT, STROBJ_NAME) && !isStringRuntime)
      thisType = SymbolType(TY_STROBJ); // Change this type from struct(String) to strobj

    // Return struct type on constructor call
    return node->setEvaluatedSymbolType(thisType);
  }

  // If the callee is a procedure, return type bool
  if (spiceFunc->isProcedure() || spiceFunc->returnType.is(TY_DYN))
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));

  // Retrieve the return type of the function
  SymbolType returnType = spiceFunc->returnType;

  if (returnType.is(TY_STRUCT)) {
    // Add struct scope to scope path
    std::string structSignature = Struct::getSignature(returnType.getSubType(), returnType.getTemplateTypes());
    SymbolTable *newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
    scopePath.pushFragment(returnType.getSubType(), newAccessScope);

    // If the return type is an external struct, initialize it
    if (!scopePathBackup.isEmpty() && scopePathBackup.getCurrentScope()->isImported(currentScope)) {
      std::string scopePrefix = scopePathBackup.getScopePrefix(!spiceFunc->isGenericSubstantiation);
      returnType = initExtStruct(currentScope, scopePrefix, returnType.getSubType(), returnType.getTemplateTypes(), node);
    }
  }

  // Map return type
  if (returnType.is(TY_STRUCT, STROBJ_NAME) && !isStringRuntime)
    returnType = SymbolType(TY_STROBJ);

  return node->setEvaluatedSymbolType(returnType);
}

std::any TypeChecker::visitArrayInitialization(ArrayInitializationNode *node) {
  // Check if all values have the same type
  int actualSize = 0;
  SymbolType actualItemType = SymbolType(TY_DYN);
  if (node->itemLst()) {
    // Set the expected array type to the contained type
    SymbolType expectedTypeBackup = expectedType;
    expectedType = expectedType.isArray() ? expectedType.getContainedTy() : expectedType;

    for (const auto &arg : node->itemLst()->args()) {
      auto itemType = any_cast<SymbolType>(visit(arg));
      if (actualItemType.is(TY_DYN)) {
        actualItemType = itemType;
      } else if (itemType != actualItemType) {
        throw SemanticError(arg, ARRAY_ITEM_TYPE_NOT_MATCHING,
                            "All provided values have to be of the same data type. You provided " +
                                actualItemType.getName(false) + " and " + itemType.getName(false));
      }
      actualSize++;
    }

    // Restore the expected array type
    expectedType = expectedTypeBackup;
  }

  // Override actual array size if the expected type has a fixed size
  actualSize = expectedType.isArray() ? expectedType.getArraySize() : actualSize;

  // Check if actual item type is known now
  if (actualItemType.is(TY_DYN)) { // Not enough info to perform type inference, because of empty array {}
    if (expectedType.is(TY_DYN))
      throw SemanticError(node, UNEXPECTED_DYN_TYPE_SA, "Not enough information to perform type inference");
    if (!expectedType.isArray())
      throw SemanticError(node, ARRAY_ITEM_TYPE_NOT_MATCHING, "Cannot initialize array for type " + expectedType.getName() + "");
    actualItemType = expectedType.getContainedTy();
  }

  return node->setEvaluatedSymbolType(actualItemType.toArray(node, actualSize));
}

std::any TypeChecker::visitStructInstantiation(StructInstantiationNode *node) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Retrieve fully qualified struct name and the scope where to search it
  std::string accessScopePrefix;
  std::string structName;
  bool structIsImported = false;
  for (unsigned int i = 0; i < node->structNameFragments.size(); i++) {
    structName = node->structNameFragments[i];
    SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
    if (symbolEntry)
      symbolEntry->isUsed = true;

    if (i < node->structNameFragments.size() - 1) {
      if (!symbolEntry)
        throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT,
                            "Symbol '" + accessScopePrefix + structName + "' was used before defined");

      accessScopePrefix += structName + ".";
      std::string tableName = symbolEntry->type.is(TY_IMPORT) ? structName : STRUCT_SCOPE_PREFIX + structName;
      accessScope = accessScope->lookupTable(tableName);
      if (accessScope->isImported(currentScope))
        structIsImported = true;
    }
  }
  assert(accessScope != nullptr);

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (node->templateTypeLst()) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes())
      concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
  }

  // Get the struct instance
  Struct *spiceStruct = accessScope->matchStruct(currentScope, structName, concreteTemplateTypes, node);
  if (!spiceStruct) {
    std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
  }
  spiceStruct->isUsed = true;

  SymbolType structType;
  if (structIsImported) { // Imported struct
    structType = initExtStruct(accessScope, accessScopePrefix, node->structNameFragments.back(), concreteTemplateTypes, node);
  } else { // Not imported
    SymbolTableEntry *structSymbol =
        currentScope->lookup(accessScopePrefix + Struct::getSignature(structName, concreteTemplateTypes));
    if (!structSymbol)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Could not find struct '" + accessScopePrefix + structName + "'");
    structType = structSymbol->type;
  }

  // Set template types to the struct
  std::vector<GenericType> genericTemplateTypes = spiceStruct->templateTypes;
  std::vector<SymbolType> templateTypes;
  for (const auto &genericType : genericTemplateTypes)
    templateTypes.emplace_back(genericType.typeChain);
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  SymbolTable *structTable = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + accessScopePrefix + structName);
  std::vector<SymbolType> fieldTypes;
  if (node->fieldLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->fieldTypes.size() != node->fieldLst()->args().size())
      throw SemanticError(node->fieldLst(), NUMBER_OF_FIELDS_NOT_MATCHING,
                          "You've passed too less/many field values. Pass either none or all of them");

    // Check if the field types are matching
    for (int i = 0; i < node->fieldLst()->args().size(); i++) {
      // Get actual type
      auto assignExpr = node->fieldLst()->args()[i];
      auto actualType = any_cast<SymbolType>(visit(assignExpr));
      // Get expected type
      SymbolTableEntry *expectedField = structTable->lookupByIndex(i);
      assert(expectedField != nullptr);
      SymbolType expectedSymbolType = expectedField->type;
      // Replace expected type with the capture name
      if (expectedSymbolType.is(TY_STRUCT))
        expectedSymbolType =
            expectedSymbolType.replaceBaseSubType(accessScopePrefix + expectedSymbolType.getBaseType().getSubType());
      // Check if type matches declaration
      if (actualType != expectedSymbolType)
        throw SemanticError(assignExpr, FIELD_TYPE_NOT_MATCHING,
                            "Expected type " + expectedSymbolType.getName(false) + " for the field '" + expectedField->name +
                                "', but got " + actualType.getName(false));
    }
  }

  // Insert anonymous symbol to keep track of dtor calls for de-allocation
  currentScope->insertAnonymous(structType, node, INITIALIZED);

  return node->setEvaluatedSymbolType(structType);
}

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  auto type = any_cast<SymbolType>(visit(node->baseDataType()));

  size_t assignExprCounter = 0;
  std::vector<AssignExprNode *> arraySizeExpr = node->arraySizeExpr();
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

          auto sizeType = any_cast<SymbolType>(visit(arraySizeExpr[assignExprCounter++]));
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
    return node->setEvaluatedSymbolType(any_cast<SymbolType>(visit(node->customDataType())));
  default:
    return node->setEvaluatedSymbolType(SymbolType(TY_DYN));
  }
}

std::any TypeChecker::visitCustomDataType(CustomDataTypeNode *node) {
  // It is a struct type -> get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
  std::string firstFragment = node->typeNameFragments.front();

  // Check if it is a String type
  bool isStringRuntime = rootScope->lookupStrict(STROBJ_NAME) != nullptr && !rootScope->lookupCaptureStrict(STROBJ_NAME);
  if (node->typeNameFragments.size() == 1 && firstFragment == STROBJ_NAME && !isStringRuntime) {
    sourceFile.requestRuntimeModule(STRING_RT);
    return node->setEvaluatedSymbolType(SymbolType(TY_STROBJ));
  }

  // Check if it is a generic type
  SymbolTableEntry *entry = currentScope->lookup(firstFragment);
  if (node->typeNameFragments.size() == 1 && !entry && currentScope->lookupGenericType(firstFragment)) {
    SymbolType symbolType = *static_cast<SymbolType *>(currentScope->lookupGenericType(firstFragment));
    return node->setEvaluatedSymbolType(symbolType);
  }

  // Get type name in format: a.b.c and retrieve the scope in parallel
  std::string accessScopePrefix;
  std::string typeName;
  bool isImported = false;
  for (unsigned int i = 0; i < node->typeNameFragments.size(); i++) {
    typeName = node->typeNameFragments[i];

    if (i < node->typeNameFragments.size() - 1)
      accessScopePrefix += typeName + ".";
    entry = accessScope->lookup(typeName);
    if (!entry)
      throw SemanticError(node, UNKNOWN_DATATYPE, "Unknown symbol '" + typeName + "'");
    if (!entry->type.isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_IMPORT}))
      throw SemanticError(node, EXPECTED_TYPE, "Expected type, but got " + entry->type.getName());

    std::string tableName = typeName;
    if (entry->type.is(TY_STRUCT)) {
      tableName = STRUCT_SCOPE_PREFIX + typeName;
    } else if (entry->type.is(TY_INTERFACE)) {
      tableName = INTERFACE_SCOPE_PREFIX + typeName;
    } else if (entry->type.is(TY_ENUM)) {
      tableName = ENUM_SCOPE_PREFIX + typeName;
    }
    accessScope = accessScope->lookupTable(tableName);
    assert(accessScope != nullptr);
    if (accessScope->isImported(currentScope))
      isImported = true;
  }

  // Enums can early-return
  if (entry->type.is(TY_ENUM))
    return SymbolType(TY_INT);

  if (entry->type.is(TY_STRUCT)) {
    // Get the concrete template types
    std::vector<SymbolType> concreteTemplateTypes;
    if (node->templateTypeLst()) {
      for (const auto &dataType : node->templateTypeLst()->dataTypes())
        concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
    }

    // Set the struct instance to used
    Struct *spiceStruct = accessScope->matchStruct(nullptr, typeName, concreteTemplateTypes, node);
    if (spiceStruct)
      spiceStruct->isUsed = true;

    if (isImported) { // Imported struct
      SymbolType symbolType = initExtStruct(accessScope, accessScopePrefix, typeName, concreteTemplateTypes, node);
      return node->setEvaluatedSymbolType(symbolType);
    }

    // Check if struct was declared
    SymbolTableEntry *structSymbol = accessScope->lookup(typeName);
    if (!structSymbol)
      throw SemanticError(node, UNKNOWN_DATATYPE, "Unknown datatype '" + typeName + "'");
    if (!structSymbol->type.is(TY_STRUCT))
      throw SemanticError(node, EXPECTED_STRUCT_TYPE, "Expected struct type, but got " + structSymbol->type.getName());
    structSymbol->isUsed = true;

    return node->setEvaluatedSymbolType(SymbolType(TY_STRUCT, typeName, {}, concreteTemplateTypes));
  } else if (entry->type.is(TY_INTERFACE)) {
    // Check if template types are given
    if (node->templateTypeLst())
      throw SemanticError(node->templateTypeLst(), INTERFACE_WITH_TEMPLATE_LIST,
                          "Referencing interfaces with template lists is not allowed");

    // Set the interface instance to used
    Interface *spiceInterface = accessScope->lookupInterface(typeName);
    if (spiceInterface)
      spiceInterface->isUsed = true;

    // Check if interface was declared
    SymbolTableEntry *interfaceSymbol = accessScope->lookup(typeName);
    if (!interfaceSymbol)
      throw SemanticError(node, UNKNOWN_DATATYPE, "Unknown datatype '" + typeName + "'");
    if (!interfaceSymbol->type.is(TY_INTERFACE))
      throw SemanticError(node, EXPECTED_INTERFACE_TYPE, "Expected interface type, but got " + interfaceSymbol->type.getName());
    interfaceSymbol->isUsed = true;

    return node->setEvaluatedSymbolType(SymbolType(TY_INTERFACE, typeName, {}, {}));
  }
  throw std::runtime_error("Base type fall-through");
}
