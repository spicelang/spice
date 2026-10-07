// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ExternalLinkerInterface.h"

#include <algorithm>
#include <future>
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

  // Windows linkers stamp the link time into the PE header. Leave it out, so that the same input always links to the same
  // binary (reproducible builds, bootstrap fixed point)
  if (cliOptions.targetTriple.isOSWindows())
    addLinkerFlag("-Wl,--no-insert-timestamp");

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
  // Wait for additional source compilations, that are still running (e.g. because the executable was restored from the
  // cache and they were not needed)
  for (const AdditionalSourceCompilation &compilation : additionalSourceCompilations | std::views::values)
    compilation.result.wait();

  // Cleanup intermediary object files
  const char *objFileExt = SystemUtil::getOutputFileExtension(cliOptions, cliOptions.outputContainer);
  if (cliOptions.outputContainer != OutputContainer::OBJECT_FILE && !cliOptions.dump.dumpToFiles) {
    for (const std::filesystem::path &path : linkedFiles)
      if (path.extension() == objFileExt)
        std::filesystem::remove(path);
    for (const AdditionalSourceCompilation &compilation : additionalSourceCompilations | std::views::values)
      std::filesystem::remove(compilation.objectFilePath);
  }
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
  std::vector<std::string> args = getInvokerArgs(linkerName, linkerPath, isGccInvoker);
  // Append output path
  args.emplace_back("-o");
  args.push_back(outputPath.string());
  // Append object files. Additional sources are compiled to object files first, which mostly happened in the background
  for (const std::filesystem::path &objectFilePath : linkedFiles) {
    const bool isAdditionalSource = std::ranges::find(additionalSourcePaths, objectFilePath) != additionalSourcePaths.end();
    args.push_back(isAdditionalSource ? compileAdditionalSource(objectFilePath).string() : objectFilePath.string());
  }
  // Append the std's runtime library search path and the linker flags
  std::ranges::move(getTrailingArgs(), std::back_inserter(args));
  if (linkLibMath)
    args.emplace_back("-lm");
  // Print status message
  if (cliOptions.printDebugOutput) {
    const std::string command = SystemUtil::renderCommandForDisplay(linkerInvokerPath, args);
    std::cout << "\nLinking with: " << linkerInvokerName << " (invoker) / " << linkerName << " (linker)"; // LCOV_EXCL_LINE
    std::cout << "\nLinker command: " << command;                                                         // LCOV_EXCL_LINE
    std::cout << "\nEmitting executable to path: " << outputPath.string() << "\n";                        // LCOV_EXCL_LINE
  }

  // Call the linker
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(linkerInvokerPath, args);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                    // LCOV_EXCL_LINE
    const std::string command = SystemUtil::renderCommandForDisplay(linkerInvokerPath, args);             // LCOV_EXCL_LINE
    const std::string errorMessage = "Linker exited with non-zero exit code\nLinker command: " + command; // LCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                        // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !output.empty())    // LCOV_EXCL_LINE
    std::cout << "Linking result: " << output << "\n\n"; // LCOV_EXCL_LINE

  // Print link time
  if (cliOptions.printDebugOutput)                                                    // LCOV_EXCL_LINE
    std::cout << "Total link time: " << timer.getDurationMilliseconds() << " ms\n\n"; // LCOV_EXCL_LINE
}

/**
 * Get the leading arguments for the linker invoker, that select the linker and the target
 *
 * @param linkerName Name of the linker
 * @param linkerPath Path to the linker
 * @param isGccInvoker Whether the linker invoker is GCC
 * @return Leading arguments
 */
std::vector<std::string> ExternalLinkerInterface::getInvokerArgs(const char *linkerName, const std::string &linkerPath,
                                                                 bool isGccInvoker) const {
  std::vector<std::string> args;
  // GCC 16 dropped '-fuse-ld=ld'; skip when using GCC with the default BFD linker
  if (!isGccInvoker || std::string_view(linkerName) != LINKER_NAME_LD)
    args.push_back("-fuse-ld=" + linkerPath);
  // '--target=' is clang-only; GCC uses target-specific toolchain prefixes instead
  if (!isGccInvoker)
    args.push_back("--target=" + cliOptions.targetTriple.str());
  return args;
}

/**
 * Get the trailing arguments for the linker invoker: the library search path of the std and the expanded linker flags
 *
 * @return Trailing arguments
 */
std::vector<std::string> ExternalLinkerInterface::getTrailingArgs() const {
  std::vector<std::string> args;
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
  for (const std::string &linkerFlag : linkerFlags) {
    auto it = expandedLinkerFlags.find(linkerFlag);
    if (it == expandedLinkerFlags.end())
      it = expandedLinkerFlags.emplace(linkerFlag, SystemUtil::expandLinkerFlag(linkerFlag)).first;
    std::ranges::copy(it->second, std::back_inserter(args));
  }

  return args;
}

/**
 * Get the arguments for the linker invoker to compile an additional source to an object file. These are the same arguments,
 * that the linker invoker would use to compile the additional source as part of the link command.
 *
 * @param additionalSource Additional source file
 * @param objectFilePath Path of the object file to emit
 * @return Compile arguments
 */
std::vector<std::string>
ExternalLinkerInterface::getAdditionalSourceCompileArgs(const std::filesystem::path &additionalSource,
                                                        const std::filesystem::path &objectFilePath) const {
  const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
  const auto [linkerName, linkerPath] = SystemUtil::findLinker(cliOptions);
  const bool isGccInvoker = std::string_view(linkerInvokerName) == LINKER_INVOKER_NAME_GCC;
  std::vector<std::string> args = getInvokerArgs(linkerName, linkerPath, isGccInvoker);
  args.emplace_back("-c");
  args.emplace_back("-o");
  args.push_back(objectFilePath.string());
  args.push_back(additionalSource.string());
  std::ranges::move(getTrailingArgs(), std::back_inserter(args));
  return args;
}

/**
 * Get the path of the object file, an additional source is compiled to
 *
 * @param additionalSource Additional source file
 * @return Object file path
 */
std::filesystem::path
ExternalLinkerInterface::getAdditionalSourceObjectFilePath(const std::filesystem::path &additionalSource) const {
  // Additional sources with the same file name may come from different directories, so add the index of the source
  const auto index = std::ranges::find(additionalSourcePaths, additionalSource) - additionalSourcePaths.begin();
  const char *objFileExt = SystemUtil::getOutputFileExtension(cliOptions, OutputContainer::OBJECT_FILE);
  const std::string fileName = additionalSource.filename().string() + "." + std::to_string(index) + "." + objFileExt;
  return cliOptions.outputDir / fileName;
}

/**
 * Start compiling the additional sources to object files in the background, so that this overlaps with the back end. The
 * linker waits for the results and only compiles an additional source again, if its compile arguments changed in the
 * meantime (e.g. because source files, that were restored from the cache, added further linker flags).
 */
void ExternalLinkerInterface::startAdditionalSourceCompilation() {
  if (cliOptions.outputContainer != OutputContainer::EXECUTABLE && cliOptions.outputContainer != OutputContainer::SHARED_LIBRARY)
    return;

  const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
  for (const std::filesystem::path &additionalSource : additionalSourcePaths) {
    if (additionalSourceCompilations.contains(additionalSource.string()))
      continue;
    AdditionalSourceCompilation compilation;
    compilation.objectFilePath = getAdditionalSourceObjectFilePath(additionalSource);
    compilation.args = getAdditionalSourceCompileArgs(additionalSource, compilation.objectFilePath);
    compilation.result = std::async(std::launch::async, [linkerInvokerPath, args = compilation.args] {
                           return SystemUtil::exec(linkerInvokerPath, args, true);
                         }).share();
    additionalSourceCompilations.emplace(additionalSource.string(), std::move(compilation));
  }
}

/**
 * Compile an additional source to an object file or take the result of the compilation, that was started in the background
 *
 * @param additionalSource Additional source file
 * @return Object file path
 */
std::filesystem::path ExternalLinkerInterface::compileAdditionalSource(const std::filesystem::path &additionalSource) const {
  const std::filesystem::path objectFilePath = getAdditionalSourceObjectFilePath(additionalSource);
  std::vector<std::string> args = getAdditionalSourceCompileArgs(additionalSource, objectFilePath);

  // Compile it now, if it was not compiled in the background or with different arguments
  auto it = additionalSourceCompilations.find(additionalSource.string());
  if (it == additionalSourceCompilations.end() || it->second.args != args) {
    if (it != additionalSourceCompilations.end())
      it->second.result.wait(); // Do not let two compilations write the same object file at the same time
    const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
    AdditionalSourceCompilation compilation;
    compilation.objectFilePath = objectFilePath;
    compilation.args = std::move(args);
    std::promise<ExecResult> promise;
    promise.set_value(SystemUtil::exec(linkerInvokerPath, compilation.args, true));
    compilation.result = promise.get_future().share();
    it = additionalSourceCompilations.insert_or_assign(additionalSource.string(), std::move(compilation)).first;
  }

  // Check for compile error
  const ExecResult &result = it->second.result.get();
  if (result.exitCode != 0) {
    const auto [linkerInvokerName, linkerInvokerPath] = SystemUtil::findLinkerInvoker();
    const std::string command = SystemUtil::renderCommandForDisplay(linkerInvokerPath, it->second.args);
    const std::string errorMessage = "Compiling the additional source '" + additionalSource.string() +
                                     "' failed\nCompile command: " + command + "\n" + result.output;
    throw LinkerError(LINKER_ERROR, errorMessage);
  }
  return objectFilePath;
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
    std::cout << "\nArchiving with: " << archiverName;                                              // LCOV_EXCL_LINE
    std::cout << "\nArchiver command: " << SystemUtil::renderCommandForDisplay(archiverPath, args); // LCOV_EXCL_LINE
    std::cout << "\nEmitting static library to path: " << outputPath.string() << "\n";              // LCOV_EXCL_LINE
  }

  // Call the archiver
  Timer timer;
  timer.start();
  const auto [output, exitCode] = SystemUtil::exec(archiverPath, args);
  timer.stop();

  // Check for linker error
  if (exitCode != 0) {                                                                                        // LCOV_EXCL_LINE
    const std::string command = SystemUtil::renderCommandForDisplay(archiverPath, args);                      // LCOV_EXCL_LINE
    const std::string errorMessage = "Archiver exited with non-zero exit code\nArchiver command: " + command; // LCOV_EXCL_LINE
    throw LinkerError(LINKER_ERROR, errorMessage);                                                            // LCOV_EXCL_LINE
  }

  // Print linker result if appropriate
  if (cliOptions.printDebugOutput && !output.empty())      // LCOV_EXCL_LINE
    std::cout << "Archiving result: " << output << "\n\n"; // LCOV_EXCL_LINE

  // Print link time
  if (cliOptions.printDebugOutput)                                                       // LCOV_EXCL_LINE
    std::cout << "Total archive time: " << timer.getDurationMilliseconds() << " ms\n\n"; // LCOV_EXCL_LINE
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
  // Group boundary markers must not be deduplicated: each archive group gets its own pair,
  // and deduplication would collapse multiple groups into one misplaced pair.
  if (flag == "-Wl,--start-group" || flag == "-Wl,--end-group") {
    linkerFlags.push_back(flag);
    return;
  }
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
  if (std::ranges::find(additionalSourcePaths, additionalSource) == additionalSourcePaths.end())
    additionalSourcePaths.push_back(additionalSource);
}

/**
 * Link against libmath a.k.a. -lm
 */
void ExternalLinkerInterface::requestLibMathLinkage() { linkLibMath = true; }

} // namespace spice::compiler