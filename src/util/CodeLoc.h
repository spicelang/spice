// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>
#include <utility>

#include <Token.h>

#include <util/GlobalDefinitions.h>

namespace spice::compiler {

struct CodeLoc {
public:
  // Constructors
  CodeLoc(size_t line, size_t col, std::filesystem::path sourceFilePath = "")
      : line(line), col(col), sourceFilePath(std::move(sourceFilePath)) {}
  explicit CodeLoc(const antlr4::Token *token, std::filesystem::path sourceFilePath = "")
      : line(token->getLine()), col(token->getCharPositionInLine() + 1), sourceFilePath(std::move(sourceFilePath)){};

  // Public members
  std::filesystem::path sourceFilePath;
  size_t line;
  size_t col;

  // Public methods
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] std::string toPrettyString() const;
  [[nodiscard]] std::string toPrettyLine() const;
  [[nodiscard]] std::string toPrettyLineAndColumn() const;

  // Operators
  ALWAYS_INLINE friend bool operator==(const CodeLoc &a, const CodeLoc &b) {
    return a.sourceFilePath == b.sourceFilePath && a.line == b.line && a.col == b.col;
  }
  ALWAYS_INLINE friend bool operator<(const CodeLoc &a, const CodeLoc &b) {
    return a.line == b.line ? a.col < b.col : a.line < b.line;
  }
  ALWAYS_INLINE friend bool operator>(const CodeLoc &a, const CodeLoc &b) {
    return a.line == b.line ? a.col > b.col : a.line > b.line;
  }
};

} // namespace spice::compiler