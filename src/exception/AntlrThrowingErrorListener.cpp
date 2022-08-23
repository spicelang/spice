// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AntlrThrowingErrorListener.h"

#include <exception/LexerParserError.h>
#include <util/CodeLoc.h>

void AntlrThrowingErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {
  CodeLoc codeLoc = CodeLoc("", line, charPositionInLine);
  throw LexerParserError(codeLoc, mode == LEXER ? TOKENIZING_FAILED : PARSING_FAILED, msg);
}