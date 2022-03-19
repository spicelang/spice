// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AntlrThrowingErrorListener.h"

#include "LexerParserError.h"

void AntlrThrowingErrorListener::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                                             size_t charPositionInLine, const std::string& msg, std::exception_ptr e) {
    // Create message fragments
    std::string errorType = mode == LEXER ? "Lexer error" : "Parser error";
    std::string codePos = std::to_string(line) + ":" + std::to_string(charPositionInLine);
    // Throw error
    throw LexerParserError(errorType + " at line " + codePos + " " + msg);
}
