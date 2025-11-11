// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "driver/TestDriver.h"

#include <gtest/gtest.h>

// GCOV_EXCL_START
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
  TestDriver driver;
  driver.createInterface();
  driver.addOptions();
  // Parse command line args
  driver.parse(argc, argv);
  // Run tests
  return RUN_ALL_TESTS();
}

// GCOV_EXCL_STOP