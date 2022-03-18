// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

enum CompilerWarningType {
    UNUSED_FUNCTION,
    UNUSED_PROCEDURE,
    UNUSED_STRUCT,
    UNUSED_IMPORT,
    UNUSED_VARIABLE,
    ARRAY_TOO_MANY_VALUES,
    INDEX_EXCEEDS_ARRAY_SIZE,
    NEGATIVE_VALUE_TO_UNSIGNED_VAR
};

/**
 * Compiler warning template engine
 */
class CompilerWarning {
public:
    // Constructors
    explicit CompilerWarning(const antlr4::Token& token, CompilerWarningType type, const std::string& message);

    void print() const;
private:
    // Members
    std::string warningMessage{};

    [[nodiscard]] std::string getMessagePrefix(CompilerWarningType warningType) const;
};