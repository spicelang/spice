// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CliError.h"

namespace spice::compiler {

/**
 * @param type Type of the error
 * @param message Error message suffix
 */
CliError::CliError(const CliErrorType &type, const std::string &message) {
  errorMessage = "[Error|CLI] " + getMessagePrefix(type) + ": " + message;
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
 * @param errorType Type of the error
 * @return Prefix string for the error type
 */
std::string CliError::getMessagePrefix(CliErrorType errorType) {
  switch (errorType) {
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
  case FEATURE_NOT_SUPPORTED_WHEN_DOCKERIZED:
    return "Feature not supported when dockerized";
  case INVALID_BUILD_MODE:
    return "Invalid build mode";
  case COMING_SOON_CLI:
    return "Coming soon";
  }
  return "Unknown error";
}

} // namespace spice::compiler