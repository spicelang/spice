// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "CLIInterface.h"

#include <exception/CliError.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

namespace spice::compiler {

void CLIInterface::createInterface() {
  // Allow positional args
  app.allow_windows_style_options();
  app.allow_extras();
  app.positionals_at_end();
  app.footer("(c) Marc Auberer 2021-2023");

  // Add version flag
  std::string versionName = std::string(SPICE_VERSION);
  std::string builtBy = std::string(SPICE_BUILT_BY);
  std::string versionString = "Spice version " + versionName + "\nbuilt by: " + builtBy + "\n\n(c) Marc Auberer 2021-2023";
  app.set_version_flag("--version,-v", versionString);

  // Create sub-commands
  addBuildSubcommand();
  addRunSubcommand();
  addInstallSubcommand();
  addUninstallSubcommand();

  app.final_callback([&]() {
    if (!shouldCompile)
      return;

    // If the binary should be installed, set the output path to the Spice bin directory
    if (shouldInstall) {
      std::string installPath = FileUtil::getSpiceBinDir();
      FileUtil::createDirs(installPath);
      installPath += FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#if OS_WINDOWS
      installPath += ".exe";
#endif
      cliOptions.outputPath = installPath;
    }

    // Ensure that both, the output path and the output dir have valid values
    if (cliOptions.outputPath.empty())
      cliOptions.outputPath = ".";
    if (cliOptions.outputPath == "." || cliOptions.outputPath == "..") {
      cliOptions.outputPath = FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#if OS_WINDOWS
      cliOptions.outputPath += ".exe";
#endif
    }

    // Add relative prefix to filename
    if (cliOptions.mainSourceFile.find("/\\") == std::string::npos)
      cliOptions.mainSourceFile = "./" + cliOptions.mainSourceFile;

    // Get temporary directory of system
    std::string tmpDir = std::filesystem::temp_directory_path().string();
    if (tmpDir.back() != '/' && tmpDir.back() != '\\')
      tmpDir += FileUtil::DIR_SEPARATOR;

    // Set cacheDir to <system-tmp-dir>/spice/cache
    std::stringstream cacheDir;
    cacheDir << tmpDir << "spice" << FileUtil::DIR_SEPARATOR << "cache";
    cliOptions.outputDir = cacheDir.str();

    // Set outputDir to <system-tmp-dir>/spice/objects
    std::stringstream outputDir;
    uint64_t millis = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    outputDir << tmpDir << "spice" << FileUtil::DIR_SEPARATOR << "output" << FileUtil::DIR_SEPARATOR << std::to_string(millis);
    cliOptions.outputDir = outputDir.str();

    // Set output path to output dir if running is enabled
    if (shouldRun) {
      cliOptions.outputPath = cliOptions.outputDir + FileUtil::DIR_SEPARATOR;
      cliOptions.outputPath += FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
    }

    // Create the output dir if it does not exist already
    if (!FileUtil::dirExists(cliOptions.outputDir))
      FileUtil::createDirs(cliOptions.outputDir);
  });
}

/**
 * Validates if all necessary cli options were provided.
 *
 * @throws InvalidCliOptionsException if there were an invalid combination of cli options provided
 */
void CLIInterface::validate() const {
  // Check if all three of --target-arch, --target-vendor and --target-os are provided or none of them
  if (!((cliOptions.targetArch.empty() && cliOptions.targetVendor.empty() && cliOptions.targetOs.empty()) ||
        (!cliOptions.targetArch.empty() && !cliOptions.targetVendor.empty() && !cliOptions.targetOs.empty()))) {
    throw CliError(INCOMPLETE_TARGET_TRIPLE, "You need to provide all three of --target-arch, --target-vendor and --target-os");
  }

  // Error out when opt level > 0 and debug info enabled
  if (cliOptions.optLevel > 0 && cliOptions.generateDebugInfo)
    throw CliError(
        OPT_DEBUG_INFO_INCOMPATIBILITY,
        "Optimization does not work reliably when emitting debug info. The cli argument -g only works in combination with -O0.");
}

/**
 * Initialize the cli options based on the input of the user
 */
void CLIInterface::enrich() {
  // Propagate target information
  llvm::Triple defaultTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
  if (cliOptions.targetTriple.empty() && cliOptions.targetArch.empty()) { // We have nothing -> obtain the host triplet
    cliOptions.targetTriple = defaultTriple.getTriple();
    cliOptions.targetArch = defaultTriple.getArchName();
    cliOptions.targetVendor = defaultTriple.getVendorName();
    cliOptions.targetOs = defaultTriple.getOSName();
    cliOptions.isNativeTarget = true;
  } else if (cliOptions.targetTriple.empty()) { // We have arch, vendor and os -> obtain triplet
    llvm::Triple triple = llvm::Triple(cliOptions.targetArch, cliOptions.targetVendor, cliOptions.targetOs);
    cliOptions.targetTriple = triple.getTriple();
    cliOptions.isNativeTarget = triple == defaultTriple;
  } else { // Obtain arch, vendor and os by the triplet
    llvm::Triple triple = llvm::Triple(cliOptions.targetTriple);
    cliOptions.targetArch = triple.getArchName();
    cliOptions.targetVendor = triple.getVendorName();
    cliOptions.targetOs = triple.getOSName();
    cliOptions.isNativeTarget = triple == defaultTriple;
  }
}

/**
 * Add build subcommand to cli interface
 */
void CLIInterface::addBuildSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("build", "Builds your Spice program and emits an executable");
  subCmd->alias("b");
  subCmd->ignore_case();
  subCmd->configurable();
  subCmd->callback([&]() {
    shouldCompile = true; // Requires the source file to be compiled
  });

  addCompileSubcommandOptions(subCmd);

  // --target-triple
  subCmd->add_option<std::string>("--target,-t,--target-triple", cliOptions.targetTriple,
                                  "Target triple for the emitted executable (for cross-compiling)");
  // --target-arch
  subCmd->add_option<std::string>("--target-arch", cliOptions.targetArch,
                                  "Target arch for emitted executable (for cross-compiling)");
  // --target-vendor
  subCmd->add_option<std::string>("--target-vendor", cliOptions.targetVendor,
                                  "Target vendor for emitted executable (for cross-compiling)");
  // --target-os
  subCmd->add_option<std::string>("--target-os", cliOptions.targetOs, "Target os for emitted executable (for cross-compiling)");

  // --output
  subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");
  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");
}

/**
 * Add run subcommand to cli interface
 */
void CLIInterface::addRunSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("run", "Builds your Spice program and runs it immediately");
  subCmd->alias("r");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    shouldCompile = shouldRun = true; // Requires the source file to be compiled
  });

  addCompileSubcommandOptions(subCmd);

  // --output
  subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");
  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");
}

/**
 * Add install subcommand to cli interface
 */
void CLIInterface::addInstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd =
      app.add_subcommand("install", "Builds your Spice program and installs it to a directory in the PATH variable");
  subCmd->alias("i");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    shouldCompile = shouldInstall = true; // Requires the source file to be compiled
  });

  addCompileSubcommandOptions(subCmd);
}

/**
 * Add uninstall subcommand to cli interface
 */
void CLIInterface::addUninstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("uninstall", "Builds your Spice program and runs it immediately");
  subCmd->alias("u");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    std::string installPath = FileUtil::getSpiceBinDir();
    installPath += FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#if OS_WINDOWS
    installPath += ".exe";
#endif
    if (!FileUtil::fileExists(installPath)) {
      CompilerWarning(UNINSTALL_FAILED, "The executable was not found at the expected location").print();
      return;
    }
    FileUtil::deleteFile(installPath);
    std::cout << "Successfully uninstalled.\n";
  });

  // Source file
  subCmd->add_option<std::string>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

void CLIInterface::addCompileSubcommandOptions(CLI::App *subCmd) {
  // --debug-output
  subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");
  // --dump-cst
  subCmd->add_flag<bool>("--dump-cst,-cst", cliOptions.dumpCST, "Dump CST as serialized string and SVG image");
  // --dump-ast
  subCmd->add_flag<bool>("--dump-ast,-ast", cliOptions.dumpAST, "Dump AST as serialized string and SVG image");
  // --dump-symtab
  subCmd->add_flag<bool>("--dump-symtab,-symtab", cliOptions.dumpSymbolTables, "Dump serialized symbol tables");
  // --dump-ir
  subCmd->add_flag<bool>("--dump-ir,-ir", cliOptions.dumpIR, "Dump LLVM-IR");
  // --dump-assembly
  subCmd->add_flag<bool>("--dump-assembly,-asm,-s", cliOptions.dumpAssembly, "Dump Assembly code");

  // --jobs
  subCmd->add_option<unsigned short>("--jobs,-j", cliOptions.compileJobCount, "Compile jobs (threads), used for compilation");
  // --ignore-cache
  subCmd->add_option<bool>("--ignore-cache", cliOptions.ignoreCache, "Force re-compilation of all source files");
  // --disable-ast-opt
  subCmd->add_option<bool>("--disable-ast-opt", cliOptions.disableAstOpt, "Disable first order optimizations on the AST");

  // Opt levels
  subCmd->add_flag_callback(
      "-O0", [&]() { cliOptions.optLevel = 0; }, "Disable optimization for the output executable.");
  subCmd->add_flag_callback(
      "-O1", [&]() { cliOptions.optLevel = 1; }, "Optimization level 1. Only basic optimization is executed.");
  subCmd->add_flag_callback(
      "-O2", [&]() { cliOptions.optLevel = 2; }, "Optimization level 2. More advanced optimization is applied.");
  subCmd->add_flag_callback(
      "-O3", [&]() { cliOptions.optLevel = 3; }, "Optimization level 3. Aggressive optimization for best performance.");
  subCmd->add_flag_callback(
      "-Os", [&]() { cliOptions.optLevel = 4; }, "Optimization level s. Size optimization for output executable.");
  subCmd->add_flag_callback(
      "-Oz", [&]() { cliOptions.optLevel = 5; }, "Optimization level z. Aggressive optimization for best size.");

  // Source file
  subCmd->add_option<std::string>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int CLIInterface::parse(int argc, char **argv) {
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
  return 0;
}

/**
 * Executes the built executable
 */
void CLIInterface::runBinary() const {
  // Print status message
  if (cliOptions.printDebugOutput)
    std::cout << "Running executable ...\n\n";

  // Run executable
  int exitCode = std::system(cliOptions.outputPath.c_str()) / 256;
  if (exitCode != 0)
    throw CliError(NON_ZERO_EXIT_CODE, "Your Spice executable exited with non-zero exit code " + std::to_string(exitCode));
}

} // namespace spice::compiler