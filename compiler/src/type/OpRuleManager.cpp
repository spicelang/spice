// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

SymbolType OpRuleManager::getPlusResultType(const SymbolType& lhs, const SymbolType& rhs) {
    return validateOperation(plusOpRules, "+", lhs, rhs);
}

SymbolType OpRuleManager::getMinusResultType(const SymbolType& lhs, const SymbolType& rhs) {
    return validateOperation(minusOpRules, "-", lhs, rhs);
}





SymbolType OpRuleManager::validateOperation(const std::vector<BinaryOpRule>& opRules, const std::string& opName,
                                            const SymbolType& lhs, const SymbolType& rhs) {
    for (auto& rule : opRules) {
        if (std::get<0>(rule) == lhs && std::get<1>(rule) == rhs)
            return std::get<2>(rule);
    }
    throw printErrorMessage(opName, lhs, rhs);
}

SemanticError OpRuleManager::printErrorMessage(const std::string& operatorName, SymbolType lhs, SymbolType rhs) {
    return SemanticError(OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + operatorName +
                        "' operator on types " + lhs.getName() + " and " + rhs.getName());
}