// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include "antlr4-runtime.h"
#include "SpiceLexer.h"
#include "SpiceParser.h"
#include "analyzer/AnalyzerVisitor.h"
#include "ir/GeneratorVisitor.h"

using namespace antlr4;

int main(int argc, char** argv) {
    // Read from file
    std::ifstream stream;
    stream.open(argv[1]);
    antlr4::ANTLRInputStream input(stream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens);

    // Execute syntactical analysis
    antlr4::tree::ParseTree *tree = parser.entry();
    AnalyzerVisitor().visit(tree);

    // Execute ir generator
    GeneratorVisitor generator = GeneratorVisitor();
    generator.init();
    generator.visit(tree);
    generator.emit();

    // Return with positive result code
    return 0;
}