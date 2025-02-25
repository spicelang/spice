// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/cli11/CLI11.hpp"

// GCOV_EXCL_START
namespace spice::testing {

struct TestDriverCliOptions {
  bool updateRefs = false;
  bool runBenchmarks = false;
  bool enableLeakDetection = false;
  bool isGitHubActions = false;
};

/**
 * Helper class to set up the cli interface and command line parser
 */
class Driver {
public:
  // Constructors
  explicit Driver() = default;

  // Public methods
  void createInterface();
  void addOptions();
  int parse(int argc, char **argv);

private:
  // Private members
  CLI::App app = CLI::App{"Spice Test Runner", "spice"};
};

} // namespace spice::testing
// GCOV_EXCL_STOP