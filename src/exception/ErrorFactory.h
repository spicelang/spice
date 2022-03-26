// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <exception/CliError.h>
#include <exception/IRError.h>
#include <exception/LinkerError.h>
#include <exception/SemanticError.h>

#include "Token.h"

class ErrorFactory {
public:
  // Constructors
  ErrorFactory() = default;
  explicit ErrorFactory(std::string fileName) : fileName(std::move(fileName)){};

  // Public methods
  [[nodiscard]] SemanticError get(const antlr4::Token &token, SemanticErrorType type, const std::string &message) const;
  [[nodiscard]] IRError get(const antlr4::Token &token, IRErrorType type, const std::string &message) const;
  [[nodiscard]] IRError get(IRErrorType type, const std::string &message) const;
  [[nodiscard]] static CliError get(CliErrorType type, const std::string &message);
  [[nodiscard]] static LinkerError get(LinkerErrorType type, const std::string &message);

private:
  // Members
  const std::string fileName;
};