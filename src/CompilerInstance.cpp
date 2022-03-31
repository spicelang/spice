// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <exception/AntlrThrowingErrorListener.h>
#include <util/FileUtil.h>

/**
 * Compiles a single source file to an object
 *
 * @param context LLVM context which holds global LLVM core data
 * @param builder LLVM IR builder for generating IR with the LLVM API
 * @param moduleRegistry Module Registry, to manage project-wide module information
 * @param threadFactory Thread Factory, used for generating project-wide thread information
 * @param options Cli options that were passed to the executable
 * @param linker Linker interface
 * @param sourceFile Full path to a file (absolute or relative)
 * @param requiresMainFct true = main source file, false = not main source file
 * @param stdFile true = std source file, false = not std source file
 *
 * @return Symbol table of this program part
 */
SymbolTable *CompilerInstance::CompileSourceFile(const std::shared_ptr<llvm::LLVMContext> &context,
                                                 const std::shared_ptr<llvm::IRBuilder<>> &builder,
                                                 ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory,
                                                 CliOptions *options, LinkerInterface *linker, const std::string &sourceFile,
                                                 bool requiresMainFct, bool stdFile) {
  // Analyze main source file
  SymbolTable *symbolTable =
      analyzeSourceFile(context, builder, moduleRegistry, threadFactory, options, linker, sourceFile, requiresMainFct, stdFile);

  // Generate main source file
  generateSourceFile(context, builder, moduleRegistry, threadFactory, options, linker, sourceFile, requiresMainFct, symbolTable);

  return symbolTable;
}

SymbolTable *CompilerInstance::analyzeSourceFile(const std::shared_ptr<llvm::LLVMContext> &context,
                                                 const std::shared_ptr<llvm::IRBuilder<>> &builder,
                                                 ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory,
                                                 CliOptions *options, LinkerInterface *linker, const std::string &sourceFile,
                                                 bool requiresMainFct, bool stdFile) {
  // Read from file
  std::ifstream stream(sourceFile);
  if (!stream)
    throw std::runtime_error("Source file at path '" + sourceFile + "' does not exist.");

  // Create error handlers for lexer and parser
  AntlrThrowingErrorListener lexerErrorHandler = AntlrThrowingErrorListener(LEXER);
  AntlrThrowingErrorListener parserErrorHandler = AntlrThrowingErrorListener(PARSER);

  // Tokenize input
  antlr4::ANTLRInputStream input(stream);
  SpiceLexer lexer(&input);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexerErrorHandler);
  antlr4::CommonTokenStream tokens((antlr4::TokenSource *)&lexer);

  // Parse input to AST
  SpiceParser parser(&tokens); // Check for syntax errors
  parser.removeErrorListeners();
  parser.addErrorListener(&parserErrorHandler);
  antlr4::tree::ParseTree *ast = parser.entry(); // Get AST

  // Execute syntactical analysis
  SymbolTable *symbolTable;
  AnalyzerVisitor analyzer =
      AnalyzerVisitor(context, builder, moduleRegistry, threadFactory, options, linker, sourceFile, requiresMainFct, stdFile);
  symbolTable = analyzer.visit(ast).as<SymbolTable *>(); // Check for semantic errors
  if (options->printDebugOutput) {                       // GCOV_EXCL_START
    // Print symbol table
    std::cout << std::endl << "Symbol table of file " << sourceFile << ":" << std::endl << std::endl;
    std::cout << symbolTable->toJSON().dump(2) << std::endl;
  } // GCOV_EXCL_STOP

  return symbolTable;
}

void CompilerInstance::generateSourceFile(const std::shared_ptr<llvm::LLVMContext> &context,
                                          const std::shared_ptr<llvm::IRBuilder<>> &builder, ModuleRegistry *moduleRegistry,
                                          ThreadFactory *threadFactory, CliOptions *options, LinkerInterface *linker,
                                          const std::string &sourceFile, bool requiresMainFct, SymbolTable *symbolTable) {
  // Read from file
  std::ifstream stream(sourceFile);
  if (!stream)
    throw std::runtime_error("Source file at path '" + sourceFile + "' does not exist.");

  // Create error handlers for lexer and parser
  AntlrThrowingErrorListener lexerErrorHandler = AntlrThrowingErrorListener(LEXER);
  AntlrThrowingErrorListener parserErrorHandler = AntlrThrowingErrorListener(PARSER);

  // Tokenize input
  antlr4::ANTLRInputStream input(stream);
  SpiceLexer lexer(&input);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexerErrorHandler);
  antlr4::CommonTokenStream tokens((antlr4::TokenSource *)&lexer);

  // Parse input to AST
  SpiceParser parser(&tokens); // Check for syntax errors
  parser.removeErrorListeners();
  parser.addErrorListener(&parserErrorHandler);
  antlr4::tree::ParseTree *ast = parser.entry(); // Get AST

  // Get file name from file path
  std::string fileName = FileUtil::getFileName(sourceFile);
  std::string objectFlePath = options->outputDir + "/" + fileName + ".o";

  // Execute generator
  GeneratorVisitor generator = GeneratorVisitor(context, builder, moduleRegistry, threadFactory, symbolTable, options, linker,
                                                sourceFile, objectFlePath, requiresMainFct);
  generator.init();                // Initialize code generation
  generator.visit(ast);            // Generate IR code
  if (options->printDebugOutput) { // GCOV_EXCL_START
    // Dump unoptimized IR code
    std::cout << std::endl << "IR code:" << std::endl;
    generator.dumpIR();
  } // GCOV_EXCL_STOP

  if (options->optLevel >= 1 && options->optLevel <= 5) {
    generator.optimize();            // Optimize IR code
    if (options->printDebugOutput) { // GCOV_EXCL_START
      // Dump optimized IR code
      std::cout << std::endl << "Optimized IR code:" << std::endl;
      generator.dumpIR();
    } // GCOV_EXCL_STOP
  }

  // Emit object file for specified platform
  generator.emit();

  // Add object file to the linker interface
  if (linker)
    linker->addObjectFilePath(objectFlePath);
}