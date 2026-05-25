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
  app.add_flag<bool>("--update-refs", testDriverCliOptions.updateRefs, "Update test reference files");
  // --run-benchmarks
  app.add_flag<bool>("--run-benchmarks", testDriverCliOptions.runBenchmarks, "Also run benchmarks and check baseline values");
  // --leak-detection
  app.add_flag<bool>("--leak-detection", testDriverCliOptions.enableLeakDetection, "Use Valgrind on tests to detect memory leaks");
  // --is-github-actions
  app.add_flag<bool>("--is-github-actions", testDriverCliOptions.isGitHubActions, "Skip tests that are not supported to run on GitHub Actions");
  // --skip-sanitizer-tests
  app.add_flag<bool>("--skip-sanitizer-tests", testDriverCliOptions.skipSanitizerTests, "Skip tests that exercise Spice language sanitizers (e.g. ASAN, TSAN, MSAN, TYSAN)");
  // --verbose
  app.add_flag<bool>("--verbose", testDriverCliOptions.isVerbose, "Print debug output for the test runner");
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
    return 0;
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
}

} // namespace spice::testing

// GCOV_EXCL_STOP