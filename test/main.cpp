// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <gtest/gtest.h>

bool updateRefs = false;

/**
 * Entry point to the Spice testing suite
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char **argv) { // Call ./spicetest update-refs
  // Parse cli args
  std::vector<std::string> args;
  for (size_t i = 1; i < argc; i++)
    args.emplace_back(argv[i]);

  // Extract cli args
  updateRefs = !args.empty() && args[0] == "true";

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// GCOV_EXCL_STOP