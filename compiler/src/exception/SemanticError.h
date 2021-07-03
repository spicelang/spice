// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

enum SemanticErrorType {
    REFERENCED_UNDEFINED_FUNCTION,
    REFERENCED_UNDEFINED_PROCEDURE,
    REFERENCED_UNDEFINED_VARIABLE,
    FUNCTION_WITHOUT_RETURN_STMT,
    OPERATOR_WRONG_DATA_TYPE
};

class SemanticError : public std::exception {
public:
    // Constructors
    explicit SemanticError(SemanticErrorType type, const std::string& message) {
        std::string messagePrefix;
        switch (type) {
            case REFERENCED_UNDEFINED_FUNCTION:
                messagePrefix = "Referenced undefined function";
                break;
            case REFERENCED_UNDEFINED_PROCEDURE:
                messagePrefix = "Referenced undefined procedure";
                break;
            case REFERENCED_UNDEFINED_VARIABLE:
                messagePrefix = "Referenced undefined variable";
                break;
            case FUNCTION_WITHOUT_RETURN_STMT:
                messagePrefix = "Missing return statement";
                break;
            case OPERATOR_WRONG_DATA_TYPE:
                messagePrefix = "Wrong data type for operator";
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
