// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"
#include "analyzer/PreAnalyzerVisitor.h"

#include <algorithm>
#include <utility>

#include <util/FileUtil.h>

SourceFile::SourceFile(CliOptions *options, SourceFile *parent, std::string name, const std::string &filePath, bool stdFile) {
  this->name = std::move(name);
  this->filePath = filePath;
  this->objectFilePath = options->outputDir + "/" + FileUtil::getFileName(filePath) + ".o";
  this->stdFile = stdFile;
  this->parent = parent;
  this->options = options;

  // Read from file
  std::ifstream fileInputStream(filePath);
  if (!fileInputStream)
    throw std::runtime_error("Source file at path '" + filePath + "' does not exist.");

  // Create error handlers for lexer and parser
  antlrCtx.lexerErrorHandler = std::make_shared<AntlrThrowingErrorListener>(LEXER);
  antlrCtx.parserErrorHandler = std::make_shared<AntlrThrowingErrorListener>(PARSER);

  // Tokenize input
  antlrCtx.inputStream = std::make_shared<antlr4::ANTLRInputStream>(fileInputStream);
  antlrCtx.lexer = std::make_shared<SpiceLexer>(antlrCtx.inputStream.get());
  antlrCtx.lexer->removeErrorListeners();
  antlrCtx.lexer->addErrorListener(antlrCtx.lexerErrorHandler.get());
  antlrCtx.tokenStream = std::make_shared<antlr4::CommonTokenStream>(antlrCtx.lexer.get());

  // Parse input to AST
  antlrCtx.parser = std::make_shared<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  // Create symbol table
  if (parent == nullptr) { // Main source file
    symbolTable = std::make_shared<SymbolTable>(nullptr, true);
  } else { // Imported source file
    symbolTable = std::make_shared<SymbolTable>(parent->symbolTable.get(), false);
  }
}

void SourceFile::preAnalyze(CliOptions *options) {
  // Pre-analyze this source file
  PreAnalyzerVisitor preAnalyzer = PreAnalyzerVisitor(options, this);
  preAnalyzer.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();

  // Analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile->preAnalyze(options);
}

SymbolTable *SourceFile::analyze(const std::shared_ptr<llvm::LLVMContext> &context,
                                 const std::shared_ptr<llvm::IRBuilder<>> &builder, ModuleRegistry *moduleRegistry,
                                 ThreadFactory *threadFactory, LinkerInterface *linker, CliOptions *options) {
  // Analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile->analyze(context, builder, moduleRegistry, threadFactory, linker, options);

  // Analyze this source file
  analyzer = std::make_shared<AnalyzerVisitor>(context, builder, moduleRegistry, threadFactory, this, options, linker,
                                               parent == nullptr, stdFile);
  SymbolTable *symbolTable = analyzer->visit(antlrCtx.parser->entry()).as<SymbolTable *>();
  antlrCtx.parser->reset();

  // Dump symbol table
  if (options->printDebugOutput) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << symbolTable->toJSON().dump(2) << "\n";
  } // GCOV_EXCL_STOP

  return symbolTable;
}

void SourceFile::generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                          ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory, LinkerInterface *linker,
                          CliOptions *options) {
  // Generate the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile->generate(context, builder, moduleRegistry, threadFactory, linker, options);

  // Generate this source file
  generator =
      std::make_shared<GeneratorVisitor>(context, builder, moduleRegistry, threadFactory, linker, options, this, objectFilePath);
  generator->visit(antlrCtx.parser->entry());

  // Dump unoptimized IR code
  if (options->printDebugOutput) { // GCOV_EXCL_START
    std::cout << "\nIR code:\n";
    generator->dumpIR();
  } // GCOV_EXCL_STOP

  // Optimize IR code
  if (options->optLevel >= 1 && options->optLevel <= 5) {
    generator->optimize();
    // Dump optimized IR code
    if (options->printDebugOutput) { // GCOV_EXCL_START
      std::cout << "\nOptimized IR code:\n";
      generator->dumpIR();
    } // GCOV_EXCL_STOP
  }

  // Emit object file
  generator->emit();

  antlrCtx.parser->reset();

  // Add object file to the linker interface
  if (linker)
    linker->addObjectFilePath(objectFilePath);
}

void SourceFile::addDependency(const ErrorFactory *err, const antlr4::Token &token, SourceFile *parent, const std::string &name,
                               const std::string &filePath, bool stdFile) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(filePath))
    throw err->get(token, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + filePath + "'");

  // Add the dependency
  dependencies.insert({name, std::make_shared<SourceFile>(options, parent, name, filePath, stdFile)});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const {
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check all own dependencies recursively
  return std::ranges::any_of(dependencies, [&](auto const &d) { return d.second->isAlreadyImported(filePathSearch); });
}