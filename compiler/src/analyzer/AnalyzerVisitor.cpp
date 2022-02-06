// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

#include <CompilerInstance.h>
#include <analyzer/OpRuleManager.h>
#include <symbol/SymbolSpecifiers.h>
#include <util/ModuleRegistry.h>
#include <util/ScopeIdUtil.h>
#include <util/FileUtil.h>
#include "util/CompilerWarning.h"
#include <exception/SemanticError.h>

antlrcpp::Any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext* ctx) {
    // --- Pre-traversing actions
    // ...

    // --- Traverse AST
    visitChildren(ctx);

    // --- Post traversing actions
    // Check if the visitor got a main function
    if (requiresMainFunction && !hasMainFunction)
        throw SemanticError(*ctx->start, MISSING_MAIN_FUNCTION, "No main function found");

    // Print compiler warnings once the whole ast is present, but not for std files
    if (requiresMainFunction && !stdFile) {
        SymbolTable* rootScope = currentScope;
        while (rootScope->getParent()) rootScope = rootScope->getParent();
        rootScope->printCompilerWarnings();
    }

    // Return the symbol table for further use in following compile phases
    return currentScope;
}

antlrcpp::Any AnalyzerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    std::string mainSignature = MAIN_FUNCTION_NAME + "()";

    // Check if the function is already defined
    if (currentScope->lookup(mainSignature))
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

    // Insert function name into the root symbol table
    SymbolType symbolType = SymbolType(TY_FUNCTION);
    currentScope->insert(mainSignature, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);

    // Create the function scope
    currentScope = currentScope->createChildBlock(mainSignature);

    // Declare variable for the return value in the function scope
    SymbolType returnType = SymbolType(TY_INT);
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), INITIALIZED, *ctx->start, false);
    currentScope->lookup(RETURN_VARIABLE_NAME)->setUsed();

    // Visit parameters
    parameterMode = true;
    if (ctx->paramLstDef()) visit(ctx->paramLstDef());
    parameterMode = false;

    // Visit statements in new scope
    visit(ctx->stmtLst());

    // Return to global scope
    currentScope = currentScope->getParent();

    // Confirm main function
    hasMainFunction = true;
    currentScope->lookup(mainSignature)->setUsed();

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext* ctx) {
    // Save the old scope to restore later
    SymbolTable* oldScope = currentScope;

    // Check if this is a global function or a method
    bool isMethod = false;
    std::string functionName = ctx->IDENTIFIER().back()->toString();
    if (ctx->IDENTIFIER().size() > 1) { // Method
        isMethod = true;
        // Change to the struct scope
        currentScope = currentScope->lookupTable("struct:" + ctx->IDENTIFIER()[0]->toString());
    }

    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);

    // Visit params in new scope
    parameterMode = true;
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLstDef())
        paramTypes = visit(ctx->paramLstDef()).as<std::vector<SymbolType>>();
    parameterMode = false;

    // Declare 'this' variable in new scope
    if (isMethod) {
        std::string structName = ctx->IDENTIFIER().front()->toString();
        SymbolTableEntry* structEntry = currentScope->lookup(structName);
        SymbolType thisType = structEntry->getType().toPointer();
        SymbolSpecifiers thisTypeSpecifiers = SymbolSpecifiers(thisType);
        thisTypeSpecifiers.setConst(true);
        currentScope->insert(THIS_VARIABLE_NAME, thisType, thisTypeSpecifiers, INITIALIZED, *ctx->start, false);
    }

    // Declare variable for the return value in the function scope
    SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
    if (returnType.isPointer())
        throw SemanticError(*ctx->start, COMING_SOON_SA,
                            "Spice currently not supports pointer return types due to not supporting heap allocations.");
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start, false);

    // Return to old scope
    currentScope = currentScope->getParent();

    // Insert function into the symbol table
    FunctionSignature signature = FunctionSignature(functionName, paramTypes);

    // Check if the function is already defined
    if (currentScope->lookup(signature.toString()))
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE,
                            "Function '" + signature.toString() + "' is declared twice");
    SymbolType symbolType = SymbolType(TY_FUNCTION);
    currentScope->insert(signature.toString(), symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);
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

    // Restore old scope
    currentScope = oldScope;

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    // Save the old scope to restore later
    SymbolTable* oldScope = currentScope;

    // Check if this is a global function or a method
    bool isMethod = false;
    std::string procedureName = ctx->IDENTIFIER().back()->toString();
    if (ctx->IDENTIFIER().size() > 1) {
        isMethod = true;
        // Change to the struct scope
        currentScope = currentScope->lookupTable("struct:" + ctx->IDENTIFIER()[0]->toString());
    }

    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);

    // Visit params in new scope
    parameterMode = true;
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLstDef())
        paramTypes = visit(ctx->paramLstDef()).as<std::vector<SymbolType>>();
    parameterMode = false;

    // Declare 'this' variable in new scope
    if (isMethod) {
        std::string structName = ctx->IDENTIFIER().front()->toString();
        SymbolTableEntry* structEntry = currentScope->lookup(structName);
        SymbolType thisSymbolType = structEntry->getType().toPointer();
        SymbolSpecifiers thisSymbolSpecifiers = SymbolSpecifiers(thisSymbolType);
        thisSymbolSpecifiers.setConst(true);
        currentScope->insert(THIS_VARIABLE_NAME, thisSymbolType, thisSymbolSpecifiers, INITIALIZED, *ctx->start, false);
    }

    // Return to old scope
    currentScope = currentScope->getParent();

    // Insert procedure into the symbol table
    FunctionSignature signature = FunctionSignature(procedureName, paramTypes);

    // Check if the procedure is already defined
    if (currentScope->lookup(signature.toString()))
        throw SemanticError(*ctx->start, PROCEDURE_DECLARED_TWICE,
                            "Procedure '" + signature.toString() + "' is declared twice");
    SymbolType symbolType = SymbolType(TY_PROCEDURE);
    currentScope->insert(signature.toString(), symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);
    currentScope->pushSignature(signature);

    // Rename function scope block to support function overloading
    currentScope->renameChildBlock(scopeId, signature.toString());

    // Go down again in scope
    currentScope = currentScope->getChild(signature.toString());

    // Visit statement list in new scope
    visit(ctx->stmtLst());

    // Return to old scope
    currentScope = oldScope;

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitExtDecl(SpiceParser::ExtDeclContext* ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();

    std::vector<SymbolType> paramTypes;
    if (ctx->typeLst()) {
        // Check if a param is dyn
        for (auto& param : ctx->typeLst()->dataType()) {
            SymbolType paramType = visit(param).as<SymbolType>();
            if (paramType.is(TY_DYN))
                throw SemanticError(*param->start, UNEXPECTED_DYN_TYPE_SA,
                                    "Dyn data type is not allowed as param type for external functions");
            paramTypes.push_back(paramType);
        }
    }

    if (ctx->dataType()) { // Function
        // Check if return type is dyn
        SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
        if (returnType.is(TY_DYN))
            throw SemanticError(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA,
                                "Dyn data type is not allowed as return type for external functions");

        FunctionSignature signature = FunctionSignature(functionName, paramTypes);
        SymbolType symbolType = SymbolType(TY_FUNCTION);
        currentScope->insert(signature.toString(), symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);
        currentScope->pushSignature(signature);
        // Add return symbol for function
        SymbolTable* functionTable = currentScope->createChildBlock(signature.toString());
        functionTable->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start, false);
    } else { // Procedure
        FunctionSignature signature = FunctionSignature(functionName, paramTypes);
        SymbolType symbolType = SymbolType(TY_PROCEDURE);
        currentScope->insert(signature.toString(), symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);
        currentScope->pushSignature(signature);
    }

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitStructDef(SpiceParser::StructDefContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();
    // Check if struct already exists in this scope
    if (currentScope->lookup(structName))
        throw SemanticError(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate struct '" + structName + "'");
    // Create a new table entry for the struct
    SymbolType symbolType = SymbolType(TY_STRUCT, structName);
    currentScope->insert(structName, symbolType, SymbolSpecifiers(symbolType), DECLARED, *ctx->start, false);
    // Visit field list in a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Insert a field for each field list entry
    for (auto& field : ctx->field()) {
        std::string fieldName = field->IDENTIFIER()->toString();
        SymbolType fieldType = visit(field->dataType()).as<SymbolType>();

        // Build symbol specifiers
        SymbolSpecifiers fieldTypeSpecifiers = SymbolSpecifiers(symbolType);
        if (field->declSpecifiers()) {
            for (auto& specifier : field->declSpecifiers()->declSpecifier()) {
                if (specifier->CONST()) {
                    // Struct fields cannot be const
                    throw SemanticError(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                                        "Struct fields cannot have the const specifier attached");
                } else if (specifier->SIGNED()) {
                    fieldTypeSpecifiers.setSigned(true);
                } else if (specifier->UNSIGNED()) {
                    fieldTypeSpecifiers.setSigned(false);
                }
            }
        }

        currentScope->insert(fieldName, fieldType, fieldTypeSpecifiers, DECLARED, *field->start, false);
    }
    // Return to the old scope
    currentScope = currentScope->getParent();
    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) {
    std::string variableName = ctx->IDENTIFIER()->toString();

    // Check if symbol already exists in the symbol table
    if (currentScope->lookup(variableName))
        throw SemanticError(*ctx->start, VARIABLE_DECLARED_TWICE,
                            "The global variable '" + variableName + "' was declared more than once");

    // Insert variable name to symbol table
    SymbolType symbolType = visit(ctx->dataType()).as<SymbolType>();

    SymbolState state = DECLARED;
    if (ctx->value()) { // Variable is initialized here
        SymbolType valueType = visit(ctx->value()).as<SymbolType>();
        // Infer type
        if (symbolType.is(TY_DYN)) {
            symbolType = valueType;
        } else if (symbolType != valueType) {
            throw SemanticError(*ctx->value()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Cannot apply the assign operator on different data types. You provided " +
                                symbolType.getName(false) + " and " + valueType.getName(false));
        }
        state = INITIALIZED;
    }

    // Check if the type is missing
    if (symbolType.is(TY_DYN))
        throw SemanticError(*ctx->dataType()->start, GLOBAL_OF_TYPE_DYN,
                            "Global variables must have an explicit data type");

    // Create symbol specifiers
    SymbolSpecifiers symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    if (ctx->declSpecifiers()) {
        for (auto& specifier : ctx->declSpecifiers()->declSpecifier()) {
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
            }
        }
    }

    // Insert into symbol table
    currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, state, *ctx->start, parameterMode);

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit loop variable declaration in new scope
    visit(ctx->declStmt());
    // Visit condition in new scope
    SymbolType conditionType = visit(ctx->assignExpr()[0]).as<SymbolType>();
    if (!conditionType.is(TY_BOOL))
        throw SemanticError(*ctx->assignExpr()[0]->start, CONDITION_MUST_BE_BOOL,
                            "For loop condition must be of type bool");
    // Visit incrementer in new scope
    visit(ctx->assignExpr()[1]);
    // Visit statement list in new scope
    nestedLoopCounter++;
    visit(ctx->stmtLst());
    nestedLoopCounter--;
    // Return to old scope
    currentScope = currentScope->getParent();
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) {
    auto head = ctx->foreachHead();

    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);

    // Check type of the array
    SymbolType arrayType = visit(head->assignExpr()).as<SymbolType>();
    if (!arrayType.isArray() && !arrayType.is(TY_STRING))
        throw SemanticError(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                            "Can only apply foreach loop on an array type. You provided " + arrayType.getName(false));

    // Check index assignment or declaration
    SymbolType indexType;
    if (head->declStmt().size() >= 2) {
        indexType = visit(head->declStmt().front()).as<SymbolType>();

        // Set declared variable to initialized, because we increment it internally in the loop
        if (!head->declStmt().front()->assignExpr()) {
            std::string varName = head->declStmt().front()->IDENTIFIER()->toString();
            currentScope->lookup(varName)->updateState(INITIALIZED,
                                                       *head->declStmt().front()->IDENTIFIER()->getSymbol());
        }

        // Check if index type is int
        if (!indexType.is(TY_INT))
            throw SemanticError(*head->declStmt().front()->start, ARRAY_INDEX_NO_INTEGER,
                                "Index in foreach loop must be of type int. You provided " + indexType.getName(false));
    } else {
        // Declare the variable with the default index variable name
        SymbolType symbolType = SymbolType(TY_INT);
        SymbolSpecifiers symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
        symbolTypeSpecifiers.setConst(true);
        currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, symbolType, symbolTypeSpecifiers, INITIALIZED, *ctx->start, false);
    }

    // Check type of the item
    SymbolType itemType = visit(head->declStmt().back()).as<SymbolType>();
    std::string itemVarName = head->declStmt().back()->IDENTIFIER()->toString();
    SymbolTableEntry* itemVarSymbol = currentScope->lookup(itemVarName);
    assert(itemVarSymbol != nullptr);
    if (itemType.is(TY_DYN)) {
        itemType = arrayType.getContainedTy();
        itemVarSymbol->updateType(itemType, false);
    } else {
        if (itemType != arrayType.getContainedTy())
            throw SemanticError(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Foreach loop item type does not match array type. Expected " +
                                arrayType.getName(false) + ", provided " + itemType.getName(false));
    }
    itemVarSymbol->updateState(INITIALIZED, *head->declStmt().back()->IDENTIFIER()->getSymbol());

    // Visit statement list in new scope
    nestedLoopCounter++;
    visit(ctx->stmtLst());
    nestedLoopCounter--;

    // Return to old scope
    currentScope = currentScope->getParent();

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
    if (!conditionType.is(TY_BOOL))
        throw SemanticError(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL,
                            "While loop condition must be of type bool");
    // Visit statement list in new scope
    nestedLoopCounter++;
    visit(ctx->stmtLst());
    nestedLoopCounter--;
    // Return to old scope
    currentScope = currentScope->getParent();
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignExpr()).as<SymbolType>();
    if (!conditionType.is(TY_BOOL))
        throw SemanticError(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL,
                            "If condition must be of type bool");
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    // Visit else statement if it exists
    if (ctx->elseStmt()) visit(ctx->elseStmt());
    return SymbolType(TY_BOOL);
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
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitParamLstDef(SpiceParser::ParamLstDefContext* ctx) {
    std::vector<SymbolType> paramTypes;
    for (auto& param : ctx->declStmt()) {
        SymbolType paramType = visit(param).as<SymbolType>();
        if (paramType.is(TY_DYN))
            throw SemanticError(*param->start, FCT_PARAM_IS_TYPE_DYN,
                                "Type of parameter '" + param->IDENTIFIER()->toString() + "' is invalid");
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

    // Get the type of the symbol
    SymbolType symbolType = visit(ctx->dataType()).as<SymbolType>();

    // Visit the right side
    SymbolState initialState = DECLARED;
    if (ctx->assignExpr()) {
        SymbolType rhsTy = visit(ctx->assignExpr()).as<SymbolType>();
        // Check if type has to be inferred or both types are fixed
        symbolType = symbolType.is(TY_DYN) ? rhsTy : OpRuleManager::getAssignResultType(*ctx->start,symbolType,
                                                                                        rhsTy, true);
        initialState = INITIALIZED;

        // If the rhs is of type array and was the array initialization, there must be a size attached
        if (symbolType.isArray() && symbolType.getArraySize() == 0 && currentVarName.empty())
            throw SemanticError(*ctx->dataType()->start, ARRAY_SIZE_INVALID,
                                "The declaration of an array type must have a size attached");
    }

    if (parameterMode && symbolType.isArray()) // Change array type to pointer type for function/procedure parameters
        symbolType = symbolType.getContainedTy().toPointer();

    // Build symbol specifiers
    SymbolSpecifiers symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    if (ctx->declSpecifiers()) {
        for (auto& specifier : ctx->declSpecifiers()->declSpecifier()) {
            if (specifier->CONST()) {
                symbolTypeSpecifiers.setConst(true);
            } else if (specifier->SIGNED()) {
                symbolTypeSpecifiers.setSigned(true);
            } else if (specifier->UNSIGNED()) {
                symbolTypeSpecifiers.setSigned(false);
            }
        }
    }

    // Insert variable into symbol table
    currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, initialState, *ctx->start, parameterMode);

    return symbolType;
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    // Check if imported library exists
    std::string importPath = ctx->STRING_LITERAL()->toString();
    importPath = importPath.substr(1, importPath.size() - 2);

    // Check if source file exists
    std::string filePath;
    bool isStdFile = false;
    if (importPath.rfind("std/", 0) == 0) { // Include source file from standard library
        isStdFile = true;
        std::string sourceFileIden = importPath.substr(importPath.find("std/") + 4);
        // Find std library
        std::string stdPath;
        if (FileUtil::fileExists("/usr/lib/spice/std")) {
            stdPath = "/usr/lib/spice/std/";
        } else if (FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
            stdPath = std::string(std::getenv("SPICE_STD_DIR"));
            if (stdPath.rfind('/') != stdPath.size() - 1) stdPath += "/";
        } else {
            throw SemanticError(*ctx->STRING_LITERAL()->getSymbol(), STD_NOT_FOUND,
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
            throw SemanticError(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                                "The source file '" + importPath + ".spice' was not found in standard library");
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
            throw SemanticError(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                                "The source file '" + importPath + ".spice' does not exist");
        }
    }

    // Kick off the compilation of the imported source file
    SymbolTable* nestedTable = CompilerInstance::CompileSourceFile(filePath, targetArch, targetVendor, targetOs, objectDir,
                                                                   debugOutput, optLevel, false, isStdFile);

    // Create symbol of type TYPE_IMPORT in the current scope
    std::string importIden = ctx->IDENTIFIER()->toString();
    SymbolType symbolType = SymbolType(TY_IMPORT);
    currentScope->insert(importIden, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start, false);

    // Mount symbol table of the imported source file into the current scope
    nestedTable->setImported();
    currentScope->mountChildBlock(importIden, nestedTable);

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    SymbolTableEntry* returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);

    // Check if there is a value attached to the return statement
    SymbolType returnType;
    if (ctx->assignExpr()) {
        // Procedure returns may not have a value
        if (returnVariable == nullptr)
            throw SemanticError(*ctx->assignExpr()->start, RETURN_WITH_VALUE_IN_PROCEDURE,
                                "Return statements in procedures may not have a value attached");

        // Visit the value
        returnType = visit(ctx->assignExpr()).as<SymbolType>();

        // Check data type of return statement
        if (returnVariable->getType().is(TY_DYN)) {
            // Set explicit return type to the return variable
            returnVariable->updateType(returnType, false);
        } else {
            // Check if return type matches with function definition
            if (returnType != returnVariable->getType())
                throw SemanticError(*ctx->assignExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                                    "Passed wrong data type to return statement. Expected " +
                                    returnVariable->getType().getName(false) + " but got " + returnType.getName(false));
        }

        // Set the return variable to initialized
        returnVariable->updateState(INITIALIZED, *ctx->start);
    } else {
        // Check if result variable is initialized
        if (returnVariable->getState() != INITIALIZED)
            throw SemanticError(*ctx->start, RETURN_WITHOUT_VALUE_RESULT,
                                "Return without value, but result variable is not initialized yet");
        returnType = returnVariable->getType();
    }
    returnVariable->setUsed();
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
    return SymbolType(TY_INT);
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
    return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext* ctx) {
    if (ctx->printfCall()) return visit(ctx->printfCall());
    if (ctx->sizeOfCall()) return visit(ctx->sizeOfCall());
    throw std::runtime_error("Internal compiler error: Could not find builtin function");
}

antlrcpp::Any AnalyzerVisitor::visitPrintfCall(SpiceParser::PrintfCallContext* ctx) {
    std::string templateString = ctx->STRING_LITERAL()->toString();
    templateString = templateString.substr(1, templateString.size() - 2);

    // Check if assignment types match placeholder types
    std::size_t index = templateString.find_first_of('%');
    int placeholderCount = 0;
    while (index != std::string::npos) {
        auto assignment = ctx->assignExpr()[placeholderCount];
        SymbolType assignmentType = visit(assignment).as<SymbolType>();
        switch (templateString[index + 1]) {
            case 'c': {
                if (!assignmentType.is(TY_CHAR))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects char, but got " + assignmentType.getName(false));
                placeholderCount++;
                break;
            }
            case 'd':
            case 'i':
            case 'o':
            case 'u':
            case 'x':
            case 'X': {
                if (!assignmentType.isOneOf({ TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_BOOL }))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects int, byte or bool, but got " +
                                        assignmentType.getName(false));
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
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects double, but got " + assignmentType.getName(false));
                placeholderCount++;
                break;
            }
            case 's': {
                if (!assignmentType.is(TY_STRING) && !assignmentType.isPointerOf(TY_CHAR) && !assignmentType.isArrayOf(TY_CHAR))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects string, but got " + assignmentType.getName(false));
                placeholderCount++;
                break;
            }
            case 'p': {
                if (!assignmentType.isPointer() && !assignmentType.isArray())
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects pointer, but got " + assignmentType.getName(false));
                placeholderCount++;
                break;
            }
        }
        index = templateString.find_first_of('%', index + 1);
    }

    // Check if the number of placeholders matches the number of params
    if (placeholderCount != ctx->assignExpr().size())
        throw SemanticError(*ctx->start, PRINTF_TYPE_ERROR,
                            "Number of placeholders does not match the number of passed arguments");

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext* ctx) {
    // Nothing to check here. Sizeof builtin can handle any type
    return SymbolType(TY_INT);
}

antlrcpp::Any AnalyzerVisitor::visitAssignExpr(SpiceParser::AssignExprContext* ctx) {
    // Visit the right side
    currentVarName = ""; // Reset the current variable name
    scopePrefix = ""; // Reset the scope prefix
    scopePath.clear(); // Clear the scope path

    // Check if there is an assign operator applied
    if (ctx->assignOp()) { // This is an assignment
        // Get symbol type of right side
        SymbolType rhsTy = visit(ctx->assignExpr()).as<SymbolType>();

        // Visit the left side
        currentVarName = ""; // Reset the current variable name
        scopePrefix = ""; // Reset the scope prefix
        scopePath.clear(); // Clear the scope path
        SymbolType lhsTy = visit(ctx->prefixUnaryExpr()).as<SymbolType>();
        std::string variableName = currentVarName;

        // Take a look at the operator
        if (ctx->assignOp()->ASSIGN()) {
            rhsTy = OpRuleManager::getAssignResultType(*ctx->start, lhsTy, rhsTy, false);
        } else if (ctx->assignOp()->PLUS_EQUAL()) {
            rhsTy = OpRuleManager::getPlusEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->MINUS_EQUAL()) {
            rhsTy = OpRuleManager::getMinusEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->MUL_EQUAL()) {
            rhsTy = OpRuleManager::getMulEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->DIV_EQUAL()) {
            rhsTy = OpRuleManager::getDivEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->REM_EQUAL()) {
            rhsTy = OpRuleManager::getRemEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->SHL_EQUAL()) {
            rhsTy = OpRuleManager::getSHLEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->SHR_EQUAL()) {
            rhsTy = OpRuleManager::getSHREqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->AND_EQUAL()) {
            rhsTy = OpRuleManager::getAndEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->OR_EQUAL()) {
            rhsTy = OpRuleManager::getOrEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->assignOp()->XOR_EQUAL()) {
            rhsTy = OpRuleManager::getXorEqualResultType(*ctx->start, lhsTy, rhsTy);
        }

        if (!variableName.empty()) { // Variable is involved on the left side
            SymbolTableEntry* symbolTableEntry = currentScope->lookup(variableName);

            // Check if the symbol exists
            if (!symbolTableEntry)
                throw SemanticError(*ctx->prefixUnaryExpr()->start, REFERENCED_UNDEFINED_VARIABLE,
                                    "The variable '" + variableName +"' was referenced before defined");

            // Perform type inference
            if (lhsTy.is(TY_DYN)) symbolTableEntry->updateType(rhsTy, false);

            // Update state in symbol table
            if (!symbolTableEntry->getType().isOneOf({ TY_FUNCTION, TY_PROCEDURE }))
                symbolTableEntry->updateState(INITIALIZED, *ctx->prefixUnaryExpr()->start);

            // Print compiler warning if the rhs size exceeds the lhs size
            if (lhsTy.isArray() && rhsTy.getArraySize() > lhsTy.getArraySize())
                CompilerWarning(*ctx->assignExpr()->start, ARRAY_TOO_MANY_VALUES, "You provided more values "
                                      "than your array can hold. Excess variables are being ignored by the compiler.").print();
        }

        return rhsTy;
    }

    // This is a fallthrough case, just visit the ternary expression
    return visit(ctx->ternaryExpr());
}

antlrcpp::Any AnalyzerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) {
    // Check if there is a ternary operator applied
    if (ctx->children.size() > 1) {
        auto condition = ctx->logicalOrExpr()[0];
        SymbolType conditionType = visit(condition).as<SymbolType>();
        SymbolType trueType = visit(ctx->logicalOrExpr()[1]).as<SymbolType>();
        SymbolType falseType = visit(ctx->logicalOrExpr()[2]).as<SymbolType>();
        // Check if the condition evaluates to boolean
        if (!conditionType.is(TY_BOOL))
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
            lhsTy = OpRuleManager::getLogicalOrResultType(*ctx->start, lhsTy, rhsTy);
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
            lhsTy = OpRuleManager::getLogicalAndResultType(*ctx->start, lhsTy, rhsTy);
        }
        return lhsTy;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    // Check if a bitwise or operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->bitwiseXorExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseXorExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->bitwiseXorExpr()[i]).as<SymbolType>();
            lhsTy = OpRuleManager::getBitwiseOrResultType(*ctx->start, lhsTy, rhsTy);
        }
        return lhsTy;
    }
    return visit(ctx->bitwiseXorExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext* ctx) {
    // Check if a bitwise xor operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->bitwiseAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->bitwiseAndExpr()[i]).as<SymbolType>();
            lhsTy = OpRuleManager::getBitwiseXorResultType(*ctx->start, lhsTy, rhsTy);
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
            lhsTy = OpRuleManager::getBitwiseAndResultType(*ctx->start, lhsTy, rhsTy);
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

        if (ctx->EQUAL()) // Operator was equal
            return OpRuleManager::getEqualResultType(*ctx->start, lhsTy, rhsTy);
        else if (ctx->NOT_EQUAL()) // Operator was not equal
            return OpRuleManager::getNotEqualResultType(*ctx->start, lhsTy, rhsTy);
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) {
    // Check if a relational operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->shiftExpr()[0]).as<SymbolType>();
        SymbolType rhsTy = visit(ctx->shiftExpr()[1]).as<SymbolType>();

        if (ctx->LESS()) // Operator was less
            return OpRuleManager::getLessResultType(*ctx->start, lhsTy, rhsTy);
        else if (ctx->GREATER()) // Operator was greater
            return OpRuleManager::getGreaterResultType(*ctx->start, lhsTy, rhsTy);
        else if (ctx->LESS_EQUAL()) // Operator was less equal
            return OpRuleManager::getLessEqualResultType(*ctx->start, lhsTy, rhsTy);
        else if (ctx->GREATER_EQUAL()) // Operator was greater equal
            return OpRuleManager::getGreaterEqualResultType(*ctx->start, lhsTy, rhsTy);
    }
    return visit(ctx->shiftExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitShiftExpr(SpiceParser::ShiftExprContext* ctx) {
    // Check if at least one shift operator is applied
    if (ctx->children.size() > 1) {
        SymbolType lhsTy = visit(ctx->additiveExpr()[0]).as<SymbolType>();
        SymbolType rhsTy = visit(ctx->additiveExpr()[1]).as<SymbolType>();

        if (ctx->SHL()) // Operator was shl
            return OpRuleManager::getShiftLeftResultType(*ctx->start, lhsTy, rhsTy);
        else if (ctx->SHR()) // Operator was shr
            return OpRuleManager::getShiftRightResultType(*ctx->start, lhsTy, rhsTy);
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
            const size_t tokenType = op->getSymbol()->getType();
            auto next = ctx->multiplicativeExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (tokenType == SpiceParser::PLUS) { // Operator was plus
                OpRuleManager::getPlusResultType(*next->start, currentType, nextType);
            } else if (tokenType == SpiceParser::MINUS) { // Operator was minus
                OpRuleManager::getMinusResultType(*next->start, currentType, nextType);
            }

            operatorIndex += 2;
        }
        return currentType;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    // Check if at least one multiplicative operator is applied
    if (ctx->castExpr().size() > 1) {
        SymbolType currentType = visit(ctx->castExpr()[0]).as<SymbolType>();
        // Check if data types are compatible
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->castExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            const size_t tokenType = op->getSymbol()->getType();
            auto next = ctx->castExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (tokenType == SpiceParser::MUL) { // Operator is mul
                OpRuleManager::getMulResultType(*next->start, currentType, nextType);
            } else if (tokenType == SpiceParser::DIV) { // Operator is div
                OpRuleManager::getDivResultType(*next->start, currentType, nextType);
            } else if (tokenType == SpiceParser::REM) { // Operator is rem
                OpRuleManager::getRemResultType(*next->start, currentType, nextType);
            }

            operatorIndex += 2;
        }
        return currentType;
    }
    return visit(ctx->castExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitCastExpr(SpiceParser::CastExprContext* ctx) {
    antlrcpp::Any rhs = visit(ctx->prefixUnaryExpr());

    if (ctx->LPAREN()) { // Cast is applied
        SymbolType dstType = visit(ctx->dataType()).as<SymbolType>();
        return OpRuleManager::getCastResultType(*ctx->start, dstType, rhs.as<SymbolType>());
    }

    return rhs;
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    SymbolType lhs = visit(ctx->postfixUnaryExpr()).as<SymbolType>();

    unsigned int tokenCounter = 0;
    while (tokenCounter < ctx->children.size() -1) {
        auto* token = dynamic_cast<SpiceParser::PrefixUnaryOpContext*>(ctx->children[tokenCounter]);
        if (token->MINUS()) { // Consider - operator
            lhs = OpRuleManager::getPrefixMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->PLUS_PLUS()) { // Consider ++ operator
            lhs = OpRuleManager::getPrefixPlusPlusResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->MINUS_MINUS()) { // Consider -- operator
            lhs = OpRuleManager::getPrefixMinusMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->NOT()) { // Consider ! operator
            lhs = OpRuleManager::getPrefixNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->BITWISE_NOT()) { // Consider ~ operator
            lhs = OpRuleManager::getPrefixBitwiseNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->MUL()) { // Consider * operator
            lhs = OpRuleManager::getPrefixMulResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->BITWISE_AND()) { // Consider & operator
            lhs = OpRuleManager::getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
        } else if (token->LOGICAL_AND()) { // Consider doubled & operator
            lhs = OpRuleManager::getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
            lhs = OpRuleManager::getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
        }
        tokenCounter++;
    }

    return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    SymbolType lhs = visit(ctx->atomicExpr()).as<SymbolType>();

    unsigned int tokenCounter = 1;
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        const size_t tokenType = token->getSymbol()->getType();
        if (tokenType == SpiceParser::LBRACKET) { // Subscript operator
            tokenCounter++; // Consume LBRACKET

            std::string arrayName = currentVarName; // Save array name
            ScopePath scopePathBackup = scopePath; // Save scope path
            scopePrefix += "[idx]";

            auto* rule = dynamic_cast<antlr4::RuleContext*>(ctx->children[tokenCounter]);
            SymbolType indexType = visit(rule).as<SymbolType>();
            tokenCounter++; // Consume assignExpr

            if (!indexType.is(TY_INT))
                throw SemanticError(*ctx->start, ARRAY_INDEX_NO_INTEGER, "Array index must be of type int");
            if (!lhs.isOneOf({ TY_ARRAY, TY_STRING, TY_PTR }))
                throw SemanticError(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                                    "Can only apply subscript operator on array type, got " + lhs.getName(true));

            // Get array item type
            lhs = lhs.getContainedTy();

            currentVarName = arrayName; // Restore array name
            scopePath = scopePathBackup; // Restore scope path

            // Retrieve scope for the new scope path fragment
            if (lhs.isBaseType(TY_STRUCT)) { // Struct
                SymbolTable* accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
                assert(accessScope != nullptr);

                SymbolTable* newAccessScope = accessScope->lookupTable("struct:" + lhs.getBaseType().getSubType());
                assert(newAccessScope != nullptr);
                // Push the retrieved scope to the scope path
                scopePath.pushFragment("[idx]", newAccessScope);
            }
        } else if (tokenType == SpiceParser::LPAREN) { // Consider function call
            tokenCounter++; // Consume LPAREN
            std::string functionName = currentVarName;

            // Save the scope path to restore it after visiting the params
            ScopePath scopePathBackup = scopePath;

            // Visit params
            std::vector<SymbolType> paramTypes;
            auto* paramLst = dynamic_cast<SpiceParser::ParamLstContext*>(ctx->children[tokenCounter]);
            if (paramLst != nullptr) {
                for (auto& param : paramLst->assignExpr()) {
                    SymbolType paramType = visit(param).as<SymbolType>();
                    if (paramType.isArray()) paramType = paramType.getContainedTy().toPointer();
                    paramTypes.push_back(paramType);
                }
            }
            tokenCounter++; // Consume paramLst

            // Restore scope path
            scopePath = scopePathBackup;

            // Get function scope
            FunctionSignature signature = FunctionSignature(functionName, paramTypes);
            SymbolTable* functionParentScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
            functionParentScope = functionParentScope->lookupTableWithSignature(signature.toString());
            if (!functionParentScope)
                throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
                                    "Function/Procedure '" + signature.toString() + "' could not be found");
            scopePath.pushFragment(signature.toString(), functionParentScope);
            currentVarName = signature.toString();

            // Get function entry
            SymbolTableEntry* functionEntry = functionParentScope->lookup(signature.toString());
            assert(functionEntry != nullptr);
            functionEntry->setUsed(); // Set the function to used

            // Add function call to the signature queue of the current scope
            currentScope->pushSignature(signature);

            // Search for symbol table of called function/procedure to read parameters
            if (functionEntry->getType().is(TY_FUNCTION)) {
                SymbolTable* functionTable = functionParentScope->getChild(signature.toString());
                // Get return type of called function
                SymbolTableEntry* returnValueEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
                assert(returnValueEntry != nullptr);
                SymbolType returnType = returnValueEntry->getType();
                // Structs from outside the module require more initialization
                if (returnType.is(TY_STRUCT) && scopePath.getCurrentScope()->isImported())
                    return initExtStruct(*ctx->start, scopePath.getCurrentScope(),
                                         returnType.getSubType(),
                                         scopePrefix + "." + returnType.getSubType());
                lhs = returnType;
            } else {
                lhs = SymbolType(TY_BOOL);
            }
        } else if (tokenType == SpiceParser::DOT) { // Consider member access
            tokenCounter++; // Consume dot
            scopePrefix += ".";
            // Visit rhs
            auto* postfixUnary = dynamic_cast<SpiceParser::PostfixUnaryExprContext*>(ctx->children[tokenCounter]);
            lhs = visit(postfixUnary).as<SymbolType>();
        } else if (tokenType == SpiceParser::PLUS_PLUS) { // Consider ++ operator
            lhs = OpRuleManager::getPostfixPlusPlusResultType(*ctx->atomicExpr()->start, lhs);
        } else if (tokenType == SpiceParser::MINUS_MINUS) { // Consider -- operator
            lhs = OpRuleManager::getPostfixMinusMinusResultType(*ctx->atomicExpr()->start, lhs);
        }
        tokenCounter++; // Consume token
    }

    if (lhs.is(TY_INVALID))
        throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_VARIABLE,
                            "Variable '" + currentVarName + "' was referenced before declared");

    return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    if (ctx->IDENTIFIER()) {
        currentVarName = ctx->IDENTIFIER()->toString();
        scopePrefix += currentVarName;

        // Check if this is a reserved keyword
        if (std::find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), currentVarName) != RESERVED_KEYWORDS.end())
            throw SemanticError(*ctx->start, RESERVED_KEYWORD, "'' is a reserved keyword for future"
                                    " development of the language. Please use another identifier instead");

        // Load symbol table entry
        SymbolTable* accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        assert(accessScope != nullptr);
        SymbolTableEntry* entry = accessScope->lookup(currentVarName);

        // Check if symbol exists. If it does not exist, just return because it could be the function name of a function call
        // The existence of the variable is checked in the visitPostfixUnaryExpr method.
        if (!entry) return SymbolType(TY_INVALID);

        // Set symbol to used
        entry->setUsed();

        // Retrieve scope for the new scope path fragment
        SymbolTable* newAccessScope = accessScope;
        if (entry->getType().is(TY_IMPORT)) { // Import
            newAccessScope = accessScope->lookupTable(entry->getName());
        } else if (entry->getType().isBaseType(TY_STRUCT)) { // Struct
            newAccessScope = accessScope->lookupTable("struct:" + entry->getType().getBaseType().getSubType());
        }
        assert(newAccessScope != nullptr);

        // Otherwise, push the retrieved scope to the scope path
        scopePath.pushFragment(currentVarName, newAccessScope);

        return entry->getType();
    }
    if (ctx->builtinCall()) return visit(ctx->builtinCall());
    return visit(ctx->assignExpr());
}

antlrcpp::Any AnalyzerVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    if (ctx->primitiveValue()) return visit(ctx->primitiveValue());

    if (ctx->NIL()) {
        SymbolType nilType = visit(ctx->dataType()).as<SymbolType>();
        if (nilType.is(TY_DYN))
            throw SemanticError(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA,
                                "Nil must have an explicit type");
        return nilType;
    }

    if (!ctx->IDENTIFIER().empty()) { // Struct instantiation
        // Retrieve fully qualified struct name and the scope where to search it
        std::string fullyQualifiedStructName;
        SymbolTable* structScope = currentScope;
        for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
            std::string iden = ctx->IDENTIFIER()[i]->toString();
            fullyQualifiedStructName += fullyQualifiedStructName.empty() ? iden : "." + iden;
            if (i < ctx->IDENTIFIER().size() -1) {
                SymbolTableEntry* entry = structScope->lookup(iden);
                if (!entry)
                    throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                                        "Struct '" + fullyQualifiedStructName + "' was used before defined");
                // Check the type of the symbol table entry
                if (entry->getType().is(TY_IMPORT)) {
                    structScope = structScope->lookupTable(iden);
                } else if (entry->getType().is(TY_STRUCT)) {
                    structScope = structScope->lookupTable("struct:" + iden);
                } else {
                    throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                                        "The variable '" + iden + "' is of type " + entry->getType().getName(false) +
                                        ". Expected struct or import");
                }
            }
        }

        // Check if a symbol is existing with that fully qualified name
        SymbolTableEntry* structSymbol = currentScope->lookup(fullyQualifiedStructName);
        if (!structSymbol) { // Not found
            // Trigger an external struct initialization which loads the struct from another source file and modifies the
            // symbol table accordingly
            initExtStruct(*ctx->IDENTIFIER()[0]->getSymbol(), structScope,
                          ctx->IDENTIFIER().back()->toString(), fullyQualifiedStructName);
            // Reload the struct symbol
            structSymbol = currentScope->lookup(fullyQualifiedStructName);
        }

        // Check if the symbol is of the expected struct type
        if (!structSymbol->getType().is(TY_STRUCT, fullyQualifiedStructName))
            throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                                "Struct '" + fullyQualifiedStructName + "' was used before defined");
        structSymbol->setUsed();

        // Check if the number of fields matches
        SymbolTable* structTable = currentScope->lookupTable("struct:" + fullyQualifiedStructName);
        if (ctx->paramLst()) { // Check if any fields are passed. Empty braces are also allowed
            if (structTable->getFieldCount() != ctx->paramLst()->assignExpr().size())
                throw SemanticError(*ctx->paramLst()->start, NUMBER_OF_FIELDS_NOT_MATCHING,
                                    "You've passed too less/many field values. Pass either none or all of them");

            // Check if the field types are matching
            for (int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
                // Get actual type
                auto ternary = ctx->paramLst()->assignExpr()[i];
                SymbolType actualType = visit(ternary).as<SymbolType>();
                // Get expected type
                SymbolTableEntry* expectedField = structTable->lookupByIndexInCurrentScope(i);
                assert(expectedField != nullptr);
                SymbolType expectedType = expectedField->getType();
                // Check if type matches declaration
                if (actualType != expectedType)
                    throw SemanticError(*ternary->start, FIELD_TYPE_NOT_MATCHING,
                                        "Expected type " + expectedType.getName(false) + " for the field '" +
                                        expectedField->getName() + "', but got " + actualType.getName(false));
            }
        }

        return structSymbol->getType();
    }

    if (ctx->LBRACE()) { // Array initialization
        // Check if all values have the same type
        SymbolType expectedItemType = SymbolType(TY_DYN);
        unsigned int actualSize = 0;
        if (ctx->paramLst()) {
            for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
                SymbolType itemType = visit(ctx->paramLst()->assignExpr()[i]).as<SymbolType>();
                if (expectedItemType.is(TY_DYN)) {
                    expectedItemType = itemType;
                } else if (itemType != expectedItemType) {
                    throw SemanticError(*ctx->paramLst()->assignExpr()[i]->start, ARRAY_ITEM_TYPE_NOT_MATCHING,
                                        "All provided values have to be of the same data type. You provided " +
                                        expectedItemType.getName(false) + " and " + itemType.getName(false));
                }
                actualSize++;
            }
        }
        return expectedItemType.toArray(actualSize);
    }

    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext* ctx) {
    if (ctx->DOUBLE()) return SymbolType(TY_DOUBLE);
    if (ctx->INTEGER()) return SymbolType(TY_INT);
    if (ctx->SHORT()) return SymbolType(TY_SHORT);
    if (ctx->LONG()) return SymbolType(TY_LONG);
    if (ctx->CHAR_LITERAL()) return SymbolType(TY_CHAR);
    if (ctx->STRING_LITERAL()) return SymbolType(TY_STRING);
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    SymbolType type = visit(ctx->baseDataType()).as<SymbolType>();

    unsigned int tokenCounter = 1;
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        if (token->getSymbol()->getType() == SpiceParser::MUL) { // Consider de-referencing operators
            type = type.toPointer();
        } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider array bracket pairs
            tokenCounter++; // Consume LBRACKET
            token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
            unsigned int size = 0; // Default to 0 when no size is attached
            if (token->getSymbol()->getType() == SpiceParser::INTEGER) { // Size is attached
                int signedSize = std::stoi(token->toString());
                // Check if size >1
                if (signedSize <= 1)
                    throw SemanticError(*token->getSymbol(), ARRAY_SIZE_INVALID,
                                        "The size of an array must be > 1");
                size = signedSize;
                tokenCounter++; // Consume INTEGER
            }
            type = type.toArray(size);
        }
        tokenCounter++;
    }

    return type;
}

antlrcpp::Any AnalyzerVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext* ctx) {
    if (ctx->TYPE_DOUBLE()) return SymbolType(TY_DOUBLE);
    if (ctx->TYPE_INT()) return SymbolType(TY_INT);
    if (ctx->TYPE_SHORT()) return SymbolType(TY_SHORT);
    if (ctx->TYPE_LONG()) return SymbolType(TY_LONG);
    if (ctx->TYPE_BYTE()) return SymbolType(TY_BYTE);
    if (ctx->TYPE_CHAR()) return SymbolType(TY_CHAR);
    if (ctx->TYPE_STRING()) return SymbolType(TY_STRING);
    if (ctx->TYPE_BOOL()) return SymbolType(TY_BOOL);
    if (!ctx->IDENTIFIER().empty()) { // Struct type
        // Get type name in format: a.b.c
        std::string structName = ctx->IDENTIFIER()[0]->toString();
        for (unsigned int i = 1; i < ctx->IDENTIFIER().size(); i++) structName += "." + ctx->IDENTIFIER()[i]->toString();

        if (scopePath.getCurrentScope()) { // Within function call
            if (scopePath.getCurrentScope()->isImported()) { // Function call to imported function
                return initExtStruct(*ctx->start, scopePath.getCurrentScope(),
                                     ctx->IDENTIFIER()[0]->toString(), structName);
            } else { // Function call to local function
                // Check if struct was declared
                SymbolTableEntry* structSymbol = scopePath.getCurrentScope()->lookup(structName);
                if (!structSymbol)
                    throw SemanticError(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
                structSymbol->setUsed();
                return SymbolType(TY_STRUCT, structName);
            }
        } else { // Not within function call
            // Check if struct was declared
            SymbolTableEntry* structSymbol = currentScope->lookup(structName);
            if (!structSymbol)
                throw SemanticError(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
            structSymbol->setUsed();
            return SymbolType(TY_STRUCT, structName);
        }
    }
    return SymbolType(TY_DYN);
}

SymbolType AnalyzerVisitor::initExtStruct(const antlr4::Token& token, SymbolTable* sourceScope,
                                          const std::string& oldStructName, const std::string& newStructName) {
    // Check if struct was declared
    SymbolTableEntry* structSymbol = sourceScope->lookup(oldStructName);
    if (!structSymbol)
        throw SemanticError(token, UNKNOWN_DATATYPE, "Unknown datatype '" + newStructName + "'");
    structSymbol->setUsed();
    SymbolTable* structTable = sourceScope->lookupTable("struct:" + oldStructName);

    // Initialize potential structs for field types
    for (unsigned int i = 0; i < structTable->getFieldCount(); i++) {
        SymbolType fieldTy = structTable->lookupByIndexInCurrentScope(i)->getType();
        if (fieldTy.is(TY_STRUCT)) {
            std::string structName = fieldTy.getSubType();
            initExtStruct(token, sourceScope, structName, scopePrefix + "." + structName);
        } else if (fieldTy.isPointerOf(TY_STRUCT)) {
            std::string structName = fieldTy.getContainedTy().getSubType();
            initExtStruct(token, sourceScope, structName, scopePrefix + "." + structName);
        }
    }

    // Update symbol type in the sub-table
    structTable->updateSymbolTypes(structSymbol->getType(), SymbolType(TY_STRUCT, newStructName));

    // Get root scope
    SymbolTable* rootScope = currentScope;
    while (rootScope->getParent()) rootScope = rootScope->getParent();

    // Copy struct symbol and struct table to the root scope of the current source file
    SymbolType newStructTy = SymbolType(TY_STRUCT, newStructName);
    rootScope->insert(newStructName, newStructTy, SymbolSpecifiers(newStructTy), DECLARED,
                      structSymbol->getDefinitionToken(), false);
    rootScope->lookup(newStructName)->setUsed();
    rootScope->mountChildBlock("struct:" + newStructName, structTable);
    rootScope->lookupTable("struct:" + newStructName)->setImported();
    return newStructTy;
}
