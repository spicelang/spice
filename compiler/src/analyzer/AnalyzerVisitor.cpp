// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

antlrcpp::Any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext* ctx) {
    // Pre-traversing actions

    // Traverse AST
    visitChildren(ctx);

    // Check if the visitor got a main function
    if (mustHaveMainFunction && !hasMainFunction)
        throw SemanticError(*ctx->start, MISSING_MAIN_FUNCTION, "No main function found");

    // Post traversing actions

    // Return the symbol table to the main program for further compile phases
    return currentScope;
}

antlrcpp::Any AnalyzerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    std::string mainSignature = "main()";
    // Check if the function is already defined
    if (currentScope->lookup(mainSignature)) {
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE,
                            "Main function is declared twice");
    }
    // Insert function name into the root symbol table
    currentScope->insert(mainSignature, SymbolType(TYPE_FUNCTION), INITIALIZED, true, false);
    // Create a new scope
    currentScope = currentScope->createChildBlock("main()");
    // Declare variable for the return value
    SymbolType returnType = SymbolType(TYPE_INT);
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, INITIALIZED, false, false);
    // Visit parameters
    parameterMode = true;
    if (ctx->paramLstDef()) visit(ctx->paramLstDef());
    parameterMode = false;
    // Visit statements in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    // Confirm main function
    hasMainFunction = true;
    return returnType;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext* ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit params in new scope
    parameterMode = true;
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLstDef())
        paramTypes = visit(ctx->paramLstDef()).as<std::vector<SymbolType>>();
    parameterMode = false;
    // Declare variable for the return value in new scope
    SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, DECLARED, false, false);
    // Return to old scope
    currentScope = currentScope->getParent();
    // Insert function into the symbol table
    FunctionSignature signature = FunctionSignature(functionName, paramTypes);
    // Check if the function is already defined
    if (currentScope->lookup(signature.toString())) {
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE,
                            "Function '" + signature.toString() + "' is declared twice");
    }
    currentScope->insert(signature.toString(), SymbolType(TYPE_FUNCTION), INITIALIZED, true, false);
    currentScope->pushSignature(signature);
    // Rename function scope block to support function overloading
    currentScope->renameChildBlock(scopeId, signature.toString());
    // Go down again in scope
    currentScope = currentScope->getChild(signature.toString());
    // Visit statements in new scope
    visit(ctx->stmtLst());
    // Check if return variable is now initialized
    if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
        throw SemanticError(*ctx->start, FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");
    // Return to old scope
    currentScope = currentScope->getParent();
    return returnType;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    std::string procedureName = ctx->IDENTIFIER()->toString();
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit params in new scope
    parameterMode = true;
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLstDef())
        paramTypes = visit(ctx->paramLstDef()).as<std::vector<SymbolType>>();
    parameterMode = false;
    // Return to old scope
    currentScope = currentScope->getParent();
    // Insert procedure into the symbol table
    FunctionSignature signature = FunctionSignature(procedureName, paramTypes);
    // Check if the procedure is already defined
    if (currentScope->lookup(signature.toString())) {
        throw SemanticError(*ctx->start, PROCEDURE_DECLARED_TWICE,
                            "Procedure '" + signature.toString() + "' is declared twice");
    }
    currentScope->insert(signature.toString(), SymbolType(TYPE_PROCEDURE), INITIALIZED, true, false);
    currentScope->pushSignature(signature);
    // Rename function scope block to support function overloading
    currentScope->renameChildBlock(scopeId, signature.toString());
    // Go down again in scope
    currentScope = currentScope->getChild(signature.toString());
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitStructDef(SpiceParser::StructDefContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();
    // Check if struct already exists in this scope
    if (currentScope->lookup(structName))
        throw SemanticError(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate struct '" + structName + "'");
    // Create a new table entry for the struct
    currentScope->insert(structName, SymbolType(TYPE_STRUCT, structName), DECLARED, true, false);
    // Visit field list in a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    visit(ctx->fieldLst());
    // Return to the old scope
    currentScope = currentScope->getParent();
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit assignment in new scope
    visit(ctx->assignExpr()[0]);
    // Visit condition in new scope
    SymbolType conditionType = visit(ctx->assignExpr()[1]).as<SymbolType>();
    if (!conditionType.is(TYPE_BOOL))
        throw SemanticError(*ctx->assignExpr()[1]->start, CONDITION_MUST_BE_BOOL,
                            "For loop condition must be of type bool");
    // Visit incrementer in new scope
    visit(ctx->assignExpr()[2]);
    // Visit statement list in new scope
    nestedLoopCounter++;
    visit(ctx->stmtLst());
    nestedLoopCounter--;
    // Return to old scope
    currentScope = currentScope->getParent();
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
    if (!conditionType.is(TYPE_BOOL))
        throw SemanticError(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL,
                            "While loop condition must be of type bool");
    // Visit statement list in new scope
    nestedLoopCounter++;
    visit(ctx->stmtLst());
    nestedLoopCounter--;
    // Return to old scope
    currentScope = currentScope->getParent();
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
    if (!conditionType.is(TYPE_BOOL))
        throw SemanticError(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL,
                            "If condition must be of type bool");
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    // Visit else statement if it exists
    if (ctx->elseStmt()) visit(ctx->elseStmt());
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitElseStmt(SpiceParser::ElseStmtContext* ctx) {
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
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitParamLstDef(SpiceParser::ParamLstDefContext* ctx) {
    std::vector<SymbolType> paramTypes;
    for (auto& param : ctx->declStmt()) { // Parameters without default value
        SymbolType paramType = visit(param).as<SymbolType>();
        std::string paramName = param->IDENTIFIER()->toString();
        if (paramType.is(TYPE_DYN))
            throw SemanticError(*param->start, FCT_PARAM_IS_TYPE_DYN,
                                "Type of parameter '" + paramName + "' is invalid");
        paramTypes.push_back(paramType);
    }
    for (auto& param : ctx->assignExpr()) { // Parameters with default value
        SymbolType paramType = visit(param).as<SymbolType>();
        paramTypes.push_back(paramType);
    }
    return paramTypes;
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext* ctx) {
    std::string variableName = ctx->IDENTIFIER()->toString();
    // Check if symbol already exists in the symbol table
    if (currentScope->lookup(variableName))
        throw SemanticError(*ctx->start, VARIABLE_DECLARED_TWICE,
                            "The variable '" + variableName + "' was declared more than once");
    // Insert variable name to symbol table
    SymbolType type = visit(ctx->dataType()).as<SymbolType>();
    currentScope->insert(variableName, type, DECLARED, ctx->CONST(), parameterMode);
    return type;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext* ctx) {
    std::vector<std::string> functionNamespace;
    for (auto& segment : ctx->IDENTIFIER()) functionNamespace.push_back(segment->toString());
    std::string functionName = functionNamespace.back();
    // Visit params
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLstCall()) {
        for (auto& param : ctx->paramLstCall()->assignExpr())
            paramTypes.push_back(visit(param).as<SymbolType>());
    }
    // Check if function signature exists in symbol table
    FunctionSignature signature = FunctionSignature(functionName, paramTypes);
    functionNamespace.back() = signature.toString();
    SymbolTable* entryTable = currentScope->lookupTableWithSymbol(functionNamespace);
    if (!entryTable)
        throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
                            "Function/Procedure '" + signature.toString() + "' could not be found");
    SymbolTableEntry* entry = entryTable->lookup(signature.toString());
    if (!entry)
        throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
                            "Function/Procedure '" + signature.toString() + "' could not be found");
    // Add function call to the signature queue of the current scope
    currentScope->pushSignature(signature);
    // Search for symbol table of called function/procedure to read parameters
    if (entry->getType().is(TYPE_FUNCTION)) {
        entryTable = entryTable->getChild(signature.toString());
        // Get return type of called function
        return entryTable->lookup(RETURN_VARIABLE_NAME)->getType();
    }
    return SymbolType(TYPE_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitNewStmt(SpiceParser::NewStmtContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();
    std::string structScope = ScopeIdUtil::getScopeId(ctx);
    // Check if the struct is defined
    SymbolTableEntry* structSymbol = currentScope->lookup(structName);
    if (!structSymbol || !structSymbol->getType().is(TYPE_STRUCT) ||
        structSymbol->getType().getSubType() != structName) {
        throw SemanticError(*ctx->IDENTIFIER()->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                            "Struct '" + structName + "' was used before defined");
    }

    // Get the symbol table where the struct is defined
    SymbolTable* structTable = currentScope->lookupTable(structScope);
    // Check if the number of fields matches
    if (structTable->getSymbolsCount() != ctx->fieldLstAssignment()->ternaryExpr().size())
        throw SemanticError(*ctx->start, NUMBER_OF_FIELDS_NOT_MATCHING, "You've passed too less/many field values");

    // Check if the field types are matching
    for (int i = 0; i < ctx->fieldLstAssignment()->ternaryExpr().size(); i++) {
        // Get actual type
        auto ternary = ctx->fieldLstAssignment()->ternaryExpr()[i];
        SymbolType actualType = visit(ternary).as<SymbolType>();
        // Get expected type
        SymbolTableEntry* expectedField = structTable->lookupByIndexInCurrentScope(i);
        SymbolType expectedType = expectedField->getType();
        // Check if type matches declaration
        if (actualType != expectedType)
            throw SemanticError(*ternary->start, FIELD_TYPE_NOT_MATCHING,
                                "Expected type " + expectedType.getName() + " for the field '" +
                                expectedField->getName() + "', but got " + actualType.getName());
    }

    return SymbolType(TYPE_STRUCT, structName);
}

antlrcpp::Any AnalyzerVisitor::visitArrayInitStmt(SpiceParser::ArrayInitStmtContext* ctx) {
    // Visit data type
    SymbolType dataType = visit(ctx->dataType()).as<SymbolType>();
    SymbolType indexType = visit(ctx->value()).as<SymbolType>();

    // Check if index type is an integer
    if (!indexType.is(TYPE_INT))
        throw SemanticError(*ctx->value()->start, ARRAY_SIZE_NO_INTEGER, "The size must be an integer, provided " +
                            indexType.getName());

    return dataType.getArrayType();
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    // Check if imported library exists
    std::string importPath = ctx->STRING()->toString();
    importPath = importPath.substr(1, importPath.size() - 2);

    // Check if source file exists
    std::string filePath;
    if (importPath.rfind("std/", 0) == 0) { // Include source file from standard library
        std::string sourceFileIden = importPath.substr(importPath.find("std/") + 4);
        // Find std library
        std::string stdPath;
        if (FileUtil::fileExists("/usr/lib/spice/std")) {
            stdPath = "/usr/lib/spice/std/";
        } else if (FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
            stdPath = std::string(std::getenv("SPICE_STD_DIR"));
            if (stdPath.rfind('/') != stdPath.size() - 1) stdPath += "/";
        } else {
            throw SemanticError(STD_NOT_FOUND,
                                "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
        }
        // Check if source file exists
        if (FileUtil::fileExists(stdPath + sourceFileIden + ".spice")) {
            filePath = stdPath + sourceFileIden + ".spice";
        } else if (FileUtil::fileExists(stdPath + sourceFileIden + "_" + targetOs + ".spice")) {
            filePath = stdPath + sourceFileIden + "_" + targetOs + ".spice";
        } else if (FileUtil::fileExists(stdPath + sourceFileIden + "_" + targetOs + "_" + targetArch + ".spice")) {
            filePath = stdPath + sourceFileIden + "_" + targetOs + "_" + targetArch + ".spice";
        } else {
            throw SemanticError(IMPORTED_FILE_NOT_EXISTING, "The source file '" + importPath +
                                                            ".spice' was not found in std library");
        }
    } else { // Include own source file
        // Check in module registry if the file can be imported
        std::string sourceFileDir = FileUtil::getFileDir(mainSourceFile);
        ModuleRegistry* registry = ModuleRegistry::getInstance();
        registry->addModule(sourceFileDir + "/" + importPath);
        // Import file
        if (FileUtil::fileExists(sourceFileDir + "/" + importPath + ".spice")) {
            filePath = sourceFileDir + "/" + importPath + ".spice";
        } else if (FileUtil::fileExists(sourceFileDir + "/" + importPath + "_" + targetOs + ".spice")) {
            filePath = sourceFileDir + "/" + importPath + "_" + targetOs + ".spice";
        } else if (FileUtil::fileExists(sourceFileDir + "/" + importPath + "_" + targetOs + "_" + targetArch + ".spice")) {
            filePath = sourceFileDir + "/" + importPath + "_" + targetOs + "_" + targetArch + ".spice";
        } else {
            throw SemanticError(IMPORTED_FILE_NOT_EXISTING, "The source file '" + importPath +
                                                            ".spice' does not exist");
        }
    }

    // Kick off the compilation of the imported source file
    SymbolTable* nestedTable = CompilerInstance::CompileSourceFile(filePath, targetArch, targetVendor, targetOs, objectDir,
                                                                   debugOutput, optLevel, false);

    // Create symbol of type TYPE_IMPORT in the current scope
    std::string importIden = ctx->IDENTIFIER()->toString();
    currentScope->insert(importIden, SymbolType(TYPE_IMPORT), INITIALIZED, true, false);

    // Mount symbol table of the imported source file into the current scope
    currentScope->mountChildBlock(importIden, nestedTable);

    return SymbolType(TYPE_STRING);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    SymbolType returnType = visit(ctx->assignExpr()).as<SymbolType>();
    // Check if return variable is in the symbol table
    SymbolTableEntry* returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (!returnVariable)
        throw SemanticError(*ctx->start, RETURN_STMT_WITHOUT_FUNCTION,
                            "Cannot assign return statement to a function");
    // Check data type of return statement
    if (returnVariable->getType().is(TYPE_DYN)) {
        // Set explicit return type to the return variable
        returnVariable->updateType(returnType);
    } else {
        // Check if return type matches with function definition
        if (returnType != returnVariable->getType())
            throw SemanticError(*ctx->assignExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Passed wrong data type to return statement. Expected " + returnType.getName() +
                                " but got " + returnVariable->getType().getName());
    }
    // Set the return variable to initialized
    returnVariable->updateState(INITIALIZED);
    return returnType;
}

antlrcpp::Any AnalyzerVisitor::visitBreakStmt(SpiceParser::BreakStmtContext* ctx) {
    int breakCount = 1;
    if (ctx->INTEGER()) {
        // Check if the stated number is valid
        breakCount = std::stoi(ctx->INTEGER()->toString());
        if (breakCount < 1)
            throw SemanticError(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                                "Break count must be >= 1, you provided " + ctx->INTEGER()->toString());
    }
    // Check if we can break this often
    if (breakCount > nestedLoopCounter)
        throw SemanticError(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                            "We can only break " + std::to_string(nestedLoopCounter) + " time(s) here");
    return SymbolType(TYPE_INT);
}

antlrcpp::Any AnalyzerVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext* ctx) {
    int continueCount = 1;
    if (ctx->INTEGER()) {
        // Check if the stated number is valid
        continueCount = std::stoi(ctx->INTEGER()->toString());
        if (continueCount < 1)
            throw SemanticError(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                                "Continue count must be >= 1, you provided " + ctx->INTEGER()->toString());
    }
    // Check if we can continue this often
    if (continueCount > nestedLoopCounter)
        throw SemanticError(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                            "We can only continue " + std::to_string(nestedLoopCounter) + " time(s) here");
    return SymbolType(TYPE_INT);
}

antlrcpp::Any AnalyzerVisitor::visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) {
    std::string templateString = ctx->STRING()->toString();
    templateString = templateString.substr(1, templateString.size() - 2);

    // Check if assignment types match placeholder types
    std::size_t index = templateString.find_first_of('%');
    int placeholderCount = 0;
    while (index != std::string::npos) {
        auto assignment = ctx->assignExpr()[placeholderCount];
        SymbolType assignmentType = visit(assignment).as<SymbolType>();
        switch (templateString[index + 1]) {
            case 'c':
            case 'd':
            case 'i':
            case 'o':
            case 'x':
            case 'X': {
                if (!assignmentType.isOneOf({ TYPE_INT, TYPE_BOOL }))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects int or bool, but got " + assignmentType.getName());
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
                if (!assignmentType.is(TYPE_DOUBLE))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects double, but got " + assignmentType.getName());
                placeholderCount++;
                break;
            }
            case 's': {
                if (!assignmentType.is(TYPE_STRING))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects string, but got " + assignmentType.getName());
                placeholderCount++;
                break;
            }
            case 'p': {
                if (!assignmentType.isOneOf({ TYPE_DOUBLE_PTR, TYPE_INT_PTR, TYPE_STRING_PTR, TYPE_BOOL_PTR, TYPE_STRUCT_PTR }))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects pointer, but got " + assignmentType.getName());
                placeholderCount++;
                break;
            }
        }
        index = templateString.find_first_of('%', index + 1);
    }

    return SpiceBaseVisitor::visitPrintfStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAssignExpr(SpiceParser::AssignExprContext* ctx) {
    // Take a look on the right side
    SymbolType rhsTy;
    if (ctx->ternaryExpr()) { // Ternary
        rhsTy = visit(ctx->ternaryExpr()).as<SymbolType>();
    } else if (ctx->newStmt()) { // NewStmt
        rhsTy = visit(ctx->newStmt()).as<SymbolType>();
    } else {
        rhsTy = visit(ctx->arrayInitStmt()).as<SymbolType>();
    }

    // Check if there is an assign operator applied
    if (ctx->declStmt() || !ctx->IDENTIFIER().empty()) {
        // Take a look on the left side
        std::string variableName;
        SymbolType lhsTy;
        antlr4::Token* token;
        SymbolTableEntry* symbolTableEntry;
        if (ctx->declStmt()) { // Variable was declared in this line
            visit(ctx->declStmt());
            variableName = ctx->declStmt()->IDENTIFIER()->toString();
            token = ctx->declStmt()->IDENTIFIER()->getSymbol();
            // Get symbol table entry
            symbolTableEntry = currentScope->lookup(variableName);
        } else { // Variable was declared before and is referenced here
            variableName = ctx->IDENTIFIER()[0]->toString();
            token = ctx->IDENTIFIER()[0]->getSymbol();
            variableName = IdentifierUtil::getVarNameFromIdentList(ctx->IDENTIFIER());
            // Get symbol table entry
            symbolTableEntry = IdentifierUtil::getSymbolTableEntryByIdenList(currentScope, ctx->IDENTIFIER());
        }

        lhsTy = symbolTableEntry->getType();

        // If it is a pointer to the value, resolve the type
        if (ctx->MUL()) lhsTy = lhsTy.getScalarType();

        // If it is an array, resolve the type
        if (ctx->LBRACKET()) {
            // Check if the index is an integer
            SymbolType indexType = visit(ctx->value()).as<SymbolType>();
            if (!indexType.is(TYPE_INT))
                throw SemanticError(*ctx->value()->start, ARRAY_SIZE_NO_INTEGER,
                                    "The index must be an integer, provided " + indexType.getName());
            // Resolve type
            lhsTy = lhsTy.getItemType();
        }

        // If left type is dyn, set left type to right type
        if (lhsTy.is(TYPE_DYN)) {
            lhsTy = rhsTy;
            symbolTableEntry->updateType(rhsTy);
        }

        // Take a look at the operator
        if (ctx->ASSIGN_OP()) {
            if (lhsTy != rhsTy)
                throw SemanticError(*ctx->ASSIGN_OP()->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply the assign operator on same data types");
        } else if (ctx->PLUS_EQUAL()) {
            if (!lhsTy.matches(rhsTy, TYPE_DOUBLE) && !lhsTy.matches(rhsTy, TYPE_INT) && !lhsTy.matches(rhsTy, TYPE_STRING))
                throw SemanticError(*ctx->PLUS_EQUAL()->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply '+=' operator on two doubles, two ints or two strings");
        } else if (ctx->MINUS_EQUAL()) {
            if (!lhsTy.matches(rhsTy, TYPE_DOUBLE) && !lhsTy.matches(rhsTy, TYPE_INT))
                throw SemanticError(*ctx->MINUS_EQUAL()->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply '-=' operator on two doubles or two ints");
        } else if (ctx->MUL_EQUAL()) {
            if (!lhsTy.matches(rhsTy, TYPE_DOUBLE) && !lhsTy.matches(rhsTy, TYPE_INT))
                throw SemanticError(*ctx->MUL_EQUAL()->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply '*=' operator on two doubles or two ints");
        } else if (ctx->DIV_EQUAL()) {
            if (!lhsTy.matches(rhsTy, TYPE_DOUBLE) && !lhsTy.matches(rhsTy, TYPE_INT))
                throw SemanticError(*ctx->DIV_EQUAL()->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply '/=' operator on two doubles or two ints");
        }
        // Update variable in symbol table
        symbolTableEntry->updateState(INITIALIZED);
    }

    // Return the rhs type
    return rhsTy;
}

antlrcpp::Any AnalyzerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) {
    // Check if there is a ternary operator applied
    if (ctx->children.size() > 1) {
        auto condition = ctx->logicalOrExpr()[0];
        SymbolType conditionType = visit(condition).as<SymbolType>();
        SymbolType trueType = visit(ctx->logicalOrExpr()[1]).as<SymbolType>();
        SymbolType falseType = visit(ctx->logicalOrExpr()[2]).as<SymbolType>();
        // Check if the condition evaluates to boolean
        if (!conditionType.is(TYPE_BOOL))
            throw SemanticError(*condition->start, OPERATOR_WRONG_DATA_TYPE,
                                "Condition operand in ternary must be a bool");
        // Check if trueType and falseType are matching
        if (trueType != falseType)
            throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                                "True and false operands in ternary must be of same data type");
        return trueType;
    }
    return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) {
    // Check if a logical or operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->logicalAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->logicalAndExpr()[i]).as<SymbolType>();
            // Allow logical or operator for booleans
            if (lhsTy.matches(rhsTy, TYPE_BOOL)) {
                lhsTy = SymbolType(TYPE_BOOL);
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,"Can only apply '||' operator to booleans");
        }
        return lhsTy;
    }
    return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) {
    // Check if a logical and operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->bitwiseOrExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->bitwiseOrExpr()[i]).as<SymbolType>();
            // Allow logical and operator for booleans
            if (lhsTy.matches(rhsTy, TYPE_BOOL)) {
                lhsTy = SymbolType(TYPE_BOOL);
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE, "Can only apply '&&' operator to booleans");
        }
        return lhsTy;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    // Check if a bitwise or operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->bitwiseAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->bitwiseAndExpr()[i]).as<SymbolType>();
            // Allow bitwise or operator for booleans
            if (lhsTy.matches(rhsTy, TYPE_BOOL)) {
                lhsTy = SymbolType(TYPE_BOOL);
                continue;
            }
            // Allow bitwise or operator for integers
            if (lhsTy.matches(rhsTy, TYPE_INT)) {
                lhsTy = SymbolType(TYPE_INT);
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                                "Can only apply '|' operator to booleans and integers");
        }
        return lhsTy;
    }
    return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) {
    // Check if a bitwise and operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->equalityExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->equalityExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->equalityExpr()[i]).as<SymbolType>();
            // Allow bitwise and operator for booleans
            if (lhsTy.matches(rhsTy, TYPE_BOOL)) {
                lhsTy = SymbolType(TYPE_BOOL);
                continue;
            }
            // Allow bitwise and operator for integers
            if (lhsTy.matches(rhsTy, TYPE_INT)) {
                lhsTy = SymbolType(TYPE_INT);
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                                "Can only apply '&' operator to booleans and integers");
        }
        return lhsTy;
    }
    return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) {
    // Check if at least one equality operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->relationalExpr()[0]).as<SymbolType>();
        SymbolType rhsTy = visit(ctx->relationalExpr()[1]).as<SymbolType>();
        if (lhsTy.matches(rhsTy, TYPE_DOUBLE)) return SymbolType(TYPE_BOOL); // Can compare double with double
        if (lhsTy.is(TYPE_DOUBLE) && rhsTy.is(TYPE_INT)) return SymbolType(TYPE_BOOL); // Can compare double with int
        if (lhsTy.is(TYPE_INT) && rhsTy.is(TYPE_DOUBLE)) return SymbolType(TYPE_BOOL); // Can compare int with double
        if (lhsTy.matches(rhsTy, TYPE_INT)) return SymbolType(TYPE_BOOL); // Can compare int with int
        if (lhsTy.matches(rhsTy, TYPE_STRING)) return SymbolType(TYPE_BOOL); // Can compare string with string
        if (lhsTy.matches(rhsTy, TYPE_BOOL)) return SymbolType(TYPE_BOOL); // Can compare bool with bool
        // Every other combination is invalid
        throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                            "Can't compare " + lhsTy.getName() + " and " + rhsTy.getName() +
                            " with '==' or '!=' operators");
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) {
    // Check if at least one relational operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->additiveExpr()[0]).as<SymbolType>();
        SymbolType rhsTy = visit(ctx->additiveExpr()[1]).as<SymbolType>();
        if (lhsTy.matches(rhsTy, TYPE_DOUBLE))
            return SymbolType(TYPE_BOOL); // Can compare double with double
        if (lhsTy.is(TYPE_DOUBLE) && rhsTy.is(TYPE_INT))
            return SymbolType(TYPE_BOOL); // Can compare double with int
        if (lhsTy.is(TYPE_INT) && rhsTy.is(TYPE_DOUBLE))
            return SymbolType(TYPE_BOOL); // Can compare int with double
        if (lhsTy.matches(rhsTy, TYPE_INT))
            return SymbolType(TYPE_BOOL); // Can compare int with int
        // Every other combination is invalid
        throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                            "Can only compare doubles or ints with one another with a relational operator");
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) {
    // Check if at least one additive operator is applied
    if (ctx->multiplicativeExpr().size() > 1) {
        SymbolType currentType = visit(ctx->multiplicativeExpr()[0]).as<SymbolType>();
        // Check if data types are compatible
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            auto next = ctx->multiplicativeExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (op == ctx->getToken(SpiceParser::PLUS, 0)) { // Operator was plus
                // Check all combinations
                if (currentType.is(TYPE_DOUBLE)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4.3 + 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4.3 + 4
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4.3 + "Test"
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4.3 + true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and bool for '+' operator");
                    }
                } else if (currentType.is(TYPE_INT)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4 + 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4 + 5
                        currentType = SymbolType(TYPE_INT);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4 + "Test"
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4 + true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and bool for '+' operator");
                    }
                } else if (currentType.is(TYPE_STRING)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: "Test" + 6.1
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: "Test" + 5
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: "Test" + "Test"
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: "Test" + true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands string and bool for '+' operator");
                    }
                } else if (currentType.is(TYPE_BOOL)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and " + nextType.getName() + " for '+' operator");
                }
            } else { // Operator was minus
                // Check all combinations
                if (currentType.is(TYPE_DOUBLE)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4.3 - 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4.3 - 4
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4.3 - "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and string for '-' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4.3 - true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and bool for '-' operator");
                    }
                } else if (currentType.is(TYPE_INT)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4 - 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4 - 5
                        currentType = SymbolType(TYPE_INT);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4 - "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and string for '-' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4 - true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and bool for '-' operator");
                    }
                } else if (currentType.is(TYPE_STRING)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and " + nextType.getName() + " for '-' operator");
                } else if (currentType.is(TYPE_BOOL)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and " + nextType.getName() + " for '-' operator");
                }
            }
            operatorIndex += 2;
        }
        return currentType;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    // Check if at least one multiplicative operator is applied
    if (ctx->prefixUnaryExpr().size() > 1) {
        SymbolType currentType = visit(ctx->prefixUnaryExpr()[0]).as<SymbolType>();
        // Check if data types are compatible
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->prefixUnaryExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            auto next = ctx->prefixUnaryExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (op == ctx->getToken(SpiceParser::MUL, 0)) { // Operator was mul
                if (currentType.is(TYPE_DOUBLE)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4.3 * 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4.3 * 4
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4.3 * "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and string for '*' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4.3 * true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and bool for '*' operator");
                    }
                } else if (currentType.is(TYPE_INT)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4 * 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4 * 5
                        currentType = SymbolType(TYPE_INT);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4 * "Test"
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4 * true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and bool for '*' operator");
                    }
                } else if (currentType.is(TYPE_STRING)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: "Test" * 6.1
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands string and double for '*' operator");
                    } else if (nextType.is(TYPE_INT)) { // e.g.: "Test" * 5
                        currentType = SymbolType(TYPE_STRING);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: "Test" * "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands string and string for '*' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: "Test" * true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands string and bool for '*' operator");
                    }
                } else if (currentType.is(TYPE_BOOL)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and " + nextType.getName() + " for '*' operator");
                }
            } else { // Operator was a div
                if (currentType.is(TYPE_DOUBLE)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4.3 / 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4.3 / 4
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4.3 / "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and string for '/' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4.3 / true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands double and bool for '/' operator");
                    }
                } else if (currentType.is(TYPE_INT)) {
                    if (nextType.is(TYPE_DOUBLE)) { // e.g.: 4 / 6.1
                        currentType = SymbolType(TYPE_DOUBLE);
                    } else if (nextType.is(TYPE_INT)) { // e.g.: 4 / 5
                        currentType = SymbolType(TYPE_INT);
                    } else if (nextType.is(TYPE_STRING)) { // e.g.: 4 / "Test"
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and string for '/' operator");
                    } else if (nextType.is(TYPE_BOOL)) { // e.g.: 4 / true
                        throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                            "Incompatible operands int and bool for '/' operator");
                    }
                } else if (currentType.is(TYPE_STRING)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and " + nextType.getName() + " for '/' operator");
                } else if (currentType.is(TYPE_BOOL)) {
                    throw SemanticError(*next->start, OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and " + nextType.getName() + " for '/' operator");
                }
            }
            operatorIndex += 2;
        }
        return currentType;
    }
    return visit(ctx->prefixUnaryExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    antlrcpp::Any prefixUnary = visit(ctx->postfixUnaryExpr());

    // Ensure integer when '++' or '--' is applied
    if (ctx->PLUS_PLUS() || ctx->MINUS_MINUS()) {
        if (!prefixUnary.as<SymbolType>().is(TYPE_INT))
            throw SemanticError(*ctx->postfixUnaryExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Prefix '++' or '--' can only be applied to an identifier of type integer");
    }

    // Ensure right return type if not is applied
    if (ctx->NOT()) {
        /* If not applied to double, return bool (evaluates later to variable == 0.0)
           If not applied to int, return bool (evaluates later to variable == 0)
           If not applied to string, return bool (evaluates later to variable == "")
           If not applied to bool, return bool (evaluates later to variable == false)*/
        return SymbolType(TYPE_BOOL);
    }

    return prefixUnary;
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    antlrcpp::Any atomicExpr = visit(ctx->atomicExpr());

    // Ensure integer when '++' or '--' is applied
    if (ctx->PLUS_PLUS() || ctx->MINUS_MINUS()) {
        if (!atomicExpr.as<SymbolType>().is(TYPE_INT))
            throw SemanticError(*ctx->atomicExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Postfix '++' or '--' can only be applied to an identifier of type integer");
    }

    return atomicExpr;
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->assignExpr()) return visit(ctx->assignExpr());
    return visit(ctx->value());
}

antlrcpp::Any AnalyzerVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    if (ctx->DOUBLE()) return SymbolType(TYPE_DOUBLE);
    if (ctx->INTEGER()) return SymbolType(TYPE_INT);
    if (ctx->STRING()) return SymbolType(TYPE_STRING);
    if (ctx->TRUE() || ctx->FALSE()) return SymbolType(TYPE_BOOL);
    if (!ctx->IDENTIFIER().empty()) {
        std::string variableName = IdentifierUtil::getVarNameFromIdentList(ctx->IDENTIFIER());
        // Get symbol table entry
        SymbolTableEntry* entry = IdentifierUtil::getSymbolTableEntryByIdenList(currentScope, ctx->IDENTIFIER());
        SymbolType valueType = entry->getType();
        // Consider referencing operator
        if (ctx->BITWISE_AND()) valueType = valueType.getPointerType();
        // Consider de-referencing operator
        if (ctx->MUL()) valueType = valueType.getScalarType();
        // Consider array brackets
        if (ctx->LBRACKET()) {
            // Check if value is integer
            SymbolType indexType = visit(ctx->value()).as<SymbolType>();
            if (!indexType.is(TYPE_INT))
                throw SemanticError(*ctx->value()->start, ARRAY_SIZE_NO_INTEGER,
                                    "The index must be an integer, provided " + indexType.getName());
            // Forward the item type
            valueType = valueType.getItemType();
        }
        return valueType;
    }
    return visit(ctx->functionCall());
}

antlrcpp::Any AnalyzerVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    SymbolType type = SymbolType(TYPE_DYN);

    if (ctx->TYPE_DOUBLE()) type = SymbolType(TYPE_DOUBLE);
    if (ctx->TYPE_INT()) type = SymbolType(TYPE_INT);
    if (ctx->TYPE_STRING()) type = SymbolType(TYPE_STRING);
    if (ctx->TYPE_BOOL()) type = SymbolType(TYPE_BOOL);
    if (ctx->IDENTIFIER()) { // Struct type
        std::string structName = ctx->IDENTIFIER()->toString();

        // Check if struct was declared
        SymbolTableEntry* structSymbol = currentScope->lookup(structName);
        if (!structSymbol)
            throw SemanticError(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");

        type = SymbolType(TYPE_STRUCT, structName);
    }

    // Check for de-referencing operator
    if (ctx->MUL()) type = type.getPointerType();
    // Check for array brackets
    if (ctx->LBRACKET()) type = type.getArrayType();

    return type;
}