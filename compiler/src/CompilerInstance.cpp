// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <util/FileUtil.h>
#include <exception/AntlrThrowingErrorListener.h>

/**
 * Compiles a single source file to an object
 *
 * @param context LLVM context which holds global LLVM core data
 * @param builder LLVM IR builder for generating IR with the LLVM API
 * @param moduleRegistry Module Registry, to manage project-wide module information
 * @param threadFactory Thread Factory, used for generating project-wide thread information
 * @param sourceFile Full path to a file (absolute or relative)
 * @param targetArch Target architecture: e.g.: x86_64
 * @param targetVendor Target vendor: e.g.: w64
 * @param targetOs Target OS: e.g.: windows
 * @param objectDir Full path to an output file (absolute or relative)
 * @param debugOutput Set to true to show compiler debug output
 * @param optLevel Number in range 1-3 to control optimization level
 * @param requiresMainFct true = main source file, false = not main source file
 * @param stdFile true = std source file, false = not std source file
 *
 * @return Symbol table of this program part
 */
SymbolTable* CompilerInstance::CompileSourceFile(
        const std::shared_ptr<llvm::LLVMContext>& context,
        const std::shared_ptr<llvm::IRBuilder<>>& builder,
        ModuleRegistry* moduleRegistry,
        ThreadFactory* threadFactory,
        const std::string& sourceFile,
        const std::string& targetArch,
        const std::string& targetVendor,
        const std::string& targetOs,
        const std::string& objectDir,
        bool debugOutput,
        int optLevel,
        bool requiresMainFct,
        bool stdFile
) {
    // Read from file
    std::ifstream stream(sourceFile);
    if (!stream) throw std::runtime_error("Source file at path '" + sourceFile + "' does not exist.");

    // Create error handlers for lexer and parser
    AntlrThrowingErrorListener lexerErrorHandler = AntlrThrowingErrorListener(LEXER);
    AntlrThrowingErrorListener parserErrorHandler = AntlrThrowingErrorListener(PARSER);

    // Tokenize input
    antlr4::ANTLRInputStream input(stream);
    SpiceLexer lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrorHandler);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);

    // Parse input to AST
    SpiceParser parser(&tokens); // Check for syntax errors
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrorHandler);
    antlr4::tree::ParseTree* tree = parser.entry(); // Get AST

    // Execute syntactical analysis
    SymbolTable* symbolTable;
    AnalyzerVisitor analyzer = AnalyzerVisitor(
            context,
            builder,
            moduleRegistry,
            threadFactory,
            sourceFile,
            targetArch,
            targetVendor,
            targetOs,
            objectDir,
            debugOutput,
            optLevel,
            requiresMainFct,
            stdFile
    );
    symbolTable = analyzer.visit(tree).as<SymbolTable*>(); // Check for semantic errors
    if (debugOutput) { // GCOV_EXCL_START
        // Print symbol table
        std::cout << std::endl << "Symbol table of file " << sourceFile << ":" << std::endl << std::endl;
        std::cout << symbolTable->toJSON().dump(2) << std::endl;
    } // GCOV_EXCL_STOP

    // Get file name from file path
    std::string fileName = FileUtil::getFileName(sourceFile);

    // Execute generator
    GeneratorVisitor generator = GeneratorVisitor(
            context,
            builder,
            threadFactory,
            symbolTable,
            fileName,
            targetArch,
            targetVendor,
            targetOs,
            objectDir + "/" + fileName + ".o",
            debugOutput,
            optLevel,
            requiresMainFct
    );
    generator.init(); // Initialize code generation
    generator.visit(tree); // Generate IR code
    if (debugOutput) { // GCOV_EXCL_START
        // Dump unoptimized IR code
        std::cout  << std::endl << "IR code:" << std::endl;
        generator.dumpIR();
    } // GCOV_EXCL_STOP

    if (optLevel >= 1 && optLevel <= 3) {
        generator.optimize(); // Optimize IR code
        if (debugOutput) { // GCOV_EXCL_START
            // Dump optimized IR code
            std::cout  << std::endl << "Optimized IR code:" << std::endl;
            generator.dumpIR();
        } // GCOV_EXCL_STOP
    }

    generator.emit(); // Emit object file for specified platform

    return symbolTable;
}
