// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include <SourceFile.h>
#include <cli/CLIInterface.h>
#include <exception/CliError.h>
#include <exception/IRError.h>
#include <exception/LexerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <util/FileUtil.h>

using namespace spice::compiler;

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
    SourceFile *mainSourceFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, cliOptions.mainSourceFile, false);

    // Run compile pipeline for main source file. All dependent source files are triggered by their parents
    mainSourceFile->runFrontEnd();
    mainSourceFile->runMiddleEnd();
    mainSourceFile->runBackEnd();

    // Link the target executable (link object files to executable)
    resourceManager.linker.prepare();
    resourceManager.linker.link();

    // Print compiler warnings
    mainSourceFile->collectAndPrintWarnings();

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
  CLIInterface cli;
  cli.createInterface();
  try {
    cli.parse(argc, argv);

    // Cancel here if we do not have to compile
    if (!cli.shouldCompile)
      return EXIT_SUCCESS;

    cli.enrich(); // Prepare the cli options

    // Kick off the compilation process
    if (!compileProject(cli.cliOptions))
      return EXIT_FAILURE;

    // Execute
    if (cli.cliOptions.execute)
      cli.runBinary();

    return EXIT_SUCCESS;
  } catch (CliError &e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  }
}