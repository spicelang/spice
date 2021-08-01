// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

enum SemanticErrorType {
    REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
    CAN_ONLY_CALL_FUNCTION_OR_PROCEDURE,
    REFERENCED_UNDEFINED_VARIABLE,
    VARIABLE_DECLARED_TWICE,
    FUNCTION_WITHOUT_RETURN_STMT,
    RETURN_STMT_WITHOUT_FUNCTION,
    OPERATOR_WRONG_DATA_TYPE,
    REASSIGN_CONST_VARIABLE,
    CONDITION_MUST_BE_BOOL,
    PARAMETER_TYPES_DO_NOT_MATCH,
    MISSING_MAIN_FUNCTION,
    FCT_PARAM_IS_TYPE_DYN,
};

class SemanticError : public std::exception {
public:
    // Constructors
    explicit SemanticError(SemanticErrorType type, const std::string& message) {
        std::string messagePrefix;
        switch (type) {
            case REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE:
                messagePrefix = "Referenced undefined function";
                break;
            case CAN_ONLY_CALL_FUNCTION_OR_PROCEDURE:
                messagePrefix = "Can only call function or procedure";
                break;
            case REFERENCED_UNDEFINED_VARIABLE:
                messagePrefix = "Referenced undefined variable";
                break;
            case VARIABLE_DECLARED_TWICE:
                messagePrefix = "Multiple declarations of the same variable";
                break;
            case FUNCTION_WITHOUT_RETURN_STMT:
                messagePrefix = "Missing return statement";
                break;
            case RETURN_STMT_WITHOUT_FUNCTION:
                messagePrefix = "Return statement outside function";
                break;
            case OPERATOR_WRONG_DATA_TYPE:
                messagePrefix = "Wrong data type for operator";
                break;
            case REASSIGN_CONST_VARIABLE:
                messagePrefix = "Cannot re-assign constant variable";
                break;
            case CONDITION_MUST_BE_BOOL:
                messagePrefix = "Condition must be bool";
                break;
            case PARAMETER_TYPES_DO_NOT_MATCH:
                messagePrefix = "Parameter types do not match";
                break;
            case MISSING_MAIN_FUNCTION:
                messagePrefix = "Spice programs must contain a main function";
                break;
            case FCT_PARAM_IS_TYPE_DYN:
                messagePrefix = "Parameter type dyn not valid in function/procedure definition without default value";
                break;
        }
        errorMessage = messagePrefix + ": " + message;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
