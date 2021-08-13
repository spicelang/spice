// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <iostream>
#include <filesystem>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include "analyzer/AnalyzerVisitor.h"
#include "generator/GeneratorVisitor.h"

/**
 * Extracts the name of a file from its full path and returns it
 *
 * @param filePath Full path to the file (absolute or relative)
 */
std::string getFileName(const std::string& filePath) {
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = filePath.rfind(sep, filePath.length());
    if (i != std::string::npos) return(filePath.substr(i + 1, filePath.length() - i));
    return "";
}

/**
 * Compiles a single source file to an object
 *
 * @param filePath Full path to a file (absolute or relative)
 * @param targetTriple Target triplet string: e.g.: x86_64-w64-windows-gnu
 * @param outputPath Full path to an output file (absolute or relative)
 * @param debugOutput Set to true to show compiler debug output
 * @param optimizerLevel Number in range 1-3 to control optimization level
 * @return Return code of the compile process
 */
int compileSourceFile(
        const std::string& filePath,
        const std::string& targetTriple,
        const std::string& objectDir,
        bool debugOutput,
        int optimizerLevel
) {
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

    try {
        // Get file name from file path
        std::string fileName = getFileName(filePath);
        std::cout << fileName << std::endl;

        // Execute generator
        GeneratorVisitor generator = GeneratorVisitor(symbolTable);
        generator.init(fileName); // Initialize code generation
        generator.visit(tree); // Generate IR code
        if (debugOutput) { // Dump un-optimized IR code, if debug output is enabled
            std::cout << "\nIR code:" << std::endl;
            generator.dumpIR();
        }

        generator.optimize(optimizerLevel); // Optimize IR code
        if (debugOutput) { // Dump optimized IR code, if debug output is enabled
            std::cout << "\nOptimized IR code:" << std::endl;
            generator.dumpIR();
        }

        generator.emit(targetTriple, objectDir + "/" + fileName + ".o"); // Emit object file for specified platform
    } catch (IRError& e) {
        std::cout << e.what() << std::endl;
        return 1; // Exit with negative result code
    }
    return 0;
}

/**
 * Entry point to the Spice compiler
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    // Extract args from cli
    std::string filePath = args[1];
    std::string targetTriple = args[2]; // Default for dev host: x86_64-w64-windows-gnu
    std::string outputPath = args[3];
    bool debugOutput = args[4] == "true";
    int optimizerLevel = std::stoi(args[5]);

    /*
     * Compile main source file. All files, that are included by the main source file will call the 'compileSourceFile'
     * function again.
     */
    return compileSourceFile(filePath, targetTriple, outputPath, debugOutput, optimizerLevel);
}