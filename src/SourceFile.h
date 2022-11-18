// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>

#include <ast/ASTNodes.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <global/RuntimeModuleManager.h>
#include <scope/Scope.h>
#include <util/CompilerWarning.h>

#include <llvm/IR/IRBuilder.h>

// Forward declarations
class GlobalResourceManager;
class EntryNode;
class ASTNode;
class Timer;

enum CompilerStageIOType {
  IO_CODE = 0,
  IO_TOKENS = 1,
  IO_CST = 2,
  IO_AST = 3,
  IO_IR = 4,
  IO_OBJECT_FILE = 5,
};

struct SourceFileAntlrCtx {
  // Create error handlers for lexer and parser
  std::unique_ptr<AntlrThrowingErrorListener> lexerErrorHandler;
  std::unique_ptr<AntlrThrowingErrorListener> parserErrorHandler;
  std::unique_ptr<antlr4::ANTLRInputStream> inputStream;
  std::unique_ptr<SpiceLexer> lexer;
  std::unique_ptr<antlr4::CommonTokenStream> tokenStream;
  std::unique_ptr<SpiceParser> parser;
};

struct CompilerOutput {
  std::string cstString;
  std::string astString;
  std::string symbolTableString;
  std::string irString;
  std::string irOptString;
  std::vector<CompilerWarning> warnings;
};

struct NameRegistryEntry {
  std::string predecessorName;
  SymbolTableEntry *targetEntry;
  Scope *targetScope;
};

class SourceFile {
public:
  // Constructors
  explicit SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name, const std::string &filePath,
                      bool stdFile);

  // Friend classes
  friend class RuntimeModuleManager;

  // Compiler pipeline triggers
  void runLexer();
  void runParser();
  void runCSTVisualizer();
  void runASTBuilder();
  void runASTOptimizer();
  void runASTVisualizer();
  void runImportCollector();
  void runSymbolTableBuilder();
  void runTypeChecker();

private:
  void runTypeCheckerFirst();
  void runTypeCheckerSecond();

public:
  void runBorrowChecker();
  void runEscapeAnalyzer();
  void runIRGenerator();
  void runIROptimizer();
  void runObjectEmitter();
  void concludeCompilation();

  // Shortcuts
  void runFrontEnd();
  void runMiddleEnd();
  void runBackEnd();

  // Public methods
  [[nodiscard]] std::shared_ptr<SourceFile> createSourceFile(const std::string &dependencyName, const std::string &path,
                                                             bool isStdFile);
  void addDependency(const std::shared_ptr<SourceFile> &sourceFile, const ASTNode *declNode, const std::string &name,
                     const std::string &path);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;
  void collectAndPrintWarnings();
  void requestRuntimeModule(const RuntimeModuleName &moduleName);
  [[nodiscard]] const NameRegistryEntry *getNameRegistryEntry(std::string symbolName) const;

  // Public fields
  std::string name;
  std::string fileName;
  std::string filePath;
  std::string fileDir;
  std::string objectFilePath;
  bool stdFile = false;
  bool mainFile = true;
  SourceFileAntlrCtx antlrCtx;
  CompilerOutput compilerOutput;
  SourceFile *parent;
  std::string cacheKey;
  bool restoredFromCache = false;
  std::unique_ptr<EntryNode> ast;
  std::unique_ptr<Scope> globalScope;
  std::unique_ptr<llvm::Module> llvmModule;
  std::map<std::string, std::pair<std::shared_ptr<SourceFile>, const ASTNode *>> dependencies;
  std::map<std::string, NameRegistryEntry> exportedNameRegistry;

private:
  // Private fields
  GlobalResourceManager &resourceManager;

  // Private methods
  void visualizerPreamble(std::stringstream &output) const;
  void visualizerOutput(std::string outputName, const std::string &output) const;
  void printStatusMessage(const std::string &stage, const CompilerStageIOType &in, const CompilerStageIOType &out,
                          const Timer *timer) const;
};