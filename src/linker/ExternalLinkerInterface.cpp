// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"
#include "util/Timer.h"

#include <iostream>

#include <exception/CompilerError.h>
#include <exception/LinkerError.h>
#include <util/FileUtil.h>

namespace spice::compiler {

void ExternalLinkerInterface::prepare() {
  // Set target to linker
  addLinkerFlag("--target=" + cliOptions.targetTriple.str());

  // Static linking
  if (cliOptions.staticLinking)
    addLinkerFlag("-static");

  // Stripping symbols
  if (!cliOptions.generateDebugInfo)
    addLinkerFlag("-Wl,-s");

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
  assert(!outputPath.empty());

  // Build the linker command
  std::stringstream linkerCommandBuilder;
  const auto [linkerInvokerName, linkerInvokerPath] = FileUtil::findLinkerInvoker();
  linkerCommandBuilder << linkerInvokerPath;
  const auto [linkerName, linkerPath] = FileUtil::findLinker(cliOptions);
  linkerCommandBuilder << " -fuse-ld=" << linkerPath;
  // Append linker flags
  for (const std::string &linkerFlag : linkerFlags)
    linkerCommandBuilder << " " << linkerFlag;
  // Append output path
  linkerCommandBuilder << " -o " << outputPath.string();
  // Append object files
  for (const std::string &objectFilePath : objectFilePaths)
    linkerCommandBuilder << " " << objectFilePath;

  // Print status message
  if (cliOptions.printDebugOutput) {
    std::cout << "\nLinking with: " << linkerInvokerName << " (invoker) / " << linkerName << " (linker)"; // GCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n";                        // GCOV_EXCL_LINE
  }

  // Call the linker
  Timer timer;
  timer.start();
  const std::string linkerCommand = linkerCommandBuilder.str();
  const auto [output, exitCode] = FileUtil::exec(linkerCommand);
  timer.stop();

  // Check for linker error
  if (exitCode != 0)                                                          // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, "Linker exited with non-zero exit code"); // GCOV_EXCL_LINE

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !output.empty())    // GCOV_EXCL_LINE
    std::cout << "Linking result: " << output << "\n\n"; // GCOV_EXCL_LINE

  // Print link time
  if (cliOptions.printDebugOutput)                                                    // GCOV_EXCL_LINE
    std::cout << "Total link time: " << timer.getDurationMilliseconds() << " ms\n\n"; // GCOV_EXCL_LINE
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
 * @param flag Linker flag
 */
void ExternalLinkerInterface::addLinkerFlag(const std::string &flag) { linkerFlags.push_back(flag); }

/**
 * Add another source file to compile and link in (C or C++)
 *
 * @param additionalSource Additional source file
 */
void ExternalLinkerInterface::addAdditionalSourcePath(std::filesystem::path additionalSource) {
  // Check if the file exists
  if (!exists(additionalSource)) {                                                                           // GCOV_EXCL_LINE
    const std::string msg = "The additional source file '" + additionalSource.string() + "' does not exist"; // GCOV_EXCL_LINE
    throw CompilerError(IO_ERROR, msg);                                                                      // GCOV_EXCL_LINE
  } // GCOV_EXCL_LINE

  // Add the file to the linker
  additionalSource.make_preferred();
  addObjectFilePath(additionalSource.string());
}

} // namespace spice::compiler