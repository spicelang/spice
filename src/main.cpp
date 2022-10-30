// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <cli/CliInterface.h>
#include <global/GlobalResourceManager.h>
#include <SourceFile.h>
#include <exception/CliError.h>
#include <exception/IRError.h>
#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>

/**
 * Compile main source file. All files, that are included by the main source file will be resolved recursively.
 *
 * @param cliOptions Command line options
 * @return Successful or not
 */
bool compileProject(CliOptions &cliOptions) {
  try {
    // Instantiate GlobalResourceManager
    GlobalResourceManager resourceManager(cliOptions);

    // Create source file instance for main source file
    SourceFile mainSourceFile(resourceManager, nullptr, "root", cliOptions.mainSourceFile, false);

    // Visualize the parse tree (only runs in debug mode)
    mainSourceFile.visualizeCST();

    // Transform CST to an AST
    mainSourceFile.buildAST();

    // Visualize the AST (only runs in debug mode)
    mainSourceFile.visualizeAST();

    // Pre-analyze the project (collect imports, etc.)
    mainSourceFile.preAnalyze();

    // Analyze the project (semantic analysis, build symbol table, type inference, type checking, etc.)
    mainSourceFile.analyze();

    // Re-analyze the project (resolve generic functions/procedures/structs, etc.)
    mainSourceFile.reAnalyze();

    // Generate the project (Coming up with the LLVM types of structs or other types in the root scope)
    mainSourceFile.generate();

    // Optimize the project
    mainSourceFile.optimize();

    // Emit the object files for every particular source file
    mainSourceFile.emitObjectFile();

    // Link the target executable (Link object files to executable)
    resourceManager.linker.link();

    // Print compiler warnings
    mainSourceFile.printWarnings();

    return true;
  } catch (LexerError &e) {
    std::cout << e.what() << "\n";
  } catch (ParserError &e) {
    std::cout << e.what() << "\n";
  } catch (SemanticError &e) {
    std::cout << e.what() << "\n";
  } catch (IRError &e) {
    std::cout << e.what() << "\n";
  }
  return false;
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
  CliInterface cli;
  cli.createInterface();
  try {
    cli.parse(argc, argv);
    if (cli.shouldCompile) {
      cli.validate(); // Check if all required fields are present
      cli.enrich();   // Prepare the cli options

      if (!compileProject(cli.cliOptions)) // Kick off the compiling process
        return EXIT_FAILURE;

      if (cli.shouldRun)
        cli.runBinary(); // Run executable if required
    }
  } catch (CliError &e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}