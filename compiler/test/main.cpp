// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

bool updateRefs;

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
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    // Extract cli args
    updateRefs = args[1] == "true";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}