// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

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
 * @param errorType Type of the error
 * @return Prefix string for the error type
 */
std::string CompilerError::getMessagePrefix(CompilerErrorType errorType) {
  switch (errorType) {
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
  case BOOTSTRAP_NOT_FOUND:
    return "Bootstrap compiler not found";
  case UNHANDLED_BRANCH:            // LCOV_EXCL_LINE
    return "Unhandled code branch"; // LCOV_EXCL_LINE
  case TYPE_CHECKER_RUNS_EXCEEDED:
    return "Type-checker runs exceeded";
  case TARGET_NOT_AVAILABLE:
    return "Selected target not available";
  case OOM:
    return "An out of memory error occurred";
  case INVALID_FUNCTION:       // LCOV_EXCL_LINE
    return "Invalid function"; // LCOV_EXCL_LINE
  case INVALID_MODULE:         // LCOV_EXCL_LINE
    return "Invalid module";   // LCOV_EXCL_LINE
  }
  assert_fail("Unknown error"); // GCOV_EXCL_LINE
  return "Unknown error";       // GCOV_EXCL_LINE
}

} // namespace spice::compiler