// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include "visualizer/DependencyGraphVisualizer.h"

#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>

#include <ast/ASTBuilder.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/CompilerError.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <importcollector/ImportCollector.h>
#include <irgenerator/IRGenerator.h>
#include <iroptimizer/IROptimizer.h>
#include <linker/BitcodeLinker.h>
#include <objectemitter/ObjectEmitter.h>
#include <symboltablebuilder/SymbolTable.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/MacroDefs.h>
#include <typechecker/TypeChecker.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>
#include <util/SystemUtil.h>
#include <util/Timer.h>
#include <visualizer/ASTVisualizer.h>
#include <visualizer/CSTVisualizer.h>

namespace spice::compiler {

SourceFile::SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name,
                       const std::filesystem::path &filePath, bool stdFile)
    : name(std::move(name)), filePath(filePath), isStdFile(stdFile), parent(parent),
      builder(resourceManager.cliOptions.useLTO ? resourceManager.ltoContext : context), resourceManager(resourceManager),
      cliOptions(resourceManager.cliOptions) {
  // Deduce fileName and fileDir
  fileName = std::filesystem::path(filePath).filename().string();
  fileDir = std::filesystem::path(filePath).parent_path().string();

  // Search after the selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw CompilerError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  // Create the target machine
  llvm::TargetOptions opt;
  opt.MCOptions.AsmVerbose = true;
  opt.MCOptions.PreserveAsmComments = true;
  const std::string &cpuName = resourceManager.cpuName;
  const std::string &features = resourceManager.cpuFeatures;
  const llvm::Triple &targetTriple = cliOptions.targetTriple;
  llvm::TargetMachine *targetMachineRaw = target->createTargetMachine(targetTriple, cpuName, features, opt, llvm::Reloc::PIC_);
  targetMachine = std::unique_ptr<llvm::TargetMachine>(targetMachineRaw);
}

void SourceFile::runLexer() {
  if (isMainFile)
    resourceManager.totalTimer.start();

  // Check if this stage has already been done
  if (previousStage >= LEXER)
    return;

  Timer timer(&compilerOutput.times.lexer);
  timer.start();

  // Read from the input source file
  std::ifstream fileInputStream(filePath);
  if (!fileInputStream)
    throw CompilerError(SOURCE_FILE_NOT_FOUND, "Source file at path '" + filePath.string() + "' does not exist.");

  // Tokenize input
  antlrCtx.inputStream = std::make_unique<antlr4::ANTLRInputStream>(fileInputStream);
  antlrCtx.lexer = std::make_unique<SpiceLexer>(antlrCtx.inputStream.get());
  antlrCtx.lexer->removeErrorListeners();
  antlrCtx.lexerErrorHandler = std::make_unique<AntlrThrowingErrorListener>(ThrowingErrorListenerMode::LEXER, this);
  antlrCtx.lexer->addErrorListener(antlrCtx.lexerErrorHandler.get());
  antlrCtx.tokenStream = std::make_unique<antlr4::CommonTokenStream>(antlrCtx.lexer.get());

  // Calculate cache key
  std::stringstream cacheKeyString;
  cacheKeyString << std::hex << std::hash<std::string>{}(antlrCtx.tokenStream->getText());
  cacheKey = cacheKeyString.str();

  // Try to load from cache
  if (!cliOptions.ignoreCache)
    restoredFromCache = resourceManager.cacheManager.lookupSourceFile(this);

  previousStage = LEXER;
  timer.stop();
  printStatusMessage("Lexer", IO_CODE, IO_TOKENS, compilerOutput.times.lexer);
}

void SourceFile::runParser() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= PARSER)
    return;

  Timer timer(&compilerOutput.times.parser);
  timer.start();

  // Parse input
  antlrCtx.parser = std::make_unique<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parserErrorHandler = std::make_unique<AntlrThrowingErrorListener>(ThrowingErrorListenerMode::PARSER, this);
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  previousStage = PARSER;
  timer.stop();
  printStatusMessage("Parser", IO_TOKENS, IO_CST, compilerOutput.times.parser);
}

void SourceFile::runCSTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!cliOptions.dump.dumpCST && !cliOptions.testMode))
    return;
  // Check if this stage has already been done
  if (previousStage >= CST_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.cstVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  CSTVisualizer cstVisualizer(resourceManager, this, antlrCtx.lexer.get(), antlrCtx.parser.get());
  dotCode << " " << std::any_cast<std::string>(cstVisualizer.visit(antlrCtx.parser->entry())) << "}";
  antlrCtx.parser->reset();

  // Dump the serialized CST string and the SVG file
  if (cliOptions.dump.dumpCST || cliOptions.testMode)
    compilerOutput.cstString = dotCode.str();

  if (cliOptions.dump.dumpCST)
    visualizerOutput("CST", compilerOutput.cstString);

  previousStage = CST_VISUALIZER;
  timer.stop();
  printStatusMessage("CST Visualizer", IO_CST, IO_CST, compilerOutput.times.cstVisualizer);
}

void SourceFile::runASTBuilder() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= AST_BUILDER)
    return;

  Timer timer(&compilerOutput.times.astBuilder);
  timer.start();

  // Build AST for this source file
  ASTBuilder astBuilder(resourceManager, this, antlrCtx.inputStream.get());
  ast = std::any_cast<EntryNode *>(astBuilder.visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();

  // Create global scope
  globalScope = std::make_unique<Scope>(nullptr, this, ScopeType::GLOBAL, &ast->codeLoc);

  previousStage = AST_BUILDER;
  timer.stop();
  printStatusMessage("AST Builder", IO_CST, IO_AST, compilerOutput.times.astBuilder);
}

void SourceFile::runASTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;
  if (!cliOptions.dump.dumpAST && !cliOptions.testMode)
    return;
  // Check if this stage has already been done
  if (previousStage >= AST_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.astVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  ASTVisualizer astVisualizer(resourceManager, this);
  dotCode << " " << std::any_cast<std::string>(astVisualizer.visit(ast)) << "}";

  // Dump the serialized AST string and the SVG file
  compilerOutput.astString = dotCode.str();

  if (cliOptions.dump.dumpAST)
    visualizerOutput("AST", compilerOutput.astString);

  previousStage = AST_VISUALIZER;
  timer.stop();
  printStatusMessage("AST Visualizer", IO_AST, IO_AST, compilerOutput.times.astVisualizer);
}

void SourceFile::runImportCollector() { // NOLINT(misc-no-recursion)
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IMPORT_COLLECTOR)
    return;

  Timer timer(&compilerOutput.times.importCollector);
  timer.start();

  // Collect the imports for this source file
  ImportCollector importCollector(resourceManager, this);
  importCollector.visit(ast);

  previousStage = IMPORT_COLLECTOR;
  timer.stop();

  // Run first part of pipeline for the imported source file
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runFrontEnd();

  printStatusMessage("Import Collector", IO_AST, IO_AST, compilerOutput.times.importCollector);
}

void SourceFile::runSymbolTableBuilder() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= SYMBOL_TABLE_BUILDER)
    return;

  Timer timer(&compilerOutput.times.symbolTableBuilder);
  timer.start();

  // The symbol tables of all dependencies are present at this point, so we can merge the exported name registries in
  for (const auto &[importName, sourceFile] : dependencies)
    mergeNameRegistries(*sourceFile, importName);

  // Build symbol table of the current file
  SymbolTableBuilder symbolTableBuilder(resourceManager, this);
  symbolTableBuilder.visit(ast);

  previousStage = SYMBOL_TABLE_BUILDER;
  timer.stop();
  printStatusMessage("Symbol Table Builder", IO_AST, IO_AST, compilerOutput.times.symbolTableBuilder);
}

void SourceFile::runTypeCheckerPre() { // NOLINT(misc-no-recursion)
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= TYPE_CHECKER_PRE)
    return;

  // Type-check all dependencies first
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runTypeCheckerPre();

  Timer timer(&compilerOutput.times.typeCheckerPre);
  timer.start();

  // Then type-check the current file
  TypeChecker typeChecker(resourceManager, this, TC_MODE_PRE);
  typeChecker.visit(ast);

  previousStage = TYPE_CHECKER_PRE;
  timer.stop();
  printStatusMessage("Type Checker Pre", IO_AST, IO_AST, compilerOutput.times.typeCheckerPre);
}

void SourceFile::runTypeCheckerPost() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache, this stage has already been done, or not all dependants finished type checking
  if (restoredFromCache || !haveAllDependantsBeenTypeChecked())
    return;

  Timer timer(&compilerOutput.times.typeCheckerPost);
  timer.start();

  // Start type-checking loop. The type-checker can request a re-execution. The max number of type-checker runs is limited
  TypeChecker typeChecker(resourceManager, this, TC_MODE_POST);
  unsigned short typeCheckerRuns = 0;
  while (reVisitRequested) {
    typeCheckerRuns++;
    totalTypeCheckerRuns++;
    reVisitRequested = false;

    // Type-check the current file first. Multiple times, if requested
    timer.resume();
    typeChecker.visit(ast);
    timer.pause();

    // Then type-check all dependencies
    for (SourceFile *sourceFile : dependencies | std::views::values)
      sourceFile->runTypeCheckerPost();
  }

  checkForSoftErrors();

  // Check if all dyn variables were type-inferred successfully
  globalScope->ensureSuccessfulTypeInference();

  previousStage = TYPE_CHECKER_POST;
  timer.stop();
  printStatusMessage("Type Checker Post", IO_AST, IO_AST, compilerOutput.times.typeCheckerPost, typeCheckerRuns);

  // Save the JSON version in the compiler output
  if (cliOptions.dump.dumpSymbolTable || cliOptions.testMode)
    compilerOutput.symbolTableString = globalScope->getSymbolTableJSON().dump(/*indent=*/2);

  // Dump symbol table
  if (cliOptions.dump.dumpSymbolTable)
    dumpOutput(compilerOutput.symbolTableString, "Symbol Table", "symbol-table.json");
}

void SourceFile::runDependencyGraphVisualizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;
  if (!cliOptions.dump.dumpDependencyGraph && !cliOptions.testMode)
    return;
  // Check if this stage has already been done
  if (previousStage >= DEP_GRAPH_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.depGraphVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  DependencyGraphVisualizer depGraphVisualizer(resourceManager, this);
  depGraphVisualizer.getDependencyGraph(dotCode);
  dotCode << "}";

  // Dump the serialized AST string and the SVG file
  compilerOutput.depGraphString = dotCode.str();

  if (cliOptions.dump.dumpDependencyGraph)
    visualizerOutput("Dependency Graph", compilerOutput.depGraphString);

  previousStage = DEP_GRAPH_VISUALIZER;
  timer.stop();
  printStatusMessage("AST Visualizer", IO_AST, IO_AST, compilerOutput.times.depGraphVisualizer);
}

void SourceFile::runIRGenerator() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_GENERATOR)
    return;

  Timer timer(&compilerOutput.times.irGenerator);
  timer.start();

  // Create the LLVM module for this source file
  llvm::LLVMContext &llvmContext = cliOptions.useLTO ? resourceManager.ltoContext : context;
  llvmModule = std::make_unique<llvm::Module>(fileName, llvmContext);

  // Generate this source file
  IRGenerator irGenerator(resourceManager, this);
  irGenerator.visit(ast);

  // Print warning if the verifier is disabled
  if (isMainFile && cliOptions.disableVerifier) {
    const std::string warningMessage = "The LLVM verifier passes are disabled. Please use this cli option with caution.";
    compilerOutput.warnings.emplace_back(VERIFIER_DISABLED, warningMessage);
  }

  // Save the ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump unoptimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irString, "Unoptimized IR Code", "ir-code.ll");

  previousStage = IR_GENERATOR;
  timer.stop();
  printStatusMessage("IR Generator", IO_AST, IO_IR, compilerOutput.times.irGenerator);
}

void SourceFile::runDefaultIROptimizer() {
  assert(!cliOptions.useLTO);

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage > IR_OPTIMIZER || (previousStage == IR_OPTIMIZER && !cliOptions.testMode))
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.start();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizeDefault();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irOptString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code",
               "ir-code-O" + std::to_string(static_cast<uint8_t>(cliOptions.optLevel)) + ".ll");

  previousStage = IR_OPTIMIZER;
  timer.stop();
  printStatusMessage("IR Optimizer", IO_IR, IO_IR, compilerOutput.times.irOptimizer);
}

void SourceFile::runPreLinkIROptimizer() {
  assert(cliOptions.useLTO);

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.start();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizePreLink();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irOptString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code (pre-link)", "ir-code-lto-pre-link.ll");

  timer.pause();
}

void SourceFile::runBitcodeLinker() {
  assert(cliOptions.useLTO);

  // Skip if this is not the main source file
  if (!isMainFile)
    return;

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.resume();

  // Link all source files together
  BitcodeLinker linker(resourceManager);
  linker.link();

  timer.pause();
}

void SourceFile::runPostLinkIROptimizer() {
  assert(cliOptions.useLTO);

  // Skip if this is not the main source file
  if (!isMainFile)
    return;

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.resume();

  // Optimize LTO module
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizePostLink();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode) {
    llvm::Module *module = resourceManager.ltoModule.get();
    compilerOutput.irOptString = IRGenerator::getIRString(module, cliOptions);
  }

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code (post-Link)", "ir-code-lto-post-link.ll");

  previousStage = IR_OPTIMIZER;
  timer.stop();
  printStatusMessage("IR Optimizer", IO_IR, IO_IR, compilerOutput.times.irOptimizer);
}

void SourceFile::runObjectEmitter() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= OBJECT_EMITTER)
    return;

  // Skip if LTO is enabled and this is not the main source file
  if (cliOptions.useLTO && !isMainFile)
    return;

  Timer timer(&compilerOutput.times.objectEmitter);
  timer.start();

  // Deduce an object file path
  std::filesystem::path objectFilePath = cliOptions.outputDir / filePath.filename();
  objectFilePath.replace_extension("o");

  // Emit object for this source file
  const ObjectEmitter objectEmitter(resourceManager, this);
  objectEmitter.emit(objectFilePath);

  // Save assembly string in the compiler output
  if (cliOptions.isNativeTarget && (cliOptions.dump.dumpAssembly || cliOptions.testMode))
    objectEmitter.getASMString(compilerOutput.asmString);

  // Dump assembly code
  if (cliOptions.dump.dumpAssembly)
    dumpOutput(compilerOutput.asmString, "Assembly code", "assembly-code.s");

  // Add the object file to the linker objects
  resourceManager.linker.addObjectFilePath(objectFilePath.string());

  previousStage = OBJECT_EMITTER;
  timer.stop();
  printStatusMessage("Object Emitter", IO_IR, IO_OBJECT_FILE, compilerOutput.times.objectEmitter);
}

void SourceFile::concludeCompilation() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= FINISHED)
    return;

  // Cache the source file
  if (!cliOptions.ignoreCache)
    resourceManager.cacheManager.cacheSourceFile(this);

  // Save type registry as string in the compiler output
  if (isMainFile && (cliOptions.dump.dumpTypes || cliOptions.testMode))
    compilerOutput.typesString = TypeRegistry::dump();

  // Dump type registry
  if (isMainFile && cliOptions.dump.dumpTypes)
    dumpOutput(compilerOutput.typesString, "Type Registry", "type-registry.out");

  // Save cache statistics as string in the compiler output
  if (isMainFile && (cliOptions.dump.dumpCacheStats || cliOptions.testMode))
    dumpCacheStats();

  // Dump lookup cache statistics
  if (isMainFile && cliOptions.dump.dumpCacheStats)
    dumpOutput(compilerOutput.cacheStats, "Cache Statistics", "cache-stats.out");

  if (cliOptions.printDebugOutput)
    std::cout << "Finished compiling " << fileName << std::endl;

  previousStage = FINISHED;
}

void SourceFile::runFrontEnd() { // NOLINT(misc-no-recursion)
  runLexer();
  CHECK_ABORT_FLAG_V()
  runParser();
  CHECK_ABORT_FLAG_V()
  runCSTVisualizer();
  CHECK_ABORT_FLAG_V()
  runASTBuilder();
  CHECK_ABORT_FLAG_V()
  runASTVisualizer();
  CHECK_ABORT_FLAG_V()
  runImportCollector();
  CHECK_ABORT_FLAG_V()
  runSymbolTableBuilder();
  CHECK_ABORT_FLAG_V()
}

void SourceFile::runMiddleEnd() {
  // We need two runs here due to generics.
  // The first run to determine all concrete function/struct/interface substantiations
  runTypeCheckerPre(); // Visit the dependency tree from bottom to top
  CHECK_ABORT_FLAG_V()
  // The second run to ensure, also generic scopes are type-checked properly
  runTypeCheckerPost(); // Visit the dependency tree from top to bottom in topological order
  CHECK_ABORT_FLAG_V()
  // Visualize dependency graph
  runDependencyGraphVisualizer();
  CHECK_ABORT_FLAG_V()
}

void SourceFile::runBackEnd() { // NOLINT(misc-no-recursion)
  // Run backend for all dependencies first
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runBackEnd();

  runIRGenerator();
  CHECK_ABORT_FLAG_V()
  if (cliOptions.useLTO) {
    runPreLinkIROptimizer();
    CHECK_ABORT_FLAG_V()
    runBitcodeLinker();
    CHECK_ABORT_FLAG_V()
    runPostLinkIROptimizer();
    CHECK_ABORT_FLAG_V()
  } else {
    runDefaultIROptimizer();
    CHECK_ABORT_FLAG_V()
  }
  runObjectEmitter();
  CHECK_ABORT_FLAG_V()
  concludeCompilation();

  if (isMainFile) {
    resourceManager.totalTimer.stop();
    if (cliOptions.printDebugOutput)
      dumpCompilationStats();
  }
}

void SourceFile::addDependency(SourceFile *sourceFile, const ASTNode *declNode, const std::string &dependencyName,
                               const std::string &path) {
  // Check if this would cause a circular dependency
  std::stack<const SourceFile *> dependencyCircle;
  if (isAlreadyImported(path, dependencyCircle)) {
    // Build the error message
    std::stringstream errorMessage;
    errorMessage << "Circular import detected while importing '" << sourceFile->fileName << "':\n\n";
    errorMessage << CommonUtil::getCircularImportMessage(dependencyCircle);
    throw SemanticError(declNode, CIRCULAR_DEPENDENCY, errorMessage.str());
  }

  // Add the dependency
  sourceFile->isMainFile = false;
  dependencies.insert({dependencyName, sourceFile});

  // Add the dependant
  sourceFile->dependants.push_back(this);
}

bool SourceFile::imports(const SourceFile *sourceFile) const {
  return std::ranges::any_of(dependencies, [=](const auto &dependency) { return dependency.second == sourceFile; });
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch, // NOLINT(misc-no-recursion)
                                   std::stack<const SourceFile *> &circle) const {
  circle.push(this);

  // Check if the current source file corresponds to the path to search
  if (std::filesystem::equivalent(filePath, filePathSearch))
    return true;

  // Check dependants recursively
  for (const SourceFile *dependant : dependants)
    if (dependant->isAlreadyImported(filePathSearch, circle))
      return true;

  // If no dependant was found, remove the current source file from the circle to continue with the next sibling
  circle.pop();
  return false;
}

SourceFile *SourceFile::requestRuntimeModule(RuntimeModule runtimeModule) {
  // Check if the module was already imported
  if (isRuntimeModuleAvailable(runtimeModule))
    return resourceManager.runtimeModuleManager.getModule(runtimeModule);
  return resourceManager.runtimeModuleManager.requestModule(this, runtimeModule);
}

bool SourceFile::isRuntimeModuleAvailable(RuntimeModule runtimeModule) const { return importedRuntimeModules & runtimeModule; }

void SourceFile::addNameRegistryEntry(const std::string &symbolName, uint64_t typeId, SymbolTableEntry *entry, Scope *scope,
                                      bool keepNewOnCollision, SymbolTableEntry *importEntry) {
  if (keepNewOnCollision || !exportedNameRegistry.contains(symbolName)) // Overwrite potential existing entry
    exportedNameRegistry[symbolName] = {symbolName, typeId, entry, scope, importEntry};
  else // Name collision => we must remove the existing entry
    exportedNameRegistry.erase(symbolName);
}

const NameRegistryEntry *SourceFile::getNameRegistryEntry(const std::string &symbolName) const {
  if (!exportedNameRegistry.contains(symbolName))
    return nullptr;

  // Resolve registry entry for the given name
  assert(exportedNameRegistry.contains(symbolName));
  const NameRegistryEntry *entry = &exportedNameRegistry.at(symbolName);

  // Mark the import entry as used
  if (entry->importEntry != nullptr)
    entry->importEntry->used = true;

  return entry;
}

llvm::Type *SourceFile::getLLVMType(const Type *type) {
  // Check if the type is already in the mapping
  const auto it = typeToLLVMTypeMapping.find(type);
  if (it != typeToLLVMTypeMapping.end())
    return it->second;

  // If not, generate the LLVM type
  llvm::Type *llvmType = type->toLLVMType(this);
  typeToLLVMTypeMapping[type] = llvmType;
  return llvmType;
}

void SourceFile::checkForSoftErrors() const {
  // Check if there are any soft errors and if so, print them
  if (!resourceManager.errorManager.softErrors.empty()) {
    std::stringstream errorStream;
    errorStream << "There are unresolved errors. Please fix them and recompile.";
    for (const auto &[codeLoc, message] : resourceManager.errorManager.softErrors)
      errorStream << "\n\n" << message;
    throw CompilerError(UNRESOLVED_SOFT_ERRORS, errorStream.str());
  }
}

void SourceFile::collectAndPrintWarnings() { // NOLINT(misc-no-recursion)
  // Print warnings for all dependencies
  for (SourceFile *sourceFile : dependencies | std::views::values)
    if (!sourceFile->isStdFile)
      sourceFile->collectAndPrintWarnings();
  // Collect warnings for this file
  if (!ignoreWarnings)
    globalScope->collectWarnings(compilerOutput.warnings);
  // Print warnings for this file
  for (const CompilerWarning &warning : compilerOutput.warnings)
    warning.print();
}

const SourceFile *SourceFile::getRootSourceFile() const { // NOLINT(misc-no-recursion)
  return isMainFile ? this : parent->getRootSourceFile();
}

bool SourceFile::isRT(RuntimeModule runtimeModule) const {
  assert(IDENTIFYING_TOP_LEVEL_NAMES.contains(runtimeModule));
  const char *topLevelName = IDENTIFYING_TOP_LEVEL_NAMES.at(runtimeModule);
  if (!exportedNameRegistry.contains(topLevelName))
    return false;
  return exportedNameRegistry.at(topLevelName).targetEntry->scope == globalScope.get();
}

bool SourceFile::haveAllDependantsBeenTypeChecked() const {
  return std::ranges::all_of(dependants, [](const SourceFile *dependant) { return dependant->totalTypeCheckerRuns >= 1; });
}

/**
 * Acquire all publicly visible symbols from the imported source file and put them in the name registry of the current one.
 * But only do that for the symbols that are actually defined in the imported source file. Do not allow transitive dependencies.
 * Here, we also register privately visible symbols to know that the symbol exist. The error handling regarding the visibility
 * is issued later in the pipeline.
 *
 * @param importedSourceFile Imported source file
 * @param importName First fragment of all fully qualified symbol names from that import
 */
void SourceFile::mergeNameRegistries(const SourceFile &importedSourceFile, const std::string &importName) {
  // Retrieve import entry
  SymbolTableEntry *importEntry = globalScope->lookupStrict(importName);
  assert(importEntry != nullptr || importName.starts_with("__")); // Runtime imports start with two underscores

  for (const auto &[originalName, entry] : importedSourceFile.exportedNameRegistry) {
    // Skip if we introduce a transitive dependency
    if (entry.targetScope->sourceFile->globalScope != importedSourceFile.globalScope)
      continue;
    // Add the fully qualified name
    std::string newName = importName;
    newName += SCOPE_ACCESS_TOKEN;
    newName += originalName;
    exportedNameRegistry.insert({newName, {newName, entry.typeId, entry.targetEntry, entry.targetScope, importEntry}});
    // Add the shortened name, considering the name collision
    const bool keepOnCollision = importedSourceFile.alwaysKeepSymbolsOnNameCollision;
    addNameRegistryEntry(originalName, entry.typeId, entry.targetEntry, entry.targetScope, keepOnCollision, importEntry);
  }
}

void SourceFile::dumpCacheStats() {
  std::stringstream cacheStats;
  cacheStats << FunctionManager::dumpLookupCacheStatistics() << std::endl;
  cacheStats << StructManager::dumpLookupCacheStatistics() << std::endl;
  cacheStats << InterfaceManager::dumpLookupCacheStatistics() << std::endl;
  compilerOutput.cacheStats = cacheStats.str();
}

void SourceFile::dumpCompilationStats() const {
  const size_t sourceFileCount = resourceManager.sourceFiles.size();
  const size_t totalLineCount = resourceManager.getTotalLineCount();
  const size_t totalTypeCount = TypeRegistry::getTypeCount();
  const size_t allocatedBytes = resourceManager.astNodeAlloc.getTotalAllocatedSize();
  const size_t allocationCount = resourceManager.astNodeAlloc.getAllocationCount();
  const size_t totalDuration = resourceManager.totalTimer.getDurationMilliseconds();
  std::cout << "\nSuccessfully compiled " << std::to_string(sourceFileCount) << " source file(s)";
  std::cout << " or " << std::to_string(totalLineCount) << " lines in total.\n";
  std::cout << "Total number of blocks allocated via BlockAllocator: " << CommonUtil::formatBytes(allocatedBytes);
  std::cout << " in " << std::to_string(allocationCount) << " allocations.\n";
#ifndef NDEBUG
  resourceManager.astNodeAlloc.printAllocatedClassStatistic();
#endif
  std::cout << "Total number of types: " << std::to_string(totalTypeCount) << "\n";
  std::cout << "Total compile time: " << std::to_string(totalDuration) << " ms\n";
}

void SourceFile::dumpOutput(const std::string &content, const std::string &caption, const std::string &fileSuffix) const {
  if (cliOptions.dump.dumpToFiles) {
    // Dump to file
    const std::string dumpFileName = filePath.stem().string() + "-" + fileSuffix;
    std::filesystem::path dumpFilePath = cliOptions.outputDir / dumpFileName;
    dumpFilePath.make_preferred();
    FileUtil::writeToFile(dumpFilePath, content);
  } else {
    // Dump to console
    std::cout << "\n" << caption << ":\n" << content;
  }

  // If the abort after dump is requested, set the abort compilation flag
  if (cliOptions.dump.abortAfterDump) {
    // If this is an IR dump whilst having optimization enabled, we may not abort when dumping unoptimized IR,
    // because we also have to dump the optimized IR
    if (cliOptions.dump.dumpIR && fileSuffix == "ir-code.ll") {
      resourceManager.abortCompilation = cliOptions.optLevel == OptLevel::O0;
    } else {
      resourceManager.abortCompilation = true;
    }
  }
}

void SourceFile::visualizerPreamble(std::stringstream &output) const {
  if (isMainFile)
    output << "digraph {\n rankdir=\"TB\";\n";
  else
    output << "subgraph {\n";
  output << " label=\"" << filePath.generic_string() << "\";\n";
}

void SourceFile::visualizerOutput(std::string outputName, const std::string &output) const {
  if (cliOptions.dump.dumpToFiles) {
    // Check if graphviz is installed
    // GCOV_EXCL_START
    if (!SystemUtil::isGraphvizInstalled())
      throw CompilerError(IO_ERROR, "Please check if you have installed Graphviz and added it to the PATH variable");
    // GCOV_EXCL_STOP

    // Write to a dot file
    std::ranges::transform(outputName, outputName.begin(), ::tolower);
    dumpOutput(output, outputName, outputName + ".dot");

    // Generate SVG. This only works if the dot code was dumped into a file
    std::cout << "\nGenerating SVG file ... ";
    const std::string dotFileName = filePath.stem().string() + "-" + outputName + ".dot";
    std::filesystem::path dotFilePath = cliOptions.outputDir / dotFileName;
    std::filesystem::path svgFilePath = dotFilePath;
    svgFilePath.replace_extension("svg");
    dotFilePath.make_preferred();
    svgFilePath.make_preferred();
    SystemUtil::exec("dot -T svg -o" + svgFilePath.string() + " " + dotFilePath.string());
    std::cout << "done.\nSVG file can be found at: " << svgFilePath << "\n";
  } else {
    // Dump to console
    std::cout << "\nSerialized " << outputName << ":\n\n" << output << "\n";
  }

  // If the abort after dump is requested, set the abort compilation flag
  if (cliOptions.dump.abortAfterDump)
    resourceManager.abortCompilation = true;
}

void SourceFile::printStatusMessage(const char *stage, const CompileStageIOType &in, const CompileStageIOType &out,
                                    uint64_t stageRuntime, unsigned short stageRuns) const {
  if (cliOptions.printDebugOutput) {
    static constexpr const char *const compilerStageIoTypeName[6] = {"Code", "Tokens", "CST", "AST", "IR", "Obj"};
    // Build output string
    std::stringstream outputStr;
    outputStr << "[" << stage << "] for " << fileName << ": ";
    outputStr << compilerStageIoTypeName[in] << " --> " << compilerStageIoTypeName[out];
    outputStr << " (" << std::to_string(stageRuntime) << " ms";
    if (stageRuns > 0)
      outputStr << "; " << std::to_string(stageRuns) << " run(s)";
    outputStr << ")\n";
    // Print
    std::cout << outputStr.str();
  }
}

} // namespace spice::compiler