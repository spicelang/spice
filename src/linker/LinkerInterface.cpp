// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "LinkerInterface.h"

#include <iostream>
#include <stdexcept>

#include <exception/LinkerError.h>
#include <global/ThreadFactory.h>
#include <util/FileUtil.h>

namespace spice::compiler {

const char *const LINKER_EXECUTABLE_NAME = "gcc";

/**
 * Start the linking process
 */
void LinkerInterface::link() const {
  if (FileUtil::isCommandAvailable(std::string(LINKER_EXECUTABLE_NAME))) // GCOV_EXCL_START
    throw LinkerError(LINKER_NOT_FOUND, "Please check if you have installed " + std::string(LINKER_EXECUTABLE_NAME) +
                                            " and added it to the PATH variable"); // GCOV_EXCL_STOP

  // Check if the output path was set
  if (outputPath.empty())
    throw std::runtime_error("Internal compiler error: Output path for the linker was not set"); // GCOV_EXCL_LINE

  // Build the linker command
  std::stringstream linkerCommandBuilder;
  linkerCommandBuilder << LINKER_EXECUTABLE_NAME;
  if (threadFactory.isUsingThreads())
    linkerCommandBuilder << " -pthread";
  for (const auto &linkerFlag : linkerFlags)
    linkerCommandBuilder << " " + linkerFlag;
  linkerCommandBuilder << " -o " + outputPath;
  for (const auto &objectFilePath : objectFilePaths)
    linkerCommandBuilder << " " + objectFilePath;

  // Print status message
  if (cliOptions.printDebugOutput)
    std::cout << "\nEmitting executable to path: " << outputPath << "\n"; // GCOV_EXCL_LINE

  // Call the linker
  const std::string linkerCommand = linkerCommandBuilder.str();
  ExecResult result = FileUtil::exec(linkerCommand);

  // Check for linker error
  if (result.exitCode != 0)
    throw LinkerError(LINKER_ERROR, "Linker exited with non-zero exit code");

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !result.output.empty())
    std::cout << "Linking result: " << result.output << "\n"; // GCOV_EXCL_LINE
}

/**
 * Add another object file to be linked when calling 'link()'
 *
 * @param objectFilePath Path to the object file
 */
void LinkerInterface::addObjectFilePath(const std::string &objectFilePath) { objectFilePaths.push_back(objectFilePath); }

/**
 * Add another linker flag for the call to the linker executable
 *
 * @param linkerFlag Linker flag
 */
void LinkerInterface::addLinkerFlag(const std::string &flag) { linkerFlags.push_back(flag); }

} // namespace spice::compiler