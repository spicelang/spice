// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

struct CodeLoc {
public:
  // Constructors
  CodeLoc(size_t line, size_t col) : line(line), col(col) {}
  CodeLoc(const antlr4::Token *token) : line(token->getLine()), col(token->getCharPositionInLine() + 1) {}

  // Public members
  size_t line;
  size_t col;

  // Public methods
  std::string toString() const;
};