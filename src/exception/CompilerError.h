// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

namespace spice::compiler {

enum CompilerErrorType { INTERNAL_ERROR, IO_ERROR, UNHANDLED_BRANCH, SOURCE_FILE_NOT_FOUND, TYPE_CHECKER_RUNS_EXCEEDED };

/**
 * Custom exception for errors, occurring in the general context of the compiler
 */
class CompilerError : public std::exception {
public:
  // Constructors
  CompilerError(const CompilerErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(CompilerErrorType errorType);
};

} // namespace spice::compiler