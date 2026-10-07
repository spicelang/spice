// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <exception/LexerError.h>
#include <exception/LinkerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <llvm/IR/Module.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTable.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/InterfaceManager.h>
#include <typechecker/StructManager.h>
#include <util/FileUtil.h>
#include <util/SystemUtil.h>

#include "driver/TestDriver.h"
#include "util/BootstrapUtil.h"
#include "util/TestUtil.h"

using namespace spice::compiler;

namespace spice::testing {

extern TestDriverCliOptions testDriverCliOptions;

namespace {

/**
 * Re-run the IR generator for the given source file, discarding the module that was built for it before.
 *
 * Since the IR generator emits parts of the IR depending on the selected opt level, a module can only be used to check
 * the dump of the opt level it was generated for.
 *
 * @param sourceFile Source file to re-generate the IR for
 */
void reGenerateIR(SourceFile *sourceFile) {
  // Rewind the stage marker, so that the IR generator does not consider this file as already done
  sourceFile->previousStage = DEP_GRAPH_VISUALIZER;
  sourceFile->runIRGenerator();
}

/**
 * Re-run the IR generator for the given source file and all its (transitive) dependencies, dependencies first.
 *
 * This is only required for LTO builds, where the modules of all source files are merged into the LTO module, which is
 * the one that gets dumped. Without LTO, only the module of the main source file ends up in the dump, so re-generating
 * the dependencies would be wasted work.
 *
 * @param sourceFile Source file to re-generate the IR for
 * @param visitedSet Already visited source files (guards against cycles, caused by circular imports)
 */
void reGenerateIRForLTO(SourceFile *sourceFile, std::unordered_set<const SourceFile *> &visitedSet) { // NOLINT(misc-no-recursion)
  if (!visitedSet.insert(sourceFile).second)
    return;

  // Re-generate the dependencies first, mirroring the order of SourceFile::runBackEnd()
  for (SourceFile *dependency : sourceFile->dependencies | std::views::values)
    reGenerateIRForLTO(dependency, visitedSet);

  reGenerateIR(sourceFile);

  // The optimizer stages of the main source file are driven by the test runner itself. All other files run their
  // pre-link optimization as part of their own back end, so it has to be repeated here.
  if (!sourceFile->isMainFile)
    sourceFile->runPreLinkIROptimizer();
}

} // namespace

static void execTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase))
    GTEST_SKIP();

  // Create fake cli options
  const std::filesystem::path mainSourceFilePath = testCase.testPath / REF_NAME_SOURCE;
  CliOptions cliOptions = {
      /* mainSourceFile= */ mainSourceFilePath,
      /* targetTriple= */ {},
      /* targetArch= */ TARGET_UNKNOWN, // GCOV_EXCL_LINE - coverage tool bug
      /* targetVendor= */ TARGET_UNKNOWN,
      /* targetOs= */ TARGET_UNKNOWN,
      /* isNativeTarget= */ true,
      /* useCPUFeatures*/ false, // Disabled because it makes the refs differ on different machines
      /* execute= */ false,      // If we set this to 'true', the compiler will not emit object files
      /* cacheDir= */ "./cache",
      /* outputDir= */ "./",
      /* outputPath= */ "",
      /* buildMode= */ BuildMode::DEBUG,
      /* outputContainer= */ OutputContainer::EXECUTABLE,
      // Compile serially: the test cases are already spread over processes by gtest-parallel, so a parallel back end
      // would only oversubscribe the machine, and the reference outputs stay strictly deterministic this way.
      /* compileJobCount= */ 1,
      /* ignoreCache */ true,
      /* llvmArgs= */ "",
      /* printDebugOutput= */ false,
      CliOptions::DumpSettings{
          /* dumpAST= */ false,
          /* dumpSymbolTables= */ false,
          /* dumpTypes= */ false,
          /* dumpCacheStats= */ false,
          /* dumpDependencyGraph= */ false,
          /* dumpIR= */ false,
          /* dumpAssembly= */ false,
          /* dumpObjectFile= */ false,
          /* dumpToFiles= */ false,
          /* abortAfterDump */ false,
      },
      /* namesForIRValues= */ true,
      /* useLifetimeMarkers= */ false,
      /* useTBAAMetadata */ false,
      /* optLevel= */ OptLevel::O0,
      /* useLTO= */ false,
      /* backend= */ Backend::LLVM,
      /* noEntryFct= */ exists(testCase.testPath / CTL_RUN_BUILTIN_TESTS),
      /* generateTestMain= */ exists(testCase.testPath / CTL_RUN_BUILTIN_TESTS),
      /* staticLinking= */ false,
      CliOptions::InstrumentationSettings{
          /* debugInfoLevel= */ DebugInfoLevel::NONE,
          /* codeCoverage= */ testDriverCliOptions.enableCoverage,
          /* sanitizer= */ Sanitizer::NONE,
      },
      /* disableVerifier= */ false,
      /* testMode= */ true,
      /* comparableOutput= */ true,
      /* buildVars= */ {},
  };
  static_assert(sizeof(CliOptions::DumpSettings) == 10, "CliOptions::DumpSettings struct size changed");
  static_assert(sizeof(CliOptions::InstrumentationSettings) == 3, "CliOptions::InstrumentationSettings struct size changed");
#if defined(__clang__) && defined(__apple_build_version__)
  // some std types for Apple Clang are smaller than for GCC and Clang
  static_assert(sizeof(CliOptions) == 320, "CliOptions struct size changed");
#else
  static_assert(sizeof(CliOptions) == 448, "CliOptions struct size changed");
#endif

  // Parse test args
  std::vector<std::string> args = {"spice", "build"};
  TestUtil::parseTestArgs(cliOptions.mainSourceFile, args);
  args.push_back(mainSourceFilePath.string());

  bool explicitlySelectedTarget = false;
  std::vector<const char *> argv;
  argv.reserve(args.size());
  for (const std::string &arg : args) {
    if (arg.starts_with("--target"))
      explicitlySelectedTarget = true;
    argv.push_back(arg.c_str());
  }
  Driver driver(cliOptions, true);
  driver.parse(static_cast<int>(argv.size()), argv.data());
  driver.enrich();

  // If this is a cross-compilation test, we want to emit the target information in IR. For this we need to set native to false
  if (explicitlySelectedTarget)
    cliOptions.isNativeTarget = false;

  // Redirect all build artifacts to a per-test directory. Driver::enrich() resets these to shared paths (e.g. "./" and a
  // shared temp cache dir), which would cause concurrently running test processes to clobber each other's object files and
  // executables. Using a unique directory per test keeps parallel runs (e.g. via gtest-parallel) collision-free.
  const std::filesystem::path artifactDir = TestUtil::prepareArtifactDir(testCase);
  const std::filesystem::path executablePath = TestUtil::getExecutablePath(artifactDir);
  cliOptions.outputDir = artifactDir;
  cliOptions.cacheDir = artifactDir / "cache";
  std::filesystem::create_directories(cliOptions.cacheDir);

  // Instantiate GlobalResourceManager
  GlobalResourceManager resourceManager(cliOptions);

  try {
    // Create source file instance for main source file
    SourceFile *mainSourceFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, cliOptions.mainSourceFile, false);

    // Run Lexer and Parser
    mainSourceFile->runLexer();
    mainSourceFile->runParser();

    // Build and optimize AST
    mainSourceFile->runASTBuilder();

    // Check AST
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_SYNTAX_TREE, [&] {
      mainSourceFile->runASTVisualizer();
      return mainSourceFile->compilerOutput.astString;
    });

    // Execute import collector and semantic analysis stages
    mainSourceFile->runImportCollector();
    mainSourceFile->runSymbolTableBuilder();
    mainSourceFile->runMiddleEnd(); // TypeChecker pre + post

    // Check symbol table output (check happens here to include updates from type checker)
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_SYMBOL_TABLE,
                            [&] { return mainSourceFile->globalScope->getSymbolTableJSON().dump(/*indent=*/2); });

    // Fail if an error was expected
    if (TestUtil::doesRefExist(testCase.testPath / REF_NAME_ERROR_OUTPUT)) // GCOV_EXCL_LINE
      FAIL() << "Expected error, but got no error";                        // GCOV_EXCL_LINE

    // Check dependency graph
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_DEP_GRAPH, [&] {
      mainSourceFile->runDependencyGraphVisualizer();
      return mainSourceFile->compilerOutput.depGraphString;
    });

    // Run backend for all dependencies
    for (SourceFile *sourceFile : mainSourceFile->dependencies | std::views::values)
      sourceFile->runBackEnd();

    // Keep track of the opt level the IR in the module was generated with
    OptLevel generatedOptLevel = cliOptions.optLevel;
    // Execute IR generator in normal or debug mode
    mainSourceFile->runIRGenerator();

    // Coverage counters are woven in by the optimizer pipeline, which the IR-check loop below only runs for opt levels
    // that have a reference file to compare against. Most test cases only check execution output and have no IR
    // reference at all, so without this, their linked binary would never actually carry the GCOV counters. Force one
    // optimizer run at the default opt level in that case.
    if (cliOptions.instrumentation.codeCoverage) {
      const bool anyOptIrRefExists = std::ranges::any_of(
          REF_NAME_OPT_IR, [&](const char *const ref) { return TestUtil::doesRefExist(testCase.testPath / ref); });
      if (!anyOptIrRefExists)
        mainSourceFile->runDefaultIROptimizer();
    }

    // Check IR code
    for (uint8_t i = 0; i <= 5; i++) {
      TestUtil::checkRefMatch(
          testCase.testPath / REF_NAME_OPT_IR[i],
          [&] {
            cliOptions.optLevel = static_cast<OptLevel>(i);

            // The IR generator emits parts of the IR depending on the selected opt level, so the IR has to be
            // generated from scratch whenever we check the dump of an opt level other than the one the current module
            // was built for. Otherwise all dumps would show the IR for the opt level that was active when the IR
            // generator ran for the first time.
            if (cliOptions.optLevel != generatedOptLevel) {
              if (cliOptions.useLTO) {
                // The bitcode linker moves the module of every source file into the LTO module, so all of them have to
                // be re-generated. The LTO module itself is started from scratch to not link the same symbols twice.
                resourceManager.ltoModule = std::make_unique<llvm::Module>(LTO_FILE_NAME, resourceManager.ltoContext);
                std::unordered_set<const SourceFile *> visitedSet;
                reGenerateIRForLTO(mainSourceFile, visitedSet);
              } else {
                reGenerateIR(mainSourceFile);
              }
              generatedOptLevel = cliOptions.optLevel;
            }

            if (cliOptions.useLTO) {
              mainSourceFile->runPreLinkIROptimizer();
              mainSourceFile->runBitcodeLinker();
              mainSourceFile->runPostLinkIROptimizer();
            } else {
              mainSourceFile->runDefaultIROptimizer();
            }

            return mainSourceFile->compilerOutput.irOptString;
          },
          [&](std::string &expectedOutput, std::string &actualOutput) {
            if (cliOptions.instrumentation.emitsDebugInfo()) {
              // Remove the lines, containing paths on the local file system
              TestUtil::eraseLinesBySubstring(expectedOutput, " = !DIFile(filename:");
              TestUtil::eraseLinesBySubstring(actualOutput, " = !DIFile(filename:");
            }
          },
          true);
    }

    // Link the bitcode if not happened yet
    if (cliOptions.useLTO && cliOptions.optLevel == OptLevel::O0)
      mainSourceFile->runBitcodeLinker();

    // Check assembly code (only when not running test on GitHub Actions)
    bool objectFilesEmitted = false;
    // GCOV_EXCL_START
    if (!testDriverCliOptions.isGitHubActions) {
      TestUtil::checkRefMatch(testCase.testPath / REF_NAME_ASM, [&] {
        mainSourceFile->runObjectEmitter();
        objectFilesEmitted = true;

        return mainSourceFile->compilerOutput.asmString;
      });
    }
    // GCOV_EXCL_STOP

    // Check warnings
    mainSourceFile->collectAndPrintWarnings();
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_WARNING_OUTPUT, [&] {
      std::stringstream actualWarningString;
      for (const CompilerWarning &warning : mainSourceFile->compilerOutput.warnings)
        actualWarningString << warning.warningMessage << "\n";
      return actualWarningString.str();
    });

    // Do linking and conclude compilation
    const bool needsNormalRunForOutput = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXECUTION_OUTPUT);
    const bool needsNormalRunForExitCode = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXIT_CODE);
    const bool needsDebuggerRun = TestUtil::doesRefExist(testCase.testPath / REF_NAME_GDB_OUTPUT);
    if (needsNormalRunForOutput || needsNormalRunForExitCode || needsDebuggerRun) {
      // Emit main source file object if not done already
      if (!objectFilesEmitted)
        mainSourceFile->runObjectEmitter();

      // Conclude the compilation
      mainSourceFile->concludeCompilation();

      // Prepare linker
      resourceManager.linker.outputPath = executablePath;

      // Prepare and run linker
      resourceManager.linker.prepare();
      resourceManager.linker.run();
      resourceManager.linker.cleanup();
    }

    // Check type registry output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_TYPE_REGISTRY, [&] { return TypeRegistry::dump(); });

    // Check cache stats output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_CACHE_STATS, [&] {
      std::stringstream cacheStats;
      cacheStats << FunctionManager::dumpLookupCacheStatistics() << std::endl;
      cacheStats << StructManager::dumpLookupCacheStatistics() << std::endl;
      cacheStats << InterfaceManager::dumpLookupCacheStatistics() << std::endl;
      return cacheStats.str();
    });

    const bool checkExecutionOutput = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXECUTION_OUTPUT);
    const bool checkExecutionExitCode = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXIT_CODE);
    if (checkExecutionOutput || checkExecutionExitCode) {
      const std::filesystem::path cliFlagsFile = testCase.testPath / INPUT_NAME_CLI_FLAGS;
      // Execute binary
      std::stringstream cmd;
      if (testDriverCliOptions.enableLeakDetection)
        cmd << "valgrind -q --leak-check=full --suppressions=../../valgrind.supp --num-callers=100 --error-exitcode=1 ";
      cmd << executablePath.string();
      if (exists(cliFlagsFile))
        cmd << " " << TestUtil::getFileContentLinesVector(cliFlagsFile).at(0);
      const auto [output, exitCode] = SystemUtil::exec(cmd.str(), checkExecutionOutput);

      // Check if the execution output matches the expected output
      const auto getActualOutput = [&] { return output; };
      TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXECUTION_OUTPUT, getActualOutput);

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
      // Check if the exit code matches the expected one
      // If no exit code ref file exists, check against 0
      const auto getActualExitCode = [&] { return std::to_string(exitCode); };
      const bool refExists = TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXIT_CODE, getActualExitCode);
      if (!refExists) {
        EXPECT_EQ(0, exitCode) << "Program exited with non-zero exit code";
      }
#endif
    }

    // Check if the debugger output matches the expected output
    // GCOV_EXCL_START
    if (!testDriverCliOptions.isGitHubActions) { // GDB tests are currently not support on GH actions
      TestUtil::checkRefMatch(
          testCase.testPath / REF_NAME_GDB_OUTPUT,
          [&] {
            // Execute debugger script
            std::filesystem::path gdbScriptPath = testCase.testPath / CTL_DEBUG_SCRIPT;
            EXPECT_TRUE(std::filesystem::exists(gdbScriptPath)) << "Debug output requested, but debug script not found";
            gdbScriptPath.make_preferred();
            // -nx: ignore any local/system .gdbinit so a developer's own GDB setup (e.g. globally installed
            // pretty printers) cannot change test output; tests that want the pretty printers source them
            // explicitly from within their debug script instead.
            const std::string cmd = "gdb -nx -x " + gdbScriptPath.string() + " " + executablePath.string();
            const auto [output, exitCode] = SystemUtil::exec(cmd);

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
            EXPECT_EQ(0, exitCode) << "GDB exited with non-zero exit code when running debug script";
#endif

            return output;
          },
          [&](std::string &expectedOutput, std::string &actualOutput) {
            // Do not compare against the GDB header
            TestUtil::eraseGDBHeader(expectedOutput);
            TestUtil::eraseGDBHeader(actualOutput);
          });
    }
    // GCOV_EXCL_STOP
  } catch (LexerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (ParserError &error) {
    TestUtil::handleError(testCase, error);
  } catch (SemanticError &error) {
    TestUtil::handleError(testCase, error);
  } catch (CompilerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (LinkerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (std::exception &error) {         // GCOV_EXCL_LINE
    TestUtil::handleError(testCase, error); // GCOV_EXCL_LINE
  } // GCOV_EXCL_LINE

  SUCCEED();
}

/**
 * Runs a test case against the bootstrap compiler, built at the start of the test run. The bootstrap compiler is invoked
 * like the host compiler would be invoked by a user, since it cannot be driven stage by stage from here.
 *
 * The following reference outputs are checked: the serialized AST, the raised error, the warnings of the main source file,
 * the IR code, and the execution output and exit code of the compiled program. Beyond that, each test case checks that the
 * bootstrap compiler runs through all stages without crashing or raising an unexpected error.
 */
static void execBootstrapTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase))
    GTEST_SKIP();

  const std::filesystem::path mainSourceFilePath = testCase.testPath / REF_NAME_SOURCE;
  const std::filesystem::path artifactDir = TestUtil::prepareArtifactDir(testCase);
  const std::filesystem::path executablePath = TestUtil::getExecutablePath(artifactDir);
  const bool checkAST = TestUtil::doesRefExist(testCase.testPath / REF_NAME_SYNTAX_TREE);
  const bool checkExecutionOutput = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXECUTION_OUTPUT);
  const bool checkExecutionExitCode = TestUtil::doesRefExist(testCase.testPath / REF_NAME_EXIT_CODE);
  const bool needsExecutable = checkExecutionOutput || checkExecutionExitCode;

  // Assemble the command line, mirroring the one the test runner passes to the host compiler
  const auto buildArgs = [&](const std::filesystem::path &outputPath) {
    // Like the host test runner, bypass the compilation cache: the IR dumps would be empty for files restored from it
    std::vector<std::string> args = {"build", "--test-mode", "--ignore-cache"};
    TestUtil::parseTestArgs(mainSourceFilePath, args);
    if (exists(testCase.testPath / CTL_RUN_BUILTIN_TESTS))
      args.emplace_back("--no-entry");
    args.emplace_back("--output");
    args.push_back(outputPath.string());
    return args;
  };
  std::vector<std::string> args = buildArgs(executablePath);
  // Like the host test runner, only link an executable if it gets executed afterwards. If an error is expected, keep the
  // executable output container, like the host test runner does: some errors (e.g. a missing main function) are only raised
  // for executables
  const bool expectsError = TestUtil::doesRefExist(testCase.testPath / REF_NAME_ERROR_OUTPUT);
  if (!needsExecutable && !expectsError) {
    args.emplace_back("--output-container");
    args.emplace_back("obj");
  }
  if (checkAST)
    args.emplace_back("--dump-ast");
  args.push_back(mainSourceFilePath.string());

  // Run the bootstrap compiler
  const auto [output, exitCode] = SystemUtil::exec(testDriverCliOptions.bootstrapCompilerPath, args, true);
  if (testDriverCliOptions.isVerbose)                      // GCOV_EXCL_LINE
    std::cout << "Bootstrap compiler output:\n" << output; // GCOV_EXCL_LINE

  // Check if the bootstrap compiler raised an error
  const std::filesystem::path errorRefPath = testCase.testPath / REF_NAME_ERROR_OUTPUT;
  if (const std::optional<std::string> errorMessage = BootstrapUtil::extractErrorMessage(output)) {
    if (!TestUtil::doesRefExist(errorRefPath))
      FAIL() << "Expected no error, but got: " << *errorMessage;
    TestUtil::checkRefMatch(errorRefPath, [&] { return *errorMessage; });
    return;
  }
  if (exitCode != 0)
    FAIL() << "Bootstrap compiler exited with code " << exitCode << ":\n" << output;

  // Check AST
  TestUtil::checkRefMatch(testCase.testPath / REF_NAME_SYNTAX_TREE, [&] {
    const std::optional<std::string> astString = BootstrapUtil::extractSerializedAST(output);
    EXPECT_TRUE(astString.has_value()) << "Bootstrap compiler did not dump the AST:\n" << output;
    return astString.value_or("");
  });

  // Fail if an error was expected
  if (TestUtil::doesRefExist(errorRefPath))
    FAIL() << "Expected error, but got no error";

  // Check warnings
  TestUtil::checkRefMatch(testCase.testPath / REF_NAME_WARNING_OUTPUT, [&] { return BootstrapUtil::extractWarnings(output); });

  // Check IR code. The host checks the IR after running the optimizer pipeline of each opt level, for which a reference exists.
  // The bootstrap compiler dumps the optimized IR of every source file into the output dir, so it is compiled once per opt level.
  for (uint8_t i = 0; i <= 5; i++) {
    TestUtil::checkRefMatch(
        testCase.testPath / REF_NAME_OPT_IR[i],
        [&] {
          const std::filesystem::path irArtifactDir = artifactDir / ("ir-O" + std::to_string(i));
          std::filesystem::create_directories(irArtifactDir);
          std::vector<std::string> irArgs = buildArgs(irArtifactDir / "object.o");
          irArgs.emplace_back("-O" + std::string(1, BOOTSTRAP_OPT_LEVEL_NAMES[i]));
          irArgs.emplace_back("--output-container");
          irArgs.emplace_back("obj");
          irArgs.emplace_back("--dump-ir");
          irArgs.emplace_back("--dump-to-files");
          irArgs.push_back(mainSourceFilePath.string());
          const auto [irOutput, irExitCode] = SystemUtil::exec(testDriverCliOptions.bootstrapCompilerPath, irArgs, true);
          EXPECT_EQ(0, irExitCode) << "Bootstrap compiler exited with code " << irExitCode << ":\n" << irOutput;
          // With LTO, the bootstrap compiler dumps the IR of the LTO module after the post-link optimization
          const bool useLTO = std::ranges::find(irArgs, "-lto") != irArgs.end();
          const std::string irDumpName =
              useLTO ? "source-ir-code-lto-post-link.ll" : "source-ir-code-O" + std::to_string(i) + ".ll";
          const std::filesystem::path irDumpPath = irArtifactDir / irDumpName;
          if (!exists(irDumpPath))
            return std::string();
          return FileUtil::getFileContent(irDumpPath);
        },
        [&](std::string &expectedOutput, std::string &actualOutput) {
          // The LLVM C API, which the bootstrap compiler uses, cannot mark global values as dso_local
          BootstrapUtil::eraseDSOLocalMarkers(expectedOutput);
          BootstrapUtil::eraseDSOLocalMarkers(actualOutput);
          // The bootstrap compiler names itself differently in the producer string
          BootstrapUtil::normalizeProducerString(actualOutput);
        },
        true);
  }

  // Check execution output and exit code
  if (needsExecutable) {
    // Execute binary
    std::stringstream cmd;
    cmd << executablePath.string();
    const std::filesystem::path cliFlagsFile = testCase.testPath / INPUT_NAME_CLI_FLAGS;
    if (exists(cliFlagsFile))
      cmd << " " << TestUtil::getFileContentLinesVector(cliFlagsFile).at(0);
    const auto [programOutput, programExitCode] = SystemUtil::exec(cmd.str(), checkExecutionOutput);

    // Check if the execution output matches the expected output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXECUTION_OUTPUT, [&] { return programOutput; });

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
    // Check if the exit code matches the expected one. If no exit code ref file exists, check against 0
    const bool refExists = TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXIT_CODE,
                                                   [&] { return std::to_string(programExitCode); });
    if (!refExists) {
      EXPECT_EQ(0, programExitCode) << "Program exited with non-zero exit code";
    }
#endif
  }

  SUCCEED();
}

/**
 * Runs a test case against the host compiler or, in bootstrap mode, against the bootstrap compiler
 */
static void runTestCase(const TestCase &testCase) {
  if (testDriverCliOptions.bootstrapMode)
    execBootstrapTestCase(testCase);
  else
    execTestCase(testCase);
}

namespace {

/**
 * Runs a lint test case against the bootstrap compiler: invokes its `lint` subcommand and compares the emitted
 * findings against lint.out. The bootstrap compiler colorizes its findings, so the captured output is
 * de-colorized before being matched against the plain-text reference.
 */
void execBootstrapLinterTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase))
    GTEST_SKIP();

  const std::filesystem::path mainSourceFilePath = testCase.testPath / REF_NAME_SOURCE;

  // Assemble the command line
  const std::vector<std::string> args = {"lint", mainSourceFilePath.string()};

  // Run the bootstrap compiler
  const auto [output, exitCode] = SystemUtil::exec(testDriverCliOptions.bootstrapCompilerPath, args, true);
  if (testDriverCliOptions.isVerbose)                      // GCOV_EXCL_LINE
    std::cout << "Bootstrap compiler output:\n" << output; // GCOV_EXCL_LINE

  // Check if the bootstrap compiler raised an error
  const std::filesystem::path errorRefPath = testCase.testPath / REF_NAME_ERROR_OUTPUT;
  if (const std::optional<std::string> errorMessage = BootstrapUtil::extractErrorMessage(output)) {
    if (!TestUtil::doesRefExist(errorRefPath))
      FAIL() << "Expected no error, but got: " << *errorMessage;
    TestUtil::checkRefMatch(errorRefPath, [&] { return *errorMessage; });
    return;
  }
  if (exitCode != 0)
    FAIL() << "Bootstrap compiler exited with code " << exitCode << ":\n" << output;

  // Fail if an error was expected
  if (TestUtil::doesRefExist(errorRefPath))
    FAIL() << "Expected error, but got no error";

  // Check lint findings against the reference (de-colorized)
  TestUtil::checkRefMatch(testCase.testPath / REF_NAME_LINT_OUTPUT, [&] { return BootstrapUtil::stripAnsiCodes(output); });

  SUCCEED();
}

} // namespace

class CommonTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(CommonTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, CommonTests, ::testing::ValuesIn(TestUtil::collectTestCases("common", false)),
                         TestUtil::NameResolver());

class LexerTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(LexerTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, LexerTests, ::testing::ValuesIn(TestUtil::collectTestCases("lexer", false)), TestUtil::NameResolver());

class ParserTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(ParserTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ParserTests, ::testing::ValuesIn(TestUtil::collectTestCases("parser", false)),
                         TestUtil::NameResolver());

class SymbolTableBuilderTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(SymbolTableBuilderTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, SymbolTableBuilderTests, ::testing::ValuesIn(TestUtil::collectTestCases("symboltablebuilder", true)),
                         TestUtil::NameResolver());

class TypeCheckerTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(TypeCheckerTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, TypeCheckerTests, ::testing::ValuesIn(TestUtil::collectTestCases("typechecker", true)),
                         TestUtil::NameResolver());

class IRGeneratorTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(IRGeneratorTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, IRGeneratorTests, ::testing::ValuesIn(TestUtil::collectTestCases("irgenerator", true)),
                         TestUtil::NameResolver());

class StdTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(StdTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, StdTests, ::testing::ValuesIn(TestUtil::collectTestCases("std", true)), TestUtil::NameResolver());

class BenchmarkTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(BenchmarkTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BenchmarkTests, ::testing::ValuesIn(TestUtil::collectTestCases("benchmark", false)),
                         TestUtil::NameResolver());

class ExampleTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(ExampleTests, ) { runTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ExampleTests, ::testing::ValuesIn(TestUtil::collectTestCases("examples", false)),
                         TestUtil::NameResolver());

class LinterTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(LinterTests, ) {
  if (!testDriverCliOptions.bootstrapMode)
    GTEST_SKIP() << "Linter tests run against the bootstrap compiler only";
  execBootstrapLinterTestCase(GetParam());
}
INSTANTIATE_TEST_SUITE_P(, LinterTests, ::testing::ValuesIn(TestUtil::collectTestCases("linter", false)),
                         TestUtil::NameResolver());

} // namespace spice::testing