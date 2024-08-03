// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <exception>
#include <string>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;

enum LexerErrorType : uint8_t { TOKENIZING_FAILED };

/**
 * Custom exception for errors, occurring while lexing
 */
class LexerError final : public std::exception {
public:
  // Constructors
  LexerError(const CodeLoc &codeLoc, const LexerErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;
  [[nodiscard]] static std::string getMessagePrefix(LexerErrorType errorType);

  // Public members
  std::string errorMessage;
};

} // namespace spice::compiler