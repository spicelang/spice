// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/cli11/CLI11.hpp"

// Undef conflicting macros (only problematic on Windows)
#undef TRUE
#undef FALSE
#undef CONST

// GCOV_EXCL_START
namespace spice::testing {

struct TestDriverCliOptions {
  bool updateRefs = false;
  bool runBenchmarks = false;
  bool enableLeakDetection = false;
  bool isGitHubActions = false;
  bool isVerbose = false;
};

/**
 * Helper class to set up the cli interface and command line parser
 */
class TestDriver {
public:
  // Public methods
  void createInterface();
  void addOptions();
  int parse(int argc, char **argv);

private:
  // Private members
  CLI::App app = CLI::App{"Spice Test Runner", "spicetest"};
};

} // namespace spice::testing
// GCOV_EXCL_STOP