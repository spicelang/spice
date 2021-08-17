// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

/**
 * Compiles a single source file to an object
 *
 * @param mainSourceFile Full path to a file (absolute or relative)
 * @param targetTriple Target triplet string: e.g.: x86_64-w64-windows-gnu
 * @param outputPath Full path to an output file (absolute or relative)
 * @param debugOutput Set to true to show compiler debug output
 * @param optLevel Number in range 1-3 to control optimization level
 *
 * @return Symbol table of this program part
 */
SymbolTable* CompilerInstance::CompileSourceFile(
        std::string& mainSourceFile,
        const std::string& targetTriple,
        const std::string& objectDir,
        bool debugOutput,
        int optLevel,
        bool mustHaveMainFunction
) {
    // Read from file
    std::ifstream stream;
    stream.open(mainSourceFile);
    antlr4::ANTLRInputStream input(stream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens); // Check for syntax errors
    antlr4::tree::ParseTree* tree = parser.entry(); // Get AST

    // Execute syntactical analysis
    SymbolTable* symbolTable;
    try {
        AnalyzerVisitor analyzer = AnalyzerVisitor(
                mainSourceFile,
                targetTriple,
                objectDir,
                debugOutput,
                optLevel,
                mustHaveMainFunction
        );
        symbolTable = analyzer.visit(tree).as<SymbolTable*>(); // Check for semantic errors
        if (debugOutput) { // Print symbol table in debug mode
            std::cout << "\nSymbol table of file " << mainSourceFile << ":\n" << std::endl;
            std::cout << symbolTable->toString() << std::endl;
        }
    } catch (SemanticError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with negative result code
    }

    try {
        // Get file name from file path
        std::string fileName = FileUtil::getFileName(mainSourceFile);

        // Execute generator
        GeneratorVisitor generator = GeneratorVisitor(
                symbolTable,
                fileName,
                targetTriple,
                objectDir + "/" + fileName + ".o",
                debugOutput,
                optLevel,
                mustHaveMainFunction
        );
        generator.init(); // Initialize code generation
        generator.visit(tree); // Generate IR code
        if (debugOutput) { // Dump un-optimized IR code, if debug output is enabled
            std::cout << "\nIR code:" << std::endl;
            generator.dumpIR();
        }

        generator.optimize(); // Optimize IR code
        if (debugOutput) { // Dump optimized IR code, if debug output is enabled
            std::cout << "\nOptimized IR code:" << std::endl;
            generator.dumpIR();
        }

        generator.emit(); // Emit object file for specified platform
    } catch (IRError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with negative result code
    }
    return symbolTable;
}
