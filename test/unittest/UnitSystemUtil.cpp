// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <cstdlib>

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

} // namespace spice::testing

// LCOV_EXCL_STOP