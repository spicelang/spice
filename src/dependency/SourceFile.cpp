// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"
#include "analyzer/PreAnalyzerVisitor.h"

#include <algorithm>
#include <utility>

#include <util/FileUtil.h>

SourceFile::SourceFile(SourceFile *parent, std::string name, const std::string &filePath, bool stdFile) {
  this->name = std::move(name);
  this->filePath = filePath;
  this->objectFilePath = FileUtil::getFileName(filePath) + ".o";
  this->stdFile = stdFile;
  this->parent = parent;

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
  return analyzer->visit(antlrCtx.parser->entry()).as<SymbolTable *>();
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
  generator->optimize();
}

void SourceFile::addDependency(const ErrorFactory *err, const antlr4::Token &token, SourceFile *parent, const std::string &name,
                               const std::string &filePath, bool stdFile) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(filePath))
    throw err->get(token, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + filePath + "'");

  // Add the dependency
  dependencies.insert({name, std::make_shared<SourceFile>(parent, name, filePath, stdFile)});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const {
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check all own dependencies recursively
  return std::ranges::any_of(dependencies, [&](auto const &d) { return d.second->isAlreadyImported(filePathSearch); });
}