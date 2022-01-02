// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

antlrcpp::Any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext* ctx) {
    // Pre-traversing actions

    // Traverse AST
    visitChildren(ctx);

    // Check if the visitor got a main function
    if (mustHaveMainFunction && !hasMainFunction)
        throw SemanticError(*ctx->start, MISSING_MAIN_FUNCTION, "No main function found");

    // Post traversing actions
    if (!stdFile) { // Do not print compiler warnings for std files
        SymbolTable* rootScope = currentScope;
        while (rootScope->getParent()) rootScope = rootScope->getParent();
        rootScope->printCompilerWarnings();
    }

    // Return the symbol table to the main program for further compile phases
    return currentScope;
}

antlrcpp::Any AnalyzerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    std::string mainSignature = MAIN_FUNCTION_NAME + "()";
    // Check if the function is already defined
    if (currentScope->lookup(mainSignature))
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE, "Main function is declared twice");
    // Insert function name into the root symbol table
    currentScope->insert(mainSignature, SymbolType(TY_FUNCTION), INITIALIZED, *ctx->start, true, false);
    // Create a new scope
    currentScope = currentScope->createChildBlock(mainSignature);
    // Declare variable for the return value
    SymbolType returnType = SymbolType(TY_INT);
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, INITIALIZED, *ctx->start, false, false);
    currentScope->lookup(RETURN_VARIABLE_NAME)->setUsed();
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
    currentScope->lookup(mainSignature)->setUsed();
    return returnType;
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
        currentScope->insert(THIS_VARIABLE_NAME, thisType, INITIALIZED, *ctx->start, true, false);
    }
    // Declare variable for the return value in new scope
    SymbolType returnType = visit(ctx->dataType()).as<SymbolType>();
    if (returnType.isPointer())
        throw SemanticError(*ctx->start, COMING_SOON_SA,
                            "Spice currently not supports pointer return types due to not supporting heap allocations.");
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, DECLARED, *ctx->start, false, false);
    // Return to old scope
    currentScope = currentScope->getParent();
    // Insert function into the symbol table
    FunctionSignature signature = FunctionSignature(functionName, paramTypes);
    // Check if the function is already defined
    if (currentScope->lookup(signature.toString())) {
        throw SemanticError(*ctx->start, FUNCTION_DECLARED_TWICE,
                            "Function '" + signature.toString() + "' is declared twice");
    }
    currentScope->insert(signature.toString(), SymbolType(TY_FUNCTION), INITIALIZED, *ctx->start, true, false);
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
    return returnType;
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
        currentScope->insert(THIS_VARIABLE_NAME, structEntry->getType().toPointer(), INITIALIZED, *ctx->start, true, false);
    }
    // Return to old scope
    currentScope = currentScope->getParent();
    // Insert procedure into the symbol table
    FunctionSignature signature = FunctionSignature(procedureName, paramTypes);
    // Check if the procedure is already defined
    if (currentScope->lookup(signature.toString())) {
        throw SemanticError(*ctx->start, PROCEDURE_DECLARED_TWICE,
                            "Procedure '" + signature.toString() + "' is declared twice");
    }
    currentScope->insert(signature.toString(), SymbolType(TY_PROCEDURE), INITIALIZED, *ctx->start, true, false);
    currentScope->pushSignature(signature);
    // Rename function scope block to support function overloading
    currentScope->renameChildBlock(scopeId, signature.toString());
    // Go down again in scope
    currentScope = currentScope->getChild(signature.toString());
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = oldScope;
    return SymbolType(TY_BOOL);
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
        currentScope->insert(signature.toString(), SymbolType(TY_FUNCTION), INITIALIZED, *ctx->start, true, false);
        currentScope->pushSignature(signature);
        // Add return symbol for function
        SymbolTable* functionTable = currentScope->createChildBlock(signature.toString());
        functionTable->insert(RETURN_VARIABLE_NAME, returnType, DECLARED, *ctx->start, false, false);
    } else { // Procedure
        FunctionSignature signature = FunctionSignature(functionName, paramTypes);
        currentScope->insert(signature.toString(), SymbolType(TY_PROCEDURE), INITIALIZED, *ctx->start, true, false);
        currentScope->pushSignature(signature);
    }

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitStructDef(SpiceParser::StructDefContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();
    // Check if struct already exists in this scope
    if (currentScope->lookup(structName))
        throw SemanticError(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate struct '" + structName + "'");
    // Create a new table entry for the struct
    currentScope->insert(structName, SymbolType(TY_STRUCT, structName), DECLARED, *ctx->start, true, false);
    // Visit field list in a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    visit(ctx->fieldLst());
    // Return to the old scope
    currentScope = currentScope->getParent();
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) {
    std::string variableName = ctx->IDENTIFIER()->toString();

    // Check if symbol already exists in the symbol table
    if (currentScope->lookup(variableName))
        throw SemanticError(*ctx->start, VARIABLE_DECLARED_TWICE,
                            "The variable '" + variableName + "' was declared more than once");

    // Insert variable name to symbol table
    SymbolType dataType = visit(ctx->dataType()).as<SymbolType>();

    SymbolState state = DECLARED;
    if (ctx->value()) { // Variable is initialized here
        SymbolType valueType = visit(ctx->value()).as<SymbolType>();
        // Infer type
        if (dataType.is(TY_DYN)) {
            dataType = valueType;
        } else if (dataType != valueType) {
            throw SemanticError(*ctx->value()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Cannot apply the assign operator on different data types. You provided " +
                                dataType.getName() + " and " + valueType.getName());
        }
        state = INITIALIZED;
    }

    // Check if the type is missing
    if (dataType.is(TY_DYN))
        throw SemanticError(*ctx->dataType()->start, GLOBAL_OF_TYPE_DYN,
                            "Global variables must have an explicit data type");

    // Insert into symbol table
    currentScope->insert(variableName, dataType, state, *ctx->start, ctx->CONST(), parameterMode);

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit assignment in new scope
    visit(ctx->assignExpr()[0]);
    // Visit condition in new scope
    SymbolType conditionType = visit(ctx->assignExpr()[1]).as<SymbolType>();
    if (!conditionType.is(TY_BOOL))
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
    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) {
    auto head = ctx->foreachHead();

    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId);

    // Check type of the array
    SymbolType arrayType = visit(head->assignExpr().back()).as<SymbolType>();
    if (!arrayType.isArray())
        throw SemanticError(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                            "Can only apply foreach loop on an array type. You provided " + arrayType.getName());

    // Check index assignment or declaration
    SymbolType indexType;
    if (head->declStmt().size() >= 2) { // declStmt COMMA declStmt COLON assignExpr
        indexType = visit(head->declStmt().front()).as<SymbolType>();
        currentScope->lookup(head->declStmt().front()->IDENTIFIER()->toString())->updateState(INITIALIZED);
        if (!indexType.is(TY_INT))
            throw SemanticError(*head->declStmt().front()->start, ARRAY_INDEX_NO_INTEGER,
                                "Index in foreach loop must be of type int. You provided " + indexType.getName());
    } else if (head->assignExpr().size() >= 2) { // assignExpr COMMA declStmt COLON assignExpr
        indexType = visit(head->assignExpr().front()).as<SymbolType>();
        if (!indexType.is(TY_INT))
            throw SemanticError(*head->declStmt().front()->start, ARRAY_INDEX_NO_INTEGER,
                                "Index in foreach loop must be of type int. You provided " + indexType.getName());
    } else { // declStmt COLON assignExpr
        // Declare the variable with the default index variable name
        currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, SymbolType(TY_INT), INITIALIZED,
                             *ctx->start, true, false);
    }

    // Check type of the item
    SymbolType itemType = visit(head->declStmt().back()).as<SymbolType>();
    currentScope->lookup(head->declStmt().back()->IDENTIFIER()->toString())->updateState(INITIALIZED);
    if (itemType.is(TY_DYN)) {
        itemType = arrayType.getContainedTy();
    } else {
        if (itemType != arrayType.getContainedTy())
            throw SemanticError(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                                "Foreach loop item type does not match array type. Expected " + arrayType.getName() +
                                ", provided " + itemType.getName());
    }

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
    for (auto& param : ctx->declStmt()) { // Parameters without default value
        SymbolType paramType = visit(param).as<SymbolType>();
        std::string paramName = param->IDENTIFIER()->toString();
        if (paramType.is(TY_DYN))
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
    if (parameterMode && type.isArray()) // Change array type to pointer type for function/procedure parameters
        type = type.getContainedTy().toPointer();
    currentScope->insert(variableName, type, DECLARED, *ctx->start, ctx->CONST(), parameterMode);
    return type;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext* ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();

    // Visit params
    std::vector<SymbolType> paramTypes;
    if (ctx->paramLst()) {
        for (auto& param : ctx->paramLst()->assignExpr())
            paramTypes.push_back(visit(param).as<SymbolType>());
    }
    FunctionSignature signature = FunctionSignature(functionName, paramTypes);

    // Check if function signature exists in symbol table
    SymbolTable* functionEntryTable = functionCallParentScope->lookupTableWithSymbol({ signature.toString() });
    if (!functionEntryTable)
        throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
                            "Function/Procedure '" + signature.toString() + "' could not be found");
    SymbolTableEntry* functionEntry = functionEntryTable->lookup(signature.toString());
    if (!functionEntry)
        throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
                            "Function/Procedure '" + signature.toString() + "' could not be found");
    // Set the function usage
    functionEntry->setUsed();
    // Add function call to the signature queue of the current scope
    functionCallParentScope->pushSignature(signature);

    // Search for symbol table of called function/procedure to read parameters
    if (functionEntry->getType().is(TY_FUNCTION)) {
        SymbolTable* functionTable = functionEntryTable->getChild(signature.toString());
        // Get return type of called function
        SymbolType returnType = functionTable->lookup(RETURN_VARIABLE_NAME)->getType();
        // Structs from outside the module require more initialization
        if (returnType.is(TY_STRUCT) && functionCallParentScope->isImported())
            return initExtStruct(*ctx->start, returnType.getSubType(),
                                 scopePrefix + "." + returnType.getSubType());
        return returnType;
    }
    return SymbolType(TY_BOOL); // Bool with value 'true' for procedures;
}

antlrcpp::Any AnalyzerVisitor::visitNewStmt(SpiceParser::NewStmtContext* ctx) {
    SymbolType dataType = visit(ctx->dataType()).as<SymbolType>();
    std::string variableName = ctx->IDENTIFIER()[0]->toString();
    std::string structName = ctx->IDENTIFIER()[1]->toString();
    std::string structScope = ScopeIdUtil::getScopeId(ctx);

    // Check if symbol already exists in the symbol table
    if (currentScope->lookup(variableName))
        throw SemanticError(*ctx->start, VARIABLE_DECLARED_TWICE,
                            "The variable '" + variableName + "' was declared more than once");

    // Check if the struct is defined
    SymbolTableEntry* structSymbol = currentScope->lookup(structName);
    if (!structSymbol || !structSymbol->getType().is(TY_STRUCT) || structSymbol->getType().getSubType() != structName)
        throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                            "Struct '" + structName + "' was used before defined");
    structSymbol->setUsed();

    // Infer type
    if (dataType.is(TY_DYN)) dataType = structSymbol->getType();

    // Get the symbol table where the struct is defined
    SymbolTable* structTable = currentScope->lookupTable(structScope);
    // Check if the number of fields matches
    if (ctx->paramLst()) { // Also allow the empty initialization
        if (structTable->getFieldCount() != ctx->paramLst()->assignExpr().size())
            throw SemanticError(*ctx->paramLst()->start, NUMBER_OF_FIELDS_NOT_MATCHING,
                                "You've passed too less/many field values");

        // Check if the field types are matching
        for (int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            // Get actual type
            auto ternary = ctx->paramLst()->assignExpr()[i];
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
    }

    // Insert into symbol table
    currentScope->insert(variableName, dataType, INITIALIZED, *ctx->start, ctx->CONST(), false);

    return SymbolType(TY_STRUCT, structName);
}

antlrcpp::Any AnalyzerVisitor::visitArrayInitStmt(SpiceParser::ArrayInitStmtContext* ctx) {
    // Visit data type
    std::string variableName = ctx->IDENTIFIER()->toString();
    SymbolType dataType = visit(ctx->dataType()).as<SymbolType>();
    SymbolType sizeType = visit(ctx->value()).as<SymbolType>();

    // Check if size is >1
    int size = std::stoi(ctx->value()->INTEGER()->toString());
    if (size <= 1)
        throw SemanticError(*ctx->value()->start, ARRAY_SIZE_INVALID, "The size of an array must be > 1");

    // Check if size type is an integer
    if (!sizeType.is(TY_INT))
        throw SemanticError(*ctx->value()->start, ARRAY_SIZE_NO_INTEGER,
                            "The size must be an integer, provided " + sizeType.getName());

    // Check if all values have the same type
    SymbolType expectedItemType = SymbolType(TY_DYN);
    if (ctx->paramLst()) {
        for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            SymbolType itemType = visit(ctx->paramLst()->assignExpr()[i]).as<SymbolType>();
            if (expectedItemType.is(TY_DYN)) {
                expectedItemType = itemType;
            } else if (itemType != expectedItemType) {
                throw SemanticError(*ctx->paramLst()->assignExpr()[i]->start, ARRAY_ITEM_TYPE_NOT_MATCHING,
                                    "All provided values have to be of the same data type. You provided " +
                                    expectedItemType.getName() + " and " + itemType.getName());
            }
        }

        // Compiler warning when the number of provided values exceeds the array size
        if (ctx->paramLst()->assignExpr().size() > size)
            CompilerWarning(*ctx->paramLst()->assignExpr()[size -1]->start, ARRAY_TOO_MANY_VALUES,
                            "You provided more values than your array can hold. Excess variables are "
                            "being ignored by the compiler.").print();
    }

    // Infer type
    if (dataType.is(TY_DYN)) {
        if (expectedItemType.is(TY_DYN))
            throw SemanticError(*ctx->dataType()->start, UNKNOWN_DATATYPE,
                                "Was not able to infer the data type of this array");

        dataType = expectedItemType.toArray();
    }

    // Create new symbol in the current scope
    currentScope->insert(variableName, dataType.toArray(), INITIALIZED, *ctx->start, ctx->CONST(), parameterMode);

    return dataType.toArray();
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    // Check if imported library exists
    std::string importPath = ctx->STRING()->toString();
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
            throw SemanticError(*ctx->STRING()->getSymbol(), STD_NOT_FOUND,
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
            throw SemanticError(*ctx->STRING()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
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
            throw SemanticError(*ctx->STRING()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                                "The source file '" + importPath + ".spice' does not exist");
        }
    }

    // Kick off the compilation of the imported source file
    SymbolTable* nestedTable = CompilerInstance::CompileSourceFile(filePath, targetArch, targetVendor, targetOs, objectDir,
                                                                   debugOutput, optLevel, false, isStdFile);

    // Create symbol of type TYPE_IMPORT in the current scope
    std::string importIden = ctx->IDENTIFIER()->toString();
    currentScope->insert(importIden, SymbolType(TY_IMPORT), INITIALIZED, *ctx->start, true, false);

    // Mount symbol table of the imported source file into the current scope
    nestedTable->setImported();
    currentScope->mountChildBlock(importIden, nestedTable);

    return SymbolType(TY_BOOL);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    // Check if return variable is in the symbol table
    SymbolTableEntry* returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (!returnVariable) throw std::runtime_error("Internal compiler error: Cannot assign return statement to a function");

    // Check if there is a value attached to the return statement
    SymbolType returnType;
    if (ctx->assignExpr()) {
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
                                    returnVariable->getType().getName() + " but got " + returnType.getName());
        }

        // Set the return variable to initialized
        returnVariable->updateState(INITIALIZED);
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
    std::string templateString = ctx->STRING()->toString();
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
                                        "Template string expects char, but got " + assignmentType.getName());
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
                                        "Template string expects int, byte or bool, but got " + assignmentType.getName());
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
                                        "Template string expects double, but got " + assignmentType.getName());
                placeholderCount++;
                break;
            }
            case 's': {
                if (!assignmentType.is(TY_STRING) && !assignmentType.isPointerOf(TY_CHAR) && !assignmentType.isArrayOf(TY_CHAR))
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects string, but got " + assignmentType.getName());
                placeholderCount++;
                break;
            }
            case 'p': {
                if (!assignmentType.isPointer())
                    throw SemanticError(*assignment->start, PRINTF_TYPE_ERROR,
                                        "Template string expects pointer, but got " + assignmentType.getName());
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
    // Take a look on the right side
    SymbolType rhsTy = visit(ctx->ternaryExpr()).as<SymbolType>();

    // Check if there is an assign operator applied
    if (ctx->declStmt() || ctx->idenValue()) {
        // Take a look on the left side
        SymbolType lhsTy;
        antlr4::Token* token;
        SymbolTableEntry* symbolTableEntry;
        bool allowTypeInference = true;
        bool allowStateUpdate = true;

        if (ctx->declStmt()) { // Variable was declared in this line
            lhsTy = visit(ctx->declStmt()).as<SymbolType>();
            std::string variableName = ctx->declStmt()->IDENTIFIER()->toString();
            token = ctx->declStmt()->IDENTIFIER()->getSymbol();
            // Get symbol table entry
            symbolTableEntry = currentScope->lookup(variableName);
        } else { // Variable was declared before and is referenced here
            lhsTy = visit(ctx->idenValue()).as<SymbolType>();
            token = ctx->idenValue()->start;
            // Get symbol table entry
            if (ctx->idenValue()->IDENTIFIER().size() == 1) {
                symbolTableEntry = currentScope->lookup(ctx->idenValue()->IDENTIFIER()[0]->toString());
            } else {
                allowTypeInference = false; // Types in structs are not inferable
                allowStateUpdate = false;
            }
        }

        // Take a look at the operator
        if (ctx->ASSIGN_OP()) {
            // If left type is dyn, do type inference
            if (lhsTy.is(TY_DYN) && allowTypeInference) {
                lhsTy = rhsTy;
                symbolTableEntry->updateType(rhsTy, false);
            }

            // Update variable in symbol table
            if (allowStateUpdate) symbolTableEntry->updateState(INITIALIZED);

            return OpRuleManager::getAssignResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->PLUS_EQUAL()) {
            return OpRuleManager::getPlusEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->MINUS_EQUAL()) {
            return OpRuleManager::getMinusEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->MUL_EQUAL()) {
            return OpRuleManager::getMulEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->DIV_EQUAL()) {
            return OpRuleManager::getDivEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->SHL_EQUAL()) {
            return OpRuleManager::getSHLEqualResultType(*ctx->start, lhsTy, rhsTy);
        } else if (ctx->SHR_EQUAL()) {
            return OpRuleManager::getSHREqualResultType(*ctx->start, lhsTy, rhsTy);
        }
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
        SymbolType lhsTy = visit(ctx->bitwiseAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            SymbolType rhsTy = visit(ctx->bitwiseAndExpr()[i]).as<SymbolType>();
            lhsTy = OpRuleManager::getBitwiseOrResultType(*ctx->start, lhsTy, rhsTy);
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
        else // Operator was not equal
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
        else // Operator was greater equal
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
        else // Operator was shr
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
            auto next = ctx->multiplicativeExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (op->getSymbol()->getType() == SpiceParser::PLUS) { // Operator was plus
                OpRuleManager::getPlusResultType(*next->start, currentType, nextType);
            } else { // Operator was minus
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
    if (ctx->prefixUnaryExpr().size() > 1) {
        SymbolType currentType = visit(ctx->prefixUnaryExpr()[0]).as<SymbolType>();
        // Check if data types are compatible
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->prefixUnaryExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            auto next = ctx->prefixUnaryExpr()[i];
            SymbolType nextType = visit(next).as<SymbolType>();

            if (op->getSymbol()->getType() == SpiceParser::MUL) { // Operator is mul
                OpRuleManager::getMulResultType(*next->start, currentType, nextType);
            } else if (op->getSymbol()->getType() == SpiceParser::DIV) { // Operator is div
                OpRuleManager::getDivResultType(*next->start, currentType, nextType);
            } else { // Operator is rem
                OpRuleManager::getRemResultType(*next->start, currentType, nextType);
            }

            operatorIndex += 2;
        }
        return currentType;
    }
    return visit(ctx->prefixUnaryExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    antlrcpp::Any lhs = visit(ctx->postfixUnaryExpr());

    if (ctx->PLUS_PLUS()) { // Ensure valid type when '++' is applied
        return OpRuleManager::getPrefixPlusPlusResultType(*ctx->postfixUnaryExpr()->start, lhs.as<SymbolType>());
    } else if (ctx->MINUS_MINUS()) { // Ensure valid type when '--' is applied
        return OpRuleManager::getPrefixMinusMinusResultType(*ctx->postfixUnaryExpr()->start, lhs.as<SymbolType>());
    } else if (ctx->NOT()) { // Ensure valid type if '!' is applied
        return OpRuleManager::getNotResultType(*ctx->postfixUnaryExpr()->start, lhs.as<SymbolType>());
    }

    return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    antlrcpp::Any lhs = visit(ctx->castExpr());

    if (ctx->PLUS_PLUS()) { // Ensure valid type when '++' is applied
        return OpRuleManager::getPostfixPlusPlusResultType(*ctx->castExpr()->start, lhs.as<SymbolType>());
    } else if (ctx->MINUS_MINUS()) { // Ensure valid type when '--' is applied
        return OpRuleManager::getPostfixMinusMinusResultType(*ctx->castExpr()->start, lhs.as<SymbolType>());
    }

    return lhs;
}

antlrcpp::Any AnalyzerVisitor::visitCastExpr(SpiceParser::CastExprContext* ctx) {
    antlrcpp::Any rhs = visit(ctx->atomicExpr());

    if (ctx->LPAREN()) { // Cast is applied
        SymbolType dstType = visit(ctx->dataType()).as<SymbolType>();
        return OpRuleManager::getCastResultType(*ctx->start, dstType, rhs.as<SymbolType>());
    }

    return rhs;
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    if (ctx->idenValue()) return visit(ctx->idenValue());
    if (ctx->builtinCall()) return visit(ctx->builtinCall());
    return visit(ctx->assignExpr());
}

antlrcpp::Any AnalyzerVisitor::visitIdenValue(SpiceParser::IdenValueContext* ctx) {
    SymbolType symbolType;
    SymbolTableEntry* entry;
    unsigned int tokenCounter = 0;
    unsigned int assignCounter = 0;
    unsigned int functionCallCounter = 0;
    unsigned int referenceOperations = 0;
    unsigned int dereferenceOperations = 0;
    SymbolTable* scope = currentScope;
    scopePrefix = "";

    // Consider referencing operators
    referenceOperations += ctx->BITWISE_AND().size();
    tokenCounter += referenceOperations;

    // Consider de-referencing operator
    dereferenceOperations += ctx->MUL().size();
    tokenCounter += dereferenceOperations;

    // Loop through children
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        if (!token) { // Got rule context / non terminal symbol
            auto* rule = dynamic_cast<antlr4::RuleContext*>(ctx->children[tokenCounter]);
            unsigned int ruleIndex = rule->getRuleIndex();
            if (ruleIndex == SpiceParser::RuleFunctionCall) { // Consider function call
                // Set function call parent scope
                functionCallParentScope = scope;
                // Visit function call
                symbolType = visit(ctx->functionCall()[functionCallCounter]).as<SymbolType>();
                // Reset values
                functionCallParentScope = nullptr;

                functionCallCounter++;
            }
        } else if (token->getSymbol()->getType() == SpiceParser::IDENTIFIER) { // Consider identifier
            std::string variableName = token->toString();
            entry = scope->lookup(variableName);
            if (!entry)
                throw SemanticError(*ctx->start, REFERENCED_UNDEFINED_VARIABLE,
                                    "Variable '" + variableName + "' was referenced before declared");
            symbolType = entry->getType();
            entry->setUsed();
        } else if (token->getSymbol()->getType() == SpiceParser::DOT) { // Consider dot operator
            // Check this operation is valid on this type
            if (symbolType.is(TY_STRUCT) || symbolType.isPointerOf(TY_STRUCT)) {
                // De-reference automatically if it is a struct pointer
                if (symbolType.isPointerOf(TY_STRUCT)) symbolType = symbolType.getContainedTy();
                // Change to new scope
                std::string structName = symbolType.getSubType();
                scopePrefix += scopePrefix.empty() ? structName : "." + structName;
                scope = scope->lookupTable("struct:" + structName);
                // Check if the table exists
                if (!scope)
                    throw SemanticError(*token->getSymbol(), REFERENCED_UNDEFINED_STRUCT_FIELD,
                                        "Referenced undefined struct '" + structName + "'");
            } else if (symbolType.is(TY_IMPORT)) {
                // Change to new scope
                std::string importName = entry->getName();
                scope = scope->lookupTable(importName);
                scopePrefix += scopePrefix.empty() ? importName : "." + importName;
                // Check if the table exists
                if (!scope)
                    throw SemanticError(*token->getSymbol(), REFERENCED_UNDEFINED_STRUCT_FIELD,
                                        "Referenced undefined import '" + importName + "'");
            } else {
                throw SemanticError(*token->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply member access operator on " + symbolType.getName());
            }
        } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider subscript operator
            // Check this operation is valid on this type
            if (!symbolType.isArray() && !symbolType.isPointer())
                throw SemanticError(*token->getSymbol(), OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply subscript operator on " + symbolType.getName());
            // Check if the index is an integer
            SymbolType indexType = visit(ctx->assignExpr()[assignCounter]).as<SymbolType>();
            if (!indexType.is(TY_INT))
                throw SemanticError(*ctx->assignExpr()[assignCounter]->start, ARRAY_INDEX_NO_INTEGER,
                                    "Array index must be of type int, you provided " + indexType.getName());
            // Promote the array/pointer element type
            symbolType = symbolType.getContainedTy();
            // Increase counters
            assignCounter++;
            tokenCounter += 2; // To consume the assignExpr and the RBRACKET
        }
        // Increase counter
        tokenCounter++;
    }

    // Apply referencing operators if necessary
    for (unsigned int i = 0; i < referenceOperations; i++)
        symbolType = symbolType.toPointer();

    // Apply de-referencing operators if necessary
    for (unsigned int i = 0; i < dereferenceOperations; i++)
        symbolType = symbolType.getContainedTy();

    return symbolType;
}

antlrcpp::Any AnalyzerVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    if (ctx->DOUBLE()) return SymbolType(TY_DOUBLE);
    if (ctx->INTEGER()) return SymbolType(TY_INT);
    if (ctx->CHAR()) return SymbolType(TY_CHAR);
    if (ctx->STRING()) return SymbolType(TY_STRING);
    if (ctx->TRUE() || ctx->FALSE()) return SymbolType(TY_BOOL);
    return nullptr;
}

antlrcpp::Any AnalyzerVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    SymbolType type = SymbolType(TY_DYN);

    if (ctx->TYPE_DOUBLE()) type = SymbolType(TY_DOUBLE);
    if (ctx->TYPE_INT()) type = SymbolType(TY_INT);
    if (ctx->TYPE_SHORT()) type = SymbolType(TY_SHORT);
    if (ctx->TYPE_LONG()) type = SymbolType(TY_LONG);
    if (ctx->TYPE_BYTE()) type = SymbolType(TY_BYTE);
    if (ctx->TYPE_CHAR()) type = SymbolType(TY_CHAR);
    if (ctx->TYPE_STRING()) type = SymbolType(TY_STRING);
    if (ctx->TYPE_BOOL()) type = SymbolType(TY_BOOL);
    if (!ctx->IDENTIFIER().empty()) { // Struct type
        // Get type name in format: a.b.c
        std::string structName = ctx->IDENTIFIER()[0]->toString();
        for (unsigned int i = 1; i < ctx->IDENTIFIER().size(); i++) structName += "." + ctx->IDENTIFIER()[i]->toString();

        if (functionCallParentScope) { // Within function call
            if (functionCallParentScope->isImported()) { // Function call to imported function
                type = initExtStruct(*ctx->start, ctx->IDENTIFIER()[0]->toString(), structName);
            } else { // Function call to local function
                // Check if struct was declared
                SymbolTableEntry* structSymbol = functionCallParentScope->lookup(structName);
                if (!structSymbol)
                    throw SemanticError(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
                structSymbol->setUsed();
                type = SymbolType(TY_STRUCT, structName);
            }
        } else { // Not within function call
            // Check if struct was declared
            SymbolTableEntry* structSymbol = currentScope->lookup(structName);
            if (!structSymbol)
                throw SemanticError(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
            structSymbol->setUsed();
            type = SymbolType(TY_STRUCT, structName);
        }
    }

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
                if (signedSize <= 1)
                    throw SemanticError(*token->getSymbol(), ARRAY_SIZE_INVALID,
                                        "The size of an array must be > 1");
                tokenCounter++; // Consume INTEGER
            }
            type = type.toArray(size);
        }
        tokenCounter++;
    }

    return type;
}

SymbolType AnalyzerVisitor::initExtStruct(const antlr4::Token& token, const std::string& oldStructName,
                                          const std::string& newStructName) {
    // Check if struct was declared
    SymbolTableEntry* structSymbol = functionCallParentScope->lookup(oldStructName);
    if (!structSymbol)
        throw SemanticError(token, UNKNOWN_DATATYPE, "Unknown datatype '" + newStructName + "'");
    structSymbol->setUsed();
    SymbolTable* structTable = functionCallParentScope->lookupTable("struct:" + oldStructName);

    // Initialize potential structs for field types
    for (unsigned int i = 0; i < structTable->getFieldCount(); i++) {
        SymbolType fieldType = structTable->lookupByIndexInCurrentScope(i)->getType();
        if (fieldType.is(TY_STRUCT)) {
            std::string structName = fieldType.getSubType();
            initExtStruct(token, structName, scopePrefix + "." + structName);
        } else if (fieldType.isPointerOf(TY_STRUCT)) {
            std::string structName = fieldType.getContainedTy().getSubType();
            initExtStruct(token, structName, scopePrefix + "." + structName);
        }
    }

    // Update symbol type in the sub-table
    structTable->updateSymbolTypes(structSymbol->getType(), SymbolType(TY_STRUCT, newStructName));

    // Get root scope
    SymbolTable* rootScope = currentScope;
    while (rootScope->getParent()) rootScope = rootScope->getParent();

    // Copy struct symbol and struct table to the root scope of the current source file
    SymbolType newStructType = SymbolType(TY_STRUCT, newStructName);
    rootScope->insert(newStructName, newStructType, DECLARED, structSymbol->getDefinitionToken(), true, false);
    rootScope->lookup(newStructName)->setUsed();
    rootScope->mountChildBlock("struct:" + newStructName, structTable);
    rootScope->lookupTable("struct:" + newStructName)->setImported();
    return newStructType;
}
