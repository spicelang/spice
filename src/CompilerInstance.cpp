// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <analyzer/PreAnalyzerVisitor.h>
#include <dependency/SourceFile.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <util/FileUtil.h>

/**
 * Compiles a single source file to an object
 *
 * @param context LLVM context which holds global LLVM core data
 * @param builder LLVM IR builder for generating IR with the LLVM API
 * @param moduleRegistry Module Registry, to manage project-wide module information
 * @param threadFactory Thread Factory, used for generating project-wide thread information
 * @param options Cli options that were passed to the executable
 * @param linker Linker interface
 * @param sourceFilePath Full path to a file (absolute or relative)
 * @param requiresMainFct true = main source file, false = not main source file
 * @param stdFile true = std source file, false = not std source file
 *
 * @return Symbol table of this program part
 */
SymbolTable *CompilerInstance::compileSourceFile(const std::shared_ptr<llvm::LLVMContext> &context,
                                                 const std::shared_ptr<llvm::IRBuilder<>> &builder,
                                                 ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory,
                                                 CliOptions *options, LinkerInterface *linker,
                                                 const std::string &sourceFilePath) {
  // Run pre-analyzer visitor to collect imports
  SourceFile mainSourceFile = SourceFile(nullptr, "root", sourceFilePath, false);
  mainSourceFile.preAnalyze(options);

  // Execute analyzer step
  SymbolTable *symbolTable = mainSourceFile.analyze(context, builder, moduleRegistry, threadFactory, linker, options);

  // Execute generator step
  mainSourceFile.generate(context, builder, moduleRegistry, threadFactory, linker, options);

  return symbolTable;
}