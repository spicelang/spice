// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CliInterface.h"

void CliInterface::createInterface() {
    // Add global options
    app.add_flag_callback("--version,-v", [&]() {

    }, "Prints the Spice version you are using");

    // Create sub-commands
    addBuildSubcommand();
    addRunSubcommand();
    addInstallSubcommand();
    addUninstallSubcommand();
}

void CliInterface::addBuildSubcommand() {
    CLI::App* subCmd = app.add_subcommand(
            "build",
            "Builds your Spice program and emits an executable"
    );
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput);
    /*std::function<void(const std::string& triple)> targetTripleCallback(const std::string& triple) {
            cliOptions.targetTriple = triple;
    };
    subCmd->add_option_function("--target,--target-triple,--target-triplet,-t", targetTripleCallback,
            "Target triple for the emitted executable (for cross-compiling)");*/
    subCmd->add_option<const std::string>("--output,-o", cliOptions.outputDir);
}

void CliInterface::addRunSubcommand() {
    CLI::App* subCmd = app.add_subcommand(
            "run",
            "Builds your Spice program and runs it immediately"
    );
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput);
}

void CliInterface::addInstallSubcommand() {
    CLI::App* cmdInstall = app.add_subcommand(
            "install",
            "Builds your Spice program and installs it to a directory in the PATH variable"
    );

}

void CliInterface::addUninstallSubcommand() {
    CLI::App* cmdUninstall = app.add_subcommand(
            "uninstall",
            "Builds your Spice program and runs it immediately"
    );
}

CLI::App& CliInterface::getApp() {
    return app;
}

CliOptions* CliInterface::getOptions() {
    return &cliOptions;
}