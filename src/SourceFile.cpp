// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include "typechecker/TypeChecker.h"
#include <analyzer/AnalyzerVisitor.h>
#include <analyzer/PreAnalyzerVisitor.h>
#include <ast/ASTBuilder.h>
#include <astoptimizer/ASTOptimizer.h>
#include <borrowchecker/BorrowChecker.h>
#include <escapeanalyzer/EscapeAnalyzer.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <generator/GeneratorVisitor.h>
#include <global/GlobalResourceManager.h>
#include <importcollector/ImportCollector.h>
#include <irgenerator/IRGenerator.h>
#include <iroptimizer/IROptimizer.h>
#include <objectemitter/ObjectEmitter.h>
#include <symbol/SymbolTable.h>
#include <util/CodeLoc.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>
#include <visualizer/ASTVisualizer.h>
#include <visualizer/CSTVisualizer.h>

SourceFile::SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name, const std::string &filePath,
                       bool stdFile)
    : resourceManager(resourceManager), name(std::move(name)), filePath(filePath), stdFile(stdFile), parent(parent) {
  this->objectFilePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + FileUtil::getFileName(filePath) + ".o";

  // Deduce fileName and fileDir
  fileName = std::filesystem::path(filePath).filename().string();
  fileDir = std::filesystem::path(filePath).parent_path().string();
}

void SourceFile::runLexer() {
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

  printStatusMessage("Lexer", IO_CODE, IO_TOKENS);
}

void SourceFile::runParser() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Parse input
  antlrCtx.parser = std::make_unique<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  printStatusMessage("Parser", IO_TOKENS, IO_CST);
}

void SourceFile::runCSTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!resourceManager.cliOptions.dumpCST && !resourceManager.cliOptions.testMode))
    return;

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  CSTVisualizer cstVisualizer(resourceManager, this, antlrCtx.lexer.get(), antlrCtx.parser.get());
  dotCode << any_cast<std::string>(cstVisualizer.visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();
  dotCode << "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.cstString = dotCode.str();
    if (resourceManager.cliOptions.dumpCST)
      visualizerOutput("CST", compilerOutput.cstString);
  }

  printStatusMessage("CST Visualizer", IO_CST, IO_CST);
}

void SourceFile::runASTBuilder() {
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Create AST
  ast = std::make_unique<EntryNode>(nullptr, CodeLoc(1, 1, filePath));

  // Build AST for this source file
  ASTBuilder astBuilder(resourceManager, this, ast.get(), antlrCtx.inputStream.get());
  astBuilder.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();

  // Create global scope
  globalScope = std::make_unique<Scope>(nullptr, SCOPE_GLOBAL);

  printStatusMessage("AST Builder", IO_CST, IO_AST);
}

void SourceFile::runASTOptimizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;

  ASTOptimizer astOptimizer(resourceManager, this);
  astOptimizer.visit(ast.get());

  printStatusMessage("AST Optimizer", IO_AST, IO_AST);
}

void SourceFile::runASTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!resourceManager.cliOptions.dumpAST && !resourceManager.cliOptions.testMode))
    return;

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  ASTVisualizer astVisualizer(resourceManager, this, ast.get());
  dotCode << any_cast<std::string>(astVisualizer.visit(ast.get()));
  dotCode << "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.astString = dotCode.str();
    if (resourceManager.cliOptions.dumpAST)
      visualizerOutput("AST", compilerOutput.astString);
  }

  printStatusMessage("AST Visualizer", IO_AST, IO_AST);
}

void SourceFile::runImportCollector() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Collect the imports for this source file
  ImportCollector importCollector(resourceManager, this);
  importCollector.visit(ast.get());

  // Run first part of pipeline for the imported source file
  for (const auto &dependency : dependencies)
    dependency.second.first->runFrontEnd();

  printStatusMessage("Import Collector", IO_AST, IO_AST);
}

void SourceFile::runTypeChecker() { // NOLINT(misc-no-recursion)
  // We need two runs here due to generics.
  // The first run to determine all concrete substantiations of potentially generic elements
  runTypeCheckerFirst(); // Visit dependency tree from bottom to top
  // The second run to ensure, also generic scopes are type-checked properly
  runTypeCheckerSecond(); // Visit dependency tree from top to bottom

  printStatusMessage("Type Checker", IO_AST, IO_AST);
}

void SourceFile::runTypeCheckerFirst() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Type-check all dependencies first
  for (const auto &[importName, sourceFile] : dependencies) {
    // Type-check the imported source file
    sourceFile.first->runTypeChecker();

    // Mount symbol table to the current one
    // sourceFile.first->globalScope->symbolTable.parent = &globalScope->symbolTable;
    // globalScope->symbolTable->mountChildBlock(importName, sourceFile.first->symbolTable.get());

    // Insert import symbol
    // SymbolType type = SymbolType(TY_IMPORT);
    // SymbolSpecifiers specifiers(type);
    // symbolTable->insert(importName, type, specifiers, INITIALIZED, sourceFile.second);
  }

  // Then type-check the current file
  TypeChecker typeChecker(resourceManager, this, 1);
  typeChecker.visit(ast.get());
}

void SourceFile::runTypeCheckerSecond() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Type-check the current file first, if requested multiple times
  bool repetitionRequired;
  unsigned short typeCheckRuns = 0;
  do {
    TypeChecker typeChecker(resourceManager, this, 1);
    repetitionRequired = any_cast<bool>(typeChecker.visit(ast.get()));
    typeCheckRuns++;
    if (typeCheckRuns >= 10)
      throw std::runtime_error("Internal compiler error: Number of type checker runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (repetitionRequired);

  // Then type-check all dependencies
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runTypeChecker();

  // Save the JSON version in the compiler output
  compilerOutput.symbolTableString = globalScope->symbolTable.toJSON().dump(2);

  // Dump symbol table
  if (resourceManager.cliOptions.dumpSymbolTables) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << compilerOutput.symbolTableString << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::runBorrowChecker() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Borrow-check all dependencies first
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runBorrowChecker();

  // Then borrow-check current file
  BorrowChecker borrowChecker(resourceManager, this);
  borrowChecker.visit(ast.get());

  printStatusMessage("Borrow Checker", IO_AST, IO_AST);
}

void SourceFile::runEscapeAnalyzer() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Escape-analyze all dependencies first
  for (const auto &[importName, sourceFile] : dependencies)
    sourceFile.first->runEscapeAnalyzer();

  // Then escape-analyze current file
  EscapeAnalyzer escapeanalyzer(resourceManager, this);
  escapeanalyzer.visit(ast.get());

  printStatusMessage("Escape Analyzer", IO_AST, IO_AST);
}

void SourceFile::runIRGenerator() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Generate the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->runIRGenerator();

  // Create LLVM module for this source file
  llvmModule = std::make_unique<llvm::Module>(FileUtil::getFileName(filePath), resourceManager.context);

  // Generate this source file
  IRGenerator irGenerator(resourceManager, this);
  irGenerator.visit(ast.get());

  // Save the JSON version in the compiler output
  compilerOutput.irString = irGenerator.getIRString();

  // Dump unoptimized IR code
  if (resourceManager.cliOptions.dumpIR) { // GCOV_EXCL_START
    std::cout << "\nUnoptimized IR code:\n";
    irGenerator.dumpIR();
    std::cout << "\n";
  } // GCOV_EXCL_STOP

  printStatusMessage("IR Generator", IO_AST, IO_IR);
}

void SourceFile::runIROptimizer() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Skip this stage if optimization is disabled
  if (resourceManager.cliOptions.optLevel < 1 || resourceManager.cliOptions.optLevel > 5)
    return;

  // Optimize the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->runIROptimizer();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.optimize();

  // Save the JSON version in the compiler output
  compilerOutput.irOptString = irOptimizer.getOptimizedIRString();

  // Dump optimized IR code
  if (resourceManager.cliOptions.dumpIR) { // GCOV_EXCL_START
    std::cout << "\nOptimized IR code:\n";
    irOptimizer.dumpOptimizedIR();
    std::cout << "\n";
  } // GCOV_EXCL_STOP

  printStatusMessage("IR Optimizer", IO_IR, IO_IR);
}

void SourceFile::runObjectEmitter() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache
  if (restoredFromCache)
    return;

  // Emit objects for the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->runIROptimizer();

  // Emit object for this source file
  ObjectEmitter objectEmitter(resourceManager, this);
  objectEmitter.emit();

  // Dump assembly code
  if (resourceManager.cliOptions.dumpAssembly) { // GCOV_EXCL_START
    std::cout << "\nAssembly code:\n";
    objectEmitter.dumpAsm();
  } // GCOV_EXCL_STOP

  printStatusMessage("IR Optimizer", IO_IR, IO_OBJECT_FILE);
}

void SourceFile::concludeCompilation() { // NOLINT(misc-no-recursion)
  // Conclude all dependencies
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->concludeCompilation();

  // Cache the source file
  if (!resourceManager.cliOptions.ignoreCache)
    resourceManager.cacheManager.cacheSourceFile(this);

  // Print warning if verifier is disabled
  if (parent == nullptr && resourceManager.cliOptions.disableVerifier) {
    std::cout << "\n";
    CompilerWarning(VERIFIER_DISABLED, "The LLVM verifier passes are disabled. Please use this cli option carefully.").print();
    std::cout << "\n";
  }

  if (resourceManager.cliOptions.printDebugOutput)
    std::cout << "Finished compiling " << fileName << ".\n";
}

void SourceFile::runFrontEnd() { // NOLINT(misc-no-recursion)
  runLexer();
  runParser();
  runCSTVisualizer();
  runASTBuilder();
  runASTOptimizer();
  runASTVisualizer();
  runImportCollector();
  runTypeCheckerFirst();
}

void SourceFile::runMiddleEnd() {
  runTypeCheckerSecond();
  runBorrowChecker();
  runEscapeAnalyzer();
}

void SourceFile::runBackEnd() {
  runIRGenerator();
  runIROptimizer();
  runObjectEmitter();
  concludeCompilation();
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
  for (const CompilerWarning &warning : globalScope->collectWarnings())
    compilerOutput.warnings.push_back(warning);
  // Print warnings for this file
  for (const CompilerWarning &warning : compilerOutput.warnings)
    warning.print();
}

void SourceFile::requestRuntimeModule(const RuntimeModuleName &moduleName) {
  resourceManager.runtimeModuleManager.requestModule(this, moduleName);
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

void SourceFile::printStatusMessage(const std::string &stage, const CompilerStageIOType &in,
                                    const CompilerStageIOType &out) const {
  if (resourceManager.cliOptions.printDebugOutput) {
    const char *compilerStageIoTypeName[] = {"Code", "Tokens", "CST", "AST", "IR", "OBJECT_FILE"};
    std::cout << "[" << stage << "] for " << fileName << ": ";
    std::cout << compilerStageIoTypeName[in] << " --> " << compilerStageIoTypeName[out] << "\n";
  }
}
