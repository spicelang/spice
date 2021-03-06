// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>

#include <llvm/IR/IRBuilder.h>

// Forward declarations
class AnalyzerVisitor;
class GeneratorVisitor;
class AntlrThrowingErrorListener;
class SymbolTable;
class ErrorFactory;
struct CliOptions;
class LinkerInterface;
class ThreadFactory;
class EntryNode;
struct CodeLoc;

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
  std::string cstString;
  std::string astString;
  std::string symbolTableString;
  std::string irString;
  std::string irOptString;
};

class SourceFile {
public:
  // Constructors
  explicit SourceFile(CliOptions &options, SourceFile *parent, std::string name, const std::string &filePath, bool stdFile);

  // Public methods
  void visualizeCST(std::string *output);
  void buildAST();
  void visualizeAST(std::string *output);
  void preAnalyze();
  void analyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
               const ThreadFactory &threadFactory);
  void reAnalyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                 ThreadFactory &threadFactory);
  void generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                ThreadFactory &threadFactory, LinkerInterface &linker);
  void addDependency(const ErrorFactory *err, const CodeLoc &codeLoc, const std::string &name, const std::string &filePath,
                     bool stdFile);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;

  // Fields
  std::string name;
  std::string fileName;
  std::string filePath;
  std::string fileDir;
  std::string objectFilePath;
  bool stdFile;
  SourceFileAntlrCtx antlrCtx;
  CompilerOutput compilerOutput;
  SourceFile *parent;
  CliOptions &options;
  std::shared_ptr<EntryNode> ast;
  std::shared_ptr<SymbolTable> symbolTable;
  std::shared_ptr<AnalyzerVisitor> analyzer;
  std::shared_ptr<GeneratorVisitor> generator;
  std::map<std::string, std::pair<std::shared_ptr<SourceFile>, const CodeLoc &>> dependencies;
};