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
    SpiceParser parser(&tokens); // Check for syntax errors

    // Execute syntactical analysis
    antlr4::tree::ParseTree *tree = parser.entry();
    AnalyzerVisitor().visit(tree); // Check for semantic errors

    // Execute generator
    GeneratorVisitor generator = GeneratorVisitor();
    generator.init(); // Initialize code generation
    generator.visit(tree); // Generate IR code
    generator.dumpIR();
    generator.optimize(); // Optimize IR code
    generator.emit(); // Emit object file for specified platform

    // Return with positive result code
    return 0;
}