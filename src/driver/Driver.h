// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <llvm/TargetParser/Triple.h>

#include "../../lib/cli11/CLI11.hpp"

// Undef conflicting macros (only problematic on Windows)
#undef TRUE
#undef FALSE
#undef CONST

namespace spice::compiler {

const char *const TARGET_UNKNOWN = "unknown";
const char *const ENV_VAR_DOCKERIZED = "SPICE_DOCKERIZED";

enum class OptLevel : uint8_t {
  O0 = 0, // No optimization
  O1 = 1, // Only basic optimizations
  O2 = 2, // Default optimization level
  O3 = 3, // Aggressively optimize for performance
  Os = 4, // Optimize for code size
  Oz = 5, // Aggressively optimize for code size
};

enum class Sanitizer : uint8_t {
  NONE = 0,    // No sanitizer
  ADDRESS = 1, // Sanitize memory accesses
  THREAD = 2,  // Sanitize threads for data race prevention
  MEMORY = 3,  // Sanitize against uninitialized memory reads
  TYPE = 4,    // Sanitize type casts and accesses
};
const char *const SANITIZER_NONE = "none";
const char *const SANITIZER_ADDRESS = "address";
const char *const SANITIZER_THREAD = "thread";
const char *const SANITIZER_MEMORY = "memory";
const char *const SANITIZER_TYPE = "type";

enum class BuildMode : uint8_t {
  DEBUG = 0,   // Default build mode, uses -O0 per default
  RELEASE = 1, // Build without debug information and with -O2 per default
  TEST = 2,    // Build with test main function and always emit assertions
};
const char *const BUILD_MODE_DEBUG = "debug";
const char *const BUILD_MODE_RELEASE = "release";
const char *const BUILD_MODE_TEST = "test";

/**
 * Representation of the various cli options
 */
struct CliOptions {
  std::filesystem::path mainSourceFile; // e.g. ./main.spice
  llvm::Triple targetTriple;            // In format: <arch><sub>-<vendor>-<sys>-<abi>
  std::string targetArch = TARGET_UNKNOWN;
  std::string targetVendor = TARGET_UNKNOWN;
  std::string targetOs = TARGET_UNKNOWN;
  bool isNativeTarget = true;
  bool useCPUFeatures = true;
  bool execute = false;
  std::filesystem::path cacheDir;         // Where the cache files go. Should always be a temp directory
  std::filesystem::path outputDir = "./"; // Where the object files go. Should always be a temp directory
  std::filesystem::path outputPath;       // Where the output binary goes.
  BuildMode buildMode = BuildMode::DEBUG; // Default build mode is debug
  unsigned short compileJobCount = 0;     // 0 for auto
  bool ignoreCache = false;
  std::string llvmArgs;
  bool printDebugOutput = false;
  struct DumpSettings {
    bool dumpCST = false;
    bool dumpAST = false;
    bool dumpSymbolTable = false;
    bool dumpTypes = false;
    bool dumpCacheStats = false;
    bool dumpDependencyGraph = false;
    bool dumpIR = false;
    bool dumpAssembly = false;
    bool dumpObjectFiles = false;
    bool dumpToFiles = false;
    bool abortAfterDump = false;
  } dump;
  bool namesForIRValues = false;
  bool useLifetimeMarkers = false;
  bool useTBAAMetadata = false;
  OptLevel optLevel = OptLevel::O0; // The default optimization level for debug build mode is O0
  bool useLTO = false;
  bool noEntryFct = false;
  bool generateTestMain = false;
  bool staticLinking = false;
  struct InstrumentationSettings {
    bool generateDebugInfo = false;
    Sanitizer sanitizer = Sanitizer::NONE;
  } instrumentation;
  bool disableVerifier = false;
  bool testMode = false;
  bool comparableOutput = false;
};

/**
 * Helper class to set up the driver and command line parser
 */
class Driver {
public:
  // Constructors
  explicit Driver(CliOptions &foreignCliOptions, bool dryRun = false);

  // Delete copy ctor and assignment operator
  Driver(const Driver &) = delete;
  Driver operator=(const Driver &) = delete;

  // Public methods
  int parse(int argc, const char *argv[]);
  void enrich() const;
  void runBinary() const;

  // Public members
  CliOptions &cliOptions;
  bool shouldCompile = false;
  bool shouldInstall = false;
  bool shouldUninstall = false;
  bool shouldExecute = false;
  bool performDryRun = false; // For unit testing purposes

private:
  // Private methods
  void addBuildSubcommand();
  void addRunSubcommand();
  void addTestSubcommand();
  void addInstallSubcommand();
  void addUninstallSubcommand();
  void addCompileSubcommandOptions(CLI::App *subCmd) const;
  void addInstrumentationOptions(CLI::App *subCmd) const;
  static void ensureNotDockerized();

  // Members
  CLI::App app = CLI::App{"Spice Programming Language", "spice"};
};

} // namespace spice::compiler