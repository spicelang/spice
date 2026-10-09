// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "BootstrapUtil.h"

#include <cstdlib>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <exception/CliError.h>
#include <exception/CompilerError.h>
#include <exception/LexerError.h>
#include <exception/LinkerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>

#include "../driver/TestDriver.h"
#include "TestUtil.h"

namespace spice::testing {

using namespace spice::compiler;

extern TestDriverCliOptions testDriverCliOptions;

/**
 * Build the sources of the bootstrap compiler with the host compiler, like 'spice build src-bootstrap/main.spice' would do
 *
 * @param executablePath Path of the executable to build
 * @param buildBuiltinTests Build the builtin tests (#[test] functions) instead of the bootstrap compiler
 * @return Successful or not
 */
static bool buildBootstrapSources(const std::filesystem::path &executablePath, bool buildBuiltinTests) {
  // Check the required environment variables
  for (const char *envVar : {"SPICE_STD_DIR", "SPICE_BOOTSTRAP_DIR", "LLVM_LIB_DIR"}) {
    if (std::getenv(envVar) == nullptr) {
      std::cerr << "Building the bootstrap compiler requires the environment variable " << envVar << " to be set\n";
      return false;
    }
  }
  const std::filesystem::path mainSourceFilePath = std::filesystem::path(std::getenv("SPICE_BOOTSTRAP_DIR")) / "main.spice";

  const char *const what = buildBuiltinTests ? "builtin tests of the bootstrap compiler" : "bootstrap compiler";
  std::cout << "Building the " << what << " from " << mainSourceFilePath.string() << " ..." << std::endl;
  try {
    const std::string outputPath = executablePath.string();
    const std::string mainSourceFile = mainSourceFilePath.string();
    std::vector<const char *> argv = {"spice", "build"};
    // Coverage instrumentation does not support LTO. Without optimizations, the coverage counters map best to the source
    if (testDriverCliOptions.bootstrapCoverage)
      argv.insert(argv.end(), {"-O0", "--coverage"});
    // Without LTO and with fewer optimizations, the sanitizer reports point to the right source locations
    else if (testDriverCliOptions.bootstrapAsan)
      argv.insert(argv.end(), {"-O1", "--sanitizer", "address"});
    // The builtin tests are built without optimizations, which keeps the build fast
    else if (buildBuiltinTests)
      argv.emplace_back("-O0");
    else
      argv.insert(argv.end(), {"-O3", "-lto"});
    // The test build mode generates a main function, which runs all builtin tests of the source files
    if (buildBuiltinTests)
      argv.insert(argv.end(), {"--build-mode", "test"});
    argv.insert(argv.end(), {"--ignore-cache", "--output", outputPath.c_str(), mainSourceFile.c_str()});
    CliOptions cliOptions;
    Driver driver(cliOptions);
    if (driver.parse(static_cast<int>(argv.size()), argv.data()) != EXIT_SUCCESS)
      return false;
    driver.enrich();

    // Run the compile pipeline, mirroring compileProject() of the host compiler
    GlobalResourceManager resourceManager(cliOptions);
    SourceFile *sourceFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, cliOptions.mainSourceFile, false);
    sourceFile->runFrontEnd();
    sourceFile->runMiddleEnd();
    resourceManager.linker.startAdditionalSourceCompilation();
    sourceFile->runBackEnd();
    resourceManager.linker.prepare();
    resourceManager.cacheManager.linkOrRestoreExecutable(resourceManager);
    resourceManager.linker.cleanup();
    // The bootstrap compiler is still incomplete, so its sources emit huge amounts of unused-symbol warnings. Do not print them
  } catch (CliError &error) {
    std::cerr << error.what() << "\n";
    return false;
  } catch (LexerError &error) {
    std::cerr << error.what() << "\n";
    return false;
  } catch (ParserError &error) {
    std::cerr << error.what() << "\n";
    return false;
  } catch (SemanticError &error) {
    std::cerr << error.what() << "\n";
    return false;
  } catch (CompilerError &error) {
    std::cerr << error.what() << "\n";
    return false;
  } catch (LinkerError &error) {
    std::cerr << error.what() << "\n";
    return false;
  }

  if (!exists(executablePath)) {
    std::cerr << "Building the bootstrap sources did not produce an executable at " << executablePath.string() << "\n";
    return false;
  }
  return true;
}

/**
 * Prepare a clean output directory and get the path of the executable within it
 *
 * @param outputDir Output directory
 * @param executableName Name of the executable (without file extension)
 * @return Path of the executable
 */
static std::filesystem::path prepareBootstrapOutputDir(const char *outputDir, const std::string &executableName) {
  const std::filesystem::path outputDirPath = std::filesystem::absolute(outputDir);
  std::error_code ec;
  std::filesystem::remove_all(outputDirPath, ec);
  std::filesystem::create_directories(outputDirPath);
#if OS_WINDOWS
  std::filesystem::path executablePath = outputDirPath / (executableName + ".exe");
#else
  std::filesystem::path executablePath = outputDirPath / executableName;
#endif
  executablePath.make_preferred();
  return executablePath;
}

/**
 * Build the bootstrap compiler with the host compiler, like 'spice build src-bootstrap/main.spice' would do.
 * On success, the path of the built executable is stored in the test driver cli options.
 *
 * @return Successful or not
 */
bool BootstrapUtil::buildBootstrapCompiler() {
  const std::filesystem::path executablePath = prepareBootstrapOutputDir(PATH_BOOTSTRAP_COMPILER_ARTIFACTS, "spice");
  if (!buildBootstrapSources(executablePath, false))
    return false;
  std::cout << "Built the bootstrap compiler: " << executablePath.string() << std::endl;
  testDriverCliOptions.bootstrapCompilerPath = executablePath.string();
  return true;
}

/**
 * Build the builtin tests (#[test] functions) of the bootstrap compiler sources with the host compiler, like
 * 'spice build --build-mode test src-bootstrap/main.spice' would do. They are built with the same instrumentation as the
 * bootstrap compiler (see --bootstrap-asan and --bootstrap-coverage).
 *
 * @return Path of the built executable, if successful
 */
std::optional<std::filesystem::path> BootstrapUtil::buildBootstrapBuiltinTests() {
  const std::filesystem::path executablePath = prepareBootstrapOutputDir(PATH_BOOTSTRAP_TESTS_ARTIFACTS, "spice-tests");
  if (!buildBootstrapSources(executablePath, true))
    return std::nullopt;
  return executablePath;
}

/**
 * The bootstrap compiler has no exceptions, so it reports compile errors via a panic:
 *
 *   Program panicked at <file>:<line>:<col>: <error message>
 *   <line>  <source code line of the panic>
 *   ...
 *
 * Extract the error message and make it comparable to the one of the host compiler. Like the host compiler, the bootstrap
 * compiler already prints the file paths of code locations relative to the directory of the main source file.
 *
 * @param output Combined stdout and stderr output of the bootstrap compiler
 * @return Error message, if the bootstrap compiler reported an error
 */
std::optional<std::string> BootstrapUtil::extractErrorMessage(const std::string &output) {
  static const std::regex PANIC_HEADER_REGEX(R"(Program panicked at [^\n]*?:(\d+):\d+: )");
  std::smatch match;
  if (!std::regex_search(output, match, PANIC_HEADER_REGEX))
    return std::nullopt;
  const size_t messageStart = match.position(0) + match.length(0);

  // The message ends where the source code snippet of the panic location begins (its line number, followed by two spaces)
  const std::string snippetStart = "\n" + match[1].str() + "  ";
  size_t messageEnd = output.find(snippetStart, messageStart);
  if (messageEnd == std::string::npos)
    messageEnd = output.find('\n', messageStart);
  std::string message =
      output.substr(messageStart, messageEnd == std::string::npos ? std::string::npos : messageEnd - messageStart);

  // Normalize path separators on Windows
  CommonUtil::replaceAll(message, "\\", "/");
  return message;
}

/**
 * Extract a serialized graph (e.g. the AST from '--dump-ast' or the dependency graph from '--dump-dependency-graph') from
 * the console output of the bootstrap compiler
 *
 * @param output Output of the bootstrap compiler
 * @param graphName Name of the graph, as printed in the caption of the dump
 * @return Serialized graph, if found
 */
std::optional<std::string> BootstrapUtil::extractSerializedGraph(const std::string &output, const char *graphName) {
  const std::string caption = "Serialized " + std::string(graphName) + ":\n\n";
  size_t start = output.find(caption);
  if (start == std::string::npos)
    return std::nullopt;
  start += caption.length();
  // All lines of the dot code are indented, except the first one and the closing brace
  const size_t end = output.find("\n}", start);
  if (end == std::string::npos)
    return std::nullopt;
  return output.substr(start, end + 2 - start);
}

/**
 * Extract the warnings of the main source file from the console output of the bootstrap compiler. It prints every warning
 * in its own line, colored yellow, after the warnings of the source files the main source file depends on. Like the host
 * test runner, only the warnings of the main source file are returned, so the ones located in other files are skipped.
 *
 * @param output Output of the bootstrap compiler
 * @return Warnings of the main source file, one per line
 */
std::string BootstrapUtil::extractWarnings(const std::string &output) {
  static const std::regex WARNING_REGEX(R"(\x1B\[33m(\[Warning\] [^\n]*?)\x1B\[0m)");
  static constexpr auto WARNING_PREFIX = "[Warning] ./";
  static constexpr auto MAIN_FILE_WARNING_PREFIX = "[Warning] ./source.spice:";
  std::stringstream warnings;
  for (auto it = std::sregex_iterator(output.begin(), output.end(), WARNING_REGEX); it != std::sregex_iterator(); ++it) {
    const std::string warning = (*it)[1].str();
    // Skip warnings, located in another source file than the main source file
    if (warning.starts_with(WARNING_PREFIX) && !warning.starts_with(MAIN_FILE_WARNING_PREFIX))
      continue;
    warnings << warning << "\n";
  }
  return warnings.str();
}

/**
 * Erase the dso_local markers from the given IR code. The LLVM C API, which the bootstrap compiler uses, offers no way to
 * mark global values as dso_local, so the IR of the host and the bootstrap compiler only differs in these markers.
 *
 * @param irCode IR code
 */
void BootstrapUtil::eraseDSOLocalMarkers(std::string &irCode) { CommonUtil::replaceAll(irCode, " dso_local ", " "); }

/**
 * Replace the implementation marker in the producer string of the bootstrap compiler with the one of the host compiler.
 * Both compilers name themselves in the producer string, so the IR and assembly code of the host and the bootstrap compiler
 * differ there.
 *
 * @param code IR or assembly code
 */
void BootstrapUtil::normalizeProducerString(std::string &code) {
  const std::string hostMarker = " [" + std::string(COMPILER_IMPLEMENTATION) + "] (https://github.com/spicelang/spice)";
  CommonUtil::replaceAll(code, " [self-hosted] (https://github.com/spicelang/spice)", hostMarker);
}

/**
 * Strip ANSI color escape sequences from the given text. The bootstrap compiler colorizes its lint findings on
 * stdout, so they need to be de-colorized before being compared against the plain-text lint.out references.
 *
 * @param text Text to strip
 * @return Text without ANSI escape sequences
 */
std::string BootstrapUtil::stripAnsiCodes(const std::string &text) {
  static const std::regex ANSI_ESCAPE_REGEX(R"(\x1B\[[0-9;]*m)");
  return std::regex_replace(text, ANSI_ESCAPE_REGEX, "");
}

/**
 * Check if the given output of the bootstrap compiler contains a report of the AddressSanitizer or LeakSanitizer, which
 * the bootstrap compiler prints, if it was built with --bootstrap-asan:
 *
 *   ==<pid>==ERROR: AddressSanitizer: heap-use-after-free on address ...
 *
 * @param output Combined stdout and stderr output of the bootstrap compiler
 * @return Sanitizer report found or not
 */
bool BootstrapUtil::containsSanitizerReport(const std::string &output) {
  static const std::regex SANITIZER_REPORT_REGEX(R"(==\d+==ERROR: (AddressSanitizer|LeakSanitizer))");
  return std::regex_search(output, SANITIZER_REPORT_REGEX);
}

/**
 * Check if the given compiler arguments request debug info. Like for the compiler, the last debug info argument wins.
 *
 * @param args Compiler arguments
 * @return Debug info requested or not
 */
bool BootstrapUtil::emitsDebugInfo(const std::vector<std::string> &args) {
  bool debugInfo = false;
  for (const std::string &arg : args)
    if (arg == "-g" || arg.starts_with("--debug-info"))
      debugInfo = arg != "--debug-info=none";
  return debugInfo;
}

} // namespace spice::testing
