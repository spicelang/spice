// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "AntlrThrowingErrorListener.h"

#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

void AntlrThrowingErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {
  const CodeLoc codeLoc(line, charPositionInLine, sourceFile);
  if (mode == ThrowingErrorListenerMode::LEXER)
    throw LexerError(codeLoc, TOKENIZING_FAILED, msg);
  else
    throw ParserError(codeLoc, PARSING_FAILED, msg);
}

} // namespace spice::compiler