// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "LexerParserError.h"

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char* LexerParserError::what() const noexcept {
    return errorMessage.c_str();
}