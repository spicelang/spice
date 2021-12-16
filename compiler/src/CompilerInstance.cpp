// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

/**
 * Compiles a single source file to an object
 *
 * @param sourceFile Full path to a file (absolute or relative)
 * @param targetArch Target architecture: e.g.: x86_64
 * @param targetVendor Target vendor: e.g.: w64
 * @param targetOs Target OS: e.g.: windows
 * @param objectDir Full path to an output file (absolute or relative)
 * @param debugOutput Set to true to show compiler debug output
 * @param optLevel Number in range 1-3 to control optimization level
 * @param mustHaveMainFunction true = main source file, false = not main source file
 *
 * @return Symbol table of this program part
 */
SymbolTable* CompilerInstance::CompileSourceFile(
        std::string& sourceFile,
        const std::string& targetArch,
        const std::string& targetVendor,
        const std::string& targetOs,
        const std::string& objectDir,
        bool debugOutput,
        int optLevel,
        bool mustHaveMainFunction,
        bool stdFile
) {
    // Read from file
    std::ifstream stream(sourceFile);
    if (!stream) throw std::runtime_error("Source file at path '" + sourceFile + "' does not exist.");

    // Parse input to AST
    antlr4::ANTLRInputStream input(stream);
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens); // Check for syntax errors
    antlr4::tree::ParseTree* tree = parser.entry(); // Get AST

    // Execute syntactical analysis
    SymbolTable* symbolTable;
    AnalyzerVisitor analyzer = AnalyzerVisitor(
            sourceFile,
            targetArch,
            targetVendor,
            targetOs,
            objectDir,
            debugOutput,
            optLevel,
            mustHaveMainFunction,
            stdFile
    );
    symbolTable = analyzer.visit(tree).as<SymbolTable*>(); // Check for semantic errors
    if (debugOutput) {
        // Print symbol table
        std::cout << "\nSymbol table of file " << sourceFile << ":\n" << std::endl;
        std::cout << symbolTable->toString() << std::endl;
    }

    // Get file name from file path
    std::string fileName = FileUtil::getFileName(sourceFile);

    // Execute generator
    GeneratorVisitor generator = GeneratorVisitor(
            symbolTable,
            fileName,
            targetArch,
            targetVendor,
            targetOs,
            objectDir + "/" + fileName + ".o",
            debugOutput,
            optLevel,
            mustHaveMainFunction
    );
    generator.init(); // Initialize code generation
    generator.visit(tree); // Generate IR code
    if (debugOutput) {
        // Dump unoptimized IR code
        std::cout << "\nIR code:" << std::endl;
        generator.dumpIR();
    }

    if (optLevel >= 1 && optLevel <= 3) {
        generator.optimize(); // Optimize IR code
        if (debugOutput) {
            // Dump optimized IR code
            std::cout << "\nOptimized IR code:" << std::endl;
            generator.dumpIR();
        }
    }

    generator.emit(); // Emit object file for specified platform

    return symbolTable;
}
