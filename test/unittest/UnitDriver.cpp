// Copyright (c) 2021-2026 ChilliBits. All rights reserved.
// LCOV_EXCL_START

#include <gtest/gtest.h>

#include <driver/Driver.h>
#include <exception/CliError.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(DriverTest, BuildSubcommandMinimal) {
  const char *argv[] = {"spice", "build", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_EQ(BuildMode::DEBUG, cliOptions.buildMode);
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
}

TEST(DriverTest, BuildSubcommandComplex) {
  const char *argv[] = {
      "spice",
      "b",
      "-d",
      "-ir",
      "-g",
      "-Os",
      "-m",
      "release",
      "-lto",
      "--sanitizer=address",
      "../../media/test-project/test.spice",
  };
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::Os, cliOptions.optLevel);        // -Os
  ASSERT_EQ(BuildMode::RELEASE, cliOptions.buildMode); // -m release
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
  ASSERT_TRUE(cliOptions.instrumentation.generateDebugInfo);           // -g
  ASSERT_EQ(Sanitizer::ADDRESS, cliOptions.instrumentation.sanitizer); // --sanitizer=address
  ASSERT_TRUE(cliOptions.useLTO);                                      // -lto
  ASSERT_TRUE(cliOptions.printDebugOutput);                            // -d
  ASSERT_TRUE(cliOptions.dump.dumpIR);                                 // -ir
  ASSERT_TRUE(cliOptions.useLifetimeMarkers);                          // implicitly due to enabled address sanitizer
}

TEST(DriverTest, RunSubcommandMinimal) {
  const char *argv[] = {"spice", "run", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
}

TEST(DriverTest, RunSubcommandComplex) {
  const char *argv[] = {"spice", "r", "-O2", "-j", "8", "-ast", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O2, cliOptions.optLevel); // -O2
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
  ASSERT_EQ(8, cliOptions.compileJobCount); // -j 8
  ASSERT_TRUE(cliOptions.dump.dumpAST);     // -ast
}

TEST(DriverTest, TestSubcommandMinimal) {
  const char *argv[] = {"spice", "test", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_EQ(BuildMode::TEST, cliOptions.buildMode); // -m test
  ASSERT_TRUE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_TRUE(cliOptions.noEntryFct);
}

TEST(DriverTest, TestSubcommandComplex) {
  const char *argv[] = {"spice", "t", "-s", "-cst", "--sanitizer=thread", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_TRUE(cliOptions.generateTestMain);
  ASSERT_TRUE(cliOptions.noEntryFct);
  ASSERT_TRUE(cliOptions.dump.dumpCST);                               // -cst
  ASSERT_TRUE(cliOptions.dump.dumpAssembly);                          // -s
  ASSERT_EQ(Sanitizer::THREAD, cliOptions.instrumentation.sanitizer); // --sanitizer=thread
}

TEST(DriverTest, InstallSubcommandMinimal) {
  const char *argv[] = {"spice", "install", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_TRUE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
}

TEST(DriverTest, UninstallSubcommandMinimal) {
  const char *argv[] = {"spice", "uninstall", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_FALSE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_TRUE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, cliOptions.optLevel);
  ASSERT_FALSE(cliOptions.generateTestMain);
  ASSERT_FALSE(cliOptions.testMode);
  ASSERT_FALSE(cliOptions.noEntryFct);
}

TEST(DriverTest, MemorySanitizerOnlyLinux) {
  const char *argv[] = {"spice", "build", "--sanitizer=memory", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);

  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
#if OS_LINUX
  driver.enrich();
  ASSERT_EQ(Sanitizer::MEMORY, cliOptions.instrumentation.sanitizer);
  ASSERT_TRUE(cliOptions.useLifetimeMarkers); // implicitly due to enabled address sanitizer
#else
  try {
    driver.enrich();
    FAIL();
  } catch (CliError &error) {
    auto errorMsg = "[Error|CLI] Feature is not supported for this target: Memory sanitizer is only supported for Linux targets";
    ASSERT_STREQ(errorMsg, error.what());
  }
#endif
}

using DriverInvalidEnumTestParam = std::pair<const char *, const char *>;
class DriverTest : public ::testing::TestWithParam<DriverInvalidEnumTestParam> {};

TEST_P(DriverTest, LengthGreaterThanZero) {
  const auto &[arg, errorMessage] = GetParam();
  const char *argv[] = {"spice", "build", arg, "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  CliOptions cliOptions;
  Driver driver(cliOptions, true);
  try {
    driver.parse(argc, argv);
    FAIL();
  } catch (CliError &error) {
    ASSERT_STREQ(errorMessage, error.what());
  }
}

const auto INVALID_ENUM_TEST_VALUES = ::testing::Values(
    DriverInvalidEnumTestParam{
        "--build-mode=unknown",
        "[Error|CLI] Invalid build mode: unknown",
    },
    DriverInvalidEnumTestParam{
        "--sanitizer=unknown",
        "[Error|CLI] Invalid sanitizer: unknown",
    });
INSTANTIATE_TEST_SUITE_P(DriverTest, DriverTest, INVALID_ENUM_TEST_VALUES);

} // namespace spice::testing

// LCOV_EXCL_STOP