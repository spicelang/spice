// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    // Insert function name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_FUNCTION, INITIALIZED);
    // Create a new scope
    std::string scopeId = "f:" + std::to_string(ctx->F()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->F()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitFunctionDef(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    std::cout << currentScope->toString() << std::endl; // ToDo: This line is only for testing, remove it in the future
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    // Insert procedure name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_PROCEDURE, INITIALIZED);
    // Create a new scope
    std::string scopeId = "p:" + std::to_string(ctx->P()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->P()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitProcedureDef(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "for:" + std::to_string(ctx->FOR()->getSymbol()->getLine()) + ":" +
            std::to_string(ctx->FOR()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitForLoop(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "while:" + std::to_string(ctx->WHILE()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->WHILE()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitWhileLoop(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    std::string scopeId = "if:" + std::to_string(ctx->IF()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->IF()->getSymbol()->getCharPositionInLine());
    // Create a new scope
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitIfStmt(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitParamLst(SpiceParser::ParamLstContext *ctx) {
    return SpiceBaseVisitor::visitParamLst(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    // Insert variable name to symbol table
    auto type = getSymbolTypeFromDataType(ctx->dataType());
    currentScope->insert(ctx->IDENTIFIER()->toString(), type, DECLARED);
    return SpiceBaseVisitor::visitDeclStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();
    // Check if function call exists in symbol table
    if (!currentScope->lookup(functionName))
        throw SemanticError(REFERENCED_UNDEFINED_FUNCTION,
                            "Function " + functionName + " was called before initialized.");
    return SpiceBaseVisitor::visitFunctionCall(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    // Check if imported library exists
    std::cout << "Reached import statement" << std::endl;
    return SpiceBaseVisitor::visitImportStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    return SpiceBaseVisitor::visitReturnStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    // Visit children
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitAssignment(ctx);

    /*if (ctx->IDENTIFIER() != nullptr) { // Assigning to an identifier, which was already declared
        std::string variableName = ctx->IDENTIFIER()->toString();
        // Check if variable exists in symbol table
        if (!currentScope->lookup(variableName))
            throw SemanticError(REFERENCED_UNDEFINED_VARIABLE,
                                "Variable " + variableName + " was reference before initialized.");
        // Update variable in symbol table
        currentScope->update(variableName, type);
        currentScope->update(variableName, INITIALIZED);
    } else { // Assigning to an identifier, which is declared in the current statement
        auto type = getSymbolTypeFromDataType(ctx->declStmt()->dataType());

    }

    // Update variable in symbol table
    currentScope->update(variableName, type);
    currentScope->update(variableName, INITIALIZED);

    if (ctx->)*/
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitTernary(SpiceParser::TernaryContext *ctx) {
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitTernary(ctx);
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalOrExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalAndExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseOrExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseAndExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitEquality(SpiceParser::EqualityContext *ctx) {
    return SpiceBaseVisitor::visitEquality(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
    return SpiceBaseVisitor::visitRelationalExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
    return SpiceBaseVisitor::visitAdditiveExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
    // Check if at least one multiplicative operator is applied
    if (ctx->children.size() > 1) {
        antlrcpp::Any currentType = visit(ctx->prefixUnary()[0]);
        // Check if data types are compatible
        for (int i = 1; i < ctx->prefixUnary().size(); i++) {
            antlrcpp::Any nextType = visit(ctx->prefixUnary()[i]);
            // Check all combinations
            if (currentType.as<SymbolType>() == TYPE_DOUBLE) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: 4.3 * 6.1
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: 4.3 * 4
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: 4.3 * "Test"
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands double and string for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: 4.3 * true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands double and bool for multiplicative operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_INT) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: 4 * 6.1
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: 4 * 5
                    currentType = TYPE_INT;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: 4.3 * "Test"
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands int and string for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: 4.3 * true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands int and bool for multiplicative operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_STRING) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: "Test" * 6.1
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and double for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: "Test" * 5
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: "Test" * "Test"
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and string for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: "Test" * true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and bool for multiplicative operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_BOOL) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: true * 6.1
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and double for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: true * 5
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and int for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: true * "Test"
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and string for multiplicative operator");
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: true * false
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and bool for multiplicative operator");
                }
            }
        }
        return currentType;
    }
    return visit(ctx->prefixUnary()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) {
    antlrcpp::Any prefixUnary = SpiceBaseVisitor::visitPrefixUnary(ctx);

    // Ensure integer when '++' or '--' is applied
    if ((ctx->PLUS_PLUS() || ctx->MINUS_MINUS()) && prefixUnary.as<SymbolType>() != TYPE_INT)
        throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "'++' or '--' only can be applied to an integer");

    // Ensure right return type if not is applied
    if (ctx->NOT()) {
        /* If not applied to double, return bool (evaluates later to variable == 0.0)
           If not applied to int, return bool (evaluates later to variable == 0)
           If not applied to string, return bool (evaluates later to variable == "")
           If not applied to bool, return bool (evaluates later to variable == false)*/
        return TYPE_BOOL;
    }

    return prefixUnary;
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) {
    antlrcpp::Any atomicExpr = visit(ctx->atomicExpr());

    // Ensure integer when '++' or '--' is applied
    if ((ctx->PLUS_PLUS() || ctx->MINUS_MINUS()) && atomicExpr.as<SymbolType>() != TYPE_INT)
        throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "'++' or '--' only can be applied to an integer");

    return atomicExpr;
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
    if (ctx->LPAREN()) // Rule: '(' assignment ')'
        return visit(ctx->assignment());
    return visit(ctx->value()); // Rule: value
}

antlrcpp::Any AnalyzerVisitor::visitValue(SpiceParser::ValueContext *ctx) {
    if (ctx->DOUBLE()) return TYPE_DOUBLE;
    if (ctx->INTEGER()) return TYPE_INT;
    if (ctx->STRING()) return TYPE_STRING;
    if (ctx->TRUE() || ctx->FALSE()) return TYPE_BOOL;
    if (ctx->IDENTIFIER()) {
        std::string variableName = ctx->IDENTIFIER()->toString();
        std::cout << "Variable name: " << variableName << std::endl;
        SymbolTableEntry* entry = currentScope->lookup(variableName);
        if (entry == nullptr)
            throw SemanticError(REFERENCED_UNDEFINED_VARIABLE, "Variable " + variableName +
                " was referenced before initialized.");
        return entry->getType();
    }
    return visit(ctx->functionCall());
}

SymbolType AnalyzerVisitor::getSymbolTypeFromDataType(SpiceParser::DataTypeContext *ctx) {
    if (ctx->TYPE_DOUBLE() != nullptr) return TYPE_DOUBLE;
    if (ctx->TYPE_INT() != nullptr) return TYPE_INT;
    if (ctx->TYPE_STRING() != nullptr) return TYPE_STRING;
    if (ctx->TYPE_BOOL() != nullptr) return TYPE_BOOL;
    return TYPE_DYN;
}
