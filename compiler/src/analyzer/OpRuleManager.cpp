// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

SymbolType OpRuleManager::getAssignResultType(const antlr4::Token& token, SymbolType lhs, SymbolType rhs) {
    // Skip type compatibility check if the lhs is of type dyn
    if (lhs.is(TY_DYN)) return rhs;
    // Allow pointers, arrays and structs of the same type straight away
    if (lhs.isOneOf({ TY_PTR, TY_ARRAY, TY_STRUCT }) && lhs == rhs) return rhs;
    // Allow char* = string
    if (lhs.isPointerOf(TY_CHAR) && rhs.is(TY_STRING)) return lhs;
    // Check primitive type combinations
    return validateBinaryOperation(token, ASSIGN_OP_RULES, "=", lhs, rhs);
}

SymbolType OpRuleManager::getPlusEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, PLUS_EQUAL_OP_RULES, "+=", lhs, rhs);
}

SymbolType OpRuleManager::getMinusEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, MINUS_EQUAL_OP_RULES, "-=", lhs, rhs);
}

SymbolType OpRuleManager::getMulEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, MUL_EQUAL_OP_RULES, "*=", lhs, rhs);
}

SymbolType OpRuleManager::getDivEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, DIV_EQUAL_OP_RULES, "/=", lhs, rhs);
}

SymbolType OpRuleManager::getRemEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, REM_EQUAL_OP_RULES, "%=", lhs, rhs);
}

SymbolType OpRuleManager::getSHLEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, SHL_EQUAL_OP_RULES, "<<=", lhs, rhs);
}

SymbolType OpRuleManager::getSHREqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, SHR_EQUAL_OP_RULES, ">>=", lhs, rhs);
}

SymbolType OpRuleManager::getAndEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, AND_EQUAL_OP_RULES, "&=", lhs, rhs);
}

SymbolType OpRuleManager::getOrEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, OR_EQUAL_OP_RULES, "|=", lhs, rhs);
}

SymbolType OpRuleManager::getXorEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, XOR_EQUAL_OP_RULES, "^=", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalAndResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, LOGICAL_AND_OP_RULES, "&&", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalOrResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, LOGICAL_OR_OP_RULES, "||", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseAndResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, BITWISE_AND_OP_RULES, "&", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseOrResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, BITWISE_OR_OP_RULES, "|", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseXorResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, BITWISE_XOR_OP_RULES, "^", lhs, rhs);
}

SymbolType OpRuleManager::getEqualResultType(const antlr4::Token& token, SymbolType lhs, SymbolType rhs) {
    // Allow pointer = pointer straight away
    if (lhs.isPointer() && rhs.isPointer()) return SymbolType(TY_BOOL);
    // Allow pointer = byte straight away
    if (lhs.isPointer() && rhs.is(TY_INT)) return SymbolType(TY_BOOL);
    // Check primitive type combinations
    return validateBinaryOperation(token, EQUAL_OP_RULES, "==", lhs, rhs);
}

SymbolType OpRuleManager::getNotEqualResultType(const antlr4::Token& token, SymbolType lhs, SymbolType rhs) {
    // Allow pointers straight away
    if (lhs.isPointer() && rhs.isPointer()) return SymbolType(TY_BOOL);
    // Check primitive type combinations
    return validateBinaryOperation(token, NOT_EQUAL_OP_RULES, "!=", lhs, rhs);
}

SymbolType OpRuleManager::getLessResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, LESS_OP_RULES, "<", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, GREATER_OP_RULES, ">", lhs, rhs);
}

SymbolType OpRuleManager::getLessEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, LESS_EQUAL_OP_RULES, "<=", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterEqualResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, GREATER_EQUAL_OP_RULES, ">=", lhs, rhs);
}

SymbolType OpRuleManager::getShiftLeftResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, SHIFT_LEFT_OP_RULES, "<<", lhs, rhs);
}

SymbolType OpRuleManager::getShiftRightResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, SHIFT_RIGHT_OP_RULES, ">>", lhs, rhs);
}

SymbolType OpRuleManager::getPlusResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, PLUS_OP_RULES, "+", lhs, rhs);
}

SymbolType OpRuleManager::getMinusResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, MINUS_OP_RULES, "-", lhs, rhs);
}

SymbolType OpRuleManager::getMulResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, MUL_OP_RULES, "*", lhs, rhs);
}

SymbolType OpRuleManager::getDivResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, DIV_OP_RULES, "/", lhs, rhs);
}

SymbolType OpRuleManager::getRemResultType(const antlr4::Token& token, const SymbolType& lhs, const SymbolType& rhs) {
    return validateBinaryOperation(token, REM_OP_RULES, "%", lhs, rhs);
}

SymbolType OpRuleManager::getPrefixPlusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_PLUS_OP_RULES, "+", lhs);
}

SymbolType OpRuleManager::getPrefixMinusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_MINUS_OP_RULES, "-", lhs);
}

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getPrefixNotResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_NOT_OP_RULES, "!", lhs);
}

SymbolType OpRuleManager::getPrefixBitwiseNotResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_BITWISE_NOT_OP_RULES, "~", lhs);
}

SymbolType OpRuleManager::getPrefixMulResultType(const antlr4::Token& token, SymbolType lhs) {
    if (!lhs.isPointer())
        throw SemanticError(token, OPERATOR_WRONG_DATA_TYPE,
                            "Cannot apply de-referencing operator on type " + lhs.getName(true));
    return lhs.getContainedTy();
}

SymbolType OpRuleManager::getPrefixBitwiseAndResultType(const antlr4::Token& token, SymbolType lhs) {
    return lhs.toPointer();
}

SymbolType OpRuleManager::getPostfixPlusPlusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, POSTFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPostfixMinusMinusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, POSTFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getCastResultType(const antlr4::Token& token, SymbolType lhs, SymbolType rhs) { // double dbl = (double) 10;
    // Allow casts string -> char*  and string -> char[]
    if (lhs.isOneOf({ TY_PTR, TY_ARRAY }) && lhs.getContainedTy().is(TY_CHAR) && rhs.is(TY_STRING)) return lhs;
    // Allow casts char* -> string and char[] -> string
    if (lhs.is(TY_STRING) && rhs.isOneOf({ TY_PTR, TY_ARRAY }) && rhs.getContainedTy().is(TY_CHAR)) return lhs;
    // Check primitive type combinations
    return validateBinaryOperation(token, CAST_OP_RULES, "(cast)", lhs, rhs);
}

SymbolType OpRuleManager::validateBinaryOperation(const antlr4::Token& token, const std::vector<BinaryOpRule>& opRules,
                                                  const std::string& opName, const SymbolType& lhs, const SymbolType& rhs) {
    for (auto& rule : opRules) {
        if (std::get<0>(rule) == lhs && std::get<1>(rule) == rhs)
            return std::get<2>(rule);
    }
    throw printErrorMessageBinary(token, opName, lhs, rhs);
}

SymbolType OpRuleManager::validateUnaryOperation(const antlr4::Token& token, const std::vector<UnaryOpRule>& opRules,
                                                 const std::string& opName, const SymbolType& lhs) {
    for (auto& rule : opRules) {
        if (std::get<0>(rule) == lhs)
            return std::get<1>(rule);
    }
    throw printErrorMessageUnary(token, opName, lhs);
}

SemanticError OpRuleManager::printErrorMessageBinary(const antlr4::Token& token, const std::string& operatorName,
                                                     SymbolType lhs, SymbolType rhs) {
    return SemanticError(token, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + operatorName +
                        "' operator on types " + lhs.getName(true) + " and " + rhs.getName(true));
}

SemanticError OpRuleManager::printErrorMessageUnary(const antlr4::Token& token, const std::string& operatorName, SymbolType lhs) {
    return SemanticError(token, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + operatorName +
                        "' operator on type " + lhs.getName(true));
}
