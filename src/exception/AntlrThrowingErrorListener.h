// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <utility>

#include <BaseErrorListener.h>

namespace spice::compiler {

// Forward declarations
class SourceFile;

enum class ThrowingErrorListenerMode : uint8_t {
  LEXER,
  PARSER,
};

class AntlrThrowingErrorListener : public antlr4::BaseErrorListener {
public:
  // Constructors
  AntlrThrowingErrorListener(ThrowingErrorListenerMode mode, SourceFile *sourceFile) : mode(mode), sourceFile(sourceFile){};

  // Public methods
  void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                   const std::string &msg, std::exception_ptr e) override;

private:
  // Private members
  ThrowingErrorListenerMode mode;
  SourceFile *sourceFile;
};

} // namespace spice::compiler