// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CliInterface.h"
#include "util/CompilerWarning.h"

#include <util/FileUtil.h>

void CliInterface::createInterface() {
    // Allow positional args
    app.allow_windows_style_options();
    app.allow_extras();
    app.footer("(c) Marc Auberer 2021-2022");

    // Add version flag
    std::string versionName = std::string(SPICE_VERSION);
    std::string builtBy = std::string(SPICE_BUILT_BY);
    std::string versionString = "Spice version " + versionName + "\nbuilt by: " + builtBy + "\n\n(c) Marc Auberer 2021-2022";
    app.set_version_flag("--version,-v", versionString);

    // Create sub-commands
    addBuildSubcommand();
    addRunSubcommand();
    addInstallSubcommand();
    addUninstallSubcommand();

    app.final_callback([&]() {
        if (!compile) return;

        // If the binary should be installed, set the output path to the Spice bin directory
        if (install) {
            cliOptions.outputPath = FileUtil::getSpiceBinDir();
            FileUtil::createDirs(cliOptions.outputPath);
            cliOptions.outputPath += FileUtil::getFileName(
                    cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#ifdef OS_WINDOWS
            cliOptions.outputPath += ".exe";
#endif
        }

        // Ensure that both, the output path and the output dir have valid values
        if (cliOptions.outputPath.empty()) cliOptions.outputPath = ".";
        if (cliOptions.outputPath == "." || cliOptions.outputPath == "..") {
            cliOptions.outputPath = FileUtil::getFileName(
                    cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#ifdef OS_WINDOWS
            cliOptions.outputPath += ".exe";
#endif
        }

        // Add relative prefix to filename
        if (cliOptions.mainSourceFile.find("/\\") != std::string::npos)
            cliOptions.mainSourceFile = "./" + cliOptions.mainSourceFile;

        // Set outputDir to <system-tmp-dir>/spice-output
        char separator = FileUtil::getDirSeparator();
        uint64_t millis = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        cliOptions.outputDir = std::filesystem::temp_directory_path().string();
        if (cliOptions.outputDir.back() != '/' && cliOptions.outputDir.back() != '\\') cliOptions.outputDir += separator;
        cliOptions.outputDir += "spice-output";
        cliOptions.outputDir += separator;
        cliOptions.outputDir += std::to_string(millis);

        // Create the output dir if it does not exist already
        if (!FileUtil::dirExists(cliOptions.outputDir)) FileUtil::createDirs(cliOptions.outputDir);
    });
}

/**
 * Validates if all necessary cli options were provided.
 *
 * @throws InvalidCliOptionsException if there were an invalid combination of cli options provided
 */
void CliInterface::validate() const {
    // Check if source file is present
    if (!FileUtil::fileExists(cliOptions.mainSourceFile))
        throw err.get(SOURCE_FILE_MISSING, "The stated main source file does not exist");

    // Check if all three of --target-arch, --target-vendor and --target-os are provided or none of them
    if (!((cliOptions.targetArch.empty() && cliOptions.targetVendor.empty() && cliOptions.targetOs.empty()) ||
        (!cliOptions.targetArch.empty() && !cliOptions.targetVendor.empty() && !cliOptions.targetOs.empty()))) {
        throw err.get(INCOMPLETE_TARGET_TRIPLE,
                      "You need to provide all three of --target-arch, --target-vendor and --target-os");
    }
}

/**
 * Add build subcommand to cli interface
 */
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
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");

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
    subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

/**
 * Add run subcommand to cli interface
 */
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
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");

    // --output
    subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

/**
 * Add install subcommand to cli interface
 */
void CliInterface::addInstallSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "install",
            "Builds your Spice program and installs it to a directory in the PATH variable"
    );
    subCmd->alias("i");
    subCmd->ignore_case();
    subCmd->callback([&]() {
        compile = install = true; // Requires the source file to be compiled
    });

    // --debug-output
    subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");

    // --output
    subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");

    // -O0, -O1, ...
    subCmd->add_flag("-O0{0},-O1{1},-O2{2},-O3{3},-Os{4},-Oz{5}", cliOptions.optLevel,
                     "Set the optimization level. -Os and -Oz are optimization modes for executable size.");

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

/**
 * Add uninstall subcommand to cli interface
 */
void CliInterface::addUninstallSubcommand() {
    // Create sub-command itself
    CLI::App* subCmd = app.add_subcommand(
            "uninstall",
            "Builds your Spice program and runs it immediately"
    );
    subCmd->alias("u");
    subCmd->ignore_case();
    subCmd->callback([&]() {
        std::string installPath = FileUtil::getSpiceBinDir() + cliOptions.outputPath;
        if (!FileUtil::fileExists(installPath)) {
            CompilerWarning(UNINSTALL_FAILED, "The executable was not found at the expected location").print();
            return;
        }
        FileUtil::deleteFile(installPath);
        std::cout << "Successfully uninstalled." << std::endl;
    });

    // Source file
    subCmd->add_option("<main-source-file>", cliOptions.mainSourceFile, "Main source file");
}

/**
 * Get cli option struct
 */
CliOptions* CliInterface::getOptions() {
    return &cliOptions;
}

/**
 * Checks if compiling is necessary
 */
bool CliInterface::shouldCompile() const {
    return compile;
}

/**
 * Checks if installing is necessary
 */
bool CliInterface::shouldInstall() const {
    return install;
}

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int CliInterface::parse(int argc, char** argv){
    CLI11_PARSE(app, argc, argv)
    return 0;
}