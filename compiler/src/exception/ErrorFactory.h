// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "Token.h"
#include "SemanticError.h"
#include "IRError.h"

class ErrorFactory {
public:
    // Constructors
    explicit ErrorFactory(std::string fileName) : fileName(std::move(fileName)) {};

    // Public methods
    SemanticError get(const antlr4::Token& token, SemanticErrorType type, const std::string& message);
    IRError get(const antlr4::Token& token, IRErrorType type, const std::string& message);
    IRError get(IRErrorType type, const std::string& message);
private:
    // Members
    std::string fileName;
};