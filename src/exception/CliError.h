// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <exception>
#include <string>

namespace spice::compiler {

// GCOV_EXCL_START

enum CliErrorType : uint8_t {
  INCOMPLETE_TARGET_TRIPLE,
  INVALID_TARGET_TRIPLE,
  SOURCE_FILE_MISSING,
  INCOMPATIBLE_OPTIONS,
  NON_ZERO_EXIT_CODE,
  FEATURE_NOT_SUPPORTED_FOR_TARGET,
  FEATURE_NOT_SUPPORTED_WHEN_DOCKERIZED,
  INVALID_BUILD_MODE,
  INVALID_OUTPUT_CONTAINER,
  INVALID_SANITIZER
};

/**
 * Custom exception for errors, occurring when parsing the cli arguments
 */
class CliError final : public std::exception {
public:
  // Constructors
  CliError(const CliErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  [[nodiscard]] static std::string getMessagePrefix(CliErrorType errorType);
};

// GCOV_EXCL_STOP

} // namespace spice::compiler