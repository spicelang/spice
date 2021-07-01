// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include "antlr4-runtime.h"
//#include "antlr/SpiceLexer.h"
//#include "antlr/SpiceParser.h"
//#include "analyzer/AnalyzerVisitor.h"

int main(int argc, char** argv) {
    // Read from file
    /*std::ifstream stream;
    stream.open(argv[1]);
    antlr4::ANTLRInputStream input(stream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens);

    // Execute syntactical analysis
    antlr4::tree::ParseTree *tree = parser.entry();
    AnalyzerVisitor().visit(tree);*/

    // Return with positive result code
    std::cout << "Hello world" << std::endl;
    return 0;
}