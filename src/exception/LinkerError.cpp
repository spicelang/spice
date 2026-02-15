// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "LinkerError.h"

#include <util/GlobalDefinitions.h>

namespace spice::compiler {

/**
 * @param type Type of the error
 * @param message Error message suffix
 */
LinkerError::LinkerError(const LinkerErrorType &type, const std::string &message) {
  errorMessage = "[Error|Linker] " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *LinkerError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param errorType Type of the error
 * @return Prefix string for the error type
 */
std::string LinkerError::getMessagePrefix(LinkerErrorType errorType) {
  switch (errorType) {
  case LINKER_INVOKER_NOT_FOUND:
    return "Linker invoker not found";
  case LINKER_NOT_FOUND:
    return "Linker not found";
  case ARCHIVER_NOT_FOUND:
    return "Archiver not found";
  case LINKER_ERROR:
    return "Linker error occurred";
  }
  assert_fail("Unknown error"); // GCOV_EXCL_LINE
  return "Unknown error";       // GCOV_EXCL_LINE
}

} // namespace spice::compiler