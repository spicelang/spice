// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include <analyzer/AnalyzerVisitor.h>
#include <analyzer/PreAnalyzerVisitor.h>
#include <ast/AstNodes.h>
#include <dependency/GlobalResourceManager.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <generator/GeneratorVisitor.h>
#include <parser/AstBuilderVisitor.h>
#include <symbol/SymbolTable.h>
#include <util/CodeLoc.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>
#include <visualizer/ASTVisualizerVisitor.h>
#include <visualizer/CSTVisualizerVisitor.h>

SourceFile::SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name, const std::string &filePath,
                       bool stdFile)
    : resourceManager(resourceManager), name(std::move(name)), filePath(filePath), stdFile(stdFile), parent(parent) {
  this->objectFilePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + FileUtil::getFileName(filePath) + ".o";

  // Deduce fileName and fileDir
  fileName = std::filesystem::path(filePath).filename().string();
  fileDir = std::filesystem::path(filePath).parent_path().string();

  // Read from file
  std::ifstream fileInputStream(filePath);
  if (!fileInputStream)
    throw std::runtime_error("Source file at path '" + filePath + "' does not exist.");

  // Create error handlers for lexer and parser
  antlrCtx.lexerErrorHandler = std::make_shared<AntlrThrowingErrorListener>(LEXER);
  antlrCtx.parserErrorHandler = std::make_shared<AntlrThrowingErrorListener>(PARSER);

  // Tokenize input
  antlrCtx.inputStream = std::make_shared<antlr4::ANTLRInputStream>(fileInputStream);
  antlrCtx.lexer = std::make_shared<SpiceLexer>(antlrCtx.inputStream.get());
  antlrCtx.lexer->removeErrorListeners();
  antlrCtx.lexer->addErrorListener(antlrCtx.lexerErrorHandler.get());
  antlrCtx.tokenStream = std::make_shared<antlr4::CommonTokenStream>(antlrCtx.lexer.get());

  // Calculate cache key
  std::stringstream cacheKeyString;
  cacheKeyString << std::hex << std::hash<std::string>{}(antlrCtx.tokenStream->getText());
  cacheKey = cacheKeyString.str();

  // Try to load from cache
  if (!resourceManager.cliOptions.ignoreCache)
    restoredFromCache = resourceManager.cacheManager.lookupSourceFile(this);

  // If we have a cache miss, execute the compiler pipeline for this source file
  if (!restoredFromCache) {
    // Parse input
    antlrCtx.parser = std::make_shared<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
    antlrCtx.parser->removeErrorListeners();
    antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
    antlrCtx.parser->removeParseListeners();

    // Create AST
    ast = std::make_shared<EntryNode>(nullptr, CodeLoc(1, 1, filePath));

    // Create symbol table
    symbolTable = std::make_shared<SymbolTable>(nullptr, SCOPE_GLOBAL, parent == nullptr, true);
  }
}

void SourceFile::visualizeCST() {
  if (restoredFromCache)
    return;

  // Only execute if enabled
  if (!resourceManager.cliOptions.dumpCST && !resourceManager.cliOptions.testMode)
    return;

  std::string dotCode = parent == nullptr ? "digraph {\n rankdir=\"TB\";\n" : "subgraph {\n";
  std::string replacedFilePath = filePath;
  CommonUtil::replaceAll(replacedFilePath, "\\", "/");
  dotCode += " label=\"" + replacedFilePath + "\";\n ";

  // Visualize the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->visualizeCST();

  // Generate dot code for this source file
  CSTVisualizerVisitor visualizerVisitor(antlrCtx.lexer, antlrCtx.parser);
  dotCode += any_cast<std::string>(visualizerVisitor.visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();

  dotCode += "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.cstString = dotCode;

    if (resourceManager.cliOptions.dumpCST) {
      // Dump to console
      std::cout << "\nSerialized CST:\n\n" << dotCode << "\n";

      // Check if the dot command exists
      if (FileUtil::isCommandAvailable("dot")) // GCOV_EXCL_START
        throw std::runtime_error(
            "Please check if you have installed 'Graphviz Dot' and added it to the PATH variable"); // GCOV_EXCL_STOP

      // Generate SVG
      std::cout << "\nGenerating SVG file ... ";
      std::string fileBasePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + "cst";
      FileUtil::writeToFile(fileBasePath + ".dot", dotCode);
      FileUtil::exec("dot -Tsvg -o" + fileBasePath + ".svg " + fileBasePath + ".dot");
      std::cout << "done.\nSVG file can be found at: " << fileBasePath << ".svg\n";
    }
  }
}

void SourceFile::buildAST() {
  if (restoredFromCache)
    return;

  // Transform the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->buildAST();

  // Transform this source file
  AstBuilderVisitor astBuilder(ast.get(), filePath, antlrCtx.inputStream.get());
  astBuilder.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();
}

void SourceFile::visualizeAST() {
  // Only execute if enabled
  if (restoredFromCache || (!resourceManager.cliOptions.dumpAST && !resourceManager.cliOptions.testMode))
    return;

  std::string dotCode = parent == nullptr ? "digraph {\n rankdir=\"TB\";\n" : "subgraph {\n";
  std::string replacedFilePath = filePath;
  CommonUtil::replaceAll(replacedFilePath, "\\", "/");
  dotCode += " label=\"" + replacedFilePath + "\";\n ";

  // Visualize the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->visualizeAST();

  // Generate dot code for this source file
  ASTVisualizerVisitor visualizerVisitor(ast.get());
  dotCode += any_cast<std::string>(visualizerVisitor.visit(ast.get()));

  dotCode += "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.astString = dotCode;

    if (resourceManager.cliOptions.dumpAST) {
      // Dump to console
      std::cout << "\nSerialized AST:\n\n" << dotCode << "\n";

      // Check if the dot command exists
      if (FileUtil::isCommandAvailable("dot")) // GCOV_EXCL_START
        throw std::runtime_error(
            "Please check if you have installed 'Graphviz Dot' and added it to the PATH variable"); // GCOV_EXCL_STOP

      // Generate SVG
      std::cout << "\nGenerating SVG file ... ";
      std::string fileBasePath = resourceManager.cliOptions.outputDir + FileUtil::DIR_SEPARATOR + "ast";
      FileUtil::writeToFile(fileBasePath + ".dot", dotCode);
      FileUtil::exec("dot -Tsvg -o" + fileBasePath + ".svg " + fileBasePath + ".dot");
      std::cout << "done.\nSVG file can be found at: " << fileBasePath << ".svg\n";
    }
  }
}

void SourceFile::preAnalyze() {
  if (restoredFromCache)
    return;

  // Pre-analyze this source file
  PreAnalyzerVisitor preAnalyzer(resourceManager.cliOptions, *this);
  preAnalyzer.visit(ast.get());
  antlrCtx.parser->reset();

  // Pre-analyze the imported source files
  for (const auto &sourceFile : dependencies) {
    sourceFile.second.first->buildAST();
    sourceFile.second.first->preAnalyze();
  }
}

void SourceFile::analyze() {
  if (restoredFromCache)
    return;

  // Analyze the imported source files
  for (const auto &[importName, sourceFile] : dependencies) {
    // Analyze the imported source file
    sourceFile.first->analyze();

    // Mount symbol table to the current one
    sourceFile.first->symbolTable->parent = symbolTable.get();
    symbolTable->mountChildBlock(importName, sourceFile.first->symbolTable.get());

    // Insert import symbol
    SymbolType type = SymbolType(TY_IMPORT);
    SymbolSpecifiers specifiers(type);
    symbolTable->insert(importName, type, specifiers, INITIALIZED, sourceFile.second);
  }

  // Analyze this source file
  analyzer = std::make_shared<AnalyzerVisitor>(resourceManager, *this, parent == nullptr);
  analyzer->visit(ast.get());
  antlrCtx.parser->reset();
}

void SourceFile::reAnalyze() {
  if (restoredFromCache)
    return;

  // Re-Analyze this source file
  bool repetitionRequired;
  unsigned int reAnalyzeCount = 0;
  do {
    repetitionRequired = any_cast<bool>(analyzer->visit(ast.get()));
    antlrCtx.parser->reset();
    reAnalyzeCount++;
    if (reAnalyzeCount >= 10)
      throw std::runtime_error("Internal compiler error: Number of analyzer runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (repetitionRequired);

  // Re-analyze the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->reAnalyze();

  // Save the JSON version in the compiler output
  compilerOutput.symbolTableString = symbolTable->toJSON().dump(2);

  // Dump symbol table
  if (resourceManager.cliOptions.dumpSymbolTables) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << compilerOutput.symbolTableString << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::generate() {
  if (restoredFromCache)
    return;

  // Generate the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->generate();

  // Generate this source file
  generator = std::make_shared<GeneratorVisitor>(resourceManager, *this, objectFilePath);
  bool repetitionRequired;
  unsigned int generateCount = 0;
  do {
    repetitionRequired = any_cast<bool>(generator->visit(ast.get()));
    antlrCtx.parser->reset();
    generateCount++;
    if (generateCount >= 10)
      throw std::runtime_error("Internal compiler error: Number of generator runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (repetitionRequired);

  antlrCtx.parser->reset();

  // Save the JSON version in the compiler output
  compilerOutput.irString = generator->getIRString();

  // Dump unoptimized IR code
  if (resourceManager.cliOptions.dumpIR) { // GCOV_EXCL_START
    std::cout << "\nUnoptimized IR code:\n";
    generator->dumpIR();
    std::cout << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::optimize() {
  if (restoredFromCache)
    return;

  // Skip this stage if optimization is disabled
  if (resourceManager.cliOptions.optLevel < 1 || resourceManager.cliOptions.optLevel > 5)
    return;

  // Optimize the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->optimize();

  generator->optimize();

  // Save the JSON version in the compiler output
  compilerOutput.irOptString = generator->getIRString();

  // Dump optimized IR code
  if (resourceManager.cliOptions.dumpIR) { // GCOV_EXCL_START
    std::cout << "\nOptimized IR code:\n";
    generator->dumpIR();
    std::cout << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::emitObjectFile() {
  if (restoredFromCache)
    return;

  // Optimize the imported source files
  for (const auto &sourceFile : dependencies)
    sourceFile.second.first->emitObjectFile();

  // Dump assembly code
  if (resourceManager.cliOptions.dumpAssembly) { // GCOV_EXCL_START
    std::cout << "\nAssembly code:\n";
    generator->dumpAsm();
  } // GCOV_EXCL_STOP

  // Emit object file and add object file to the linker interface
  generator->emit();
  resourceManager.linker.addObjectFilePath(objectFilePath);

  // Print warning if verifier is disabled
  if (parent == nullptr && resourceManager.cliOptions.disableVerifier) {
    std::cout << "\n";
    CompilerWarning(VERIFIER_DISABLED, "The LLVM verifier passes are disabled. Please use this cli option carefully.").print();
    std::cout << "\n";
  }

  // Cache the source file
  if (!resourceManager.cliOptions.ignoreCache)
    resourceManager.cacheManager.cacheSourceFile(this);
}

std::shared_ptr<SourceFile> SourceFile::createSourceFile(const std::string &name, const std::string &filePath, bool stdFile) {
  return std::make_shared<SourceFile>(resourceManager, this, name, filePath, stdFile);
}

void SourceFile::addDependency(const std::shared_ptr<SourceFile> &sourceFile, const AstNode *declAstNode, const std::string &name,
                               const std::string &filePath) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(filePath))
    throw SemanticError(declAstNode, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + filePath + "'");

  // Add the dependency
  dependencies.insert({name, {sourceFile, declAstNode}});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const {
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check parent recursively
  return parent != nullptr && parent->isAlreadyImported(filePathSearch);
}

void SourceFile::printWarnings() const {
  // Print warnings for all dependencies
  for (const auto &dependency : dependencies) {
    if (!dependency.second.first->stdFile)
      dependency.second.first->printWarnings();
  }
  // Print warnings for this file
  for (const CompilerWarning &warning : compilerOutput.warnings)
    warning.print();
}

void SourceFile::requestRuntimeModule(const RuntimeModuleName &moduleName) {
  resourceManager.runtimeModuleManager.requestModule(this, moduleName);
}