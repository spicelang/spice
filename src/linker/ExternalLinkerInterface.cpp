// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"

#include <iostream>

#include <exception/CompilerError.h>
#include <exception/LinkerError.h>
#include <util/FileUtil.h>

namespace spice::compiler {

const char *const LINKER_EXECUTABLE_NAME = "clang";

void ExternalLinkerInterface::prepare() {
  // Set target to linker
  addLinkerFlag("--target=" + cliOptions.targetTriple);

  // Static linking
  if (cliOptions.staticLinking)
    addLinkerFlag("-static");

  // Web Assembly
  if (cliOptions.targetArch == TARGET_WASM32 || cliOptions.targetArch == TARGET_WASM64) {
    addLinkerFlag("-nostdlib");
    addLinkerFlag("-Wl,--no-entry");
    addLinkerFlag("-Wl,--export-all");
  }
}

/**
 * Start the linking process
 */
void ExternalLinkerInterface::link() const {
  std::string linkerCmd(LINKER_EXECUTABLE_NAME);
  // LCOV_EXCL_START
  if (FileUtil::isCommandAvailable(linkerCmd))
    throw LinkerError(LINKER_NOT_FOUND, "Please check if you have installed " + linkerCmd + " and added it to the PATH variable");
  // LCOV_EXCL_STOP

  // Check if the output path was set
  if (outputPath.empty())                                                    // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, "Output path for the linker was not set"); // GCOV_EXCL_LINE

  // Build the linker command
  std::stringstream linkerCommandBuilder;
  linkerCommandBuilder << linkerCmd;
  // linkerCommandBuilder << " -###"; // For debugging purposes
  // Append linker flags
  for (const std::string &linkerFlag : linkerFlags)
    linkerCommandBuilder << " " << linkerFlag;
  // Append output path
  linkerCommandBuilder << " -o " << outputPath.string();
  // Append object files
  for (const std::string &objectFilePath : objectFilePaths)
    linkerCommandBuilder << " " << objectFilePath;

  // Print status message
  if (cliOptions.printDebugOutput)                                                 // GCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n"; // GCOV_EXCL_LINE

  // Call the linker
  const std::string linkerCommand = linkerCommandBuilder.str();
  ExecResult result = FileUtil::exec(linkerCommand);

  // Check for linker error
  if (result.exitCode != 0)                                                   // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, "Linker exited with non-zero exit code"); // GCOV_EXCL_LINE

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !result.output.empty())  // GCOV_EXCL_LINE
    std::cout << "Linking result: " << result.output << "\n"; // GCOV_EXCL_LINE
}

/**
 * Add another object file to be linked when calling 'link()'
 *
 * @param objectFilePath Path to the object file
 */
void ExternalLinkerInterface::addObjectFilePath(const std::string &objectFilePath) { objectFilePaths.push_back(objectFilePath); }

/**
 * Add another linker flag for the call to the linker executable
 *
 * @param linkerFlag Linker flag
 */
void ExternalLinkerInterface::addLinkerFlag(const std::string &flag) { linkerFlags.push_back(flag); }

/**
 * Add another source file to compile and link in (C or C++)
 *
 * @param additionalSource Additional source file
 */
void ExternalLinkerInterface::addAdditionalSourcePath(std::filesystem::path additionalSource) {
  // Check if the file exists
  if (!std::filesystem::exists(additionalSource)) // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR,
                        "The additional source file '" + additionalSource.string() + "' does not exist"); // GCOV_EXCL_LINE

  // Add the file to the linker
  additionalSource.make_preferred();
  addObjectFilePath(additionalSource.string());
}

} // namespace spice::compiler