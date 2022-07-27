// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include <utility>

#include <parser/AstBuilderVisitor.h>
#include <analyzer/AnalyzerVisitor.h>
#include <analyzer/PreAnalyzerVisitor.h>
#include <ast/AstNodes.h>
#include <cli/CliInterface.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <generator/GeneratorVisitor.h>
#include <linker/LinkerInterface.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>
#include <visualizer/ASTVisualizerVisitor.h>
#include <visualizer/CSTVisualizerVisitor.h>

SourceFile::SourceFile(CliOptions &options, SourceFile *parent, std::string name, const std::string &filePath, bool stdFile)
    : name(std::move(name)), filePath(filePath), stdFile(stdFile), parent(parent), options(options) {
  this->objectFilePath = options.outputDir + FileUtil::DIR_SEPARATOR + FileUtil::getFileName(filePath) + ".o";

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

  // Parse input
  antlrCtx.parser = std::make_shared<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  // Create AST
  ast = std::make_shared<EntryNode>(nullptr, CodeLoc(filePath, 1, 1));

  // Create symbol table
  symbolTable = std::make_shared<SymbolTable>(nullptr, SCOPE_GLOBAL, parent == nullptr, true);
}

/**
 * Runs the pre-analyzer on the source file. This visitor collects all imported source files on the fly and visits the afterwards
 *
 * @param options applied cli options
 */
void SourceFile::preAnalyze() {
  // Pre-analyze this source file
  PreAnalyzerVisitor preAnalyzer(options, *this);
  preAnalyzer.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();

  // Analyze the imported source files
  for (auto &[_, sourceFile] : dependencies)
    sourceFile.first->preAnalyze();
}

void SourceFile::visualizeCST(std::string *output) {
  // Only execute if enabled
  if (!options.dumpCST && !options.testMode)
    return;

  std::string dotCode = parent == nullptr ? "digraph {\n rankdir=\"TB\";\n" : "subgraph {\n";
  std::string replacedFilePath = filePath;
  CommonUtil::replaceAll(replacedFilePath, "\\", "/");
  dotCode += " label=\"" + replacedFilePath + "\";\n ";

  // Visualize the imported source files
  for (auto &[_, sourceFile] : dependencies)
    sourceFile.first->visualizeCST(output);

  // Generate dot code for this source file
  CSTVisualizerVisitor visualizerVisitor(antlrCtx.lexer, antlrCtx.parser);
  dotCode += any_cast<std::string>(visualizerVisitor.visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();

  dotCode += "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.cstString = dotCode;

    if (options.dumpCST) {
      // Dump to console
      std::cout << "\nSerialized CST:\n\n" << dotCode << "\n";

      // Check if the dot command exists
      if (FileUtil::isCommandAvailable("dot")) // GCOV_EXCL_START
        throw std::runtime_error(
            "Please check if you have installed 'Graphviz Dot' and added it to the PATH variable"); // GCOV_EXCL_STOP

      // Generate SVG
      std::cout << "\nGenerating SVG file ... ";
      std::string fileBasePath = options.outputDir + FileUtil::DIR_SEPARATOR + "cst";
      FileUtil::writeToFile(fileBasePath + ".dot", dotCode);
      std::string cmdResult = FileUtil::exec("dot -Tsvg -o" + fileBasePath + ".svg " + fileBasePath + ".dot");
      std::cout << "done.\nSVG file can be found at: " << fileBasePath << ".svg\n";
    }
  }
}

void SourceFile::buildAST() {
  // Transform the imported source files
  for (auto &[_, sourceFile] : dependencies)
    sourceFile.first->buildAST();

  // Transform this source file
  AstBuilderVisitor astBuilder(ast.get());
  astBuilder.visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();
}

void SourceFile::visualizeAST(std::string *output) {
  // Only execute if enabled
  if (!options.dumpAST && !options.testMode)
    return;

  std::string dotCode = parent == nullptr ? "digraph {\n rankdir=\"TB\";\n" : "subgraph {\n";
  std::string replacedFilePath = filePath;
  CommonUtil::replaceAll(replacedFilePath, "\\", "/");
  dotCode += " label=\"" + replacedFilePath + "\";\n ";

  // Visualize the imported source files
  for (auto &[_, sourceFile] : dependencies)
    sourceFile.first->visualizeAST(output);

  // Generate dot code for this source file
  ASTVisualizerVisitor visualizerVisitor(ast.get());
  dotCode += any_cast<std::string>(visualizerVisitor.visit(ast.get()));

  dotCode += "}";

  // If this is the root source file, output the serialized string and the SVG file
  if (parent == nullptr) {
    compilerOutput.astString = dotCode;

    if (options.dumpAST) {
      // Dump to console
      std::cout << "\nSerialized AST:\n\n" << dotCode << "\n";

      // Check if the dot command exists
      if (FileUtil::isCommandAvailable("dot")) // GCOV_EXCL_START
        throw std::runtime_error(
            "Please check if you have installed 'Graphviz Dot' and added it to the PATH variable"); // GCOV_EXCL_STOP

      // Generate SVG
      std::cout << "\nGenerating SVG file ... ";
      std::string fileBasePath = options.outputDir + FileUtil::DIR_SEPARATOR + "ast";
      FileUtil::writeToFile(fileBasePath + ".dot", dotCode);
      std::string cmdResult = FileUtil::exec("dot -Tsvg -o" + fileBasePath + ".svg " + fileBasePath + ".dot");
      std::cout << "done.\nSVG file can be found at: " << fileBasePath << ".svg\n";
    }
  }
}

void SourceFile::analyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                         const ThreadFactory &threadFactory) {
  // Analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies) {
    // Analyze the imported source file
    sourceFile.first->analyze(context, builder, threadFactory);

    // Mount symbol table to the current one
    sourceFile.first->symbolTable->setParent(symbolTable.get());
    symbolTable->mountChildBlock(importName, sourceFile.first->symbolTable.get());

    // Insert import symbol
    SymbolType type = SymbolType(TY_IMPORT);
    SymbolSpecifiers specifiers(type);
    symbolTable->insert(importName, type, specifiers, INITIALIZED, sourceFile.second);
  }

  // Analyze this source file
  analyzer = std::make_shared<AnalyzerVisitor>(context, builder, threadFactory, *this, options, parent == nullptr, stdFile);
  analyzer->visit(antlrCtx.parser->entry());
  antlrCtx.parser->reset();
}

void SourceFile::reAnalyze(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                           ThreadFactory &threadFactory) {
  // Re-Analyze this source file
  bool repetitionRequired;
  unsigned int analyzeCount = 0;
  do {
    repetitionRequired = any_cast<bool>(analyzer->visit(antlrCtx.parser->entry()));
    antlrCtx.parser->reset();
    analyzeCount++;
    if (analyzeCount >= 10)
      throw std::runtime_error("Internal compiler error: Number of analyzer runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (repetitionRequired);

  // Re-analyze the imported source files
  for (auto &[importName, sourceFile] : dependencies)
    sourceFile.first->reAnalyze(context, builder, threadFactory);

  // Save the JSON version in the compiler output
  compilerOutput.symbolTableString = symbolTable->toJSON().dump(2);

  // Dump symbol table
  if (options.dumpSymbolTables) { // GCOV_EXCL_START
    std::cout << "\nSymbol table of file " << filePath << ":\n\n";
    std::cout << compilerOutput.symbolTableString << "\n";
  } // GCOV_EXCL_STOP
}

void SourceFile::generate(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                          ThreadFactory &threadFactory, LinkerInterface &linker) {
  // Generate the imported source files
  for (auto &[_, sourceFile] : dependencies)
    sourceFile.first->generate(context, builder, threadFactory, linker);

  // Generate this source file
  generator = std::make_shared<GeneratorVisitor>(context, builder, threadFactory, linker, options, *this, objectFilePath);
  bool repetitionRequired;
  unsigned int generateCount = 0;
  do {
    repetitionRequired = std::any_cast<bool>(generator->visit(antlrCtx.parser->entry()));
    antlrCtx.parser->reset();
    generateCount++;
    if (generateCount >= 10)
      throw std::runtime_error("Internal compiler error: Number of generator runs for one source file exceeded. Please report "
                               "this as a bug on GitHub.");
  } while (repetitionRequired);

  // Save the JSON version in the compiler output
  compilerOutput.irString = generator->getIRString();

  // Dump unoptimized IR code
  if (options.dumpIR) { // GCOV_EXCL_START
    std::cout << "\nUnoptimized IR code:\n";
    generator->dumpIR();
    std::cout << "\n";
  } // GCOV_EXCL_STOP

  // Optimize IR code
  if (options.optLevel >= 1 && options.optLevel <= 5) {
    generator->optimize();

    // Save the JSON version in the compiler output
    compilerOutput.irOptString = generator->getIRString();

    // Dump optimized IR code
    if (options.dumpIR) { // GCOV_EXCL_START
      std::cout << "\nOptimized IR code:\n";
      generator->dumpIR();
      std::cout << "\n";
    } // GCOV_EXCL_STOP
  }

  // Dump assembly code
  if (options.dumpAssembly) { // GCOV_EXCL_START
    std::cout << "\nAssembly code:\n";
    generator->dumpAsm();
  } // GCOV_EXCL_STOP

  // Emit object file
  generator->emit();

  antlrCtx.parser->reset();

  // Add object file to the linker interface
  linker.addObjectFilePath(objectFilePath);

  // Print warning if verifier is disabled
  if (parent == nullptr && options.disableVerifier) {
    std::cout << "\n";
    CompilerWarning(VERIFIER_DISABLED, "The LLVM verifier passes are disabled. Please use this cli option carefully.").print();
    std::cout << "\n";
  }
}

void SourceFile::addDependency(const ErrorFactory *err, const antlr4::Token &token, const std::string &name,
                               const std::string &filePath, bool stdFile) {
  // Check if this would cause a circular dependency
  if (isAlreadyImported(filePath))
    throw err->get(token, CIRCULAR_DEPENDENCY, "Circular import detected while importing '" + filePath + "'");

  // Add the dependency
  dependencies.insert({name, {std::make_shared<SourceFile>(options, this, name, filePath, stdFile), token}});
}

bool SourceFile::isAlreadyImported(const std::string &filePathSearch) const {
  // Check if the current source file corresponds to the path to search
  if (filePath == filePathSearch)
    return true;
  // Check parent recursively
  return parent != nullptr && parent->isAlreadyImported(filePathSearch);
}