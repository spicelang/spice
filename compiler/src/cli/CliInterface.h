// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/cli11/CLI11.hpp"

struct CliOptions {
    std::string mainSourceFile;
    std::string targetTriple;
    std::string targetArch;
    std::string targetVendor;
    std::string targetOs;
    std::string outputDir;
    bool printDebugOutput;
    int optLevel; // -O0 = 0, -O1 = 1, -O2 = 2, -O3 = 3, -Os = 4, -Oz = 5
};

class CliInterface {
public:
    // Public methods
    void createInterface();
    CLI::App& getApp();
    CliOptions* getOptions();
private:
    // Private methods
    void addBuildSubcommand();
    void addRunSubcommand();
    void addInstallSubcommand();
    void addUninstallSubcommand();

    // Members
    CLI::App app = CLI::App{"Spice Programming Language", "Spice"};
    CliOptions cliOptions = CliOptions{};
};