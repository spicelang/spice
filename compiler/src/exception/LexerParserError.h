// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <exception>
#include <utility>

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class LexerParserError : public std::exception {
public:
    // Constructors
    explicit LexerParserError(std::string message): errorMessage(std::move(message)) {}

    // Public methods
    const char* what() const noexcept override;

private:
    // Members
    std::string errorMessage;
};