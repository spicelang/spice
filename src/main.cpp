// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <stdexcept>

#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/IRError.h>
#include <exception/LexerParserError.h>
#include <exception/SemanticError.h>
#include <linker/LinkerInterface.h>
#include <util/ThreadFactory.h>

/**
 * Compile main source file. All files, that are included by the main source file will be resolved recursively.
 *
 * @param options Command line options
 */
void compileProject(CliOptions &options) {
  try {
    // Prepare global LLVM assets
    std::shared_ptr<llvm::LLVMContext> context = std::make_shared<llvm::LLVMContext>();
    std::shared_ptr<llvm::IRBuilder<>> builder = std::make_shared<llvm::IRBuilder<>>(*context);

    // Prepare instance of thread factory and error factory, which have to exist exactly once per executable
    ThreadFactory threadFactory = ThreadFactory();
    ErrorFactory err{};

    // Prepare linker interface
    LinkerInterface linker = LinkerInterface(err, threadFactory, options);
    linker.setOutputPath(options.outputPath);

    // Create source file instance for main source file
    SourceFile mainSourceFile = SourceFile(options, nullptr, "root", options.mainSourceFile, false);

    // Pre-analyze the project (collect imports, etc.)
    mainSourceFile.preAnalyze(options);

    // Visualize the project (only runs in debug mode)
    mainSourceFile.visualizeAST(options, nullptr);

    // Analyze the project (semantic analysis, build symbol table, type inference, type checking, etc.)
    mainSourceFile.analyze(context, builder, threadFactory);

    // Re-analyze the project (resolve generic functions/procedures/structs, etc.)
    mainSourceFile.reAnalyze(context, builder, threadFactory);

    // Generate the project (LLVM code gen, optimization, emitting object files, etc.)
    mainSourceFile.generate(context, builder, threadFactory, linker);

    // Link the target executable (Link object files to executable)
    linker.link();
  } catch (LexerParserError &e) {
    std::cout << e.what() << "\n";
    std::exit(1); // Exit with result code other than 0
  } catch (SemanticError &e) {
    std::cout << e.what() << "\n";
    std::exit(1); // Exit with result code other than 0
  } catch (IRError &e) {
    std::cout << e.what() << "\n";
    std::exit(1); // Exit with result code other than 0
  }
}

/**
 * Entry point to the Spice compiler
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char **argv) {
  // Initialize command line parser
  CliInterface cli{};
  cli.createInterface();
  cli.parse(argc, argv);
  if (cli.shouldCompile()) {
    cli.validate();                   // Check if all required fields are present
    cli.enrich();                     // Prepare the cli options
    compileProject(cli.getOptions()); // Kick off the compiling process
    if (cli.shouldRun())
      cli.runBinary(); // Run executable if appropriate
  }

  return EXIT_SUCCESS;
}