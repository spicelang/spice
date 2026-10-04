// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <cstdlib>
#include <fstream>

#include <gtest/gtest.h>

#include <util/SystemUtil.h>

// LCOV_EXCL_START

namespace spice::testing {

using namespace spice::compiler;

TEST(SystemUtilTest, IsCommandAvailable) {
  ASSERT_TRUE(SystemUtil::isCommandAvailable("dot"));
  ASSERT_FALSE(SystemUtil::isCommandAvailable("non-existing-command"));
}

TEST(SystemUtilTest, IsGraphvizInstalled) {
  ASSERT_TRUE(SystemUtil::isGraphvizInstalled());
}

TEST(SystemUtilTest, ExpandLinkerFlagPlain) {
  const std::vector<std::string> result = SystemUtil::expandLinkerFlag("-lm");
  ASSERT_EQ(result, std::vector<std::string>({"-lm"}));
}

TEST(SystemUtilTest, ExpandLinkerFlagSingleEnvVar) {
#if OS_WINDOWS
  _putenv_s("SPICE_TEST_LINKER_FLAG_VAR", "/opt/llvm/lib");
  const std::vector<std::string> result = SystemUtil::expandLinkerFlag("-L%SPICE_TEST_LINKER_FLAG_VAR%");
#else
  setenv("SPICE_TEST_LINKER_FLAG_VAR", "/opt/llvm/lib", 1);
  const std::vector<std::string> result = SystemUtil::expandLinkerFlag("-L$SPICE_TEST_LINKER_FLAG_VAR");
#endif
  ASSERT_EQ(result, std::vector<std::string>({"-L/opt/llvm/lib"}));
}

// An env var that expands to several whitespace-separated flags (e.g. LLVM_INCLUDE_DIRS, populated by CMake/CI with
// one '-I<dir>' per LLVM include directory) must be split into one argv entry per flag, since there is no shell here
// to word-split the expansion.
TEST(SystemUtilTest, ExpandLinkerFlagMultiValueEnvVar) {
#if OS_WINDOWS
  _putenv_s("SPICE_TEST_LINKER_FLAG_MULTI", "-I/opt/llvm/include -I/opt/llvm/build/include");
  const std::vector<std::string> result = SystemUtil::expandLinkerFlag("%SPICE_TEST_LINKER_FLAG_MULTI%");
#else
  setenv("SPICE_TEST_LINKER_FLAG_MULTI", "-I/opt/llvm/include -I/opt/llvm/build/include", 1);
  const std::vector<std::string> result = SystemUtil::expandLinkerFlag("$SPICE_TEST_LINKER_FLAG_MULTI");
#endif
  ASSERT_EQ(result, std::vector<std::string>({"-I/opt/llvm/include", "-I/opt/llvm/build/include"}));
}

TEST(SystemUtilTest, GetStdTPDEFlagsWithoutShippedTPDE) {
  // A std without the TPDE libraries and headers (e.g. a source checkout) yields no flags
  const std::filesystem::path stdDir = std::filesystem::temp_directory_path() / "spice-test-std-without-tpde";
  std::filesystem::remove_all(stdDir);
  std::filesystem::create_directories(stdDir / "bindings" / "tpde");
  ASSERT_EQ(SystemUtil::getStdTPDEFlags(stdDir), "");
  std::filesystem::remove_all(stdDir);
}

TEST(SystemUtilTest, GetStdTPDEFlagsWithShippedTPDE) {
  // A std with the TPDE libraries and headers of a release package yields the include flag and the libraries, that exist,
  // in link order and inside a linker group
  const std::filesystem::path stdDir = std::filesystem::temp_directory_path() / "spice-test-std-with-tpde";
  std::filesystem::remove_all(stdDir);
  const std::filesystem::path tpdeDir = stdDir / "bindings" / "tpde";
  std::filesystem::create_directories(tpdeDir / "include" / "tpde-llvm");
  std::filesystem::create_directories(tpdeDir / "lib");
  for (const std::filesystem::path &file : {tpdeDir / "include" / "tpde-llvm" / "LLVMCompiler.hpp", tpdeDir / "lib" / "libtpde.a",
                                            tpdeDir / "lib" / "libtpde_llvm.a", tpdeDir / "lib" / "libfadec.a"})
    std::ofstream(file).put('\n');
  const std::string expected = "-I" + (tpdeDir / "include").string() + " -Wl,--start-group " +
                               (tpdeDir / "lib" / "libtpde_llvm.a").string() + " " + (tpdeDir / "lib" / "libtpde.a").string() +
                               " " + (tpdeDir / "lib" / "libfadec.a").string() + " -Wl,--end-group";
  ASSERT_EQ(SystemUtil::getStdTPDEFlags(stdDir), expected);
  // Without the TPDE LLVM library, the bindings can't use TPDE at all
  std::filesystem::remove(tpdeDir / "lib" / "libtpde_llvm.a");
  ASSERT_EQ(SystemUtil::getStdTPDEFlags(stdDir), "");
  std::filesystem::remove_all(stdDir);
}

} // namespace spice::testing

// LCOV_EXCL_STOP