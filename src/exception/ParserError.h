// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>
#include <utility>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;

enum ParserErrorType { PARSING_FAILED, NUMBER_OUT_OF_RANGE, INVALID_CHAR_LITERAL, RESERVED_KEYWORD };

/**
 * Custom exception for errors, occurring while parsing
 */
class ParserError : public std::exception {
public:
  // Constructors
  ParserError(const CodeLoc &codeLoc, const ParserErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(ParserErrorType errorType);
};

} // namespace spice::compiler