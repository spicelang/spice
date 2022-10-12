// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AntlrThrowingErrorListener.h"

#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <util/CodeLoc.h>

void AntlrThrowingErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {
  if (mode == LEXER)
    throw LexerError(CodeLoc(line, charPositionInLine), TOKENIZING_FAILED, msg);
  else
    throw ParserError(CodeLoc(line, charPositionInLine), PARSING_FAILED, msg);
}