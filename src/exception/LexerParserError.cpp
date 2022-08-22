// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "LexerParserError.h"

#include <util/CodeLoc.h>

/**
 * Constructor: Used in case that the exact code position where the error occurred is known
 *
 * @param codeLoc Code location where the error occurred
 * @param type Type of the error
 * @param message Error message suffix
 */
LexerParserError::LexerParserError(const CodeLoc &codeLoc, const LexerParserErrorType &type, const std::string &message) {
  errorMessage = "Error in " + codeLoc.toPrettyString() + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *LexerParserError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string LexerParserError::getMessagePrefix(LexerParserErrorType type) {
  switch (type) {
  case TOKENIZING_FAILED:
    return "Tokenizing failed";
  case PARSING_FAILED:
    return "Parsing failed";
  case NUMBER_OUT_OF_RANGE:
    return "Number is out of range";
  case INVALID_CHAR_LITERAL:
    return "Invalid char literal";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}