// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace spice::compiler {

enum LinkerErrorType : uint8_t {
  LINKER_NOT_FOUND,
  LINKER_ERROR,
};

/**
 * Custom exception for errors, occurring when linking the output executable
 */
class LinkerError : public std::exception {
public:
  // Constructors
  LinkerError(const LinkerErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;
  [[nodiscard]] static std::string getMessagePrefix(LinkerErrorType errorType);

  // Public members
  std::string errorMessage;
};

} // namespace spice::compiler