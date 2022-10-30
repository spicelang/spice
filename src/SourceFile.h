// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>

#include <ast/ASTNodes.h>
#include <global/RuntimeModuleManager.h>
#include <util/CompilerWarning.h>

#include <llvm/IR/IRBuilder.h>

// Forward declarations
class AnalyzerVisitor;
class GeneratorVisitor;
class AntlrThrowingErrorListener;
class Scope;
class GlobalResourceManager;
class EntryNode;
class ASTNode;

enum CompilerStageIOType {
  IO_CODE = 0,
  IO_TOKENS = 1,
  IO_CST = 2,
  IO_AST = 3,
  IO_IR = 4,
  IO_OBJECT_FILE = 5,
};

const char *COMPILER_STAGE_IO_TYPE_NAME[] = {"Code", "Tokens", "CST", "AST", "IR", "OBJECT_FILE"};

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
  std::vector<CompilerWarning> warnings;
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
  void runSemanticAnalyzer();
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
  void addDependency(const std::shared_ptr<SourceFile> &sourceFile, const ASTNode *declAstNode, const std::string &name,
                     const std::string &path);
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch) const;
  void printWarnings() const;
  void requestRuntimeModule(const RuntimeModuleName &moduleName);

  // Public fields
  std::string name;
  std::string fileName;
  std::string filePath;
  std::string fileDir;
  std::string objectFilePath;
  bool stdFile = false;
  SourceFileAntlrCtx antlrCtx;
  CompilerOutput compilerOutput;
  SourceFile *parent;
  std::string cacheKey;
  bool restoredFromCache = false;
  std::unique_ptr<EntryNode> ast;
  std::unique_ptr<Scope> globalScope;
  std::unique_ptr<llvm::Module> llvmModule;
  std::map<std::string, std::pair<std::shared_ptr<SourceFile>, const ASTNode *>> dependencies;

private:
  // Private fields
  GlobalResourceManager &resourceManager;

  // Private methods
  void visualizerPreamble(std::stringstream &output) const;
  void visualizerOutput(std::string outputName, const std::string &output) const;
  void printStatusMessage(const std::string &stage, const CompilerStageIOType &in, const CompilerStageIOType &out) const;
};