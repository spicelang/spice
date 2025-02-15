// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "ParserError.h"

#include <util/CodeLoc.h>

namespace spice::compiler {

/**
 * Constructor: Used in case that the exact code position where the error occurred is known
 *
 * @param codeLoc Code location where the error occurred
 * @param type Type of the error
 * @param message Error message suffix
 */
ParserError::ParserError(const CodeLoc &codeLoc, const ParserErrorType &type, const std::string &message) {
  errorMessage = "[Error|Parser] " + codeLoc.toPrettyString() + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *ParserError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param errorType Type of the error
 * @return Prefix string for the error type
 */
std::string ParserError::getMessagePrefix(ParserErrorType errorType) {
  switch (errorType) {
  case PARSING_FAILED:
    return "Parsing failed";
  case NUMBER_OUT_OF_RANGE:
    return "Number is out of range";
  case INVALID_QUALIFIER_COMBINATION:
    return "Invalid qualifier combination";
  case INVALID_CHAR_LITERAL:
    return "Invalid char literal";
  case INVALID_ATTR_VALUE_TYPE:
    return "Invalid attribute value type";
  case RESERVED_KEYWORD:
    return "Usage of reserved keyword";
  }
  assert_fail("Unknown error"); // GCOV_EXCL_LINE
  return "Unknown error";       // GCOV_EXCL_LINE
}

} // namespace spice::compiler