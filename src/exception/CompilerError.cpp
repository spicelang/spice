// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CompilerError.h"

#include <util/CodeLoc.h>

namespace spice::compiler {

CompilerError::CompilerError(const CompilerErrorType &type, const std::string &message) {
  errorMessage = "[Error|Compiler]:\n";
  errorMessage += getMessagePrefix(type) + ": " + message;
}

CompilerError::CompilerError(const CodeLoc &codeLoc, const CompilerErrorType &type, const std::string &message) {
  errorMessage = "[Error|Compiler] " + codeLoc.toPrettyString() + ":\n";
  errorMessage += getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *CompilerError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string CompilerError::getMessagePrefix(CompilerErrorType type) {
  switch (type) {
  case UNRESOLVED_SOFT_ERRORS:
    return "Unresolved soft errors";
  case SOURCE_FILE_NOT_FOUND:
    return "Source file not found";
  case CANT_OPEN_OUTPUT_FILE:
    return "Could not open output file";
  case WRONG_OUTPUT_TYPE:
    return "Wrong type of output file";
  case INTERNAL_ERROR:
    return "Internal compiler error";
  case IO_ERROR:
    return "I/O Error";
  case STD_NOT_FOUND:
    return "Std lib not found";
  case UNHANDLED_BRANCH:
    return "Unhandled code branch";
  case TYPE_CHECKER_RUNS_EXCEEDED:
    return "Type-checker runs exceeded";
  case TARGET_NOT_AVAILABLE:
    return "Selected target not available";
  case BRANCH_NOT_FOUND:
    return "Branch not found";
  case REFERENCED_UNDEFINED_FUNCTION_IR:
    return "Referenced undefined function";
  case PRINTF_NULL_TYPE:
    return "Printf has null type";
  case OOM:
    return "An out of memory error occurred";
  case INVALID_FUNCTION:
    return "Invalid function";
  case INVALID_MODULE:
    return "Invalid module";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler