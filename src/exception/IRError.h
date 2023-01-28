// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;

enum IRErrorType {
  TARGET_NOT_AVAILABLE,
  CANT_OPEN_OUTPUT_FILE,
  WRONG_TYPE,
  BRANCH_NOT_FOUND,
  REFERENCED_UNDEFINED_FUNCTION_IR,
  PRINTF_NULL_TYPE,
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
  IRError(const CodeLoc &codeLoc, const IRErrorType &errorType, const std::string &messageSuffix);
  IRError(const IRErrorType &errorType, const std::string &messageSuffix);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(IRErrorType errorType);
};

} // namespace spice::compiler