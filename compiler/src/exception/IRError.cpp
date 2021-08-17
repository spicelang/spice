// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "IRError.h"

IRError::IRError(const antlr4::Token& token, IRErrorType type, const std::string& message) {
    auto codeLoc = std::to_string(token.getLine()) + ":" + std::to_string(token.getCharPositionInLine() + 1);
    errorMessage = "Internal compiler error at " + codeLoc + " - " + getMessagePrefix(type) + ": " + message;
}

IRError::IRError(IRErrorType type, const std::string& message) {
    errorMessage = "Internal compiler error - " + getMessagePrefix(type) + ": " + message;
}

const char* IRError::what() const noexcept {
    return errorMessage.c_str();
}

std::string IRError::getMessagePrefix(IRErrorType type) {
    switch (type) {
        case TARGET_NOT_AVAILABLE:
            return "Selected target not available";
        case CANT_OPEN_OUTPUT_FILE:
            return "Could not open output file";
        case WRONG_TYPE:
            return "Wrong type of output file";
        case UNEXPECTED_DYN_TYPE:
            return "Unexpected type of dyn. Symbol table incomplete";
        case PRINTF_NULL_TYPE:
            return "Printf has null type";
        case VARIABLE_NOT_FOUND:
            return "Variable not found";
        case INVALID_FUNCTION:
            return "Invalid function";
        case INVALID_MODULE:
            return "Invalid module";
    }
    return "Unknown error";
}
