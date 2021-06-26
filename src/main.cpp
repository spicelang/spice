// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "main.h"

int main(int argc, char** argv) {
    std::ifstream stream;
    stream.open(argv[1]);
    ANTLRInputStream input(stream);
    SpiceLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    MyGrammarParser parser(&tokens);

    tree::ParseTree *tree = parser.key();
    TreeShapeListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    // Return with positive result code
    return 0;
}