// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include "analyzer/AnalyzerVisitor.h"
#include "generator/GeneratorVisitor.h"

using namespace antlr4;

int main(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    // Receive args from cli
    std::string filePath = args[1];
    std::string targetTriple = args[2]; // Default for dev host: x86_64-w64-windows-gnu
    std::string outputPath = args[3];
    bool debugOutput = args[4] == "true";
    int optimizerLevel = std::stoi(args[5]);

    // Read from file
    std::ifstream stream;
    stream.open(filePath);
    antlr4::ANTLRInputStream input(stream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens); // Check for syntax errors
    antlr4::tree::ParseTree* tree = parser.entry(); // Get AST

    // Execute syntactical analysis
    SymbolTable* symbolTable;
    try {
        symbolTable = AnalyzerVisitor().visit(tree).as<SymbolTable*>(); // Check for semantic errors
        if (debugOutput) std::cout << symbolTable->toString() << std::endl; // Print symbol table in debug mode
    } catch (SemanticError& e) {
        std::cout << e.what() << std::endl;
        return 1; // Exit with negative result code
    }

    // Execute generator
    try {
        GeneratorVisitor generator = GeneratorVisitor(symbolTable);
        generator.init(); // Initialize code generation
        generator.visit(tree); // Generate IR code
        if (debugOutput) {
            // Print IR code
            std::cout << "IR code:" << std::endl;
            generator.dumpIR();
        }

        generator.optimize(optimizerLevel); // Optimize IR code
        if (debugOutput) {
            // Print optimized IR code
            std::cout << "Optimized IR code:" << std::endl;
            generator.dumpIR();
        }

        generator.emit(targetTriple, outputPath); // Emit object file for specified platform
    } catch (IRError& e) {
        std::cout << e.what() << std::endl;
        return 1; // Exit with negative result code
    }

    // Exit with positive result code
    return 0;
}