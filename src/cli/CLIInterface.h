// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "../../lib/cli11/CLI11.hpp"

namespace spice::compiler {

/**
 * Representation of the various cli options
 */
struct CliOptions {
  std::string mainSourceFile; // e.g. main.spice
  std::string targetTriple;   // In format: <arch><sub>-<vendor>-<sys>-<abi>
  std::string targetArch;
  std::string targetVendor;
  std::string targetOs;
  bool execute = false;
  bool isNativeTarget = false;
  std::string cacheDir;               // Where the cache files go. Should always be a temp directory
  std::string outputDir;              // Where the object files go. Should always be a temp directory
  std::string outputPath;             // Where the output binary goes.
  unsigned short compileJobCount = 0; // 0 for auto
  bool ignoreCache = false;
  bool printDebugOutput = false;
  bool dumpCST = false;
  bool dumpAST = false;
  bool dumpIR = false;
  bool dumpAssembly = false;
  bool dumpSymbolTables = false;
  bool disableAstOpt = false;
  short optLevel = 2; // -O0 = 0, -O1 = 1, -O2 = 2, -O3 = 3, -Os = 4, -Oz = 5
  bool generateDebugInfo = false;
  bool disableVerifier = false;
  bool testMode = false;
};

/**
 * Helper class to setup the cli interface and command line parser
 */
class CLIInterface {
public:
  // Constructors
  explicit CLIInterface() = default;

  // Public methods
  void createInterface();
  int parse(int argc, char **argv);
  void validate() const;
  void enrich();
  void runBinary() const;

  // Public members
  CliOptions cliOptions;
  bool shouldCompile = false;
  bool shouldInstall = false;
  bool shouldExecute = false;

private:
  // Private methods
  void addBuildSubcommand();
  void addRunSubcommand();
  void addInstallSubcommand();
  void addUninstallSubcommand();
  void addCompileSubcommandOptions(CLI::App *subCmd);

  // Members
  CLI::App app = CLI::App{"Spice Programming Language", "spice"};
};

} // namespace spice::compiler