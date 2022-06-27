// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CliInterface.h"

#include <util/CompilerWarning.h>
#include <util/FileUtil.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

void CliInterface::createInterface() {
  // Allow positional args
  app.allow_windows_style_options();
  app.allow_extras();
  app.positionals_at_end();
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
    if (!compile)
      return;

    // If the binary should be installed, set the output path to the Spice bin directory
    if (install) {
      std::string installPath = FileUtil::getSpiceBinDir();
      FileUtil::createDirs(installPath);
      installPath += FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#ifdef OS_WINDOWS
      installPath += ".exe";
#endif
      cliOptions.outputPath = installPath;
    }

    // Ensure that both, the output path and the output dir have valid values
    if (cliOptions.outputPath.empty())
      cliOptions.outputPath = ".";
    if (cliOptions.outputPath == "." || cliOptions.outputPath == "..") {
      cliOptions.outputPath = FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#ifdef OS_WINDOWS
      cliOptions.outputPath += ".exe";
#endif
    }

    // Add relative prefix to filename
    if (cliOptions.mainSourceFile.find("/\\") == std::string::npos)
      cliOptions.mainSourceFile = "./" + cliOptions.mainSourceFile;

    // Set outputDir to <system-tmp-dir>/spice-output
    uint64_t millis = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cliOptions.outputDir = std::filesystem::temp_directory_path().string();
    if (cliOptions.outputDir.back() != '/' && cliOptions.outputDir.back() != '\\')
      cliOptions.outputDir += FileUtil::DIR_SEPARATOR;
    cliOptions.outputDir += "spice-output";
    cliOptions.outputDir += FileUtil::DIR_SEPARATOR;
    cliOptions.outputDir += std::to_string(millis);

    // Set output path to output dir if running is enabled
    if (run) {
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
void CliInterface::validate() const {
  // Check if all three of --target-arch, --target-vendor and --target-os are provided or none of them
  if (!((cliOptions.targetArch.empty() && cliOptions.targetVendor.empty() && cliOptions.targetOs.empty()) ||
        (!cliOptions.targetArch.empty() && !cliOptions.targetVendor.empty() && !cliOptions.targetOs.empty()))) {
    throw ErrorFactory::get(INCOMPLETE_TARGET_TRIPLE,
                            "You need to provide all three of --target-arch, --target-vendor and --target-os");
  }

  // Error out when opt level > 0 and debug info enabled
  if (cliOptions.optLevel > 0 && cliOptions.generateDebugInfo)
    throw ErrorFactory::get(
        OPT_DEBUG_INFO_INCOMPATIBILITY,
        "Optimization does not work reliably when emitting debug info. The cli argument -g only works in combination with -O0.");
}

/**
 * Initialize the cli options based on the input of the user
 */
void CliInterface::enrich() {
  // Propagate target information
  if (cliOptions.targetTriple.empty() && cliOptions.targetArch.empty()) { // We have nothing -> obtain the host triplet
    llvm::Triple triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    cliOptions.targetTriple = triple.getTriple();
    cliOptions.targetArch = triple.getArchName();
    cliOptions.targetVendor = triple.getVendorName();
    cliOptions.targetOs = triple.getOSName();
  } else if (cliOptions.targetTriple.empty()) { // We have arch, vendor and os -> obtain triplet
    llvm::Triple triple = llvm::Triple(cliOptions.targetArch, cliOptions.targetVendor, cliOptions.targetOs);
    cliOptions.targetTriple = triple.getTriple();
  } else { // Obtain arch, vendor and os by the triplet
    llvm::Triple triple = llvm::Triple(cliOptions.targetTriple);
    cliOptions.targetArch = triple.getArchName();
    cliOptions.targetVendor = triple.getVendorName();
    cliOptions.targetOs = triple.getOSName();
  }
  // Dump IR as well as symbol table if all debug output is enabled
  if (cliOptions.printDebugOutput) {
    cliOptions.dumpAST = true;
    cliOptions.dumpIR = true;
    cliOptions.dumpSymbolTables = true;
  }
}

/**
 * Add build subcommand to cli interface
 */
void CliInterface::addBuildSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("build", "Builds your Spice program and emits an executable");
  subCmd->alias("b");
  subCmd->ignore_case();
  subCmd->configurable();
  subCmd->callback([&]() {
    compile = true; // Requires the source file to be compiled
  });

  // --debug-output
  subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");
  // --dump-ast
  subCmd->add_flag<bool>("--dump-ast,-ast", cliOptions.dumpAST, "Dump AST as serialized string and SVG image");
  // --dump-symtab
  subCmd->add_flag<bool>("--dump-symtab,-symtab", cliOptions.dumpSymbolTables, "Dump serialized symbol tables");
  // --dump-ir
  subCmd->add_flag<bool>("--dump-ir,-ir", cliOptions.dumpIR, "Dump LLVM-IR");
  // --dump-assembly
  subCmd->add_flag<bool>("--dump-assembly,-asm,-s", cliOptions.dumpAssembly, "Dump Assembly code");

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

  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");

  // Source file
  subCmd->add_option<std::string>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

/**
 * Add run subcommand to cli interface
 */
void CliInterface::addRunSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("run", "Builds your Spice program and runs it immediately");
  subCmd->alias("r");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    compile = run = true; // Requires the source file to be compiled
  });

  // --debug-output
  subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");
  // --dump-ast
  subCmd->add_flag<bool>("--dump-ast,-ast", cliOptions.dumpAST, "Dump AST as serialized string and SVG image");
  // --dump-symtab
  subCmd->add_flag<bool>("--dump-symtab,-symtab", cliOptions.dumpSymbolTables, "Dump serialized symbol tables");
  // --dump-ir
  subCmd->add_flag<bool>("--dump-ir,-ir", cliOptions.dumpIR, "Dump LLVM-IR");
  // --dump-assembly
  subCmd->add_flag<bool>("--dump-assembly,-asm,-s", cliOptions.dumpAssembly, "Dump Assembly code");

  // --output
  subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");

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

  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");

  // Source file
  subCmd->add_option<std::string>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

/**
 * Add install subcommand to cli interface
 */
void CliInterface::addInstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd =
      app.add_subcommand("install", "Builds your Spice program and installs it to a directory in the PATH variable");
  subCmd->alias("i");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    compile = install = true; // Requires the source file to be compiled
  });

  // --debug-output
  subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");
  // --dump-ast
  subCmd->add_flag<bool>("--dump-ast,-ast", cliOptions.dumpAST, "Dump AST as serialized string and SVG image");
  // --dump-symtab
  subCmd->add_flag<bool>("--dump-symtab,-symtab", cliOptions.dumpSymbolTables, "Dump serialized symbol tables");
  // --dump-ir
  subCmd->add_flag<bool>("--dump-ir,-ir", cliOptions.dumpIR, "Dump LLVM-IR");
  // --dump-assembly
  subCmd->add_flag<bool>("--dump-assembly,-asm,-s", cliOptions.dumpAssembly, "Dump Assembly code");

  // --output
  subCmd->add_option<std::string>("--output,-o", cliOptions.outputPath, "Set the output file path");

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
 * Add uninstall subcommand to cli interface
 */
void CliInterface::addUninstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("uninstall", "Builds your Spice program and runs it immediately");
  subCmd->alias("u");
  subCmd->ignore_case();
  subCmd->callback([&]() {
    std::string installPath = FileUtil::getSpiceBinDir();
    installPath += FileUtil::getFileName(cliOptions.mainSourceFile.substr(0, cliOptions.mainSourceFile.length() - 6));
#ifdef OS_WINDOWS
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

/**
 * Get cli option struct
 *
 * @return Cli options
 */
CliOptions &CliInterface::getOptions() { return cliOptions; }

/**
 * Checks if compiling is necessary
 *
 * @return Compile or not
 */
bool CliInterface::shouldCompile() const { return compile; }

/**
 * Checks if running is necessary
 *
 * @return Run or not
 */
bool CliInterface::shouldRun() const { return run; }

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int CliInterface::parse(int argc, char **argv) {
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }
  return 0;
}

/**
 * Executes the built executable
 */
void CliInterface::runBinary() const {
  // Print status message
  if (cliOptions.printDebugOutput)
    std::cout << "Running executable ...\n";

  // Run executable
  std::system(cliOptions.outputPath.c_str());
}