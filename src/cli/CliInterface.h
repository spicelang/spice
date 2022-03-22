// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#ifdef __unix__
#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif

#include <exception/ErrorFactory.h>

#include "../../lib/cli11/CLI11.hpp"

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
  short optLevel = 2; // -O0 = 0, -O1 = 1, -O2 = 2, -O3 = 3, -Os = 4, -Oz = 5
};

/**
 * Helper class to setup the cli interface and command line parser
 */
class CliInterface {
public:
  // Public methods
  void createInterface();
  int parse(int argc, char **argv);
  void validate() const;
  CliOptions *getOptions();
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
  CliOptions cliOptions = CliOptions{};
  const ErrorFactory err = ErrorFactory();
  bool compile = false;
  bool install = false;
  bool run = false;
};