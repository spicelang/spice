// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;

enum CompilerErrorType : uint8_t {
  UNRESOLVED_SOFT_ERRORS,
  SOURCE_FILE_NOT_FOUND,
  CANT_OPEN_OUTPUT_FILE,
  WRONG_OUTPUT_TYPE,
  INTERNAL_ERROR,
  IO_ERROR,
  STD_NOT_FOUND,
  BOOTSTRAP_NOT_FOUND,
  UNHANDLED_BRANCH,
  TYPE_CHECKER_RUNS_EXCEEDED,
  TARGET_NOT_AVAILABLE,
  OOM,
  INVALID_FUNCTION,
  INVALID_MODULE
};

/**
 * Custom exception for errors, occurring in the general context of the compiler
 */
class CompilerError final : public std::exception {
public:
  // Constructors
  CompilerError(const CompilerErrorType &type, const std::string &message);
  CompilerError(const CodeLoc &codeLoc, const CompilerErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;
  [[nodiscard]] static std::string getMessagePrefix(CompilerErrorType errorType);

  // Public members
  std::string errorMessage;
};

} // namespace spice::compiler