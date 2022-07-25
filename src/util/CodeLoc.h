// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

struct CodeLoc {
public:
  // Constructors
  CodeLoc(const std::string &sourceFilePath, size_t line, size_t col) : sourceFilePath(sourceFilePath), line(line), col(col) {}
  CodeLoc(const antlr4::Token *token) : line(token->getLine()), col(token->getCharPositionInLine() + 1) {}

  // Public members
  std::string sourceFilePath;
  size_t line;
  size_t col;

  // Public methods
  std::string toString() const;
  std::string toPrettyString() const;
};