// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

antlrcpp::Any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
    // Pre-traversing action

    // Traverse AST
    antlrcpp::Any result = visitChildren(ctx);

    // Post traversing actions
    std::cout << currentScope->toString() << std::endl;
    return result;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    // Insert function name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_FUNCTION, INITIALIZED, true);
    // Create a new scope
    std::string scopeId = "f:" + std::to_string(ctx->F()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->F()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Declare variable for the return value
    SymbolType returnType = getSymbolTypeFromDataType(ctx->dataType());
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, DECLARED, false);
    // Visit parameters
    if (ctx->paramLstDef()) visit(ctx->paramLstDef());
    // Visit statements in new scope
    visit(ctx->stmtLst());
    // Check if return variable is now initialized
    if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
        throw SemanticError(FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");
    // Return to old scope
    currentScope = currentScope->getParent();
    return returnType;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    // Insert procedure name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_PROCEDURE, INITIALIZED, true);
    // Create a new scope
    std::string scopeId = "p:" + std::to_string(ctx->P()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->P()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit params in new scope
    if (ctx->paramLstDef()) visit(ctx->paramLstDef());
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    return TYPE_BOOL;
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "for:" + std::to_string(ctx->FOR()->getSymbol()->getLine()) + ":" +
            std::to_string(ctx->FOR()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit assignment in new scope
    visit(ctx->assignment()[0]);
    // Visit condition in new scope
    SymbolType conditionType = visit(ctx->assignment()[1]).as<SymbolType>();
    if (conditionType != TYPE_BOOL)
        throw SemanticError(CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");
    // Visit incrementer in new scope
    visit(ctx->assignment()[2]);
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    return TYPE_BOOL;
}

/*antlrcpp::Any AnalyzerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "foreach:" + std::to_string(ctx->FOR()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->FOR()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
}*/

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "while:" + std::to_string(ctx->WHILE()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->WHILE()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignment()).as<SymbolType>();
    if (conditionType != TYPE_BOOL)
        throw SemanticError(CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    return TYPE_BOOL;
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    std::string scopeId = "if:" + std::to_string(ctx->IF()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->IF()->getSymbol()->getCharPositionInLine());
    // Create a new scope
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit condition
    SymbolType conditionType = visit(ctx->assignment()).as<SymbolType>();
    if (conditionType != TYPE_BOOL)
        throw SemanticError(CONDITION_MUST_BE_BOOL, "If condition must be of type bool");
    // Visit statement list in new scope
    visit(ctx->stmtLst());
    // Return to old scope
    currentScope = currentScope->getParent();
    return TYPE_BOOL;
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    std::string variableName = ctx->IDENTIFIER()->toString();
    // Check if symbol already exists in the symbol table
    if (currentScope->lookup(variableName))
        throw SemanticError(VARIABLE_DECLARED_TWICE,
                            "The variable '" + variableName + "' was declared more than once");
    // Insert variable name to symbol table
    SymbolType type = getSymbolTypeFromDataType(ctx->dataType());
    currentScope->insert(variableName, type, DECLARED, ctx->CONST());
    return type;
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();
    // Check if function call exists in symbol table
    SymbolTableEntry* entry = currentScope->lookup(functionName);
    if (!entry) throw SemanticError(REFERENCED_UNDEFINED_FUNCTION,
                                    "Function " + functionName + " was called before initialized.");
    return entry->getType();
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    // Check if imported library exists
    std::string libraryIdentifier = ctx->STRING()->toString();
    // TODO: Add library check
    return TYPE_STRING;
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    SymbolType returnType = visit(ctx->assignment()).as<SymbolType>();
    // Check if return variable is in the symbol table
    SymbolTableEntry* returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
    if (!returnVariable)
        throw SemanticError(RETURN_STMT_WITHOUT_FUNCTION, "Cannot assign return statement to a function");
    // Check if return type matches with function definition
    if (returnType != returnVariable->getType())
        throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "Passed wrong data type to return statement");
    // Set the return variable to initialized
    returnVariable->updateState(INITIALIZED);
    return returnType;
}

antlrcpp::Any AnalyzerVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    // Check if there is an assign operator applied
    if (ctx->children.size() > 1) {
        std::string variableName;
        SymbolType leftType;

        // Take a look at the left side
        if (ctx->declStmt()) { // Variable was declared in this line
            visit(ctx->declStmt());
            variableName = ctx->declStmt()->IDENTIFIER()->toString();
        } else { // Variable was declared before and is referenced here
            variableName = ctx->IDENTIFIER()[0].toString();
        }
        // Retrieve the left type from the symbol table
        SymbolTableEntry* symbolTableEntry = currentScope->lookup(variableName);
        if (symbolTableEntry == nullptr)
            throw SemanticError(REFERENCED_UNDEFINED_VARIABLE, "Variable " + variableName +
                                                               " was referenced before initialized.");
        leftType = symbolTableEntry->getType();

        // Take a look at the right side
        SymbolType rightType = visit(ctx->ternary()).as<SymbolType>();
        // If left type is dyn, set left type to right type
        if (leftType == TYPE_DYN) {
            leftType = rightType;
            symbolTableEntry->updateType(rightType);
        }

        // Take a look at the operator

        if (ctx->ASSIGN_OP()) {
            if (leftType != rightType)
                throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply the assign operator to different data types");
        } else if (ctx->PLUS_EQUAL()) {
            if (!(leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) &&
                !(leftType == TYPE_INT && rightType == TYPE_INT) &&
                !(leftType == TYPE_STRING && rightType == TYPE_STRING)) {
                throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply += operator on this combination of types");
            }
        } else if (ctx->MINUS_EQUAL()) {
            if (!(leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) &&
                !(leftType == TYPE_INT && rightType == TYPE_INT)) {
                throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply -= operator on this combination of types");
            }
        } else if (ctx->MUL_EQUAL()) {
            if (!(leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) &&
                !(leftType == TYPE_INT && rightType == TYPE_INT)) {
                throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply *= operator on this combination of types");
            }
        } else if (ctx->DIV_EQUAL()) {
            if (!(leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) &&
                !(leftType == TYPE_INT && rightType == TYPE_INT)) {
                throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                    "Cannot apply /= operator on this combination of types");
            }
        } else {
            throw std::runtime_error("Internal compiler error. Fell through all assign ops");
        }
        // Update variable in symbol table
        symbolTableEntry->updateState(INITIALIZED);
        return leftType;
    }
    return visit(ctx->ternary());
}

antlrcpp::Any AnalyzerVisitor::visitTernary(SpiceParser::TernaryContext *ctx) {
    // Check if there is a ternary operator applied
    if (ctx->children.size() > 1) {
        SymbolType conditionType = visit(ctx->logicalOrExpr()[0]).as<SymbolType>();
        SymbolType trueType = visit(ctx->logicalOrExpr()[1]).as<SymbolType>();
        SymbolType falseType = visit(ctx->logicalOrExpr()[2]).as<SymbolType>();
        // Check if the condition evaluates to boolean
        if (conditionType != TYPE_BOOL)
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");
        // Check if trueType and falseType are matching
        if (trueType != falseType)
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                "True operand and false operand in ternary must be from same data type");
        return trueType;
    }
    return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
    // Check if a logical or operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->logicalAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            SymbolType rightType = visit(ctx->logicalAndExpr()[i]).as<SymbolType>();
            if (leftType == TYPE_BOOL && rightType == TYPE_BOOL) { // Allow logical or operator for booleans
                leftType = TYPE_BOOL;
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "Can only apply logical or to booleans");
        }
        return leftType;
    }
    return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
    // Check if a logical and operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->bitwiseOrExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            SymbolType rightType = visit(ctx->bitwiseOrExpr()[i]).as<SymbolType>();
            if (leftType == TYPE_BOOL && rightType == TYPE_BOOL) { // Allow logical and operator for booleans
                leftType = TYPE_BOOL;
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE, "Can only apply logical and to booleans");
        }
        return leftType;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
    // Check if a bitwise or operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->bitwiseAndExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            SymbolType rightType = visit(ctx->bitwiseAndExpr()[i]).as<SymbolType>();
            if (leftType == TYPE_BOOL && rightType == TYPE_BOOL) { // Allow bitwise or operator for booleans
                leftType = TYPE_BOOL;
                continue;
            }
            if (leftType == TYPE_INT && rightType == TYPE_INT) { // Allow bitwise or operator for integers
                leftType = TYPE_INT;
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                "Can only apply bitwise or to booleans and integers");
        }
        return leftType;
    }
    return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
    // Check if a bitwise and operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->equalityExpr()[0]).as<SymbolType>();
        for (int i = 1; i < ctx->equalityExpr().size(); i++) {
            SymbolType rightType = visit(ctx->equalityExpr()[i]).as<SymbolType>();
            if (leftType == TYPE_BOOL && rightType == TYPE_BOOL) { // Allow bitwise and operator for booleans
                leftType = TYPE_BOOL;
                continue;
            }
            if (leftType == TYPE_INT && rightType == TYPE_INT) { // Allow bitwise and operator for integers
                leftType = TYPE_INT;
                continue;
            }
            // Every other combination is invalid
            throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                "Can only apply bitwise and to booleans and integers");
        }
        return leftType;
    }
    return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
    // Check if at least one equality operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->relationalExpr()[0]).as<SymbolType>();
        SymbolType rightType = visit(ctx->relationalExpr()[1]).as<SymbolType>();
        if (leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) return TYPE_BOOL; // Can compare double with double
        if (leftType == TYPE_DOUBLE && rightType == TYPE_INT) return TYPE_BOOL; // Can compare double with int
        if (leftType == TYPE_INT && rightType == TYPE_DOUBLE) return TYPE_BOOL; // Can compare int with double
        if (leftType == TYPE_INT && rightType == TYPE_INT) return TYPE_BOOL; // Can compare int with int
        if (leftType == TYPE_STRING && rightType == TYPE_STRING) return TYPE_BOOL; // Can compare string with string
        if (leftType == TYPE_BOOL && rightType == TYPE_BOOL) return TYPE_BOOL; // Can compare bool with bool
        // Every other combination is invalid
        throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                            "Can only compare some type combinations with an equality operator");
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
    // Check if at least one relational operator is applied
    if (ctx->children.size() > 1) {
        SymbolType leftType = visit(ctx->additiveExpr()[0]).as<SymbolType>();
        SymbolType rightType = visit(ctx->additiveExpr()[1]).as<SymbolType>();
        if (leftType == TYPE_DOUBLE && rightType == TYPE_DOUBLE) return TYPE_BOOL; // Can compare double with double
        if (leftType == TYPE_DOUBLE && rightType == TYPE_INT) return TYPE_BOOL; // Can compare double with int
        if (leftType == TYPE_INT && rightType == TYPE_DOUBLE) return TYPE_BOOL; // Can compare int with double
        if (leftType == TYPE_INT && rightType == TYPE_INT) return TYPE_BOOL; // Can compare int with int
        // Every other combination is invalid
        throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                            "Can only compare doubles or ints with one another with a relational operator");
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
    // Check if at least one additive operator is applied
    if (ctx->children.size() > 1) {
        antlrcpp::Any currentType = visit(ctx->multiplicativeExpr()[0]);
        // Check if data types are compatible
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            antlrcpp::Any nextType = visit(ctx->multiplicativeExpr()[i]);
            // Check all combinations
            if (currentType.as<SymbolType>() == TYPE_DOUBLE) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: 4.3 + 6.1
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: 4.3 + 4
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: 4.3 + "Test"
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: 4.3 + true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands double and bool for additive operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_INT) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: 4 + 6.1
                    currentType = TYPE_DOUBLE;
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: 4 + 5
                    currentType = TYPE_INT;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: 4.3 + "Test"
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: 4.3 + true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands int and bool for additive operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_STRING) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: "Test" + 6.1
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: "Test" + 5
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: "Test" + "Test"
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: "Test" + true
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and bool for additive operator");
                }
            } else if (currentType.as<SymbolType>() == TYPE_BOOL) {
                if (nextType.as<SymbolType>() == TYPE_DOUBLE) { // e.g.: true + 6.1
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and double for additive operator");
                } else if (nextType.as<SymbolType>() == TYPE_INT) { // e.g.: true + 5
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and int for additive operator");
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: true + "Test"
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands string and string for additive operator");
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: true + false
                    throw SemanticError(OPERATOR_WRONG_DATA_TYPE,
                                        "Incompatible operands bool and bool for additive operator");
                }
            }
        }
        return currentType;
    }
    return visit(ctx->multiplicativeExpr()[0]);
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
                } else if (nextType.as<SymbolType>() == TYPE_STRING) { // e.g.: 4 * "Test"
                    currentType = TYPE_STRING;
                } else if (nextType.as<SymbolType>() == TYPE_BOOL) { // e.g.: 4 * true
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
    antlrcpp::Any prefixUnary = visit(ctx->postfixUnary());

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
        SymbolTableEntry* entry = currentScope->lookup(variableName);
        if (entry == nullptr)
            throw SemanticError(REFERENCED_UNDEFINED_VARIABLE, "Variable " + variableName +
                " was referenced before initialized.");
        return entry->getType();
    }
    return visit(ctx->functionCall());
}

SymbolType AnalyzerVisitor::getSymbolTypeFromDataType(SpiceParser::DataTypeContext *ctx) {
    if (ctx->TYPE_DOUBLE()) return TYPE_DOUBLE;
    if (ctx->TYPE_INT()) return TYPE_INT;
    if (ctx->TYPE_STRING()) return TYPE_STRING;
    if (ctx->TYPE_BOOL()) return TYPE_BOOL;
    return TYPE_DYN;
}
