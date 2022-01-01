// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerWarning.h"

/**
 * Constructor: Used in case that the exact code position where the warning occurred is known
 *
 * @param token Syntax token, where the warning occurred
 * @param type Type of the warning
 * @param message Warning message suffix
 */
CompilerWarning::CompilerWarning(const antlr4::Token& token, CompilerWarningType type, const std::string& message) {
    auto codeLoc = std::to_string(token.getLine()) + ":" + std::to_string(token.getCharPositionInLine() + 1);
    warningMessage = "Warning at " + codeLoc + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Print the compiler warning to the standard error output
 */
void CompilerWarning::print() {
    if (!warningMessage.empty())
        std::cout << warningMessage << std::endl;
}

/**
 * Get the prefix of the warning message for a particular error
 *
 * @param type Type of the warning
 * @return Prefix string for the warning type
 */
std::string CompilerWarning::getMessagePrefix(CompilerWarningType type) {
    switch (type) {
        case UNUSED_FUNCTION:
            return "Unused function";
        case UNUSED_PROCEDURE:
            return "Unused procedure";
        case UNUSED_STRUCT:
            return "Unused struct";
        case UNUSED_IMPORT:
            return "Unused import";
        case UNUSED_VARIABLE:
            return "Unused variable";
        case ARRAY_TOO_MANY_VALUES:
            return "Array initialization item count exceeds arrays capacity";
        case INDEX_EXCEEDS_ARRAY_SIZE:
            return "Array index exceeds its size";
    }
    return "Unknown warning";
}
