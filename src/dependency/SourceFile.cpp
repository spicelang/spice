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
  symbolTable = std::make_shared<SymbolTable>(nullptr, parent == nullptr, true);
}

void SourceFile::preAnalyze(CliOptions *options) {
  // Pre-analyze this source file
  PreAnalyzerVisitor preAnalyzer = PreAnalyzerVisitor(options, this);
  preAnalyzer.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();

  // Analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile.first->preAnalyze(options);
}

void SourceFile::analyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                         ThreadFactory *threadFactory) {
  // Analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies) {
    // Analyze the imported source file
    sourceFile.first->analyze(context, builder, threadFactory);

    // Mount symbol table to the current one
    sourceFile.first->symbolTable->setParent(symbolTable.get());
    symbolTable->mountChildBlock(importName, sourceFile.first->symbolTable.get());

    // Insert import symbol
    SymbolType type = SymbolType(TY_IMPORT);
    SymbolSpecifiers specifiers(type);
    symbolTable->insert(importName, type, specifiers, INITIALIZED, sourceFile.second);
  }

  // Analyze this source file
  analyzer = std::make_shared<AnalyzerVisitor>(context, builder, threadFactory, this, options, parent == nullptr, stdFile);
  needsReAnalyze = any_cast<bool>(analyzer->visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();
}

void SourceFile::reAnalyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                           ThreadFactory *threadFactory) {
  // Re-analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile.first->reAnalyze(context, builder, threadFactory);

  // Re-Analyze this source file
  if (needsReAnalyze) {
    analyzer->visit(antlrCtx.parser->entry());
    antlrCtx.parser->reset();
  }

  // Save the JSON version in the compiler output
  compilerOutput.symbolTableString = symbolTable->toJSON().dump(2);

  // Dump symbol table
  if (options->dumpSymbolTables) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << compilerOutput.symbolTableString << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                          ThreadFactory *threadFactory, LinkerInterface *linker) {
  // Generate the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile.first->generate(context, builder, threadFactory, linker);

  // Generate this source file
  generator = std::make_shared<GeneratorVisitor>(context, builder, threadFactory, linker, options, this, objectFilePath);
  generator->visit(antlrCtx.parser->entry());

  // Save the JSON version in the compiler output
  compilerOutput.irString = generator->getIRString();

  // Dump unoptimized IR code
  if (options->dumpIR) { // GCOV_EXCL_START
    std::cout << "\nUnoptimized IR code:\n";
    generator->dumpIR();
  } // GCOV_EXCL_STOP

  // Optimize IR code
  if (options->optLevel >= 1 && options->optLevel <= 5) {
    generator->optimize();

    // Save the JSON version in the compiler output
    compilerOutput.irOptString = generator->getIRString();

    // Dump optimized IR code
    if (options->dumpIR) { // GCOV_EXCL_START
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

void SourceFile::addDependency(const ErrorFactory *err, const antlr4::Token &token, const std::string &name,
                               const std::string &filePath, bool stdFile) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(filePath))
    throw err->get(token, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + filePath + "'");

  // Add the dependency
  dependencies.insert({name, {std::make_shared<SourceFile>(options, this, name, filePath, stdFile), token}});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const {
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check parent recursively
  return parent != nullptr && parent->isAlreadyImported(filePathSearch);
}