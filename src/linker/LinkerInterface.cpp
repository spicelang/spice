// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "LinkerInterface.h"

#include <util/FileUtil.h>

#ifdef __unix__
#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif

const char *LINKER_EXECUTABLE_NAME = "gcc";

/**
 * Start the linking process
 */
void LinkerInterface::link() {
  // Check if the linker executable exists
  std::string cmd = "which " + std::string(LINKER_EXECUTABLE_NAME) + " > /dev/null 2>&1";
#ifdef OS_WINDOWS
  cmd = "where " + std::string(LINKER_EXECUTABLE_NAME) + " > nul 2>&1";
#endif
  if (std::system(cmd.c_str())) // GCOV_EXCL_START
    throw err->get(LINKER_NOT_FOUND, "Please check if you have installed " + std::string(LINKER_EXECUTABLE_NAME) +
                                         " and added it to the PATH variable"); // GCOV_EXCL_STOP

  // Check if the output path was set
  if (outputPath.empty())
    throw std::runtime_error("Internal compiler error: Output path for the linker was not set"); // GCOV_EXCL_LINE

  // Build the linker command
  std::string linkerCommand = LINKER_EXECUTABLE_NAME;
  if (threadFactory->isUsingThreads())
    linkerCommand += " -pthread";
  for (auto &linkerFlag : linkerFlags)
    linkerCommand += " " + linkerFlag;
  linkerCommand += " -o " + outputPath;
  for (auto &objectFilePath : objectFilePaths)
    linkerCommand += " " + objectFilePath;

  // Print status message
  if (cliOptions->printDebugOutput)
    std::cout << std::endl << "Emitting executable to path: " << outputPath << std::endl; // GCOV_EXCL_LINE

  // Call the linker
  std::string result = FileUtil::exec(linkerCommand);

  // Print linker result if appropriate
  if (cliOptions->printDebugOutput && !result.empty())
    std::cout << "Linking result: " << result << std::endl; // GCOV_EXCL_LINE
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

/**
 * Set the output file path for the output executable
 *
 * @param path Output file path
 */
void LinkerInterface::setOutputPath(const std::string &path) { this->outputPath = path; }