// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"

#include <algorithm>
#include <iostream>
#include <vector>

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
  // Static linking
  if (cliOptions.outputContainer == OutputContainer::SHARED_LIBRARY) {
    addLinkerFlag("-shared");
  } else if (cliOptions.staticLinking) {
    addLinkerFlag("-static");
  }

  // The following flags only make sense if we want to emit an executable
  if (cliOptions.outputContainer != OutputContainer::EXECUTABLE)
    return;

  // Stripping symbols, on request only: the symbol table is what a run-time symbolizer such as the stack trace
  // runtime resolves addresses against, so a stripped executable prints nothing but addresses. Darwin is excluded
  // because its linker has no equivalent of '-Wl,-s'.
  if (cliOptions.stripSymbols && !cliOptions.targetTriple.isOSDarwin())
    addLinkerFlag("-Wl,-s");

  // Sanitizers
  switch (cliOptions.instrumentation.sanitizer) {
  case Sanitizer::NONE:
    break;
  case Sanitizer::ADDRESS:
    addLinkerFlag("-fsanitize=address");
    break;
  case Sanitizer::THREAD:
    addLinkerFlag("-fsanitize=thread");
    break;
  case Sanitizer::MEMORY:
    addLinkerFlag("-fsanitize=memory");
    requestLibMathLinkage();
    break;
  case Sanitizer::TYPE:
    addLinkerFlag("-fsanitize=type");
    break;
  }

  // Code coverage
  if (cliOptions.instrumentation.codeCoverage)
    addLinkerFlag("--coverage");

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

  // Find the linker invoker and linker
  const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
  const auto [linkerName, linkerPath] = SystemUtil::findLinker(cliOptions);
  const bool isGccInvoker = std::string_view(linkerInvokerName) == LINKER_INVOKER_NAME_GCC;
  const bool isClangInvoker = std::string_view(linkerInvokerName) == LINKER_INVOKER_NAME_CLANG;
  // GCC does not implement MemorySanitizer or TypeSanitizer
  const Sanitizer sanitizer = cliOptions.instrumentation.sanitizer;
  if (!isClangInvoker && (sanitizer == Sanitizer::MEMORY || sanitizer == Sanitizer::TYPE)) {
    const std::string msg = "Memory and type sanitizers require clang as the linker invoker, but 'gcc' was selected";
    throw LinkerError(SANITIZER_NOT_SUPPORTED_BY_LINKER_INVOKER, msg);
  }

  // Build the linker argument vector. Each entry is passed verbatim to the linker invoker (no shell), so file paths
  // can never be re-interpreted as shell syntax - this is what prevents command injection via attacker-controlled
  // object-file or output paths.
  std::vector<std::string> args;
  // GCC 16 dropped '-fuse-ld=ld'; skip when using GCC with the default BFD linker
  if (!isGccInvoker || std::string_view(linkerName) != LINKER_NAME_LD)
    args.push_back("-fuse-ld=" + linkerPath);
  // '--target=' is clang-only; GCC uses target-specific toolchain prefixes instead
  if (!isGccInvoker)
    args.push_back("--target=" + cliOptions.targetTriple.str());
  // Append output path
  args.emplace_back("-o");
  args.push_back(outputPath.string());
  // Append object files
  for (const std::filesystem::path &objectFilePath : linkedFiles)
    args.push_back(objectFilePath.string());
  // Append the std's own library search path, so the runtime modules can link against the static support libraries
  // that ship with the std (currently std/runtime/lib/libbacktrace.a, which std/runtime/impl/stack_trace_native.spice
  // pulls in with '-lbacktrace'). It goes ahead of the linker flags below because search directories are tried in the
  // order given, so the std links against its own archive rather than a same-named one in a directory that a
  // binding's '-L' flag happens to add. Passed verbatim rather than through addLinkerFlag(), so that a std path
  // containing '$' or a backtick is not mistaken for a variable reference or a command substitution.
  // Only for a native build: those archives are built for the host the std was installed on, so offering them while
  // linking for another target can only ever produce a mismatch - lld rejects every member outright
  // ("is incompatible with aarch64linux"). Cross-compiling a program that takes a stack trace needs a libbacktrace
  // built for the target, which has to come from the toolchain's own search path.
  if (cliOptions.isNativeTarget)
    if (const std::filesystem::path stdRuntimeLibDir = SystemUtil::getStdRuntimeLibDir(); !stdRuntimeLibDir.empty())
      args.push_back("-L" + stdRuntimeLibDir.string());
  // Append linker flags, expanding any environment-variable references or backtick command substitutions they may
  // contain (e.g. std bindings using "-L$LLVM_LIB_DIR" or "`pkg-config --cflags --libs libcurl`"); a single flag can
  // expand into several argv entries. They go behind the object files, because a '-l' naming a static archive is only
  // searched for symbols that are still undefined at the point it appears - ahead of them it would resolve nothing.
  for (const std::string &linkerFlag : linkerFlags)
    for (std::string &expandedFlag : SystemUtil::expandLinkerFlag(linkerFlag))
      args.push_back(std::move(expandedFlag));
  if (linkLibMath)
    args.emplace_back("-lm");

  // Print status message
  if (cliOptions.printDebugOutput) {
    const std::string command = SystemUtil::renderCommandForDisplay(linkerInvokerPath, args);
    std::cout << "\nLinking with: " << linkerInvokerName << " (invoker) / " << linkerName << " (linker)"; // GCOV_EXCL_LINE
    std::cout << "\nLinker command: " << command;                                                         // GCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n";                        // GCOV_EXCL_LINE
  }

  // Call the linker
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(linkerInvokerPath, args);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                    // GCOV_EXCL_LINE
    const std::string command = SystemUtil::renderCommandForDisplay(linkerInvokerPath, args);             // GCOV_EXCL_LINE
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

  // Find the archiver
  const auto [archiverName, archiverPath] = SystemUtil::findArchiver();

  // Build the archiver argument vector (passed verbatim, no shell involved)
  std::vector<std::string> args;
  args.emplace_back("rcs"); // r = insert files into archive; c = create archive if not existing, s = create archive index
  args.push_back(outputPath.string());
  for (const std::filesystem::path &path : linkedFiles)
    args.push_back(path.string());

  // Print status message
  if (cliOptions.printDebugOutput) {
    std::cout << "\nArchiving with: " << archiverName;                                      // GCOV_EXCL_LINE
    std::cout << "\nArchiver command: " << SystemUtil::renderCommandForDisplay(archiverPath, args);     // GCOV_EXCL_LINE
    std::cout << "\nEmitting static library to path: " << outputPath.string() << "\n";      // GCOV_EXCL_LINE
  }

  // Call the archiver
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(archiverPath, args);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                        // GCOV_EXCL_LINE
    const std::string command = SystemUtil::renderCommandForDisplay(archiverPath, args);                      // GCOV_EXCL_LINE
    const std::string errorMessage = "Archiver exited with non-zero exit code\nArchiver command: " + command; // GCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                            // GCOV_EXCL_LINE
  }

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
void ExternalLinkerInterface::addFileToLinkage(const std::filesystem::path &path) {
  if (std::ranges::find(linkedFiles, path) == linkedFiles.end())
    linkedFiles.push_back(path);
}

/**
 * Add another linker flag for the call to the linker executable
 *
 * @param flag Linker flag
 */
void ExternalLinkerInterface::addLinkerFlag(const std::string &flag) {
  if (std::ranges::find(linkerFlags, flag) == linkerFlags.end())
    linkerFlags.push_back(flag);
}

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