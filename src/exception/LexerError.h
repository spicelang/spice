// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>
#include <utility>

// Forward declarations
struct CodeLoc;

enum LexerErrorType { TOKENIZING_FAILED };

/**
 * Custom exception for errors, occurring while lexing
 */
class LexerError : public std::exception {
public:
  // Constructors
  explicit LexerError(const CodeLoc &codeLoc, const LexerErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(LexerErrorType errorType);
};