// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <Token.h>

namespace spice::compiler {

struct CodeLoc {
public:
  // Constructors
  CodeLoc(size_t line, size_t col, std::string sourceFilePath = "");
  explicit CodeLoc(const antlr4::Token *token, std::string sourceFilePath = "");

  // Public members
  std::string sourceFilePath;
  size_t line;
  size_t col;

  // Public methods
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] std::string toPrettyString() const;
  [[nodiscard]] std::string toPrettyLine() const;
};

} // namespace spice::compiler