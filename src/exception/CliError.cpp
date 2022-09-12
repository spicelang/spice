// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CliError.h"

/**
 * @param token Syntax token, where the error occurred
 * @param type Type of the error
 * @param message Error message suffix
 */
CliError::CliError(const CliErrorType &type, const std::string &message) {
  errorMessage = "CLI error: " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *CliError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string CliError::getMessagePrefix(CliErrorType type) {
  switch (type) {
  case INCOMPLETE_TARGET_TRIPLE:
    return "Incomplete target triple";
  case INVALID_TARGET_TRIPLE:
    return "Invalid target triple";
  case SOURCE_FILE_MISSING:
    return "Source file missing";
  case OPT_DEBUG_INFO_INCOMPATIBILITY:
    return "Cannot emit debug info with optimization enabled";
  case NON_ZERO_EXIT_CODE:
    return "Non-zero exit code";
  case COMING_SOON_CLI:
    return "Coming soon";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}