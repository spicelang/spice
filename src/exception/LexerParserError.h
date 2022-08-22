// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>
#include <utility>

// Forward declarations
struct CodeLoc;

enum LexerParserErrorType { TOKENIZING_FAILED, PARSING_FAILED, NUMBER_OUT_OF_RANGE, INVALID_CHAR_LITERAL };

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class LexerParserError : public std::exception {
public:
  // Constructors
  explicit LexerParserError(const CodeLoc &codeLoc, const LexerParserErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(LexerParserErrorType errorType);
};