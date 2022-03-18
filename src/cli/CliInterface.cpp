// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CliInterface.h"

void CliInterface::createInterface() {
    // Allow positional args
    app.allow_windows_style_options();
    app.allow_extras();

    // Add global options
    app.add_flag_callback("--version,-v", [&]() {
        std::cout << "Spice version " << SPICE_VERSION << std::endl;
        std::cout << "built by: " << SPICE_BUILT_BY << std::endl << std::endl;
        std::cout << "(c) Marc Auberer 2021-2022" << std::endl;
    }, "Prints the Spice version you are using");

    // Create sub-commands
    addBuildSubcommand();
    addRunSubcommand();
    addInstallSubcommand();
    addUninstallSubcommand();

    // Add footer
    app.footer("(c) Marc Auberer 2021-2022");
}

/**
 * Validates if all necessary cli options were provided.
 *
 * @throws InvalidCliOptionsException if there were an invalid combination of cli options provided
 */
void CliInterface::validate() {
    // Check if source file is present

    // Check if all three of --target-arch, --target-vendor and --target-os are provided or none of them
    if (!((cliOptions.targetArch.empty() && cliOptions.targetVendor.empty() && cliOptions.targetOs.empty()) ||
        (!cliOptions.targetArch.empty() && !cliOptions.targetVendor.empty() && !cliOptions.targetOs.empty()))) {
        throw err.get(INCOMPLETE_TARGET_TRIPLE,
                      "You need to provide all three of --target-arch, --target-vendor and --target-os");
    }
}

void CliInterface::addBuildSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "build",
            "Builds your Spice program and emits an executable"
    );
    subCmd->alias("b");
    subCmd->ignore_case();
    subCmd->configurable();
    subCmd->callback([&]() {
        compile = true; // Requires the source file to be compiled
    });

    // --debug-output
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput);

    // --target-triple
    std::function<void(const std::string&)> targetTripleCallback = [&](const std::string& triple) {
        cliOptions.targetTriple = triple;
        // Get arch, vendor and os from target triple string
        size_t firstIndex = triple.find('-', 0);
        size_t secondIndex = triple.find('-', firstIndex +1);
        size_t thirdIndex = triple.find('-', secondIndex +1);
        cliOptions.targetArch = triple.substr(0, firstIndex);
        cliOptions.targetVendor = triple.substr(firstIndex +1, secondIndex - firstIndex);
        cliOptions.targetOs = triple.substr(secondIndex +1, thirdIndex - secondIndex);
    };
    subCmd->add_option_function("--target,--target-triple,--target-triplet,-t", targetTripleCallback,
            "Target triple for the emitted executable (for cross-compiling)");

    // --target-arch
    subCmd->add_option("--target-arch", cliOptions.targetArch,
                                "Target arch for emitted executable (for cross-compiling)");

    // --target-vendor
    subCmd->add_option("--target-vendor", cliOptions.targetVendor,
                       "Target vendor for emitted executable (for cross-compiling)");

    // --target-os
    subCmd->add_option("--target-os", cliOptions.targetOs,
                       "Target os for emitted executable (for cross-compiling)");

    // --output
    subCmd->add_option<const std::string>("--output,-o", cliOptions.outputDir);

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

void CliInterface::addRunSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "run",
            "Builds your Spice program and runs it immediately"
    );
    subCmd->alias("r");
    subCmd->ignore_case();
    subCmd->callback([&]() {
        compile = true; // Requires the source file to be compiled
    });

    // --debug-output
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput);

    // --output
    subCmd->add_option<const std::string>("--output,-o", cliOptions.outputDir);

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

void CliInterface::addInstallSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "install",
            "Builds your Spice program and installs it to a directory in the PATH variable"
    );
    subCmd->alias("i");
    subCmd->ignore_case();
    subCmd->callback([&]() {
        compile = true; // Requires the source file to be compiled
    });

    // --debug-output
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput);

    // --output
    subCmd->add_option<const std::string>("--output,-o", cliOptions.outputDir);

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

void CliInterface::addUninstallSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "uninstall",
            "Builds your Spice program and runs it immediately"
    );
    subCmd->alias("u");
    subCmd->ignore_case();

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

CliOptions* CliInterface::getOptions() {
    return &cliOptions;
}

int CliInterface::parse(int argc, char** argv) {
    CLI11_PARSE(app, argc, argv)
    return 0;
}

bool CliInterface::shouldCompile() {
    return compile;
}