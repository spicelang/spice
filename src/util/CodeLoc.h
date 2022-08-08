// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <Token.h>

struct CodeLoc {
public:
  // Constructors
  CodeLoc(std::string sourceFilePath, size_t line, size_t col)
      : sourceFilePath(std::move(sourceFilePath)), line(line), col(col) {}
  explicit CodeLoc(std::string sourceFilePath, const antlr4::Token *token)
      : sourceFilePath(std::move(sourceFilePath)), line(token->getLine()), col(token->getCharPositionInLine() + 1) {}

  // Public members
  std::string sourceFilePath;
  size_t line;
  size_t col;

  // Public methods
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] std::string toPrettyString() const;
  [[nodiscard]] std::string toPrettyLine() const;
};