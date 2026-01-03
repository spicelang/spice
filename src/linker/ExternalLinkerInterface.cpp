// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"

#include <iostream>

#include <exception/CompilerError.h>
#include <exception/LinkerError.h>
#include <util/SystemUtil.h>
#include <util/Timer.h>

namespace spice::compiler {

void ExternalLinkerInterface::prepare() {
  // Set target to linker
  addLinkerFlag("--target=" + cliOptions.targetTriple.str());

  // Static linking
  if (cliOptions.staticLinking)
    addLinkerFlag("-static");

  // Stripping symbols
  if (!cliOptions.instrumentation.generateDebugInfo && !cliOptions.targetTriple.isOSDarwin())
    addLinkerFlag("-Wl,-s");

  // Sanitizers
  switch (cliOptions.instrumentation.sanitizer) {
  case Sanitizer::NONE:
    break;
  case Sanitizer::ADDRESS:
    addLinkerFlag("-lasan");
    break;
  case Sanitizer::THREAD:
    addLinkerFlag("-ltsan");
    break;
  case Sanitizer::MEMORY:
    addLinkerFlag("-L$(clang -print-resource-dir)/lib/x86_64-unknown-linux-gnu");
    addLinkerFlag("-lclang_rt.msan");
    requestLibMathLinkage();
    break;
  case Sanitizer::TYPE:
    addLinkerFlag("-L/usr/local/lib/clang/21/lib/x86_64-unknown-linux-gnu");
    addLinkerFlag("-lclang_rt.tysan");
    break;
  }

  // Web Assembly
  if (cliOptions.targetTriple.isWasm()) {
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
  const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
  linkerCommandBuilder << linkerInvokerPath;
  const auto [linkerName, linkerPath] = SystemUtil::findLinker(cliOptions);
  linkerCommandBuilder << " -fuse-ld=" << linkerPath;
  // Append linker flags
  for (const std::string &linkerFlag : linkerFlags)
    linkerCommandBuilder << " " << linkerFlag;
  if (linkLibMath)
    linkerCommandBuilder << " -lm";
  // Append output path
  linkerCommandBuilder << " -o " << outputPath.string();
  // Append object files
  for (const std::string &objectFilePath : objectFilePaths)
    linkerCommandBuilder << " " << objectFilePath;
  const std::string linkerCommand = linkerCommandBuilder.str();

  // Print status message
  if (cliOptions.printDebugOutput) {
    std::cout << "\nLinking with: " << linkerInvokerName << " (invoker) / " << linkerName << " (linker)"; // GCOV_EXCL_LINE
    std::cout << "\nLinker command: " << linkerCommand;                                                   // GCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n";                        // GCOV_EXCL_LINE
  }

  // Call the linker
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(linkerCommand);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                          // GCOV_EXCL_LINE
    const std::string errorMessage = "Linker exited with non-zero exit code\nLinker command: " + linkerCommand; // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                              // GCOV_EXCL_LINE
  } // GCOV_EXCL_LINE

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

/**
 * Link against libmath a.k.a. -lm
 */
void ExternalLinkerInterface::requestLibMathLinkage() {
  linkLibMath = true;
}

} // namespace spice::compiler