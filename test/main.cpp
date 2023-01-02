// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "cli/CLIInterface.h"

#include <gtest/gtest.h>

// GCOV_EXCL_START

bool updateRefs = false;
bool runBenchmarks = false;
bool skipNonGitHubTests = false;

/**
 * Entry point to the Spice testing suite
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  // Initialize command line parser
  CLIInterface cli;
  cli.createInterface();
  cli.addOptions(updateRefs, runBenchmarks, skipNonGitHubTests);
  // Parse command line args
  cli.parse(argc, argv);
  // Run tests
  return RUN_ALL_TESTS();
}

// GCOV_EXCL_STOP