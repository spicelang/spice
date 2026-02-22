// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"

#include <iostream>

#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <exception/LinkerError.h>
#include <util/GlobalDefinitions.h>
#include <util/SystemUtil.h>
#include <util/Timer.h>

namespace spice::compiler {

ExternalLinkerInterface::ExternalLinkerInterface(const CliOptions &cliOptions)
    : outputPath(cliOptions.outputPath), cliOptions(cliOptions) {}

void ExternalLinkerInterface::prepare() {
  // Set target to linker
  addLinkerFlag("--target=" + cliOptions.targetTriple.str());

  // Static linking
  if (cliOptions.outputContainer == OutputContainer::SHARED_LIBRARY) {
    addLinkerFlag("-shared");
  } else if (cliOptions.staticLinking) {
    addLinkerFlag("-static");
  }

  // The following flags only make sense if we want to emit an executable
  if (cliOptions.outputContainer != OutputContainer::EXECUTABLE)
    return;

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
    addLinkerFlag("-L$(clang -print-resource-dir)/lib/x86_64-unknown-linux-gnu");
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

void ExternalLinkerInterface::run() const {
  switch (cliOptions.outputContainer) {
  case OutputContainer::EXECUTABLE:
  case OutputContainer::SHARED_LIBRARY:
    link();
    break;
  case OutputContainer::STATIC_LIBRARY:
    archive();
    break;
  case OutputContainer::OBJECT_FILE:
    // No linking necessary
    break;
  default:
    assert_fail("Unknown output container");
  }
}

/**
 * Cleanup intermediary object files
 */
void ExternalLinkerInterface::cleanup() const {
  // Cleanup intermediary object files
  const char *objFileExt = SystemUtil::getOutputFileExtension(cliOptions, cliOptions.outputContainer);
  if (cliOptions.outputContainer != OutputContainer::OBJECT_FILE && !cliOptions.dump.dumpToFiles)
    for (const std::filesystem::path &path : linkedFiles)
      if (path.extension() == objFileExt)
        std::filesystem::remove(path);
}

/**
 * Link the object files to an executable
 */
void ExternalLinkerInterface::link() const {
  assert(!outputPath.empty());

  // Build the linker command
  std::stringstream commandBuilder;
  const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
  commandBuilder << linkerInvokerPath;
  const auto [linkerName, linkerPath] = SystemUtil::findLinker(cliOptions);
  commandBuilder << " -fuse-ld=" << linkerPath;
  // Append linker flags
  for (const std::string &linkerFlag : linkerFlags)
    commandBuilder << " " << linkerFlag;
  if (linkLibMath)
    commandBuilder << " -lm";
  // Append output path
  commandBuilder << " -o " << outputPath.string();
  // Append object files
  for (const std::filesystem::path &objectFilePath : linkedFiles)
    commandBuilder << " " << objectFilePath.string();
  const std::string command = commandBuilder.str();

  // Print status message
  if (cliOptions.printDebugOutput) {
    std::cout << "\nLinking with: " << linkerInvokerName << " (invoker) / " << linkerName << " (linker)"; // GCOV_EXCL_LINE
    std::cout << "\nLinker command: " << command;                                                         // GCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n";                        // GCOV_EXCL_LINE
  }

  // Call the linker
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(command);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                    // GCOV_EXCL_LINE
    const std::string errorMessage = "Linker exited with non-zero exit code\nLinker command: " + command; // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                        // GCOV_EXCL_LINE
  } // GCOV_EXCL_LINE

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !output.empty())    // GCOV_EXCL_LINE
    std::cout << "Linking result: " << output << "\n\n"; // GCOV_EXCL_LINE

  // Print link time
  if (cliOptions.printDebugOutput)                                                    // GCOV_EXCL_LINE
    std::cout << "Total link time: " << timer.getDurationMilliseconds() << " ms\n\n"; // GCOV_EXCL_LINE
}

/**
 * Archive the object files to a static library
 */
void ExternalLinkerInterface::archive() const {
  assert(!outputPath.empty());

  // Build the archiver command
  std::stringstream commandBuilder;
  const auto [archiverName, archiverPath] = SystemUtil::findArchiver();
  commandBuilder << archiverPath;
  commandBuilder << " rcs "; // r = insert files into archive; c = create archive if not existing, s = create archive index
  commandBuilder << outputPath.string();
  for (const std::filesystem::path &path : linkedFiles)
    commandBuilder << " " << path.string();
  const std::string command = commandBuilder.str();

  // Print status message
  if (cliOptions.printDebugOutput) {
    std::cout << "\nArchiving with: " << archiverName;                                 // GCOV_EXCL_LINE
    std::cout << "\nArchiver command: " << command;                                    // GCOV_EXCL_LINE
    std::cout << "\nEmitting static library to path: " << outputPath.string() << "\n"; // GCOV_EXCL_LINE
  }

  // Call the archiver
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(command);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                        // GCOV_EXCL_LINE
    const std::string errorMessage = "Archiver exited with non-zero exit code\nArchiver command: " + command; // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                            // GCOV_EXCL_LINE
  } // GCOV_EXCL_LINE

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !output.empty())      // GCOV_EXCL_LINE
    std::cout << "Archiving result: " << output << "\n\n"; // GCOV_EXCL_LINE

  // Print link time
  if (cliOptions.printDebugOutput)                                                       // GCOV_EXCL_LINE
    std::cout << "Total archive time: " << timer.getDurationMilliseconds() << " ms\n\n"; // GCOV_EXCL_LINE
}

/**
 * Add another object file to be linked when calling 'link()'
 *
 * @param path Path to the object file
 */
void ExternalLinkerInterface::addFileToLinkage(const std::filesystem::path &path) { linkedFiles.push_back(path); }

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
  if (!exists(additionalSource)) {
    const std::string msg = "The additional source file '" + additionalSource.string() + "' does not exist";
    throw CompilerError(IO_ERROR, msg);
  }

  // Add the file to the linker
  additionalSource = canonical(additionalSource);
  additionalSource.make_preferred();
  addFileToLinkage(additionalSource);
}

/**
 * Link against libmath a.k.a. -lm
 */
void ExternalLinkerInterface::requestLibMathLinkage() { linkLibMath = true; }

} // namespace spice::compiler