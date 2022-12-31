// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include <ast/ASTBuilder.h>
#include <astoptimizer/ASTOptimizer.h>
#include <borrowchecker/BorrowChecker.h>
#include <escapeanalyzer/EscapeAnalyzer.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <global/GlobalResourceManager.h>
#include <importcollector/ImportCollector.h>
#include <irgenerator/IRGenerator.h>
#include <iroptimizer/IROptimizer.h>
#include <objectemitter/ObjectEmitter.h>
#include <symboltablebuilder/SymbolTable.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeChecker.h>
#include <util/CodeLoc.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>
#include <util/Timer.h>
#include <visualizer/ASTVisualizer.h>
#include <visualizer/CSTVisualizer.h>

namespace spice::compiler {

SourceFile::SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name, const std::string &filePath,
                       bool stdFile)
    : resourceManager(resourceManager), tout(resourceManager.tout), name(std::move(name)), filePath(filePath), stdFile(stdFile),
      parent(parent) {
  this->objectFilePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + FileUtil::getFileName(filePath) + ".o";

  // Deduce fileName and fileDir
  fileName = std::filesystem::path(filePath).filename().string();
  fileDir = std::filesystem::path(filePath).parent_path().string();

  if (mainFile)
    resourceManager.totalTimer.start();
}

void SourceFile::runLexer() {
  Timer timer(&compilerOutput.times.lexer);
  timer.start();

  // Read from file
  std::ifstream fileInputStream(filePath);
  if (!fileInputStream)
    throw std::runtime_error("Source file at path '" + filePath + "' does not exist.");

  // Create error handlers for lexer and parser
  antlrCtx.lexerErrorHandler = std::make_unique<AntlrThrowingErrorListener>(LEXER);
  antlrCtx.parserErrorHandler = std::make_unique<AntlrThrowingErrorListener>(PARSER);

  // Tokenize input
  antlrCtx.inputStream = std::make_unique<antlr4::ANTLRInputStream>(fileInputStream);
  antlrCtx.lexer = std::make_unique<SpiceLexer>(antlrCtx.inputStream.get());
  antlrCtx.lexer->removeErrorListeners();
  antlrCtx.lexer->addErrorListener(antlrCtx.lexerErrorHandler.get());
  antlrCtx.tokenStream = std::make_unique<antlr4::CommonTokenStream>(antlrCtx.lexer.get());

  // Calculate cache key
  std::stringstream cacheKeyString;
  cacheKeyString << std::hex << std::hash<std::string>{}(antlrCtx.tokenStream->getText());
  cacheKey = cacheKeyString.str();

  // Try to load from cache
  if (!resourceManager.cliOptions.ignoreCache)
    restoredFromCache = resourceManager.cacheManager.lookupSourceFile(this);

  timer.stop();
  printStatusMessage("Lexer", IO_CODE, IO_TOKENS, compilerOutput.times.lexer);
}

void SourceFile::runParser() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.parser);
  timer.start();

  // Parse input
  antlrCtx.parser = std::make_unique<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  timer.stop();
  printStatusMessage("Parser", IO_TOKENS, IO_CST, compilerOutput.times.parser);
}

void SourceFile::runCSTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!resourceManager.cliOptions.dumpCST && !resourceManager.cliOptions.testMode))
    return;

  Timer timer(&compilerOutput.times.cstVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  CSTVisualizer cstVisualizer(resourceManager, this, antlrCtx.lexer.get(), antlrCtx.parser.get());
  dotCode << std::any_cast<std::string>(cstVisualizer.visit(antlrCtx.parser->entry())) << "}";
  antlrCtx.parser->reset();

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.cstString = dotCode.str();
    if (resourceManager.cliOptions.dumpCST)
      visualizerOutput("CST", compilerOutput.cstString);
  }

  timer.stop();
  printStatusMessage("CST Visualizer", IO_CST, IO_CST, compilerOutput.times.cstVisualizer);
}

void SourceFile::runASTBuilder() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.astBuilder);
  timer.start();

  // Create AST
  ast = std::make_unique<EntryNode>(nullptr, CodeLoc(1, 1, filePath));

  // Build AST for this source file
  ASTBuilder astBuilder(resourceManager, this, ast.get(), antlrCtx.inputStream.get());
  astBuilder.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();

  // Create global scope
  globalScope = std::make_unique<Scope>(nullptr, this, SCOPE_GLOBAL, &ast->codeLoc);

  timer.stop();
  printStatusMessage("AST Builder", IO_CST, IO_AST, compilerOutput.times.astBuilder);
}

void SourceFile::runASTOptimizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.astOptimizer);
  timer.start();

  ASTOptimizer astOptimizer(resourceManager, this);
  astOptimizer.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();
  printStatusMessage("AST Optimizer", IO_AST, IO_AST, compilerOutput.times.astOptimizer);
}

void SourceFile::runASTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!resourceManager.cliOptions.dumpAST && !resourceManager.cliOptions.testMode))
    return;

  Timer timer(&compilerOutput.times.astVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  ASTVisualizer astVisualizer(resourceManager, this, ast.get());
  dotCode << std::any_cast<std::string>(astVisualizer.visit(ast.get())) << "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.astString = dotCode.str();
    if (resourceManager.cliOptions.dumpAST)
      visualizerOutput("AST", compilerOutput.astString);
  }

  timer.stop();
  printStatusMessage("AST Visualizer", IO_AST, IO_AST, compilerOutput.times.astVisualizer);
}

void SourceFile::runImportCollector() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.importCollector);
  timer.start();

  // Collect the imports for this source file
  ImportCollector importCollector(resourceManager, this);
  importCollector.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();

  // Run first part of pipeline for the imported source file
  for (const auto &dependency : dependencies)
    dependency.second.first->runFrontEnd();

  printStatusMessage("Import Collector", IO_AST, IO_AST, compilerOutput.times.importCollector);
}

void SourceFile::runSymbolTableBuilder() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.symbolTableBuilder);
  timer.start();

  // Build symbol table of the current file
  SymbolTableBuilder symbolTableBuilder(resourceManager, this);
  symbolTableBuilder.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();
  printStatusMessage("Symbol Table Builder", IO_AST, IO_AST, compilerOutput.times.symbolTableBuilder);
}

void SourceFile::runTypeChecker() { // NOLINT(misc-no-recursion)
  // We need two runs here due to generics.
  // The first run to determine all concrete substantiations of potentially generic elements
  runTypeCheckerFirst(); // Visit dependency tree from bottom to top
  // The second run to ensure, also generic scopes are type-checked properly
  runTypeCheckerSecond(); // Visit dependency tree from top to bottom
}

void SourceFile::runTypeCheckerFirst() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Type-check all dependencies first
  for (const auto &[importName, sourceFile] : dependencies) {
    // Type-check
    sourceFile.first->runTypeCheckerFirst();
    // Merge exported names of the imported file with the existing ones
    mergeNameRegistries(*sourceFile.first, importName);
  }

  Timer timer(&compilerOutput.times.typeCheckerPre);
  timer.start();

  // Then type-check the current file
  TypeChecker typeChecker(resourceManager, this, TC_MODE_PREPARE);
  typeChecker.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();
  printStatusMessage("Type Checker Pre", IO_AST, IO_AST, compilerOutput.times.typeCheckerPre);
}

void SourceFile::runTypeCheckerSecond() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.typeCheckerPost);
  timer.start();

  // Start type-checking loop. The type-checker can request a re-execution. The max number of type-checker runs is limited
  TypeChecker typeChecker(resourceManager, this, TC_MODE_CHECK);
  unsigned short runNumber = 0;
  do {
    runNumber++;

    // Type-check the current file first, if requested multiple times
    timer.resume();
    typeChecker.visit(static_cast<EntryNode *>(ast.get()));
    timer.pause();

    // Then type-check all dependencies
    for (const auto &[importName, sourceFile] : dependencies)
      sourceFile.first->runTypeCheckerSecond();

    if (runNumber >= 25)
      throw std::runtime_error("Internal compiler error: Number of type checker runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (typeChecker.reVisitRequested);

  timer.stop();
  printStatusMessage("Type Checker Post", IO_AST, IO_AST, compilerOutput.times.typeCheckerPost);

  // Save the JSON version in the compiler output
  compilerOutput.symbolTableString = globalScope->getSymbolTableJSON().dump(/*indent=*/2);

  // Dump symbol table
  if (resourceManager.cliOptions.dumpSymbolTables) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << compilerOutput.symbolTableString << "\n\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::runBorrowChecker() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Borrow-check all dependencies first
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runBorrowChecker();

  Timer timer(&compilerOutput.times.borrowChecker);
  timer.start();

  // Then borrow-check current file
  BorrowChecker borrowChecker(resourceManager, this);
  borrowChecker.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();
  printStatusMessage("Borrow Checker", IO_AST, IO_AST, compilerOutput.times.borrowChecker);
}

void SourceFile::runEscapeAnalyzer() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Escape-analyze all dependencies first
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runEscapeAnalyzer();

  Timer timer(&compilerOutput.times.escapeAnalyzer);
  timer.start();

  // Then escape-analyze current file
  EscapeAnalyzer escapeAnalyzer(resourceManager, this);
  escapeAnalyzer.visit(static_cast<EntryNode *>(ast.get()));

  timer.stop();
  printStatusMessage("Escape Analyzer", IO_AST, IO_AST, compilerOutput.times.escapeAnalyzer);
}

void SourceFile::runIRGenerator() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.irGenerator);
  timer.start();

  // Create LLVM module for this source file
  llvmModule = std::make_unique<llvm::Module>(FileUtil::getFileName(filePath), resourceManager.context);

  // Generate this source file
  IRGenerator irGenerator(resourceManager, this);
  irGenerator.visit(static_cast<EntryNode *>(ast.get()));

  // Save the JSON version in the compiler output
  compilerOutput.irString = irGenerator.getIRString();

  // Dump unoptimized IR code
  if (resourceManager.cliOptions.dumpIR)                                  // GCOV_EXCL_LINE
    tout.println("\nUnoptimized IR code:\n" + irGenerator.getIRString()); // GCOV_EXCL_LINE

  timer.stop();
  printStatusMessage("IR Generator", IO_AST, IO_IR, compilerOutput.times.irGenerator, true);
}

void SourceFile::runIROptimizer() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Skip this stage if optimization is disabled
  if (resourceManager.cliOptions.optLevel < 1 || resourceManager.cliOptions.optLevel > 5)
    return;

  Timer timer(&compilerOutput.times.irGenerator);
  timer.start();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.optimize();

  // Save the JSON version in the compiler output
  compilerOutput.irOptString = irOptimizer.getOptimizedIRString();

  // Dump optimized IR code
  if (resourceManager.cliOptions.dumpIR)                                         // GCOV_EXCL_LINE
    tout.println("\nOptimized IR code:\n" + irOptimizer.getOptimizedIRString()); // GCOV_EXCL_LINE

  timer.stop();
  printStatusMessage("IR Optimizer", IO_IR, IO_IR, compilerOutput.times.irGenerator, true);
}

void SourceFile::runObjectEmitter() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  Timer timer(&compilerOutput.times.objectEmitter);
  timer.start();

  // Emit object for this source file
  ObjectEmitter objectEmitter(resourceManager, this);
  objectEmitter.emit();

  // Dump assembly code
  if (resourceManager.cliOptions.dumpAssembly) { // GCOV_EXCL_START
    tout.println("\nAssembly code:\n");
    objectEmitter.dumpAsm();
  } // GCOV_EXCL_STOP

  // Add object file to linker objects
  resourceManager.linker.addObjectFilePath(objectFilePath);

  timer.stop();
  printStatusMessage("Object Emitter", IO_IR, IO_OBJECT_FILE, compilerOutput.times.objectEmitter, true);
}

void SourceFile::concludeCompilation() {
  // Cache the source file
  if (!resourceManager.cliOptions.ignoreCache)
    resourceManager.cacheManager.cacheSourceFile(this);

  // Print warning if verifier is disabled
  if (parent == nullptr && resourceManager.cliOptions.disableVerifier) {
    const std::string warningMessage =
        CompilerWarning(VERIFIER_DISABLED, "The LLVM verifier passes are disabled. Please use this cli option carefully.")
            .warningMessage;
    tout.println("\n" + warningMessage);
  }

  if (resourceManager.cliOptions.printDebugOutput)
    tout.println("Finished compiling " + fileName);
}

void SourceFile::runFrontEnd() { // NOLINT(misc-no-recursion)
  runLexer();
  runParser();
  runCSTVisualizer();
  runASTBuilder();
  runASTOptimizer();
  runASTVisualizer();
  runImportCollector();
  runSymbolTableBuilder();
}

void SourceFile::runMiddleEnd() {
  runTypeCheckerFirst();
  runTypeCheckerSecond();
  runBorrowChecker();
  runEscapeAnalyzer();
}

void SourceFile::runBackEnd() { // NOLINT(misc-no-recursion)
  // Run backend for all dependencies first
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runBackEnd();

  // Submit source file compilation to the task queue
  resourceManager.threadPool.push_task([&]() {
    runIRGenerator();
    runIROptimizer();
    runObjectEmitter();
    concludeCompilation();
  });

  // Wait until all compile tasks are done
  if (mainFile)
    resourceManager.threadPool.wait_for_tasks();

  if (mainFile) {
    resourceManager.totalTimer.stop();
    std::cout << "Total compile time: " << std::to_string(resourceManager.totalTimer.getDurationMilliseconds()) << " ms\n";
  }
}

std::shared_ptr<SourceFile> SourceFile::createSourceFile(const std::string &dependencyName, const std::string &path,
                                                         bool isStdFile) {
  return std::make_shared<SourceFile>(resourceManager, this, dependencyName, path, isStdFile);
}

void SourceFile::addDependency(const std::shared_ptr<SourceFile> &sourceFile, const ASTNode *declNode,
                               const std::string &dependencyName, const std::string &path) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(path))
    throw SemanticError(declNode, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + path + "'");

  // Add the dependency
  sourceFile->mainFile = false;
  dependencies.insert({dependencyName, {sourceFile, declNode}});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const { // NOLINT(misc-no-recursion)
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check parent recursively
  return parent != nullptr && parent->isAlreadyImported(filePathSearch);
}

void SourceFile::collectAndPrintWarnings() { // NOLINT(misc-no-recursion)
  // Print warnings for all dependencies
  for (const auto &dependency : dependencies) {
    if (!dependency.second.first->stdFile)
      dependency.second.first->collectAndPrintWarnings();
  }
  // Collect warnings for this file
  globalScope->collectWarnings(compilerOutput.warnings);
  // Print warnings for this file
  for (const CompilerWarning &warning : compilerOutput.warnings)
    warning.print();
}

void SourceFile::requestRuntimeModule(const RuntimeModuleName &moduleName) {
  resourceManager.runtimeModuleManager.requestModule(this, moduleName);
}

void SourceFile::addNameRegistryEntry(const std::string &name, SymbolTableEntry *entry, Scope *scope,
                                      bool keepNewOnCollision /*=true*/, const std::string &predecessorName /*=""*/) {
  if (keepNewOnCollision || !exportedNameRegistry.contains(name)) // Overwrite potential existing entry
    exportedNameRegistry[name] = {name, entry, scope, predecessorName};
  else // Name collision => we must remove the existing entry
    exportedNameRegistry.erase(name);
}

const NameRegistryEntry *SourceFile::getNameRegistryEntry(std::string symbolName) const {
  if (!exportedNameRegistry.contains(symbolName))
    return nullptr;
  const NameRegistryEntry *registryEntry;
  do {
    registryEntry = &exportedNameRegistry.at(symbolName);
    symbolName = registryEntry->predecessorName;
  } while (!registryEntry->predecessorName.empty());
  return registryEntry;
}

/**
 * Acquire all publicly visible symbols from the imported source file and put them in the name registry of the current one.
 * Here, we also register privately visible symbols, to know that the symbol exist. The error handling regarding the visibility
 * is issued later in the pipeline.
 *
 * @param importedSourceFile Imported source file
 * @param importName First fragment of all fully-qualified symbol names from that import
 */
void SourceFile::mergeNameRegistries(const SourceFile &importedSourceFile, const std::string &importName) {
  for (const auto &[originalName, entry] : importedSourceFile.exportedNameRegistry) {
    // Add fully qualified name
    const std::string newName = importName + "::" + originalName;
    exportedNameRegistry.insert({newName, {newName, entry.targetEntry, entry.targetScope}});
    // Add the shortened name, considering the name collision
    addNameRegistryEntry(originalName, entry.targetEntry, entry.targetScope, /*keepNewOnCollision=*/false, newName);
  }
}

void SourceFile::visualizerPreamble(std::stringstream &output) const {
  if (parent == nullptr)
    output << "digraph {\n rankdir=\"TB\";\n";
  else
    output << "subgraph {\n";
  std::string replacedFilePath = filePath;
  CommonUtil::replaceAll(replacedFilePath, "\\", "/");
  output << " label=\"" + replacedFilePath + "\";\n ";
}

void SourceFile::visualizerOutput(std::string outputName, const std::string &output) const {
  // Dump to console
  std::cout << "\nSerialized " << outputName << ":\n\n" << output << "\n";

  // Check if the dot command exists
  if (FileUtil::isCommandAvailable("dot")) // GCOV_EXCL_START
    throw std::runtime_error(
        "Please check if you have installed 'Graphviz Dot' and added it to the PATH variable"); // GCOV_EXCL_STOP

  // Generate SVG
  std::cout << "\nGenerating SVG file ... ";
  std::transform(outputName.begin(), outputName.end(), outputName.begin(), ::tolower);
  std::string fileBasePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + outputName + ".dot";
  FileUtil::writeToFile(fileBasePath, output);
  FileUtil::exec("dot -Tsvg -o" + fileBasePath + ".svg " + fileBasePath + ".dot");
  std::cout << "done.\nSVG file can be found at: " << fileBasePath << ".svg\n";
}

void SourceFile::printStatusMessage(const char *stage, const CompileStageIOType &in, const CompileStageIOType &out,
                                    uint64_t &stageRuntime, bool fromThread /*=false*/) const {
  if (resourceManager.cliOptions.printDebugOutput) {
    const char *const compilerStageIoTypeName[] = {"Code", "Tokens", "CST", "AST", "IR", "OBJECT_FILE"};
    // Build output string
    std::stringstream outputStr;
    outputStr << "[" << stage << "] for " << fileName << ": ";
    outputStr << compilerStageIoTypeName[in] << " --> " << compilerStageIoTypeName[out];
    outputStr << " (" << std::to_string(stageRuntime) << " ms)\n";
    // Print
    if (fromThread) {
      tout.print(outputStr.str());
    } else {
      std::cout << outputStr.str();
    }
  }
}

} // namespace spice::compiler