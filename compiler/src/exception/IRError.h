// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>
#include <Token.h>

enum IRErrorType {
    TARGET_NOT_AVAILABLE,
    CANT_OPEN_OUTPUT_FILE,
    WRONG_TYPE,
    UNEXPECTED_DYN_TYPE,
    PRINTF_NULL_TYPE,
    VARIABLE_NOT_FOUND,
    INVALID_FUNCTION,
    INVALID_MODULE,
    COMING_SOON,
};

/**
 * Custom exception for errors, occurring in the code generation phase
 */
class IRError : public std::exception {
public:
    // Constructors
    explicit IRError(const antlr4::Token&, IRErrorType, const std::string&);

    explicit IRError(IRErrorType, const std::string&);

    // Public methods
    const char* what() const noexcept override;

private:
    // Members
    std::string errorMessage{};

    static std::string getMessagePrefix(IRErrorType);
};