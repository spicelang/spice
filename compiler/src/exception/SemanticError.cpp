// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SemanticError.h"

/**
 * Constructor: Used in case that the exact code position where the error occurred is known
 *
 * @param token Syntax token, where the error occurred
 * @param type Type of the error
 * @param message Error message suffix
 */
SemanticError::SemanticError(const antlr4::Token& token, SemanticErrorType type, const std::string& message) {
    auto codeLoc = std::to_string(token.getLine()) + ":" + std::to_string(token.getCharPositionInLine() + 1);
    errorMessage = "Semantic error at " + codeLoc + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Constructor: Used in case that the exact code position where the error occurred is unknown
 *
 * @param type Type of the error
 * @param message Error message suffix
 */
SemanticError::SemanticError(SemanticErrorType type, const std::string& message) {
    errorMessage = "Semantic error - " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char* SemanticError::what() const noexcept {
    return errorMessage.c_str();
}

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string SemanticError::getMessagePrefix(SemanticErrorType type) {
    switch (type) {
        case REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE:
            return "Referenced undefined function";
        case REFERENCED_UNDEFINED_VARIABLE:
            return "Referenced undefined variable";
        case REFERENCED_UNDEFINED_STRUCT:
            return "Referenced undefined struct";
        case VARIABLE_DECLARED_TWICE:
            return "Multiple declarations of the same variable";
        case FUNCTION_DECLARED_TWICE:
            return "Multiple declarations of a function with the same name";
        case PROCEDURE_DECLARED_TWICE:
            return "Multiple declarations of a procedure with the same name";
        case STRUCT_DECLARED_TWICE:
            return "Multiple declarations of a struct with the same name";
        case FUNCTION_WITHOUT_RETURN_STMT:
            return "Missing return statement";
        case RETURN_STMT_WITHOUT_FUNCTION:
            return "Return statement outside function";
        case OPERATOR_WRONG_DATA_TYPE:
            return "Wrong data type for operator";
        case REASSIGN_CONST_VARIABLE:
            return "Cannot re-assign constant variable";
        case CONDITION_MUST_BE_BOOL:
            return "Condition must be bool";
        case MISSING_MAIN_FUNCTION:
            return "Spice programs must contain a main function";
        case FCT_PARAM_IS_TYPE_DYN:
            return "Parameter type dyn not valid in function/procedure definition without default value";
        case INVALID_BREAK_NUMBER:
            return "Invalid number of break calls";
        case INVALID_CONTINUE_NUMBER:
            return "Invalid number of continue calls";
        case PRINTF_TYPE_ERROR:
            return "Types of printf call not matching";
        case STD_NOT_FOUND:
            return "Std lib not found";
        case IMPORTED_FILE_NOT_EXISTING:
            return "Imported source file not existing";
        case CIRCULAR_DEPENDENCY:
            return "Circular import detected";
        case UNKNOWN_DATATYPE:
            return "Unknown datatype";
    }
    return "Unknown error";
}
