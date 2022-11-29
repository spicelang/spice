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

#include "../lib/thread-pool/thread-pool.hpp"

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

struct TimerOutput {
  uint64_t lexer = 0;
  uint64_t parser = 0;
  uint64_t cstVisualizer = 0;
  uint64_t astBuilder = 0;
  uint64_t astOptimizer = 0;
  uint64_t astVisualizer = 0;
  uint64_t importCollector = 0;
  uint64_t symbolTableBuilder = 0;
  uint64_t typeCheckerPre = 0;
  uint64_t typeCheckerPost = 0;
  uint64_t borrowChecker = 0;
  uint64_t escapeAnalyzer = 0;
  uint64_t irGenerator = 0;
  uint64_t irOptimizer = 0;
  uint64_t objectEmitter = 0;
};

struct CompilerOutput {
  std::string cstString;
  std::string astString;
  std::string symbolTableString;
  std::string irString;
  std::string irOptString;
  std::vector<CompilerWarning> warnings;
  TimerOutput times;
};

struct NameRegistryEntry {
  std::string name;
  SymbolTableEntry *targetEntry;
  Scope *targetScope;
  std::string predecessorName;
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
  void addNameRegistryEntry(const std::string &name, SymbolTableEntry *entry, Scope *scope, bool keepNewOnCollision = true,
                            const std::string &predecessorName = "");
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
  std::unordered_map<std::string, std::pair<std::shared_ptr<SourceFile>, const ASTNode *>> dependencies;
  std::unordered_map<std::string, NameRegistryEntry> exportedNameRegistry;

private:
  // Private fields
  GlobalResourceManager &resourceManager;
  BS::synced_stream &tout;

  // Private methods
  void mergeNameRegistries(const SourceFile &importedSourceFile, const std::string &importName);
  void visualizerPreamble(std::stringstream &output) const;
  void visualizerOutput(std::string outputName, const std::string &output) const;
  void printStatusMessage(const std::string &stage, const CompilerStageIOType &in, const CompilerStageIOType &out,
                          const Timer *timer, uint64_t &timeCompilerOutput, bool fromThread = false) const;
};