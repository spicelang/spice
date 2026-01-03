// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <util/SystemUtil.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(SystemUtilTest, IsCommandAvailable) {
  ASSERT_TRUE(SystemUtil::isCommandAvailable("dot"));
  ASSERT_FALSE(SystemUtil::isCommandAvailable("non-existing-command"));
}

TEST(SystemUtilTest, IsGraphvizInstalled) {
  ASSERT_TRUE(SystemUtil::isGraphvizInstalled());
}

} // namespace spice::testing