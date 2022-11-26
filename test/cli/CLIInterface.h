// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/cli11/CLI11.hpp"

// GCOV_EXCL_START

/**
 * Helper class to setup the cli interface and command line parser
 */
class CLIInterface {
public:
  // Constructors
  explicit CLIInterface() = default;

  // Public methods
  void createInterface();
  void addOptions(bool &updateRefs, bool &runBenchmarks, bool &skipNonGitHubTests);
  int parse(int argc, char **argv);

private:
  // Private members
  CLI::App app = CLI::App{"Spice Test Runner", "spice"};
};

// GCOV_EXCL_STOP