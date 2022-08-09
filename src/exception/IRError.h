// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

// Forward declarations
struct CodeLoc;

enum IRErrorType {
  TARGET_NOT_AVAILABLE,
  CANT_OPEN_OUTPUT_FILE,
  WRONG_TYPE,
  BRANCH_NOT_FOUND,
  UNEXPECTED_DYN_TYPE_IR,
  PRINTF_NULL_TYPE,
  VARIABLE_NOT_FOUND,
  INVALID_FUNCTION,
  INVALID_MODULE,
  COMING_SOON_IR,
};

/**
 * Custom exception for errors, occurring in the code generation phase
 */
class IRError : public std::exception {
public:
  // Constructors
  explicit IRError(const CodeLoc &codeLoc, const IRErrorType &errorType, const std::string &messageSuffix);
  explicit IRError(const IRErrorType &errorType, const std::string &messageSuffix);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(IRErrorType errorType);
};