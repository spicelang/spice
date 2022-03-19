// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ErrorFactory.h"

SemanticError ErrorFactory::get(const antlr4::Token& token, SemanticErrorType type, const std::string& message) const {
    if (fileName.empty()) throw std::runtime_error("Internal compiler error: Calling get<SemanticError> without filename");
    return SemanticError(fileName, token, type, message);
}

IRError ErrorFactory::get(const antlr4::Token& token, IRErrorType type, const std::string& message) const {
    if (fileName.empty()) throw std::runtime_error("Internal compiler error: Calling get<IRError> without filename");
    return IRError(fileName, token, type, message);
}

IRError ErrorFactory::get(IRErrorType type, const std::string& message) const {
    if (fileName.empty()) throw std::runtime_error("Internal compiler error: Calling get<IRError> without filename");
    return IRError(fileName, type, message);
}

CliError ErrorFactory::get(CliErrorType type, const std::string& message) const {
    return CliError(type, message);
}

LinkerError ErrorFactory::get(LinkerErrorType type, const std::string& message) const {
    return LinkerError(type, message);
}