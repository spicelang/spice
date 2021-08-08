// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SemanticError.h"

SemanticError::SemanticError(const antlr4::Token& token, SemanticErrorType type, const std::string &message) {
    auto codeLoc = std::to_string(token.getLine()) + ":" + std::to_string(token.getCharPositionInLine() + 1);
    errorMessage = "Semantic error at " + codeLoc + ": " + getMessagePrefix(type) + ": " + message;
}

SemanticError::SemanticError(SemanticErrorType type, const std::string &message) {
    errorMessage = "Semantic error at - " + getMessagePrefix(type) + ": " + message;
}

const char *SemanticError::what() const noexcept {
    return errorMessage.c_str();
}

std::string SemanticError::getMessagePrefix(SemanticErrorType type) {
    switch (type) {
        case REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE: return "Referenced undefined function";
        case REFERENCED_UNDEFINED_VARIABLE: return "Referenced undefined variable";
        case VARIABLE_DECLARED_TWICE: return "Multiple declarations of the same variable";
        case FUNCTION_WITHOUT_RETURN_STMT: return "Missing return statement";
        case RETURN_STMT_WITHOUT_FUNCTION: return "Return statement outside function";
        case OPERATOR_WRONG_DATA_TYPE: return "Wrong data type for operator";
        case REASSIGN_CONST_VARIABLE: return "Cannot re-assign constant variable";
        case CONDITION_MUST_BE_BOOL: return "Condition must be bool";
        case MISSING_MAIN_FUNCTION: return "Spice programs must contain a main function";
        case FCT_PARAM_IS_TYPE_DYN: return "Parameter type dyn not valid in function/procedure definition without default value";
        case INVALID_BREAK_NUMBER: return "Invalid number of breaks";
        case PRINTF_TYPE_ERROR: return "Types of printf call not matching";
    }
    return "Unknown error";
}
