// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include "antlr4-runtime.h"
#include <antlr/SpiceLexer.h>
#include <antlr/SpiceParser.h>
#include <analyzer/AnalyzerVisitor.h>

using namespace antlr4;

int main(int argc, char** argv) {
    // Read from file
    std::ifstream stream;
    stream.open(argv[1]);
    ANTLRInputStream input(stream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SpiceParser parser(&tokens);

    // Execute syntactical analysis
    tree::ParseTree *tree = parser.entry();
    AnalyzerVisitor().visit(tree);

    // Return with positive result code
    return 0;
}