// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include "string"
#include <TokenSource.h>
#include <Token.h>

enum CompilerWarningType {
    UNUSED_FUNCTION,
    UNUSED_PROCEDURE,
    UNUSED_STRUCT,
    UNUSED_IMPORT,
    UNUSED_VARIABLE,
    ARRAY_TOO_MANY_VALUES,
};

/**
 * Compiler warning template engine
 */
class CompilerWarning {
public:
    // Constructors
    explicit CompilerWarning(const antlr4::Token& token, CompilerWarningType type, const std::string& message);

    void print();
private:
    // Members
    std::string warningMessage{};

    static std::string getMessagePrefix(CompilerWarningType);
};