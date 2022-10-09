// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>

#include <dependency/RuntimeModuleManager.h>

#include <llvm/IR/IRBuilder.h>

// Forward declarations
class AnalyzerVisitor;
class GeneratorVisitor;
class AntlrThrowingErrorListener;
class SymbolTable;
struct CliOptions;
class LinkerInterface;
class ThreadFactory;
class EntryNode;
struct AstNode;

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
  explicit SourceFile(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, ThreadFactory &threadFactory,
                      RuntimeModuleManager &runtimeModuleManager, LinkerInterface &linker, CliOptions &options,
                      SourceFile *parent, std::string name, const std::string &filePath, bool stdFile);

  // Friend classes
  friend class RuntimeModuleManager;

  // Public methods
  void visualizeCST();
  void buildAST();
  void visualizeAST();
  void preAnalyze();
  void analyze();
  void reAnalyze();
  void generate();
  void optimize();
  void emitObjectFile();
  [[nodiscard]] std::shared_ptr<SourceFile> createSourceFile(const std::string &name, const std::string &filePath, bool stdFile);
  void addDependency(const std::shared_ptr<SourceFile> &sourceFile, const AstNode *declAstNode, const std::string &name,
                     const std::string &filePath);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;
  void requestRuntimeModule(const RuntimeModuleName &moduleName);
  [[nodiscard]] SymbolTable *getRuntimeModuleScope(const RuntimeModuleName &moduleName) const;

  // Public fields
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
  std::map<std::string, std::pair<std::shared_ptr<SourceFile>, const AstNode *>> dependencies;

private:
  // Private fields
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  ThreadFactory &threadFactory;
  LinkerInterface &linker;
  RuntimeModuleManager &runtimeModuleManager;
};