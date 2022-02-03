// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <BaseErrorListener.h>

enum Mode {
    LEXER,
    PARSER
};

class AntlrThrowingErrorListener : public antlr4::BaseErrorListener {
public:
    explicit AntlrThrowingErrorListener(Mode mode): mode(mode) {};
    void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token * offendingSymbol, size_t line,
                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override;
private:
    Mode mode;
};
