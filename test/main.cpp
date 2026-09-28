// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "driver/TestDriver.h"
#include "util/BootstrapUtil.h"

#include <gtest/gtest.h>

// GCOV_EXCL_START
using namespace spice::testing;

namespace spice::testing {
extern TestDriverCliOptions testDriverCliOptions;
}

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
  if (const int parseResult = driver.parse(argc, argv); parseResult != 0)
    return parseResult;
  // In bootstrap mode, build the bootstrap compiler first, unless a pre-built one was given. Listing the tests (e.g. by
  // gtest-parallel) does not run them, so the build can be skipped then
  if (testDriverCliOptions.bootstrapMode && testDriverCliOptions.bootstrapCompilerPath.empty() && !GTEST_FLAG_GET(list_tests))
    if (!BootstrapUtil::buildBootstrapCompiler())
      return 1;
  // Run tests
  return RUN_ALL_TESTS();
}

// GCOV_EXCL_STOP