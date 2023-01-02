// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>

namespace spice::compiler {

enum LinkerErrorType {
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

private:
  // Members
  std::string errorMessage;

  [[nodiscard]] static std::string getMessagePrefix(LinkerErrorType errorType);
};

} // namespace spice::compiler