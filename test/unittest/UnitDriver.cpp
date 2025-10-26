// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <driver/Driver.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(DriverTest, TestBuildSubcommandMinimal) {
  const char *argv[] = {"spice", "build", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_EQ(BuildMode::DEBUG, driver.cliOptions.buildMode);
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
}

TEST(DriverTest, TestBuildSubcommandComplex) {
  const char *argv[] = {"spice", "b", "-d", "-ir", "-g", "-Os", "-m", "release", "-lto", "--sanitizer=address", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::Os, driver.cliOptions.optLevel);        // -Os
  ASSERT_EQ(BuildMode::RELEASE, driver.cliOptions.buildMode); // -m release
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
  ASSERT_TRUE(driver.cliOptions.instrumentation.generateDebugInfo);   // -g
  ASSERT_EQ(Sanitizer::ADDRESS, driver.cliOptions.instrumentation.sanitizer);   // --sanitizer=address
  ASSERT_TRUE(driver.cliOptions.useLTO);              // -lto
  ASSERT_TRUE(driver.cliOptions.printDebugOutput);    // -d
  ASSERT_TRUE(driver.cliOptions.dump.dumpIR); // -ir
}

TEST(DriverTest, TestRunSubcommandMinimal) {
  const char *argv[] = {"spice", "run", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
}

TEST(DriverTest, TestRunSubcommandComplex) {
  const char *argv[] = {"spice", "r", "-O2", "-j", "8", "-ast", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O2, driver.cliOptions.optLevel); // -O2
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
  ASSERT_EQ(8, driver.cliOptions.compileJobCount);     // -j 8
  ASSERT_TRUE(driver.cliOptions.dump.dumpAST); // -ast
}

TEST(DriverTest, TestTestSubcommandMinimal) {
  const char *argv[] = {"spice", "test", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_EQ(BuildMode::TEST, driver.cliOptions.buildMode); // -m test
  ASSERT_TRUE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_TRUE(driver.cliOptions.noEntryFct);
}

TEST(DriverTest, TestTestSubcommandComplex) {
  const char *argv[] = {"spice", "t", "-s", "-cst", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_TRUE(driver.shouldExecute);
  ASSERT_TRUE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_TRUE(driver.cliOptions.generateTestMain);
  ASSERT_TRUE(driver.cliOptions.noEntryFct);
  ASSERT_TRUE(driver.cliOptions.dump.dumpCST);      // -cst
  ASSERT_TRUE(driver.cliOptions.dump.dumpAssembly); // -s
}

TEST(DriverTest, TestInstallSubcommandMinimal) {
  const char *argv[] = {"spice", "install", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_TRUE(driver.shouldCompile);
  ASSERT_TRUE(driver.shouldInstall);
  ASSERT_FALSE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
}

TEST(DriverTest, TestUninstallSubcommandMinimal) {
  const char *argv[] = {"spice", "uninstall", "../../media/test-project/test.spice"};
  static constexpr int argc = std::size(argv);
  Driver driver(true);
  driver.init();
  ASSERT_EQ(EXIT_SUCCESS, driver.parse(argc, argv));
  driver.enrich();

  ASSERT_FALSE(driver.shouldCompile);
  ASSERT_FALSE(driver.shouldInstall);
  ASSERT_TRUE(driver.shouldUninstall);
  ASSERT_FALSE(driver.shouldExecute);
  ASSERT_FALSE(driver.cliOptions.execute);
  ASSERT_EQ("../../media/test-project/test.spice", driver.cliOptions.mainSourceFile.relative_path().generic_string());
  ASSERT_EQ(OptLevel::O0, driver.cliOptions.optLevel);
  ASSERT_FALSE(driver.cliOptions.generateTestMain);
  ASSERT_FALSE(driver.cliOptions.testMode);
  ASSERT_FALSE(driver.cliOptions.noEntryFct);
}

} // namespace spice::testing
