// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "Driver.h"

#include <exception/CliError.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/FileUtil.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>

namespace spice::compiler {

void Driver::init() {
  // Allow positional args
  app.positionals_at_end();
  app.allow_extras(false);
  app.footer("(c) Marc Auberer 2021-2025");

  // Add version flag
  app.set_version_flag("--version,-v", CommonUtil::buildVersionInfo());

  // Create sub-commands
  addBuildSubcommand();
  addRunSubcommand();
  addTestSubcommand();
  addInstallSubcommand();
  addUninstallSubcommand();

  app.final_callback([&] {
    // Print help text for the root command if no sub-command was given
    if (app.get_subcommands().empty()) {
      std::cout << app.help();
      return;
    }

    if (shouldInstall || shouldUninstall) {
      // Prepare the installation path
      std::filesystem::path installPath = FileUtil::getSpiceBinDir();
      installPath /= cliOptions.mainSourceFile.stem();
      if (!dryRun)
        create_directories(installPath);
#if OS_WINDOWS
      installPath.replace_extension("exe");
#endif

      // If the binary should be installed, set the output path to the Spice bin directory
      if (shouldInstall)
        cliOptions.outputPath = installPath;

      // If the binary should be uninstalled, check if the executable exists and uninstall it
      if (shouldUninstall && !dryRun) {
        if (exists(installPath) && std::filesystem::remove(installPath))
          std::cout << "Successfully uninstalled.\n";
        else
          CompilerWarning(UNINSTALL_FAILED, "The executable was not found at the expected location").print();
      }
    }

    // Abort here if we do not need to compile
    if (!shouldCompile)
      return;

    // Set output path and dir
    if (shouldExecute) {
      cliOptions.execute = true;
      const long millis = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      cliOptions.outputDir = std::filesystem::temp_directory_path() / "spice" / "output" / std::to_string(millis);
      cliOptions.outputPath = cliOptions.outputDir / cliOptions.mainSourceFile.filename();
    } else if (!cliOptions.outputPath.empty()) {
      if (is_directory(cliOptions.outputPath)) {
        cliOptions.outputDir = cliOptions.outputPath;
        cliOptions.outputPath = cliOptions.outputDir / cliOptions.mainSourceFile.filename();
      } else {
        cliOptions.outputDir = cliOptions.outputPath.parent_path();
      }
    } else {
      cliOptions.outputDir = "./";
      cliOptions.outputPath = cliOptions.outputDir / cliOptions.mainSourceFile.filename();
    }

    // Set output file extension
    if (cliOptions.targetArch == TARGET_WASM32 || cliOptions.targetArch == TARGET_WASM64) {
      cliOptions.outputPath.replace_extension("wasm");
    } else {
#if OS_WINDOWS
      cliOptions.outputPath.replace_extension("exe");
#else
      cliOptions.outputPath.replace_extension("");
#endif
    }

    // Set cache dir
    cliOptions.cacheDir = std::filesystem::temp_directory_path() / "spice" / "cache";

    // Create directories in case they not exist yet
    create_directories(cliOptions.cacheDir);
    create_directories(cliOptions.outputDir);
  });
}

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int Driver::parse(int argc, const char *argv[]) {
  try {
    app.parse(argc, argv);
    return EXIT_SUCCESS;
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
}

/**
 * Initialize the cli options based on the input of the user
 */
void Driver::enrich() {
  // Make path of given main source file canonical and relative
  cliOptions.mainSourceFile = relative(cliOptions.mainSourceFile);

  // Propagate llvm args to llvm
  if (!cliOptions.llvmArgs.empty()) {
    const std::vector<std::string> result = CommonUtil::split("llvm " + cliOptions.llvmArgs);
    std::vector<const char *> resultCStr;
    resultCStr.reserve(result.size());
    for (const std::string &str : result)
      resultCStr.push_back(str.c_str());
    llvm::cl::ParseCommandLineOptions(static_cast<int>(result.size()), resultCStr.data());
  }

  // Propagate target information
  const llvm::Triple defaultTriple(llvm::Triple::normalize(llvm::sys::getDefaultTargetTriple()));
  if (cliOptions.targetTriple.empty()) {
    if (cliOptions.targetArch == TARGET_UNKNOWN) { // We have nothing -> obtain native triplet
      cliOptions.targetTriple = defaultTriple.getTriple();
      cliOptions.targetArch = defaultTriple.getArchName();
      cliOptions.targetVendor = defaultTriple.getVendorName();
      cliOptions.targetOs = defaultTriple.getOSName();
      cliOptions.isNativeTarget = true;
    } else { // We have arch, vendor and os -> obtain triplet
      const llvm::Triple triple(cliOptions.targetArch, cliOptions.targetVendor, cliOptions.targetOs);
      cliOptions.targetTriple = triple.getTriple();
      cliOptions.isNativeTarget = triple == defaultTriple;
    }
  } else { // Obtain arch, vendor and os by the triplet
    const llvm::Triple triple(llvm::Triple::normalize(cliOptions.targetTriple));
    cliOptions.targetArch = triple.getArchName();
    cliOptions.targetVendor = triple.getVendorName();
    cliOptions.targetOs = triple.getOSName();
    cliOptions.isNativeTarget = triple == defaultTriple;
  }

  // Always preserve IR value names when dumping IR
  if (cliOptions.dumpSettings.dumpIR)
    cliOptions.namesForIRValues = true;

  // Enable test mode when test mode was selected
  if (cliOptions.buildMode == TEST) {
    cliOptions.testMode = true;
    cliOptions.noEntryFct = true;
    cliOptions.generateTestMain = true;
    cliOptions.buildMode = DEBUG;
  }
}

/**
 * Executes the built executable
 */
void Driver::runBinary() const {
  // Print status message
  if (cliOptions.printDebugOutput)
    std::cout << "Running executable ...\n\n";

  // Run executable
  std::filesystem::path executablePath = cliOptions.outputPath;
  executablePath.make_preferred();
  const int exitCode = std::system(executablePath.string().c_str()) / 256;
  if (exitCode != 0)
    throw CliError(NON_ZERO_EXIT_CODE, "Your Spice executable exited with non-zero exit code " + std::to_string(exitCode));
}

/**
 * Add build subcommand to cli interface
 */
void Driver::addBuildSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("build", "Builds your Spice program and emits an executable");
  subCmd->alias("b");
  subCmd->ignore_case();
  subCmd->configurable();
  subCmd->callback([&] {
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
  subCmd->add_option<std::filesystem::path>("--output,-o", cliOptions.outputPath, "Set the output file path");
  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");
  // --no-entry
  subCmd->add_flag<bool>("--no-entry", cliOptions.noEntryFct, "Do not generate main function");
  // --static
  subCmd->add_flag<bool>("--static", cliOptions.staticLinking, "Link statically");
  // --dump-to-files
  subCmd->add_flag<bool>("--dump-to-files", cliOptions.dumpSettings.dumpToFiles, "Redirect dumps to files instead of printing");
  // --abort-after-dump
  subCmd->add_flag<bool>("--abort-after-dump", cliOptions.dumpSettings.abortAfterDump,
                         "Abort the compilation process after dumping the first requested resource");
}

/**
 * Add run subcommand to cli interface
 */
void Driver::addRunSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("run", "Builds your Spice program and runs it immediately");
  subCmd->alias("r");
  subCmd->ignore_case();
  subCmd->callback([&] {
    shouldCompile = shouldExecute = true; // Requires the source file to be compiled
  });

  addCompileSubcommandOptions(subCmd);

  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");
}

/**
 * Add test subcommand to cli interface
 */
void Driver::addTestSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("test", "Builds your Spice program and runs all enclosed tests");
  subCmd->alias("t");
  subCmd->ignore_case();
  subCmd->callback([&] {
    shouldCompile = shouldExecute = true; // Requires the source file to be compiled
    cliOptions.testMode = true;           // Always enable assertions for tests, also in higher opt levels
    cliOptions.generateTestMain = true;   // An alternative entry function is generated
    cliOptions.noEntryFct = true;         // To not have two main functions, disable normal main
  });

  addCompileSubcommandOptions(subCmd);

  // --debug-info
  subCmd->add_flag<bool>("--debug-info,-g", cliOptions.generateDebugInfo, "Generate debug info");
  // --disable-verifier
  subCmd->add_flag<bool>("--disable-verifier", cliOptions.disableVerifier, "Disable LLVM module and function verification");
}

/**
 * Add install subcommand to cli interface
 */
void Driver::addInstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd =
      app.add_subcommand("install", "Builds your Spice program and installs it to a directory in the PATH variable");
  subCmd->alias("i");
  subCmd->ignore_case();
  subCmd->callback([&] {
    shouldCompile = true;
    shouldInstall = true;
    ensureNotDockerized();
  });

  addCompileSubcommandOptions(subCmd);
}

/**
 * Add uninstall subcommand to cli interface
 */
void Driver::addUninstallSubcommand() {
  // Create sub-command itself
  CLI::App *subCmd = app.add_subcommand("uninstall", "Builds your Spice program and runs it immediately");
  subCmd->alias("u");
  subCmd->ignore_case();
  subCmd->callback([&] {
    shouldUninstall = true;
    ensureNotDockerized();
  });

  // Source file
  subCmd->add_option<std::filesystem::path>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

void Driver::addCompileSubcommandOptions(CLI::App *subCmd) {
  const auto buildModeCallback = [&](const CLI::results_t &results) {
    std::string inputString = results.front();
    std::ranges::transform(inputString, inputString.begin(), tolower);

    if (inputString == BUILD_MODE_DEBUG)
      cliOptions.buildMode = DEBUG;
    else if (inputString == BUILD_MODE_RELEASE)
      cliOptions.buildMode = RELEASE;
    else if (inputString == BUILD_MODE_TEST)
      cliOptions.buildMode = TEST;
    else
      throw CliError(INVALID_BUILD_MODE, "Invalid build mode: " + inputString);

    return true;
  };

  // --build-mode
  subCmd->add_option("--build-mode,-m", buildModeCallback, "Build mode (debug, release, test)");
  // --llvm-args
  subCmd->add_option<std::string>("--llvm-args,-llvm", cliOptions.llvmArgs, "Additional arguments for LLVM")->join(' ');
  // --jobs
  subCmd->add_option<unsigned short>("--jobs,-j", cliOptions.compileJobCount, "Compile jobs (threads), used for compilation");
  // --ignore-cache
  subCmd->add_flag<bool>("--ignore-cache", cliOptions.ignoreCache, "Force re-compilation of all source files");
  // --use-lifetime-markers
  subCmd->add_flag<bool>("--use-lifetime-markers", cliOptions.useLifetimeMarkers,
                         "Generate lifetime markers to enhance optimizations");

  // Opt levels
  subCmd->add_flag_callback("-O0", [&] { cliOptions.optLevel = O0; }, "Disable optimization for the output executable.");
  subCmd->add_flag_callback("-O1", [&] { cliOptions.optLevel = O1; }, "Only basic optimization is executed.");
  subCmd->add_flag_callback("-O2", [&] { cliOptions.optLevel = O2; }, "More advanced optimization is applied.");
  subCmd->add_flag_callback("-O3", [&] { cliOptions.optLevel = O3; }, "Aggressive optimization for best performance.");
  subCmd->add_flag_callback("-Os", [&] { cliOptions.optLevel = Os; }, "Size optimization for output executable.");
  subCmd->add_flag_callback("-Oz", [&] { cliOptions.optLevel = Oz; }, "Aggressive optimization for best size.");
  subCmd->add_flag<bool>("-lto", cliOptions.useLTO, "Enable link time optimization (LTO)");

  // --debug-output
  subCmd->add_flag<bool>("--debug-output,-d", cliOptions.printDebugOutput, "Enable debug output");
  // --dump-cst
  subCmd->add_flag<bool>("--dump-cst,-cst", cliOptions.dumpSettings.dumpCST, "Dump CST as serialized string and SVG image");
  // --dump-ast
  subCmd->add_flag<bool>("--dump-ast,-ast", cliOptions.dumpSettings.dumpAST, "Dump AST as serialized string and SVG image");
  // --dump-symtab
  subCmd->add_flag<bool>("--dump-symtab,-symtab", cliOptions.dumpSettings.dumpSymbolTable, "Dump serialized symbol tables");
  // --dump-types
  subCmd->add_flag<bool>("--dump-types,-types", cliOptions.dumpSettings.dumpTypes, "Dump all used types");
  // --dump-ir
  subCmd->add_flag<bool>("--dump-ir,-ir", cliOptions.dumpSettings.dumpIR, "Dump LLVM-IR");
  // --dump-assembly
  subCmd->add_flag<bool>("--dump-assembly,-asm,-s", cliOptions.dumpSettings.dumpAssembly, "Dump Assembly code");
  // --dump-object-file
  subCmd->add_flag<bool>("--dump-object-file,-obj", cliOptions.dumpSettings.dumpObjectFile, "Dump object file");

  // Source file
  subCmd->add_option<std::filesystem::path>("<main-source-file>", cliOptions.mainSourceFile, "Main source file")
      ->check(CLI::ExistingFile)
      ->required();
}

/**
 * Ensure that the compiler is not running in a Docker container
 */
void Driver::ensureNotDockerized() {
  const char *envValue = std::getenv(ENV_VAR_DOCKERIZED);
  if (envValue != nullptr && std::strcmp(envValue, "true") == 0)
    throw CliError(FEATURE_NOT_SUPPORTED_WHEN_DOCKERIZED,
                   "This feature is not supported in a containerized environment. Please use the standalone version of Spice.");
}

} // namespace spice::compiler