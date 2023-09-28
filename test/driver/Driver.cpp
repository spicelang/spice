// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Driver.h"

// GCOV_EXCL_START

void Driver::createInterface() {
  // Allow positional args
  app.positionals_at_end();
  app.require_subcommand(0);
  app.allow_extras(false);
  app.footer("(c) Marc Auberer 2021-2023");

  // Add version flag
  std::string versionName = std::string(SPICE_VERSION);
  std::string builtBy = std::string(SPICE_BUILT_BY);
  std::string versionString = "Spice version " + versionName + "\nbuilt by: " + builtBy + "\n\n(c) Marc Auberer 2021-2023";
  app.set_version_flag("--version,-v", versionString);
}

void Driver::addOptions(bool &updateRefs, bool &runBenchmarks, bool &enableLeakDetection, bool &skipNonGitHubTests) {
  // --update-refs
  app.add_flag<bool>("--update-refs,-u", updateRefs, "Update test reference files");
  // --run-benchmarks
  app.add_flag<bool>("--run-benchmarks,-b", runBenchmarks, "Also run benchmarks and check baseline values");
  // --leak-detection
  app.add_flag<bool>("--leak-detection,-l", enableLeakDetection, "Use Valgrind on tests to detect memory leaks");
  // --skip-github-tests
  app.add_flag<bool>("--skip-github-tests,-gh", skipNonGitHubTests, "Skip non-working tests on GitHub Actions");
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

// GCOV_EXCL_STOP