// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <exception>
#include <string>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;

enum ParserErrorType : uint8_t {
  PARSING_FAILED,
  NUMBER_OUT_OF_RANGE,
  INVALID_QUALIFIER_COMBINATION,
  INVALID_CHAR_LITERAL,
  INVALID_ATTR_VALUE_TYPE,
  RESERVED_KEYWORD,
  RESERVED_TYPENAME,
};

/**
 * Custom exception for errors, occurring while parsing
 */
class ParserError final : public std::exception {
public:
  // Constructors
  ParserError(const CodeLoc &codeLoc, const ParserErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;
  [[nodiscard]] static std::string getMessagePrefix(ParserErrorType errorType);

  // Public members
  std::string errorMessage;
};

} // namespace spice::compiler