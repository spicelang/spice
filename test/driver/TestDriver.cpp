// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TestDriver.h"

#include "util/CommonUtil.h"

// GCOV_EXCL_START
namespace spice::testing {

TestDriverCliOptions testDriverCliOptions;

void TestDriver::createInterface() {
  // Allow positional args
  app.allow_non_standard_option_names();
  app.positionals_at_end();
  app.require_subcommand(0);
  app.allow_extras(false);
  app.footer("(c) Marc Auberer 2021-2026");

  // Add version flag
  app.set_version_flag("--version,-v", compiler::CommonUtil::buildVersionInfo());
}

void TestDriver::addOptions() {
  // --update-refs
  CLI::Option *updateRefsOpt =
      app.add_flag<bool>("--update-refs", testDriverCliOptions.updateRefs, "Update test reference files");
  // --run-benchmarks
  app.add_flag<bool>("--run-benchmarks", testDriverCliOptions.runBenchmarks, "Also run benchmarks and check baseline values");
  // --asan
  CLI::Option *asanOpt =
      app.add_flag<bool>("--asan", testDriverCliOptions.enableAsan,
                         "Compile and run test programs with AddressSanitizer (incl. leak detection) enabled, skipping all "
                         "reference output comparisons except for the execution output and exit code (the sanitizer "
                         "instrumentation changes the generated code)");
  // --is-github-actions
  app.add_flag<bool>("--is-github-actions", testDriverCliOptions.isGitHubActions,
                     "Skip tests that are not supported to run on GitHub Actions");
  // --skip-sanitizer-tests
  app.add_flag<bool>("--skip-sanitizer-tests", testDriverCliOptions.skipSanitizerTests,
                     "Skip tests that exercise Spice language sanitizers (e.g. ASAN, TSAN, MSAN, TYSAN)");
  // --verbose
  app.add_flag<bool>("--verbose", testDriverCliOptions.isVerbose, "Print debug output for the test runner");
  // --coverage
  CLI::Option *coverageOpt =
      app.add_flag<bool>("--coverage", testDriverCliOptions.enableCoverage,
                         "Compile and run test programs with Spice code coverage instrumentation enabled, skipping all "
                         "reference output comparisons (debug info and coverage counters change the generated code)");
  // Coverage mode never compares against (or writes) reference files, so combining it with --update-refs would silently
  // overwrite tracked fixtures with coverage-instrumented, no-longer-comparable output. Reject the combination outright.
  coverageOpt->excludes(updateRefsOpt);
  // Same goes for the ASAN mode. It cannot be combined with the coverage mode either, since the coverage mode does not test
  // the sanitizers
  asanOpt->excludes(updateRefsOpt)->excludes(coverageOpt);
  // --bootstrap
  app.add_flag<bool>("--bootstrap", testDriverCliOptions.bootstrapMode,
                     "Build the bootstrap compiler with the host compiler first, then run the test cases against it");
  // --bootstrap-compiler
  CLI::Option *bootstrapCompilerOpt = app.add_option<std::string>(
      "--bootstrap-compiler", testDriverCliOptions.bootstrapCompilerPath,
      "Run the test cases against the given, already built bootstrap compiler executable (implies --bootstrap)");
  // --bootstrap-coverage
  CLI::Option *bootstrapCoverageOpt =
      app.add_flag<bool>("--bootstrap-coverage", testDriverCliOptions.bootstrapCoverage,
                         "Build the bootstrap compiler with Spice code coverage instrumentation enabled, then run the test "
                         "cases against it (implies --bootstrap)");
  // --bootstrap-asan
  CLI::Option *bootstrapAsanOpt =
      app.add_flag<bool>("--bootstrap-asan", testDriverCliOptions.bootstrapAsan,
                         "Build the bootstrap compiler with AddressSanitizer instrumentation enabled, then run the test cases "
                         "against it (implies --bootstrap). Fails every test case, for which the sanitizer reports an error");
  // --bootstrap-build-only
  CLI::Option *bootstrapBuildOnlyOpt =
      app.add_flag<bool>("--bootstrap-build-only", testDriverCliOptions.bootstrapBuildOnly,
                         "Only build the bootstrap compiler, without running any test cases (implies --bootstrap). Used to "
                         "build it once before running the test cases in parallel via --bootstrap-compiler");
  // In bootstrap mode, --update-refs only updates the bootstrap refs (e.g. ir-code-bootstrap.ll), so the reference files of
  // the host compiler are never overwritten with bootstrap output. In bootstrap mode, the coverage mode instruments the
  // programs compiled by the bootstrap compiler instead.
  // Instrumenting the bootstrap compiler requires building it, so it cannot be combined with a pre-built one. Its coverage
  // data would mix with the one of the instrumented test programs, so it cannot be combined with the coverage mode either
  bootstrapCoverageOpt->excludes(updateRefsOpt)->excludes(coverageOpt)->excludes(bootstrapCompilerOpt);
  // Instrumenting the bootstrap compiler with ASAN requires building it as well. Coverage and ASAN instrumentation of the
  // bootstrap compiler are separate builds, so they cannot be combined
  bootstrapAsanOpt->excludes(updateRefsOpt)->excludes(bootstrapCompilerOpt)->excludes(bootstrapCoverageOpt);
  // Building only makes no sense, if a pre-built bootstrap compiler is given
  bootstrapBuildOnlyOpt->excludes(updateRefsOpt)->excludes(coverageOpt)->excludes(bootstrapCompilerOpt);
}

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int TestDriver::parse(int argc, char **argv) {
  try {
    app.parse(argc, argv);
    if (!testDriverCliOptions.bootstrapCompilerPath.empty() || testDriverCliOptions.bootstrapCoverage ||
        testDriverCliOptions.bootstrapAsan || testDriverCliOptions.bootstrapBuildOnly)
      testDriverCliOptions.bootstrapMode = true;
    return 0;
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
}

} // namespace spice::testing

// GCOV_EXCL_STOP