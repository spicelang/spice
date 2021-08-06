// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

enum IRErrorType {
    TARGET_NOT_AVAILABLE,
    CANT_OPEN_OUTPUT_FILE,
    WRONG_TYPE,
    UNEXPECTED_DYN_TYPE,
    PRINTF_NULL_TYPE,
    VARIABLE_NOT_FOUND,
    INVALID_FUNCTION,
    INVALID_MODULE
};

class IRError : public std::exception {
public:
    // Constructors
    explicit IRError(IRErrorType type, const std::string& message) {
        std::string messagePrefix;
        switch (type) {
            case TARGET_NOT_AVAILABLE:
                messagePrefix = "Selected target not available";
                break;
            case CANT_OPEN_OUTPUT_FILE:
                messagePrefix = "Could not open output file";
                break;
            case WRONG_TYPE:
                messagePrefix = "Wrong type of output file";
                break;
            case UNEXPECTED_DYN_TYPE:
                messagePrefix = "Unexpected type of dyn. Symbol table incomplete";
                break;
            case PRINTF_NULL_TYPE:
                messagePrefix = "Printf has null type";
                break;
            case VARIABLE_NOT_FOUND:
                messagePrefix = "Variable not found";
                break;
            case INVALID_FUNCTION:
                messagePrefix = "Invalid function";
                break;
            case INVALID_MODULE:
                messagePrefix = "Invalid module";
                break;
        }
        errorMessage = "Internal compiler error - " + messagePrefix + ": " + message;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};