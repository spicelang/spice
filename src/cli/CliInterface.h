// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <exception/ErrorFactory.h>

#include "../../lib/cli11/CLI11.hpp"

#ifdef __unix__
#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif

/**
 * Representation of the various cli options
 */
struct CliOptions {
  std::string mainSourceFile; // e.g. main.spice
  std::string targetTriple;   // In format: <arch><sub>-<vendor>-<sys>-<abi>
  std::string targetArch;
  std::string targetVendor;
  std::string targetOs;
  std::string outputDir;  // Where the object files go. Should always be a temp directory
  std::string outputPath; // Where the output binary goes.
  bool printDebugOutput = false;
  bool dumpCST = false;
  bool dumpAST = false;
  bool dumpIR = false;
  bool dumpAssembly = false;
  bool dumpSymbolTables = false;
  short optLevel = 2; // -O0 = 0, -O1 = 1, -O2 = 2, -O3 = 3, -Os = 4, -Oz = 5
  bool generateDebugInfo = false;
  bool disableVerifier = false;
  bool testMode = false;
};

/**
 * Helper class to setup the cli interface and command line parser
 */
class CliInterface {
public:
  // Constructors
  explicit CliInterface() = default;
  explicit CliInterface(CliOptions options) : cliOptions(std::move(options)) {}

  // Public methods
  void createInterface();
  int parse(int argc, char **argv);
  void validate() const;
  void enrich();
  CliOptions &getOptions();
  [[nodiscard]] bool shouldCompile() const;
  [[nodiscard]] bool shouldRun() const;
  void runBinary() const;

private:
  // Private methods
  void addBuildSubcommand();
  void addRunSubcommand();
  void addInstallSubcommand();
  void addUninstallSubcommand();

  // Members
  CLI::App app = CLI::App{"Spice Programming Language", "Spice"};
  CliOptions cliOptions{};
  const ErrorFactory err = ErrorFactory();
  bool compile = false;
  bool install = false;
  bool run = false;
};