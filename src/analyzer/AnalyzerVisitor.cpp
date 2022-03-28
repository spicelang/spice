// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

#include "util/CompilerWarning.h"
#include <CompilerInstance.h>
#include <analyzer/OpRuleManager.h>
#include <exception/SemanticError.h>
#include <symbol/SymbolSpecifiers.h>
#include <util/FileUtil.h>
#include <util/ScopeIdUtil.h>

AnalyzerVisitor::AnalyzerVisitor(const std::shared_ptr<llvm::LLVMContext> &context,
                                 const std::shared_ptr<llvm::IRBuilder<>> &builder, ModuleRegistry *moduleRegistry,
                                 ThreadFactory *threadFactory, CliOptions *options, LinkerInterface *linker,
                                 const std::string &sourceFile, bool requiresMainFct, bool isStdFile) {
  this->context = context, this->builder = builder;
  this->moduleRegistry = moduleRegistry;
  this->threadFactory = threadFactory;
  this->linker = linker;
  this->sourceFile = sourceFile;
  this->requiresMainFct = requiresMainFct;
  this->isStdFile = isStdFile;

  // Use default target triple if empty
  if (options->targetTriple.empty()) {
    llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    options->targetArch = targetTriple.getArchName();
    options->targetVendor = targetTriple.getVendorName();
    options->targetOs = targetTriple.getOSName();
  }
  this->cliOptions = options;

  // Create error factory for this specific file
  this->err = new ErrorFactory(sourceFile);

  // Create OpRuleManager
  opRuleManager = std::make_unique<OpRuleManager>(err);
}

AnalyzerVisitor::~AnalyzerVisitor() { delete this->err; }

antlrcpp::Any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  // --- Pre-traversing actions
  // Create current scope
  rootScope = currentScope = new SymbolTable(nullptr, requiresMainFct);

  // --- Traverse AST
  visitChildren(ctx);

  // --- Post traversing actions
  // Check if the visitor got a main function
  if (requiresMainFct && !hasMainFunction)
    throw err->get(*ctx->start, MISSING_MAIN_FUNCTION, "No main function found");

  // Print compiler warnings once the whole ast is present, but not for std files
  if (requiresMainFct && !isStdFile)
    rootScope->printCompilerWarnings();

  // Return the symbol table for further use in following compile phases
  return currentScope;
}

antlrcpp::Any AnalyzerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  std::string mainSignature = MAIN_FUNCTION_NAME + "()";

  // Check if the function is already defined
  if (currentScope->lookup(mainSignature))
    throw err->get(*ctx->start, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert function name into the root symbol table
  SymbolType symbolType = SymbolType(TY_FUNCTION);
  currentScope->insert(mainSignature, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start);

  // Create the function scope
  currentScope = currentScope->createChildBlock(mainSignature);

  // Declare variable for the return value in the function scope
  SymbolType returnType = SymbolType(TY_INT);
  currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), INITIALIZED, *ctx->start);
  currentScope->lookup(RETURN_VARIABLE_NAME)->setUsed();

  // Visit arguments
  argumentMode = true;
  if (ctx->argLstDef())
    visit(ctx->argLstDef());
  argumentMode = false;

  // Visit statements in new scope
  visit(ctx->stmtLst());

  // Return to global scope
  currentScope = currentScope->getParent();

  // Confirm main function
  hasMainFunction = true;
  currentScope->lookup(mainSignature)->setUsed();

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  // Save the old scope to restore later
  SymbolTable *oldScope = currentScope;

  // Check if this is a global function or a method
  bool isMethod = false;
  std::string functionName = ctx->IDENTIFIER().back()->toString();
  if (ctx->IDENTIFIER().size() > 1) { // Method
    isMethod = true;
    // Change to the struct scope
    currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER()[0]->toString());
  }

  // Get template types
  std::vector<SymbolType> templateTypes;
  if (ctx->templateDef()) {
    for (const auto &dataType : ctx->templateDef()->typeLst()->dataType())
      templateTypes.push_back(visit(dataType).as<SymbolType>());
  }

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Visit arguments in new scope
  argumentMode = true;
  ArgList argTypes;
  if (ctx->argLstDef())
    argTypes = visit(ctx->argLstDef()).as<ArgList>();
  argumentMode = false;

  // Declare 'this' variable in new scope
  SymbolType thisType = SymbolType(TY_DYN);
  if (isMethod) {
    std::string structName = ctx->IDENTIFIER().front()->toString();
    SymbolTableEntry *structEntry = currentScope->lookup(structName);
    SymbolType curThisType = thisType = structEntry->getType();
    curThisType = curThisType.toPointer(err, *ctx->start);
    auto thisTypeSpecifiers = SymbolSpecifiers(curThisType);
    thisTypeSpecifiers.setConst(true);
    currentScope->insert(THIS_VARIABLE_NAME, curThisType, thisTypeSpecifiers, INITIALIZED, *ctx->start);
  }

  // Declare variable for the return value in the function scope
  SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
  if (returnType.is(TY_DYN))
    throw err->get(*ctx->start, UNEXPECTED_DYN_TYPE_SA, "Dyn return types are not allowed");
  if (returnType.isPointer())
    throw err->get(*ctx->start, COMING_SOON_SA,
                   "Spice currently not supports pointer return types due to not supporting heap allocations.");
  currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start);

  // Return to old scope
  currentScope = currentScope->getParent();

  // Build function specifiers
  auto functionSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        functionSymbolSpecifiers.setPublic(true);
      } else if (specifier->INLINE()) {
        functionSymbolSpecifiers.setPublic(false);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a function definition");
      }
    }
  }

  // Insert function into the symbol table
  Function spiceFunc = Function(functionName, functionSymbolSpecifiers, thisType, returnType, argTypes, templateTypes);
  currentScope->insertFunction(spiceFunc, err, *ctx->IDENTIFIER().back()->getSymbol());

  // Rename / duplicate the original child block to reflect the substantiated versions of the function
  std::vector<Function> substantiatedFunctions = spiceFunc.substantiate();
  currentScope->renameChildBlock(scopeId, substantiatedFunctions[0].getSignature());
  for (int i = 0; i < substantiatedFunctions.size(); i++)
    currentScope->duplicateChildBlockEntry(substantiatedFunctions[0].getSignature(), substantiatedFunctions[i].getSignature());

  // Go down again in scope
  currentScope = currentScope->getChild(substantiatedFunctions[0].getSignature());
  assert(currentScope != nullptr);

  // Visit statements in new scope
  visit(ctx->stmtLst());

  // Check if return variable is now initialized
  if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
    throw err->get(*ctx->start, FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");

  // Restore old scope
  currentScope = oldScope;

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  // Save the old scope to restore later
  SymbolTable *oldScope = currentScope;

  // Check if this is a global function or a method
  bool isMethod = false;
  std::string procedureName = ctx->IDENTIFIER().back()->toString();
  if (ctx->IDENTIFIER().size() > 1) { // Method
    isMethod = true;
    // Change to the struct scope
    currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER()[0]->toString());
  }

  // Get template types
  std::vector<SymbolType> templateTypes;
  if (ctx->templateDef()) {
    for (const auto &dataType : ctx->templateDef()->typeLst()->dataType())
      templateTypes.push_back(visit(dataType).as<SymbolType>());
  }

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Visit arguments in new scope
  argumentMode = true;
  ArgList argTypes;
  if (ctx->argLstDef())
    argTypes = visit(ctx->argLstDef()).as<ArgList>();
  argumentMode = false;

  // Declare 'this' variable in new scope
  SymbolType thisType = SymbolType(TY_DYN);
  if (isMethod) {
    std::string structName = ctx->IDENTIFIER().front()->toString();
    SymbolTableEntry *structEntry = currentScope->lookup(structName);
    SymbolType curThisType = thisType = structEntry->getType();
    curThisType = curThisType.toPointer(err, *ctx->start);
    auto thisSymbolSpecifiers = SymbolSpecifiers(curThisType);
    thisSymbolSpecifiers.setConst(true);
    currentScope->insert(THIS_VARIABLE_NAME, curThisType, thisSymbolSpecifiers, INITIALIZED, *ctx->start);
  }

  // Return to old scope
  currentScope = currentScope->getParent();

  // Build procedure specifiers
  auto procedureSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        procedureSymbolSpecifiers.setPublic(true);
      } else if (specifier->INLINE()) {
        procedureSymbolSpecifiers.setPublic(false);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a procedure definition");
      }
    }
  }

  // Insert function into the symbol table
  Function spiceProc = Function(procedureName, procedureSymbolSpecifiers, thisType, SymbolType(TY_DYN), argTypes, templateTypes);
  currentScope->insertFunction(spiceProc, err, *ctx->IDENTIFIER().back()->getSymbol());

  // Rename / duplicate the original child block to reflect the substantiated versions of the function
  std::vector<Function> substantiatedProcedures = spiceProc.substantiate();
  currentScope->renameChildBlock(scopeId, substantiatedProcedures[0].getSignature());
  for (int i = 0; i < substantiatedProcedures.size(); i++)
    currentScope->duplicateChildBlockEntry(substantiatedProcedures[0].getSignature(), substantiatedProcedures[i].getSignature());

  // Go down again in scope
  currentScope = currentScope->getChild(substantiatedProcedures[0].getSignature());
  assert(currentScope != nullptr);

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Return to old scope
  currentScope = oldScope;

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  std::string functionName = ctx->IDENTIFIER()->toString();

  ArgList argTypes;
  if (ctx->typeLst()) {
    // Check if an argument is dyn
    for (const auto &arg : ctx->typeLst()->dataType()) {
      SymbolType argType = visit(arg).as<SymbolType>();
      if (argType.is(TY_DYN))
        throw err->get(*arg->start, UNEXPECTED_DYN_TYPE_SA, "Dyn data type is not allowed as arg type for external functions");
      argTypes.emplace_back(argType, false);
    }
  }

  if (ctx->dataType()) { // Function
    // Check if return type is dyn
    SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
    if (returnType.is(TY_DYN))
      throw err->get(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA,
                     "Dyn data type is not allowed as return type for external functions");

    // Insert function into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    Function spiceFunc = Function(functionName, symbolSpecifiers, SymbolType(TY_DYN), returnType, argTypes, {});
    currentScope->insertFunction(spiceFunc, err, *ctx->IDENTIFIER()->getSymbol());

    // Add return symbol for function
    SymbolTable *functionTable = currentScope->createChildBlock(spiceFunc.getSignature());
    functionTable->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start);
    functionTable->lookup(RETURN_VARIABLE_NAME)->setUsed();
  } else { // Procedure
    // Insert procedure into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
    Function spiceProc = Function(functionName, symbolSpecifiers, SymbolType(TY_DYN), SymbolType(TY_DYN), argTypes, {});
    currentScope->insertFunction(spiceProc, err, *ctx->IDENTIFIER()->getSymbol());
  }

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  std::string typeName = ctx->IDENTIFIER()->toString();

  // Check if type already exists in this scope
  if (currentScope->lookup(typeName))
    throw err->get(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate generic type '" + typeName + "'");

  // Build symbol specifiers
  SymbolType symbolType = SymbolType(TY_GENERIC, typeName);
  auto structSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        structSymbolSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a struct definition");
      }
    }
  }

  // Create a new symbol table entry
  currentScope->insert(typeName, symbolType, structSymbolSpecifiers, DECLARED, *ctx->start);

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) {
  std::string structName = ctx->IDENTIFIER()->toString();

  // Check if struct already exists in this scope
  if (currentScope->lookup(structName))
    throw err->get(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate struct '" + structName + "'");

  // Build symbol specifiers
  SymbolType symbolType = SymbolType(TY_STRUCT, structName);
  auto structSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        structSymbolSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a struct definition");
      }
    }
  }

  // Create a new symbol table entry for the struct
  currentScope->insert(structName, symbolType, structSymbolSpecifiers, DECLARED, *ctx->start);

  // Visit field list in a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Insert a field for each field list entry
  for (const auto &field : ctx->field()) {
    std::string fieldName = field->IDENTIFIER()->toString();
    SymbolType fieldType = visit(field->dataType()).as<SymbolType>();

    // Build symbol specifiers
    auto fieldTypeSpecifiers = SymbolSpecifiers(symbolType);
    if (field->declSpecifiers()) {
      for (const auto &specifier : field->declSpecifiers()->declSpecifier()) {
        if (specifier->CONST()) {
          // Struct fields cannot be const
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Struct fields cannot have the const specifier attached");
        } else if (specifier->SIGNED()) {
          fieldTypeSpecifiers.setSigned(true);
        } else if (specifier->UNSIGNED()) {
          fieldTypeSpecifiers.setSigned(false);
        } else if (specifier->PUBLIC()) {
          fieldTypeSpecifiers.setPublic(true);
        } else {
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Cannot use the " + specifier->getText() + " specifier on a struct field definition");
        }
      }
    }

    currentScope->insert(fieldName, fieldType, fieldTypeSpecifiers, DECLARED, *field->start);
  }

  // Return to the old scope
  currentScope = currentScope->getParent();
  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  std::string variableName = ctx->IDENTIFIER()->toString();

  // Check if symbol already exists in the symbol table
  if (currentScope->lookup(variableName))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE,
                   "The global variable '" + variableName + "' was declared more than once");

  // Check if symbol already exists in any imported module scope
  if (currentScope->lookupGlobal(variableName, true))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE,
                   "A global variable named '" + variableName +
                       "' is already declared in another module. Please use a different name.");

  // Insert variable name to symbol table
  SymbolType symbolType = visit(ctx->dataType()).as<SymbolType>();

  SymbolState state = DECLARED;
  if (ctx->value()) { // Variable is initialized here
    SymbolType valueType = visit(ctx->value()).as<SymbolType>();
    // Infer type
    if (symbolType.is(TY_DYN)) {
      symbolType = valueType;
    } else if (symbolType != valueType) {
      throw err->get(*ctx->value()->start, OPERATOR_WRONG_DATA_TYPE,
                     "Cannot apply the assign operator on different data types. You provided " + symbolType.getName(false) +
                         " and " + valueType.getName(false));
    }
    state = INITIALIZED;
  }

  // Check if the type is missing
  if (symbolType.is(TY_DYN))
    throw err->get(*ctx->dataType()->start, GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope
  if (!symbolType.isPrimitive())
    throw err->get(*ctx->dataType()->start, GLOBAL_OF_INVALID_TYPE, "Spice does not allow global variables of this type");

  // Create symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->CONST()) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->SIGNED()) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->UNSIGNED()) {
        symbolTypeSpecifiers.setSigned(false);

        // Check if there is a negative value attached. If yes, print a compiler warning
        if (ctx->MINUS())
          CompilerWarning(*ctx->MINUS()->getSymbol(), NEGATIVE_VALUE_TO_UNSIGNED_VAR,
                          "Please mind that assigning a negative value to an unsigned variable causes a wrap-around")
              .print();
      } else if (specifier->PUBLIC()) {
        symbolTypeSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a global variable definition");
      }
    }
  }

  // Insert into symbol table
  currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, state, *ctx->start);

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);
  currentScope->setCapturingRequired(); // Requires capturing because the LLVM IR will end up in a separate function

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BYTE).toPointer(err, *ctx->start);
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto head = ctx->forHead();

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Visit loop variable declaration in new scope
  visit(head->declStmt());

  // Visit condition in new scope
  SymbolType conditionType = visit(head->assignExpr()[0]).as<SymbolType>();
  if (!conditionType.is(TY_BOOL))
    throw err->get(*head->assignExpr()[0]->start, CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");

  // Visit incrementer in new scope
  visit(head->assignExpr()[1]);

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();
  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto head = ctx->foreachHead();

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Check type of the array
  expectedType = SymbolType(TY_DYN);
  SymbolType arrayType = visit(head->assignExpr()).as<SymbolType>();
  if (!arrayType.isArray() && !arrayType.is(TY_STRING))
    throw err->get(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                   "Can only apply foreach loop on an array type. You provided " + arrayType.getName(false));

  // Check index assignment or declaration
  SymbolType indexType;
  if (head->declStmt().size() >= 2) {
    indexType = visit(head->declStmt().front()).as<SymbolType>();

    // Set declared variable to initialized, because we increment it internally in the loop
    if (!head->declStmt().front()->assignExpr()) {
      std::string varName = head->declStmt().front()->IDENTIFIER()->toString();
      SymbolTableEntry *entry = currentScope->lookup(varName);
      assert(entry != nullptr);
      entry->updateState(INITIALIZED, err, *head->declStmt().front()->IDENTIFIER()->getSymbol());
    }

    // Check if index type is int
    if (!indexType.is(TY_INT))
      throw err->get(*head->declStmt().front()->start, ARRAY_INDEX_NO_INTEGER,
                     "Index in foreach loop must be of type int. You provided " + indexType.getName(false));
  } else {
    // Declare the variable with the default index variable name
    SymbolType symbolType = SymbolType(TY_INT);
    auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    symbolTypeSpecifiers.setConst(true);
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, symbolType, symbolTypeSpecifiers, INITIALIZED, *ctx->start);
  }

  // Check type of the item
  SymbolType itemType = visit(head->declStmt().back()).as<SymbolType>();
  std::string itemVarName = head->declStmt().back()->IDENTIFIER()->toString();
  SymbolTableEntry *itemVarSymbol = currentScope->lookup(itemVarName);
  assert(itemVarSymbol != nullptr);
  if (itemType.is(TY_DYN)) {
    itemType = arrayType.getContainedTy();
    itemVarSymbol->updateType(itemType, false);
  } else {
    if (itemType != arrayType.getContainedTy())
      throw err->get(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                     "Foreach loop item type does not match array type. Expected " + arrayType.getName(false) + ", provided " +
                         itemType.getName(false));
  }
  itemVarSymbol->updateState(INITIALIZED, err, *head->declStmt().back()->IDENTIFIER()->getSymbol());

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Visit condition
  SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
  if (!conditionType.is(TY_BOOL))
    throw err->get(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId);

  // Visit condition
  SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
  if (!conditionType.is(TY_BOOL))
    throw err->get(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  // Visit else statement if it exists
  if (ctx->elseStmt())
    visit(ctx->elseStmt());

  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  if (ctx->ifStmt()) { // Visit if statement in the case of an else if branch
    visit(ctx->ifStmt());
  } else { // Make a new scope in case of an else branch
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);

    // Visit statement list in new scope
    visit(ctx->stmtLst());

    // Return to old scope
    currentScope = currentScope->getParent();
  }
  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) {
  ArgList argList;
  bool metOptional = false;
  for (const auto &arg : ctx->declStmt()) {
    SymbolType argType = visit(arg).as<SymbolType>();

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (argType.is(TY_DYN))
      throw err->get(*arg->start, FCT_ARG_IS_TYPE_DYN, "Type of argument '" + arg->IDENTIFIER()->toString() + "' is invalid");

    // Ensure that no optional argument comes after a mandatory argument
    if (arg->ASSIGN()) {
      metOptional = true;
    } else if (metOptional) {
      throw err->get(*arg->start, INVALID_ARGUMENT_ORDER, "Mandatory arguments must go before any optional arguments");
    }

    argList.emplace_back(argType, metOptional);
  }
  return argList;
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  std::string variableName = ctx->IDENTIFIER()->toString();
  // Check if symbol already exists in the symbol table
  if (currentScope->lookupStrict(variableName))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE, "The variable '" + variableName + "' was declared more than once");

  // Get the type of the symbol
  SymbolType symbolType = expectedType = visit(ctx->dataType()).as<SymbolType>();

  // Visit the right side
  SymbolState initialState = DECLARED;
  if (ctx->assignExpr()) {
    SymbolType rhsTy = visit(ctx->assignExpr()).as<SymbolType>();
    // Check if type has to be inferred or both types are fixed
    symbolType = symbolType.is(TY_DYN) ? rhsTy : opRuleManager->getAssignResultType(*ctx->start, symbolType, rhsTy, true);
    initialState = INITIALIZED;

    // If the rhs is of type array and was the array initialization, there must be a size attached
    if (symbolType.isArray() && symbolType.getArraySize() == 0 && currentVarName.empty())
      throw err->get(*ctx->dataType()->start, ARRAY_SIZE_INVALID, "The declaration of an array type must have a size attached");
  }

  if (argumentMode && symbolType.isArray()) // Change array type to pointer type for function/procedure arguments
    symbolType = symbolType.getContainedTy().toPointer(err, *ctx->dataType()->start);

  // Build symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->CONST()) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->SIGNED()) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->UNSIGNED()) {
        symbolTypeSpecifiers.setSigned(false);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a local variable declaration");
      }
    }
  }

  // Insert variable into symbol table
  currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, initialState, *ctx->start);

  return symbolType;
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  // Check if imported library exists
  std::string importPath = ctx->STRING_LITERAL()->toString();
  importPath = importPath.substr(1, importPath.size() - 2);

  // Check if source file exists
  std::string filePath;
  bool foundInStd = false;
  if (importPath.rfind("std/", 0) == 0) { // Include source file from standard library
    foundInStd = true;
    std::string sourceFileIden = importPath.substr(importPath.find("std/") + 4);
    // Find std library
    std::string stdPath;
    if (FileUtil::fileExists("/usr/lib/spice/std")) {
      stdPath = "/usr/lib/spice/std/";
    } else if (FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
      stdPath = std::string(std::getenv("SPICE_STD_DIR"));
      if (stdPath.rfind('/') != stdPath.size() - 1)
        stdPath += "/";
    } else {
      throw err->get(*ctx->STRING_LITERAL()->getSymbol(), STD_NOT_FOUND,
                     "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
    }
    // Check if source file exists
    std::string defaultPath = stdPath + sourceFileIden + ".spice";
    std::string osPath = stdPath + sourceFileIden + "_" + cliOptions->targetOs + ".spice";
    std::string osArchPath = stdPath + sourceFileIden + "_" + cliOptions->targetOs + "_" + cliOptions->targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      filePath = defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      filePath = osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      filePath = osArchPath;
    } else {
      throw err->get(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                     "The source file '" + importPath + ".spice' was not found in the standard library");
    }
  } else { // Include own source file
    // Check in module registry if the file can be imported
    std::string sourceFileDir = FileUtil::getFileDir(sourceFile);
    // Import file
    std::string defaultPath = sourceFileDir + "/" + importPath + ".spice";
    std::string osPath = sourceFileDir + "/" + importPath + "_" + cliOptions->targetOs + ".spice";
    std::string osArchPath =
        sourceFileDir + "/" + importPath + "_" + cliOptions->targetOs + "_" + cliOptions->targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      filePath = defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      filePath = osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      filePath = osArchPath;
    } else {
      throw err->get(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                     "The source file '" + importPath + ".spice' does not exist");
    }
  }

  // Check if this file could cause a circular import
  if (moduleRegistry->causesCircularImport(filePath))
    throw err->get(*ctx->STRING_LITERAL()->getSymbol(), CIRCULAR_DEPENDENCY, filePath);

  // Check if the file is already or needs to be compiled
  SymbolTable *nestedTable;
  if (moduleRegistry->isAlreadyCompiled(filePath)) {
    nestedTable = moduleRegistry->getSymbolTable(filePath);
  } else {
    // Push module to module path
    moduleRegistry->pushToImportPath(filePath);

    // Kick off the compilation of the imported source file
    nestedTable = CompilerInstance::CompileSourceFile(context, builder, moduleRegistry, threadFactory, cliOptions, linker,
                                                      filePath, false, foundInStd);
    moduleRegistry->addToCompiledModules(filePath, nestedTable);

    // Pop module from module path
    moduleRegistry->popFromImportPath();
  }
  assert(nestedTable != nullptr);

  // Create symbol of type TYPE_IMPORT in the current scope
  std::string importIden = ctx->IDENTIFIER()->toString();
  SymbolType symbolType = SymbolType(TY_IMPORT);
  currentScope->insert(importIden, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start);

  // Mount symbol table of the imported source file into the current scope
  nestedTable->setImported();
  currentScope->mountChildBlock(importIden, nestedTable);

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  SymbolTableEntry *returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
  if (returnVariable) { // Return variable => function
    expectedType = returnVariable->getType();

    // Check if there is a value attached to the return statement
    if (ctx->assignExpr()) {
      // Visit the value
      SymbolType returnType = visit(ctx->assignExpr()).as<SymbolType>();

      // Check data type of return statement
      if (returnVariable->getType().is(TY_DYN)) {
        // Set explicit return type to the return variable
        returnVariable->updateType(returnType, false);
      } else {
        // Check if return type matches with function definition
        if (returnType != returnVariable->getType())
          throw err->get(*ctx->assignExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                         "Passed wrong data type to return statement. Expected " + returnVariable->getType().getName(false) +
                             " but got " + returnType.getName(false));
      }

      // Set the return variable to initialized
      returnVariable->updateState(INITIALIZED, err, *ctx->start);
    }

    // Check if result variable is initialized
    if (returnVariable->getState() != INITIALIZED)
      throw err->get(*ctx->start, RETURN_WITHOUT_VALUE_RESULT,
                     "Return without value, but result variable is not initialized yet");
    returnVariable->setUsed();

    return returnVariable->getType();
  }
  // No return variable => procedure
  if (ctx->assignExpr())
    throw err->get(*ctx->assignExpr()->start, RETURN_WITH_VALUE_IN_PROCEDURE,
                   "Return statements in procedures may not have a value attached");
  return SymbolType(TY_DYN);
}

antlrcpp::Any AnalyzerVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  int breakCount = 1;
  if (ctx->INTEGER()) {
    // Check if the stated number is valid
    breakCount = std::stoi(ctx->INTEGER()->toString());
    if (breakCount < 1)
      throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                     "Break count must be >= 1, you provided " + ctx->INTEGER()->toString());
  }
  // Check if we can break this often
  if (breakCount > nestedLoopCounter)
    throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                   "We can only break " + std::to_string(nestedLoopCounter) + " time(s) here");
  return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  int continueCount = 1;
  if (ctx->INTEGER()) {
    // Check if the stated number is valid
    continueCount = std::stoi(ctx->INTEGER()->toString());
    if (continueCount < 1)
      throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                     "Continue count must be >= 1, you provided " + ctx->INTEGER()->toString());
  }
  // Check if we can continue this often
  if (continueCount > nestedLoopCounter)
    throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                   "We can only continue " + std::to_string(nestedLoopCounter) + " time(s) here");
  return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  if (ctx->printfCall())
    return visit(ctx->printfCall());
  if (ctx->sizeOfCall())
    return visit(ctx->sizeOfCall());
  if (ctx->tidCall())
    return visit(ctx->tidCall());
  if (ctx->joinCall())
    return visit(ctx->joinCall());
  throw std::runtime_error("Internal compiler error: Could not find builtin function"); // GCOV_EXCL_LINE
}

antlrcpp::Any AnalyzerVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  std::string templateString = ctx->STRING_LITERAL()->toString();
  templateString = templateString.substr(1, templateString.size() - 2);

  // Check if assignment types match placeholder types
  std::size_t index = templateString.find_first_of('%');
  int placeholderCount = 0;
  while (index != std::string::npos) {
    // Check if there is another assignExpr
    if (ctx->assignExpr().size() <= placeholderCount)
      throw err->get(*ctx->STRING_LITERAL()->getSymbol(), PRINTF_ARG_COUNT_ERROR,
                     "The placeholder string contains more placeholders that arguments were passed");

    auto assignment = ctx->assignExpr()[placeholderCount];
    SymbolType assignmentType = visit(assignment).as<SymbolType>();
    switch (templateString[index + 1]) {
    case 'c': {
      if (!assignmentType.is(TY_CHAR))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
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
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects int, byte or bool, but got " + assignmentType.getName(false));
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
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects double, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 's': {
      if (!assignmentType.is(TY_STRING) && !assignmentType.isPointerOf(TY_CHAR) && !assignmentType.isArrayOf(TY_CHAR))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects string, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!assignmentType.isPointer() && !assignmentType.isArray())
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects pointer, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    }
    index = templateString.find_first_of('%', index + 1);
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < ctx->assignExpr().size())
    throw err->get(*ctx->start, PRINTF_ARG_COUNT_ERROR,
                   "The placeholder string contains less placeholders that arguments were passed");

  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  // Nothing to check here. Sizeof builtin can handle any type
  return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) {
  // Nothing to check here. Tid builtin has no arguments
  return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(err, *ctx->start);
  for (const auto &assignExpr : ctx->assignExpr()) {
    SymbolType argSymbolType = visit(assignExpr).as<SymbolType>();
    if (argSymbolType == bytePtr && argSymbolType.isArrayOf(bytePtr))
      throw err->get(*assignExpr->start, JOIN_ARG_MUST_BE_TID,
                     "You have to pass a thread id (byte*) or a array of thread ids (byte*[]) to to join builtin");
  }

  // Return the number of threads that were joined
  return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  // Check if there is an assign operator applied
  if (ctx->assignOp()) { // This is an assignment
    // Get symbol type of right side
    SymbolType rhsTy = visit(ctx->assignExpr()).as<SymbolType>();

    // Visit the left side
    currentVarName = ""; // Reset the current variable name
    SymbolType lhsTy = visit(ctx->prefixUnaryExpr()).as<SymbolType>();
    std::string variableName = currentVarName;

    // Take a look at the operator
    if (ctx->assignOp()->ASSIGN()) {
      rhsTy = opRuleManager->getAssignResultType(*ctx->start, lhsTy, rhsTy, false);
    } else if (ctx->assignOp()->PLUS_EQUAL()) {
      rhsTy = opRuleManager->getPlusEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->MINUS_EQUAL()) {
      rhsTy = opRuleManager->getMinusEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->MUL_EQUAL()) {
      rhsTy = opRuleManager->getMulEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->DIV_EQUAL()) {
      rhsTy = opRuleManager->getDivEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->REM_EQUAL()) {
      rhsTy = opRuleManager->getRemEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->SHL_EQUAL()) {
      rhsTy = opRuleManager->getSHLEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->SHR_EQUAL()) {
      rhsTy = opRuleManager->getSHREqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->AND_EQUAL()) {
      rhsTy = opRuleManager->getAndEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->OR_EQUAL()) {
      rhsTy = opRuleManager->getOrEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->XOR_EQUAL()) {
      rhsTy = opRuleManager->getXorEqualResultType(*ctx->start, lhsTy, rhsTy);
    }

    if (!variableName.empty()) { // Variable is involved on the left side
      // Check if the symbol exists
      if (!currentEntry)
        throw err->get(*ctx->prefixUnaryExpr()->start, REFERENCED_UNDEFINED_VARIABLE,
                       "The variable '" + variableName + "' was referenced before defined");

      // Perform type inference
      if (lhsTy.is(TY_DYN))
        currentEntry->updateType(rhsTy, false);

      // Update state in symbol table
      if (!currentEntry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE}))
        currentEntry->updateState(INITIALIZED, err, *ctx->prefixUnaryExpr()->start);

      // In case the lhs variable is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(variableName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);

      // Print compiler warning if the rhs size exceeds the lhs size
      if (lhsTy.isArray() && rhsTy.getArraySize() > lhsTy.getArraySize())
        CompilerWarning(*ctx->assignExpr()->start, ARRAY_TOO_MANY_VALUES,
                        "You provided more values "
                        "than your array can hold. Excess variables are being ignored by the compiler.")
            .print();
    }

    return rhsTy;
  } else if (ctx->ternaryExpr()) {
    return visit(ctx->ternaryExpr());
  } else if (ctx->threadDef()) {
    return visit(ctx->threadDef());
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

antlrcpp::Any AnalyzerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  // Check if there is a ternary operator applied
  if (ctx->children.size() > 1) {
    auto condition = ctx->logicalOrExpr()[0];
    SymbolType conditionType = visit(condition).as<SymbolType>();
    SymbolType trueType = visit(ctx->logicalOrExpr()[1]).as<SymbolType>();
    SymbolType falseType = visit(ctx->logicalOrExpr()[2]).as<SymbolType>();
    // Check if the condition evaluates to boolean
    if (!conditionType.is(TY_BOOL))
      throw err->get(*condition->start, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");
    // Check if trueType and falseType are matching
    if (trueType != falseType)
      throw err->get(*ctx->start, OPERATOR_WRONG_DATA_TYPE, "True and false operands in ternary must be of same data type");
    return trueType;
  }
  return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  // Check if a logical or operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->logicalAndExpr()[0]).as<SymbolType>();
    for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
      SymbolType rhsTy = visit(ctx->logicalAndExpr()[i]).as<SymbolType>();
      lhsTy = opRuleManager->getLogicalOrResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  // Check if a logical and operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->bitwiseOrExpr()[0]).as<SymbolType>();
    for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
      SymbolType rhsTy = visit(ctx->bitwiseOrExpr()[i]).as<SymbolType>();
      lhsTy = opRuleManager->getLogicalAndResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  // Check if a bitwise or operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->bitwiseXorExpr()[0]).as<SymbolType>();
    for (int i = 1; i < ctx->bitwiseXorExpr().size(); i++) {
      SymbolType rhsTy = visit(ctx->bitwiseXorExpr()[i]).as<SymbolType>();
      lhsTy = opRuleManager->getBitwiseOrResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseXorExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  // Check if a bitwise xor operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->bitwiseAndExpr()[0]).as<SymbolType>();
    for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
      SymbolType rhsTy = visit(ctx->bitwiseAndExpr()[i]).as<SymbolType>();
      lhsTy = opRuleManager->getBitwiseXorResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  // Check if a bitwise and operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->equalityExpr()[0]).as<SymbolType>();
    for (int i = 1; i < ctx->equalityExpr().size(); i++) {
      SymbolType rhsTy = visit(ctx->equalityExpr()[i]).as<SymbolType>();
      lhsTy = opRuleManager->getBitwiseAndResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  // Check if at least one equality operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->relationalExpr()[0]).as<SymbolType>();
    SymbolType rhsTy = visit(ctx->relationalExpr()[1]).as<SymbolType>();

    if (ctx->EQUAL()) // Operator was equal
      return opRuleManager->getEqualResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->NOT_EQUAL()) // Operator was not equal
      return opRuleManager->getNotEqualResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  // Check if a relational operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->shiftExpr()[0]).as<SymbolType>();
    SymbolType rhsTy = visit(ctx->shiftExpr()[1]).as<SymbolType>();

    if (ctx->LESS()) // Operator was less
      return opRuleManager->getLessResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->GREATER()) // Operator was greater
      return opRuleManager->getGreaterResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->LESS_EQUAL()) // Operator was less equal
      return opRuleManager->getLessEqualResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->GREATER_EQUAL()) // Operator was greater equal
      return opRuleManager->getGreaterEqualResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->shiftExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  // Check if at least one shift operator is applied
  if (ctx->children.size() > 1) {
    SymbolType lhsTy = visit(ctx->additiveExpr()[0]).as<SymbolType>();
    SymbolType rhsTy = visit(ctx->additiveExpr()[1]).as<SymbolType>();

    if (ctx->SHL()) // Operator was shl
      return opRuleManager->getShiftLeftResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->SHR()) // Operator was shr
      return opRuleManager->getShiftRightResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  // Check if at least one additive operator is applied
  if (ctx->multiplicativeExpr().size() > 1) {
    SymbolType currentType = visit(ctx->multiplicativeExpr()[0]).as<SymbolType>();
    // Check if data types are compatible
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
      auto *op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      const size_t tokenType = op->getSymbol()->getType();
      auto next = ctx->multiplicativeExpr()[i];
      SymbolType nextType = visit(next).as<SymbolType>();

      if (tokenType == SpiceParser::PLUS) { // Operator was plus
        opRuleManager->getPlusResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::MINUS) { // Operator was minus
        opRuleManager->getMinusResultType(*next->start, currentType, nextType);
      }

      operatorIndex += 2;
    }
    return currentType;
  }
  return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  // Check if at least one multiplicative operator is applied
  if (ctx->castExpr().size() > 1) {
    SymbolType currentType = visit(ctx->castExpr()[0]).as<SymbolType>();
    // Check if data types are compatible
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->castExpr().size(); i++) {
      auto *op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      const size_t tokenType = op->getSymbol()->getType();
      auto next = ctx->castExpr()[i];
      SymbolType nextType = visit(next).as<SymbolType>();

      if (tokenType == SpiceParser::MUL) { // Operator is mul
        opRuleManager->getMulResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::DIV) { // Operator is div
        opRuleManager->getDivResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::REM) { // Operator is rem
        opRuleManager->getRemResultType(*next->start, currentType, nextType);
      }

      operatorIndex += 2;
    }
    return currentType;
  }
  return visit(ctx->castExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  antlrcpp::Any rhs = visit(ctx->prefixUnaryExpr());

  if (ctx->LPAREN()) { // Cast is applied
    SymbolType dstType = visit(ctx->dataType()).as<SymbolType>();
    return opRuleManager->getCastResultType(*ctx->start, dstType, rhs.as<SymbolType>());
  }

  return rhs;
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  currentVarName = "";                  // Reset the current variable name
  scopePrefix = "";                     // Reset scope prefix
  scopePath.clear();                    // Clear the scope path
  currentThisType = SymbolType(TY_DYN); // Reset this type

  SymbolType lhs = visit(ctx->postfixUnaryExpr()).as<SymbolType>();

  unsigned int tokenCounter = 0;
  while (tokenCounter < ctx->children.size() - 1) {
    auto *token = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(ctx->children[tokenCounter]);
    if (token->MINUS()) { // Consider - operator
      lhs = opRuleManager->getPrefixMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->PLUS_PLUS()) { // Consider ++ operator
      lhs = opRuleManager->getPrefixPlusPlusResultType(*ctx->postfixUnaryExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err, *token->start);

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (token->MINUS_MINUS()) { // Consider -- operator
      lhs = opRuleManager->getPrefixMinusMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err, *token->start);

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (token->NOT()) { // Consider ! operator
      lhs = opRuleManager->getPrefixNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->BITWISE_NOT()) { // Consider ~ operator
      lhs = opRuleManager->getPrefixBitwiseNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->MUL()) { // Consider * operator
      lhs = opRuleManager->getPrefixMulResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->BITWISE_AND()) { // Consider & operator
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->LOGICAL_AND()) { // Consider doubled & operator
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
    }
    tokenCounter++;
  }

  return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  SymbolType lhs = visit(ctx->atomicExpr()).as<SymbolType>();

  unsigned int tokenCounter = 1;
  while (tokenCounter < ctx->children.size()) {
    auto *token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
    const size_t tokenType = token->getSymbol()->getType();
    if (tokenType == SpiceParser::LBRACKET) { // Subscript operator
      tokenCounter++;                         // Consume LBRACKET

      std::string arrayName = currentVarName; // Save array name
      ScopePath scopePathBackup = scopePath;  // Save scope path
      scopePrefix += "[idx]";

      auto *rule = dynamic_cast<antlr4::RuleContext *>(ctx->children[tokenCounter]);
      SymbolType indexType = visit(rule).as<SymbolType>();
      tokenCounter++; // Consume assignExpr

      if (!indexType.is(TY_INT))
        throw err->get(*ctx->start, ARRAY_INDEX_NO_INTEGER, "Array index must be of type int");
      if (!lhs.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
        throw err->get(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                       "Can only apply subscript operator on array type, got " + lhs.getName(true));

      // Get array item type
      lhs = lhs.getContainedTy();

      currentVarName = arrayName;  // Restore array name
      scopePath = scopePathBackup; // Restore scope path

      // Retrieve scope for the new scope path fragment
      if (lhs.isBaseType(TY_STRUCT)) { // Struct
        SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        assert(accessScope != nullptr);

        SymbolTable *newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + lhs.getBaseType().getSubType());
        assert(newAccessScope != nullptr);
        // Push the retrieved scope to the scope path
        scopePath.pushFragment("[idx]", newAccessScope);
      }
    } else if (tokenType == SpiceParser::LPAREN) { // Consider function call
      std::string functionName = currentVarName;
      std::string accessScopePrefix = scopePath.getScopeName();

      // Save the scope path to restore it after visiting the args
      ScopePath scopePathBackup = scopePath;
      SymbolType thisType = currentThisType;

      // Get template types
      std::vector<SymbolType> templateTypes;
      if (!ctx->templateDef().empty()) {
        for (const auto &dataType : ctx->templateDef()[0]->typeLst()->dataType())
          templateTypes.push_back(visit(dataType).as<SymbolType>());
        tokenCounter++; // Consume templateDef
      }

      // Visit args
      tokenCounter++; // Consume LPAREN
      std::vector<SymbolType> argTypes;
      auto *argLst = dynamic_cast<SpiceParser::ArgLstContext *>(ctx->children[tokenCounter]);
      if (argLst != nullptr) {
        for (const auto &arg : argLst->assignExpr()) {
          SymbolType argType = visit(arg).as<SymbolType>();
          if (argType.isArray())
            argType = argType.getContainedTy().toPointer(err, *arg->start);
          argTypes.push_back(argType);
        }
      }
      tokenCounter++; // Consume argLst

      // Restore scope path
      scopePath = scopePathBackup;

      // Match a function onto the requirements of the call
      SymbolTable *functionParentScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : rootScope;
      Function *spiceFunc = functionParentScope->matchFunction(functionName, thisType, argTypes, err, *token->getSymbol());
      if (!spiceFunc) {
        // Build function to get a better error message
        std::vector<std::pair<SymbolType, bool>> argTypesWithOptional;
        argTypesWithOptional.reserve(argTypes.size());
        for (auto &argType : argTypes)
          argTypesWithOptional.emplace_back(argType, false);
        Function function = Function(functionName, SymbolSpecifiers(SymbolType(TY_FUNCTION)), thisType, SymbolType(TY_DYN),
                                     argTypesWithOptional, templateTypes);
        throw err->get(*ctx->start, REFERENCED_UNDEFINED_FUNCTION,
                       "Function/Procedure '" + function.getSignature() + "' could not be found");
      }
      spiceFunc->setUsed();

      // Get function scope
      scopePath.pushFragment(spiceFunc->getSignature(), functionParentScope);
      currentVarName = spiceFunc->getSignature();

      // Get function entry
      SymbolTableEntry *functionEntry = functionParentScope->lookup(spiceFunc->getSignature());
      assert(functionEntry != nullptr);
      functionEntry->setUsed(); // Set the function to used

      // Check if the function entry has sufficient visibility
      if (functionParentScope->isImported() && !functionEntry->getSpecifiers().isPublic())
        throw err->get(*token->getSymbol(), INSUFFICIENT_VISIBILITY,
                       "Cannot access function/procedure '" + currentVarName + "' due to its private visibility");

      // Search for symbol table of called function/procedure to read arguments
      if (functionEntry->getType().is(TY_FUNCTION)) {
        SymbolTable *functionTable = functionParentScope->getChild(spiceFunc->getSignature());
        assert(functionTable != nullptr);
        // Get return type of called function
        SymbolTableEntry *returnValueEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
        assert(returnValueEntry != nullptr);
        SymbolType returnType = returnValueEntry->getType();
        // Structs from outside the module require more initialization
        if (returnType.is(TY_STRUCT) && scopePath.getCurrentScope()->isImported())
          return initExtStruct(*ctx->start, scopePath.getCurrentScope(), accessScopePrefix + ".", returnType.getSubType());
        lhs = returnType;
      } else {
        lhs = SymbolType(TY_BOOL);
      }
    } else if (tokenType == SpiceParser::DOT) { // Consider member access
      tokenCounter++;                           // Consume dot
      scopePrefix += ".";
      // Visit rhs
      auto *postfixUnary = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(ctx->children[tokenCounter]);
      lhs = visit(postfixUnary).as<SymbolType>();
    } else if (tokenType == SpiceParser::PLUS_PLUS) { // Consider ++ operator
      lhs = opRuleManager->getPostfixPlusPlusResultType(*ctx->atomicExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err, *token->getSymbol());

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (tokenType == SpiceParser::MINUS_MINUS) { // Consider -- operator
      lhs = opRuleManager->getPostfixMinusMinusResultType(*ctx->atomicExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err, *token->getSymbol());

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    }
    tokenCounter++; // Consume token
  }

  if (lhs.is(TY_INVALID))
    throw err->get(*ctx->start, REFERENCED_UNDEFINED_VARIABLE,
                   "Variable '" + currentVarName + "' was referenced before declared");

  return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  if (ctx->value())
    return visit(ctx->value());
  if (ctx->IDENTIFIER()) {
    currentVarName = ctx->IDENTIFIER()->toString();
    std::string oldScopePrefix = scopePrefix;
    scopePrefix += currentVarName;

    // Check if this is a reserved keyword
    if (std::find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), currentVarName) != RESERVED_KEYWORDS.end())
      throw err->get(*ctx->start, RESERVED_KEYWORD,
                     "'" + currentVarName +
                         "' is a reserved keyword for future development of the language. Please use another identifier instead");

    // Load symbol table entry
    SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
    assert(accessScope != nullptr);
    SymbolTableEntry *entry = currentEntry = accessScope->lookup(currentVarName);

    // Check if symbol exists. If it does not exist, just return because it could be the function name of a function call
    // The existence of the variable is checked in the visitPostfixUnaryExpr method.
    if (!entry)
      return SymbolType(TY_INVALID);

    if (accessScope->isImported()) {
      // Check if the entry is public if it is imported
      if (!entry->getSpecifiers().isPublic())
        throw err->get(*ctx->IDENTIFIER()->getSymbol(), INSUFFICIENT_VISIBILITY,
                       "Cannot access '" + currentVarName + "' due to its private visibility");

      // Check if the entry is an external global variable and needs to be imported
      if (entry->isGlobal() && !entry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
        initExtGlobal(*ctx->IDENTIFIER()->getSymbol(), accessScope, oldScopePrefix, entry->getName());
    }

    // Set symbol to used
    entry->setUsed();

    // Retrieve scope for the new scope path fragment
    SymbolTable *newAccessScope = accessScope;
    if (entry->getType().is(TY_IMPORT)) { // Import
      newAccessScope = accessScope->lookupTable(entry->getName());
    } else if (entry->getType().isBaseType(TY_STRUCT)) { // Struct
      newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + entry->getType().getBaseType().getSubType());
      currentThisType = entry->getType();
    }
    assert(newAccessScope != nullptr);

    // Otherwise, push the retrieved scope to the scope path
    scopePath.pushFragment(currentVarName, newAccessScope);

    return entry->getType();
  }
  if (ctx->builtinCall())
    return visit(ctx->builtinCall());
  return visit(ctx->assignExpr());
}

antlrcpp::Any AnalyzerVisitor::visitValue(SpiceParser::ValueContext *ctx) {
  if (ctx->primitiveValue())
    return visit(ctx->primitiveValue());

  if (ctx->NIL()) {
    SymbolType nilType = visit(ctx->dataType()).as<SymbolType>();
    if (nilType.is(TY_DYN))
      throw err->get(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA, "Nil must have an explicit type");
    return nilType;
  }

  if (!ctx->IDENTIFIER().empty()) { // Struct instantiation
    // Retrieve fully qualified struct name and the scope where to search it
    std::string accessScopePrefix;
    std::string structName;
    SymbolTable *structScope = currentScope;
    for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
      structName = ctx->IDENTIFIER()[i]->toString();
      if (i < ctx->IDENTIFIER().size() - 1) {
        accessScopePrefix += structName + ".";
        SymbolTableEntry *entry = structScope->lookup(structName);
        if (!entry)
          throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                         "Struct '" + accessScopePrefix + structName + "' was used before defined");
        // Check the type of the symbol table entry
        if (entry->getType().is(TY_IMPORT)) {
          structScope = structScope->lookupTable(structName);
        } else if (entry->getType().is(TY_STRUCT)) {
          structScope = structScope->lookupTable(STRUCT_SCOPE_PREFIX + structName);
        } else {
          throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                         "The variable '" + structName + "' is of type " + entry->getType().getName(false) +
                             ". Expected struct or import");
        }
      }
    }

    // Check if a symbol is existing with that fully qualified name
    SymbolTableEntry *structSymbol = currentScope->lookup(accessScopePrefix + structName);
    if (!structSymbol) { // Not found
      // Trigger an external struct initialization which loads the struct from another source file and modifies the
      // symbol table accordingly
      initExtStruct(*ctx->IDENTIFIER()[0]->getSymbol(), structScope, accessScopePrefix, ctx->IDENTIFIER().back()->toString());
      // Reload the struct symbol
      structSymbol = currentScope->lookup(accessScopePrefix + structName);
    }

    // Check if the symbol is of the expected struct type
    if (!structSymbol->getType().is(TY_STRUCT, accessScopePrefix + structName))
      throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                     "Struct '" + accessScopePrefix + structName + "' was used before defined");
    structSymbol->setUsed();

    // Check if the number of fields matches
    SymbolTable *structTable = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + accessScopePrefix + structName);
    if (ctx->argLst()) { // Check if any fields are passed. Empty braces are also allowed
      if (structTable->getFieldCount() != ctx->argLst()->assignExpr().size())
        throw err->get(*ctx->argLst()->start, NUMBER_OF_FIELDS_NOT_MATCHING,
                       "You've passed too less/many field values. Pass either none or all of them");

      // Check if the field types are matching
      for (int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
        // Get actual type
        auto ternary = ctx->argLst()->assignExpr()[i];
        SymbolType actualType = visit(ternary).as<SymbolType>();
        // Get expected type
        SymbolTableEntry *expectedField = structTable->lookupByIndex(i);
        assert(expectedField != nullptr);
        SymbolType expectedFieldType = expectedField->getType();
        // Check if type matches declaration
        if (actualType != expectedFieldType)
          throw err->get(*ternary->start, FIELD_TYPE_NOT_MATCHING,
                         "Expected type " + expectedFieldType.getName(false) + " for the field '" + expectedField->getName() +
                             "', but got " + actualType.getName(false));
      }
    }

    return structSymbol->getType();
  }

  if (ctx->LBRACE()) { // Array initialization
    // Check if all values have the same type
    assert(expectedType.isArray() || expectedType.is(TY_DYN));
    SymbolType expectedItemType = expectedType.isArray() ? expectedType.getContainedTy() : expectedType;
    unsigned int actualSize = 0;
    if (ctx->argLst()) {
      for (unsigned int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
        SymbolType itemType = visit(ctx->argLst()->assignExpr()[i]).as<SymbolType>();
        if (expectedItemType.is(TY_DYN)) {
          expectedItemType = itemType;
        } else if (itemType != expectedItemType) {
          throw err->get(*ctx->argLst()->assignExpr()[i]->start, ARRAY_ITEM_TYPE_NOT_MATCHING,
                         "All provided values have to be of the same data type. You provided " + expectedItemType.getName(false) +
                             " and " + itemType.getName(false));
        }
        actualSize++;
      }
    } else if (expectedType.is(TY_DYN)) { // Not enough info to perform type inference, because of empty array {}
      throw err->get(*ctx->LBRACE()->getSymbol(), UNEXPECTED_DYN_TYPE_SA, "Not enough information to perform type inference");
    }
    return expectedItemType.toArray(err, *ctx->LBRACE()->getSymbol(), actualSize);
  }

  return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) {
  if (ctx->DOUBLE())
    return SymbolType(TY_DOUBLE);
  if (ctx->INTEGER())
    return SymbolType(TY_INT);
  if (ctx->SHORT())
    return SymbolType(TY_SHORT);
  if (ctx->LONG())
    return SymbolType(TY_LONG);
  if (ctx->CHAR_LITERAL())
    return SymbolType(TY_CHAR);
  if (ctx->STRING_LITERAL())
    return SymbolType(TY_STRING);
  return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) {
  SymbolType type = visit(ctx->baseDataType()).as<SymbolType>();

  unsigned int tokenCounter = 1;
  while (tokenCounter < ctx->children.size()) {
    auto *token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
    if (token->getSymbol()->getType() == SpiceParser::MUL) { // Consider de-referencing operators
      type = type.toPointer(err, *ctx->start);
    } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider array bracket pairs
      tokenCounter++;                                                    // Consume LBRACKET
      token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
      unsigned int size = 0;                                       // Default to 0 when no size is attached
      if (token->getSymbol()->getType() == SpiceParser::INTEGER) { // Size is attached
        int signedSize = std::stoi(token->toString());
        // Check if size >1
        if (signedSize <= 1)
          throw err->get(*token->getSymbol(), ARRAY_SIZE_INVALID, "The size of an array must be > 1");
        size = signedSize;
        tokenCounter++; // Consume INTEGER
      }
      type = type.toArray(err, *ctx->start, size);
    }
    tokenCounter++;
  }

  return type;
}

antlrcpp::Any AnalyzerVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  if (ctx->TYPE_DOUBLE())
    return SymbolType(TY_DOUBLE);
  if (ctx->TYPE_INT())
    return SymbolType(TY_INT);
  if (ctx->TYPE_SHORT())
    return SymbolType(TY_SHORT);
  if (ctx->TYPE_LONG())
    return SymbolType(TY_LONG);
  if (ctx->TYPE_BYTE())
    return SymbolType(TY_BYTE);
  if (ctx->TYPE_CHAR())
    return SymbolType(TY_CHAR);
  if (ctx->TYPE_STRING())
    return SymbolType(TY_STRING);
  if (ctx->TYPE_BOOL())
    return SymbolType(TY_BOOL);
  if (!ctx->IDENTIFIER().empty()) { // Struct or generic type
    // Check if it is a generic type
    std::string firstFragment = ctx->IDENTIFIER()[0]->toString();
    SymbolTableEntry *entry = currentScope->lookup(firstFragment);
    if (ctx->IDENTIFIER().size() == 1 && entry && entry->getType().is(TY_GENERIC))
      return SymbolType(TY_GENERIC, firstFragment);

    // It is a struct type -> get the access scope
    SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

    // Get type name in format: a.b.c and retrieve the scope in parallel
    std::string accessScopePrefix;
    std::string structName;
    bool structIsImported = false;
    for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
      structName = ctx->IDENTIFIER()[i]->toString();
      if (i < ctx->IDENTIFIER().size() - 1)
        accessScopePrefix += structName + ".";
      SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
      if (!symbolEntry)
        throw err->get(*ctx->IDENTIFIER()[0]->getSymbol(), UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");

      std::string tableName = symbolEntry->getType().is(TY_IMPORT) ? structName : STRUCT_SCOPE_PREFIX + structName;
      accessScope = accessScope->lookupTable(tableName);
      if (accessScope->isImported())
        structIsImported = true;
    }

    if (structIsImported) { // Imported struct
      return initExtStruct(*ctx->start, accessScope, accessScopePrefix, structName);
    } else { // Struct in same source file
      // Check if struct was declared
      SymbolTableEntry *structSymbol = accessScope->lookup(structName);
      if (!structSymbol)
        throw err->get(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
      structSymbol->setUsed();
      return SymbolType(TY_STRUCT, structName);
    }
  }
  return SymbolType(TY_DYN);
}

SymbolType AnalyzerVisitor::initExtStruct(const antlr4::Token &token, SymbolTable *sourceScope,
                                          const std::string &structScopePrefix, const std::string &structName) {
  // Get external struct name
  std::string newStructName = structScopePrefix + structName;

  // Check if the struct is imported already
  SymbolTableEntry *newStructSymbol = sourceScope->lookup(newStructName);
  if (newStructSymbol)
    return newStructSymbol->getType();

  // Check if external struct is declared
  SymbolTableEntry *externalStructSymbol = sourceScope->lookup(structName);
  if (!externalStructSymbol)
    throw err->get(token, REFERENCED_UNDEFINED_STRUCT, "Could not find struct '" + newStructName + "'");
  externalStructSymbol->setUsed();

  // Get the associated symbolTable of the external struct symbol
  SymbolTable *externalStructTable = sourceScope->lookupTable(STRUCT_SCOPE_PREFIX + structName);

  // Initialize potential structs for field types
  for (auto &symbol : externalStructTable->getSymbols()) {
    if (symbol.second.getType().isBaseType(TY_STRUCT)) {
      std::string nestedStructName = symbol.second.getType().getBaseType().getSubType();
      // Initialize nested struct
      initExtStruct(token, sourceScope, structScopePrefix, nestedStructName);
      // Re-map type of field to the imported struct
      SymbolType newNestedStructType = symbol.second.getType().replaceSubType(structScopePrefix + nestedStructName);
      symbol.second.updateType(newNestedStructType, true);
      // Set LLVM type of nested struct to the field
      SymbolTableEntry *nestedStructEntry = rootScope->lookup(structScopePrefix + nestedStructName);
      assert(nestedStructEntry != nullptr);
      symbol.second.updateLLVMType(nestedStructEntry->getLLVMType());
    }
  }

  // Copy struct symbol and struct table to the root scope of the current source file
  SymbolType newStructTy = SymbolType(TY_STRUCT, newStructName);

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  rootScope->insert(newStructName, newStructTy, SymbolSpecifiers(newStructTy), DECLARED,
                    externalStructSymbol->getDefinitionToken());
  newStructSymbol = rootScope->lookup(newStructName);
  newStructSymbol->updateLLVMType(externalStructSymbol->getLLVMType());
  newStructSymbol->setUsed();

  // Mount the external struct table to the new position in the root scope of the current source file
  externalStructTable->setImported();
  rootScope->mountChildBlock(STRUCT_SCOPE_PREFIX + newStructName, externalStructTable);

  return newStructTy;
}

SymbolType AnalyzerVisitor::initExtGlobal(const antlr4::Token &token, SymbolTable *sourceScope,
                                          const std::string &globalScopePrefix, const std::string &globalName) {
  // Get external global var name
  std::string newGlobalName = globalScopePrefix + globalName;

  // Check if the global var is imported already
  SymbolTableEntry *newGlobalSymbol = sourceScope->lookup(newGlobalName);
  if (newGlobalSymbol)
    return newGlobalSymbol->getType();

  // Check if external global var is declared
  SymbolTableEntry *externalGlobalSymbol = sourceScope->lookup(globalName);
  if (!externalGlobalSymbol)
    throw err->get(token, REFERENCED_UNDEFINED_STRUCT, "Could not find global variable '" + newGlobalName + "'");
  externalGlobalSymbol->setUsed();
  SymbolType globalTy = externalGlobalSymbol->getType();

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  rootScope->insert(newGlobalName, globalTy, SymbolSpecifiers(globalTy), DECLARED, externalGlobalSymbol->getDefinitionToken());
  newGlobalSymbol = rootScope->lookup(newGlobalName);
  newGlobalSymbol->updateLLVMType(externalGlobalSymbol->getLLVMType());
  newGlobalSymbol->setUsed();

  return globalTy;
}