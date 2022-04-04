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

struct CompilerOutput {
  std::string symbolTableString;
  std::string irString;
  std::string irOptString;
};

class SourceFile {
public:
  // Constructors
  explicit SourceFile(ModuleRegistry *moduleRegistry, CliOptions *options, SourceFile *parent, std::string name,
                      const std::string &filePath, bool stdFile);

  // Public methods
  void preAnalyze(CliOptions *options);
  void analyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
               ThreadFactory *threadFactory, LinkerInterface *linker);
  void generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                ThreadFactory *threadFactory, LinkerInterface *linker);
  void addDependency(const ErrorFactory *err, const antlr4::Token &token, const std::string &name, const std::string &filePath,
                     bool stdFile);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;

  // Fields
  std::string name;
  std::string filePath;
  std::string objectFilePath;
  bool stdFile;
  SourceFileAntlrCtx antlrCtx;
  CompilerOutput compilerOutput;
  SourceFile *parent;
  CliOptions *options;
  std::shared_ptr<SymbolTable> symbolTable;
  std::shared_ptr<AnalyzerVisitor> analyzer;
  std::shared_ptr<GeneratorVisitor> generator;
  std::map<std::string, std::pair<std::shared_ptr<SourceFile>, const antlr4::Token &>> dependencies;

private:
  ModuleRegistry *moduleRegistry;
};