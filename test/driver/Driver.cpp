// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "Driver.h"

#include "util/CommonUtil.h"

// GCOV_EXCL_START
namespace spice::testing {

TestDriverCliOptions testDriverCliOptions;

void Driver::createInterface() {
  // Allow positional args
  app.allow_non_standard_option_names();
  app.positionals_at_end();
  app.require_subcommand(0);
  app.allow_extras(false);
  app.footer("(c) Marc Auberer 2021-2025");

  // Add version flag
  app.set_version_flag("--version,-v", compiler::CommonUtil::buildVersionInfo());
}

void Driver::addOptions() {
  // --update-refs
  app.add_flag<bool>("--update-refs,-u", testDriverCliOptions.updateRefs, "Update test reference files");
  // --run-benchmarks
  app.add_flag<bool>("--run-benchmarks,-b", testDriverCliOptions.runBenchmarks, "Also run benchmarks and check baseline values");
  // --leak-detection
  app.add_flag<bool>("--leak-detection,-l", testDriverCliOptions.enableLeakDetection, "Use Valgrind on tests to detect memory leaks");
  // --is-github-actions
  app.add_flag<bool>("--is-github-actions,-gh", testDriverCliOptions.isGitHubActions, "Skip tests that are not supported to run on GitHub Actions");
}

/**
 * Start the parsing process
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int Driver::parse(int argc, char **argv) {
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &parseError) {
    return app.exit(parseError);
  }
  return 0;
}

} // namespace spice::testing

// GCOV_EXCL_STOP