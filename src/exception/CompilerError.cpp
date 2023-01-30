// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "CompilerError.h"

namespace spice::compiler {

CompilerError::CompilerError(const CompilerErrorType &type, const std::string &message) {
  errorMessage = "[Error|Compiler]:\n";
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
  case INTERNAL_ERROR:
    return "Internal compiler error";
  case IO_ERROR:
    return "I/O Error";
  case UNHANDLED_BRANCH:
    return "Unhandled code branch";
  case SOURCE_FILE_NOT_FOUND:
    return "Source file not found";
  case TYPE_CHECKER_RUNS_EXCEEDED:
    return "Type-checker runs exceeded";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler