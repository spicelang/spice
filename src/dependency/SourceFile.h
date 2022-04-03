// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>

#include <analyzer/AnalyzerVisitor.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/ErrorFactory.h>
#include <generator/GeneratorVisitor.h>
#include <symbol/SymbolTable.h>

struct SourceFileAntlrCtx {
  // Create error handlers for lexer and parser
  std::shared_ptr<AntlrThrowingErrorListener> lexerErrorHandler;
  std::shared_ptr<AntlrThrowingErrorListener> parserErrorHandler;
  std::shared_ptr<antlr4::ANTLRInputStream> inputStream;
  std::shared_ptr<SpiceLexer> lexer;
  std::shared_ptr<antlr4::CommonTokenStream> tokenStream;
  std::shared_ptr<SpiceParser> parser;
};

class SourceFile {
public:
  // Constructors
  explicit SourceFile(CliOptions *options, SourceFile *parent, std::string name, const std::string &filePath, bool stdFile);

  // Public methods
  void preAnalyze(CliOptions *options);
  SymbolTable *analyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                       ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory, LinkerInterface *linker,
                       CliOptions *options);
  void generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory, LinkerInterface *linker, CliOptions *options);
  void addDependency(const ErrorFactory *err, const antlr4::Token &token, SourceFile *parent, const std::string &name,
                     const std::string &filePath, bool stdFile);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;

  // Fields
  std::string name;
  std::string filePath;
  std::string objectFilePath;
  bool stdFile;
  SourceFileAntlrCtx antlrCtx;
  SourceFile *parent;
  CliOptions *options;
  std::shared_ptr<SymbolTable> symbolTable;
  std::shared_ptr<AnalyzerVisitor> analyzer;
  std::shared_ptr<GeneratorVisitor> generator;
  std::map<std::string, std::shared_ptr<SourceFile>> dependencies;
};