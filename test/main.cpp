// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "driver/Driver.h"

#include <gtest/gtest.h>

// GCOV_EXCL_START

namespace spice::testing {

bool updateRefs = false;
bool runBenchmarks = false;
bool enableLeakDetection = false;
bool skipNonGitHubTests = false;

} // namespace spice::testing

using namespace spice::testing;

/**
 * Entry point to the Spice testing suite
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  // Initialize command line parser
  Driver driver;
  driver.createInterface();
  driver.addOptions(updateRefs, runBenchmarks, enableLeakDetection, skipNonGitHubTests);
  // Parse command line args
  driver.parse(argc, argv);
  // Run tests
  return RUN_ALL_TESTS();
}

// GCOV_EXCL_STOP