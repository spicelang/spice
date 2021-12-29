// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

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

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, PREFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getPostfixPlusPlusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, POSTFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPostfixMinusMinusResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, POSTFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getNotResultType(const antlr4::Token& token, const SymbolType& lhs) {
    return validateUnaryOperation(token, NOT_OP_RULES, "!", lhs);
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
                        "' operator on types " + lhs.getName() + " and " + rhs.getName());
}

SemanticError OpRuleManager::printErrorMessageUnary(const antlr4::Token& token, const std::string& operatorName, SymbolType lhs) {
    return SemanticError(token, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + operatorName +
                        "' operator on type " + lhs.getName());
}
