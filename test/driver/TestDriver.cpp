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
  // --leak-detection
  app.add_flag<bool>("--leak-detection", testDriverCliOptions.enableLeakDetection,
                     "Use Valgrind on tests to detect memory leaks");
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
  // --bootstrap
  CLI::Option *bootstrapOpt =
      app.add_flag<bool>("--bootstrap", testDriverCliOptions.bootstrapMode,
                         "Build the bootstrap compiler with the host compiler first, then run the test cases against it");
  // --bootstrap-compiler
  CLI::Option *bootstrapCompilerOpt = app.add_option<std::string>(
      "--bootstrap-compiler", testDriverCliOptions.bootstrapCompilerPath,
      "Run the test cases against the given, already built bootstrap compiler executable (implies --bootstrap)");
  // The reference files hold the output of the host compiler, so they must never be overwritten with bootstrap output. The
  // coverage mode instruments the programs compiled by the host compiler, which the bootstrap mode does not compile at all.
  bootstrapOpt->excludes(updateRefsOpt)->excludes(coverageOpt);
  bootstrapCompilerOpt->excludes(updateRefsOpt)->excludes(coverageOpt);
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
    if (!testDriverCliOptions.bootstrapCompilerPath.empty())
      testDriverCliOptions.bootstrapMode = true;
    return 0;
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
}

} // namespace spice::testing

// GCOV_EXCL_STOP