// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>
#include <TokenSource.h>
#include <Token.h>

enum SemanticErrorType {
    REFERENCED_UNDEFINED_FUNCTION_OR_PROCEDURE,
    REFERENCED_UNDEFINED_VARIABLE,
    REFERENCED_UNDEFINED_STRUCT,
    VARIABLE_DECLARED_TWICE,
    FUNCTION_DECLARED_TWICE,
    PROCEDURE_DECLARED_TWICE,
    STRUCT_DECLARED_TWICE,
    FUNCTION_WITHOUT_RETURN_STMT,
    RETURN_STMT_WITHOUT_FUNCTION,
    OPERATOR_WRONG_DATA_TYPE,
    REASSIGN_CONST_VARIABLE,
    CONDITION_MUST_BE_BOOL,
    MISSING_MAIN_FUNCTION,
    FCT_PARAM_IS_TYPE_DYN,
    INVALID_BREAK_NUMBER,
    INVALID_CONTINUE_NUMBER,
    PRINTF_TYPE_ERROR,
    STD_NOT_FOUND,
    IMPORTED_FILE_NOT_EXISTING,
    CIRCULAR_DEPENDENCY,
    UNKNOWN_DATATYPE,
    NUMBER_OF_FIELDS_NOT_MATCHING,
    FIELD_TYPE_NOT_MATCHING,
};

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class SemanticError : public std::exception {
public:
    // Constructors
    explicit SemanticError(const antlr4::Token& token, SemanticErrorType type, const std::string& message);

    explicit SemanticError(SemanticErrorType type, const std::string& message);

    // Public methods
    const char* what() const noexcept override;

private:
    // Members
    std::string errorMessage{};

    static std::string getMessagePrefix(SemanticErrorType);
};
