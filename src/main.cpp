// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <memory>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <exception/CliError.h>
#include <exception/LexerError.h>
#include <exception/LinkerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <typechecker/MacroDefs.h>
#include <util/SystemUtil.h>

using namespace spice::compiler;

// Resource manager of the successful compilation, which is deliberately not destroyed (see compileProject)
static GlobalResourceManager *keptResourceManager = nullptr;

/**
 * Compile main source file. All files, that are included by the main source file will be resolved recursively.
 *
 * @param cliOptions Command line options
 * @return Successful or not
 */
bool compileProject(const CliOptions &cliOptions) {
  try {
    // Instantiate GlobalResourceManager
    auto resourceManagerPtr = std::make_unique<GlobalResourceManager>(cliOptions);
    GlobalResourceManager &resourceManager = *resourceManagerPtr;

    // Create source file instance for main source file
    SourceFile *mainSourceFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, cliOptions.mainSourceFile, false);

    // Run compile pipeline for main source file. All dependent source files are triggered by their parents
    mainSourceFile->runFrontEnd();
    CHECK_ABORT_FLAG_B()
    mainSourceFile->runMiddleEnd();
    CHECK_ABORT_FLAG_B()

    // Compile additional C/C++ sources in the background, while the back end is running
    if (cliOptions.outputContainer != OutputContainer::OBJECT_FILE)
      resourceManager.linker.startAdditionalSourceCompilation();

    mainSourceFile->runBackEnd();
    CHECK_ABORT_FLAG_B()

    // Link the target executable (link object files to executable/library)
    if (cliOptions.outputContainer != OutputContainer::OBJECT_FILE) {
      resourceManager.linker.prepare();
      resourceManager.cacheManager.linkOrRestoreExecutable(resourceManager);
      resourceManager.linker.cleanup();
    }

    // Print compiler warnings
    mainSourceFile->collectAndPrintWarnings();

    // Freeing all the compiler data structures takes a noticeable amount of time, which is wasted right before the process
    // exits. So keep them alive. The pointer is kept in a global variable, so that leak checkers still consider the memory
    // reachable.
    keptResourceManager = resourceManagerPtr.release();

    return true;
  } catch (LexerError &e) {
    std::cout << e.what() << "\n";
  } catch (ParserError &e) {
    std::cout << e.what() << "\n";
  } catch (SemanticError &e) {
    std::cout << e.what() << "\n";
  } catch (CompilerError &e) {
    std::cout << e.what() << "\n";
  } catch (LinkerError &e) {
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
int main(int argc, const char *argv[]) {
  // Initialize command line parser
  try {
    CliOptions cliOptions;
    Driver driver(cliOptions);
    if (const int exitCode = driver.parse(argc, argv); exitCode != EXIT_SUCCESS)
      return exitCode;

    // Cancel here if we do not have to compile
    if (!driver.shouldCompile)
      return EXIT_SUCCESS;

    driver.enrich(); // Prepare the cli options

    // Let the std TPDE bindings find the TPDE libraries, that release packages ship with the std
    SystemUtil::exportStdTPDEFlags(driver.cliOptions);

    // Kick off the compilation process
    if (!compileProject(driver.cliOptions))
      return EXIT_FAILURE;

    // Execute
    if (driver.cliOptions.execute)
      driver.runBinary();

    return EXIT_SUCCESS;
  } catch (CliError &e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  }
}