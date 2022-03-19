// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

enum CliErrorType {
  INCOMPLETE_TARGET_TRIPLE,
  INVALID_TARGET_TRIPLE,
  SOURCE_FILE_MISSING,
  COMING_SOON_CLI,
};

/**
 * Custom exception for errors, occurring when parsing the cli arguments
 */
class CliError : public std::exception {
public:
  // Constructors
  explicit CliError(const CliErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  [[nodiscard]] std::string getMessagePrefix(CliErrorType errorType) const;
};