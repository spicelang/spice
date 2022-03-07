// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ErrorFactory.h"

SemanticError ErrorFactory::get(const antlr4::Token& token, SemanticErrorType type, const std::string& message) {
    return SemanticError(fileName, token, type, message);
}

IRError ErrorFactory::get(const antlr4::Token& token, IRErrorType type, const std::string& message) {
    return IRError(fileName, token, type, message);
}

IRError ErrorFactory::get(IRErrorType type, const std::string& message) {
    return IRError(fileName, type, message);
}
