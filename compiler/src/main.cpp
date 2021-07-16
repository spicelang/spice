// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include "analyzer/AnalyzerVisitor.h"
#include "generator/GeneratorVisitor.h"

using namespace antlr4;

int main(int argc, char** argv) {
    // Receive args from cli
    auto filePath = argv[1];
    auto targetTriple = argv[2]; // Default: x86_64-w64-windows-gnu

    // Read from file
    std::ifstream stream;
    stream.open(filePath);
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
    std::cout << "Normal IR code:" << std::endl;
    generator.dumpIR();
    generator.optimize(); // Optimize IR code
    std::cout << "Optimized IR code:" << std::endl;
    generator.dumpIR();
    generator.emit(targetTriple); // Emit object file for specified platform*

    // Return with positive result code
    return 0;
}