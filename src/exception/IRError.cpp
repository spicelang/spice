// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRError.h"

#include <util/CodeLoc.h>

/**
 * Constructor: Used in case that the exact code position where the error occurred is unknown
 *
 * @param codeLoc Code location where the error occurred
 * @param type Type of the error
 * @param message Error message suffix
 */
IRError::IRError(const CodeLoc &codeLoc, const IRErrorType &type, const std::string &message) {
  errorMessage = "[Error|IR] " + codeLoc.toPrettyString() + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Constructor: Used in case that the exact code position where the error occurred is unknown
 *
 * @param type Type of the error
 * @param message Error message suffix
 */
IRError::IRError(const IRErrorType &type, const std::string &message) {
  errorMessage = "[Error|IR] " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *IRError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string IRError::getMessagePrefix(IRErrorType type) {
  switch (type) {
  case TARGET_NOT_AVAILABLE:
    return "Selected target not available";
  case CANT_OPEN_OUTPUT_FILE:
    return "Could not open output file";
  case WRONG_TYPE:
    return "Wrong type of output file";
  case BRANCH_NOT_FOUND:
    return "Branch not found";
  case REFERENCED_UNDEFINED_FUNCTION_IR:
    return "Referenced undefined function";
  case UNEXPECTED_DYN_TYPE_IR:
    return "Unexpected type of dyn. Symbol table incomplete";
  case PRINTF_NULL_TYPE:
    return "Printf has null type";
  case INVALID_FUNCTION:
    return "Invalid function";
  case INVALID_MODULE:
    return "Invalid module";
  case COMING_SOON_IR:
    return "Coming soon";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}