// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <utility>

// Ignore some warnings in ANTLR generated code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <SpiceLexer.h>
#include <SpiceParser.h>
#include <Token.h>
#pragma GCC diagnostic pop

#include <ast/ASTNodes.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <global/RuntimeModuleManager.h>
#include <symboltablebuilder/Scope.h>
#include <util/CompilerWarning.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/Target/TargetMachine.h>

#include "../lib/thread-pool/thread-pool-utils.hpp"
#include "../lib/thread-pool/thread-pool.hpp"

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;
class EntryNode;
class ASTNode;
class Timer;
struct CliOptions;

enum CompileStageType : uint8_t {
  NONE,
  LEXER,
  PARSER,
  CST_VISUALIZER,
  AST_BUILDER,
  AST_VISUALIZER,
  IMPORT_COLLECTOR,
  SYMBOL_TABLE_BUILDER,
  TYPE_CHECKER_PRE,
  TYPE_CHECKER_POST,
  IR_GENERATOR,
  IR_OPTIMIZER,
  OBJECT_EMITTER,
  FINISHED
};

enum CompileStageIOType : uint8_t {
  IO_CODE,
  IO_TOKENS,
  IO_CST,
  IO_AST,
  IO_IR,
  IO_OBJECT_FILE,
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
  uint64_t astVisualizer = 0;
  uint64_t importCollector = 0;
  uint64_t symbolTableBuilder = 0;
  uint64_t typeCheckerPre = 0;
  uint64_t typeCheckerPost = 0;
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
  std::string asmString;
  std::string typesString;
  std::vector<CompilerWarning> warnings;
  TimerOutput times;
};

struct NameRegistryEntry {
  std::string name;
  uint64_t typeId; // Set for structs, interfaces, enums and aliases
  SymbolTableEntry *targetEntry;
  Scope *targetScope;
  SymbolTableEntry *importEntry = nullptr;
};

class SourceFile {
public:
  // Constructors
  SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name, const std::filesystem::path &filePath,
             bool stdFile);
  SourceFile(const SourceFile &) = delete;

  // Friend classes
  friend class RuntimeModuleManager;

  // Compiler pipeline triggers
  void runLexer();
  void runParser();
  void runCSTVisualizer();
  void runASTBuilder();
  void runASTVisualizer();
  void runImportCollector();
  void runSymbolTableBuilder();
  void runTypeChecker();

private:
  void runTypeCheckerPre();
  void runTypeCheckerPost();

public:
  void runIRGenerator();
  void runDefaultIROptimizer();
  void runPreLinkIROptimizer();
  void runBitcodeLinker();
  void runPostLinkIROptimizer();
  void runObjectEmitter();
  void concludeCompilation();

  // Shortcuts
  void runFrontEnd();
  void runMiddleEnd();
  void runBackEnd();

  // Public methods
  void addDependency(SourceFile *sourceFile, const ASTNode *declNode, const std::string &dependencyName, const std::string &path);
  [[nodiscard]] bool imports(const SourceFile *sourceFile) const;
  [[nodiscard]] bool isAlreadyImported(const std::string &filePathSearch, std::stack<const SourceFile *> &circle) const;
  SourceFile *requestRuntimeModule(RuntimeModule runtimeModule);
  bool isRuntimeModuleAvailable(RuntimeModule runtimeModule) const;
  void addNameRegistryEntry(const std::string &symbolName, uint64_t typeId, SymbolTableEntry *entry, Scope *scope,
                            bool keepNewOnCollision = true, SymbolTableEntry *importEntry = nullptr);
  [[nodiscard]] const NameRegistryEntry *getNameRegistryEntry(const std::string &symbolName) const;
  [[nodiscard]] llvm::Type *getLLVMType(const Type *type);
  void checkForSoftErrors();
  void collectAndPrintWarnings();
  const SourceFile *getRootSourceFile() const;
  bool isRT(RuntimeModule runtimeModule) const;
  ALWAYS_INLINE bool isStringRT() const { return isRT(STRING_RT); }
  ALWAYS_INLINE bool isRttiRT() const { return isRT(RTTI_RT); }

  // Public fields
  std::string name;
  std::string fileName;
  std::filesystem::path filePath;
  std::string fileDir;
  bool isStdFile = false;
  bool isMainFile = true;
  bool alwaysKeepSymbolsOnNameCollision = false;
  bool ignoreWarnings = false;
  CompileStageType previousStage = NONE;
  SourceFileAntlrCtx antlrCtx;
  CompilerOutput compilerOutput;
  SourceFile *parent;
  std::string cacheKey;
  bool restoredFromCache = false;
  EntryNode *ast = nullptr;
  std::unique_ptr<Scope> globalScope;
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::TargetMachine> targetMachine;
  std::unique_ptr<llvm::Module> llvmModule;
  std::unordered_map<std::string, SourceFile *> dependencies;
  std::vector<const SourceFile *> dependants;
  std::unordered_map<std::string, NameRegistryEntry> exportedNameRegistry;
  std::vector<const Function *> testFunctions;

private:
  // Private fields
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  BS::synced_stream &tout;
  std::unordered_map<const Type *, llvm::Type *> typeToLLVMTypeMapping;
  uint8_t importedRuntimeModules = 0;
  uint8_t totalTypeCheckerRuns = 0;

  // Private methods
  bool haveAllDependantsBeenTypeChecked() const;
  void mergeNameRegistries(const SourceFile &importedSourceFile, const std::string &importName);
  void dumpOutput(const std::string &content, const std::string &caption, const std::string &fileSuffix) const;
  void visualizerPreamble(std::stringstream &output) const;
  void visualizerOutput(std::string outputName, const std::string &output) const;
  void printStatusMessage(const char *stage, const CompileStageIOType &in, const CompileStageIOType &out, uint64_t stageRuntime,
                          bool fromThread = false, unsigned short stageRuns = 0) const;
};

} // namespace spice::compiler