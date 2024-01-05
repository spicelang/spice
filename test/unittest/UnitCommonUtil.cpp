// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <util/CommonUtil.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(CommonUtilTest, TestReplaceAll) {
  std::string test = "This is a test";
  CommonUtil::replaceAll(test, " ", "_");
  ASSERT_EQ("This_is_a_test", test);

  test = "This is a test";
  CommonUtil::replaceAll(test, "is", "was");
  ASSERT_EQ("Thwas was a test", test);

  test = "This is a test";
  CommonUtil::replaceAll(test, "is", "is");
  ASSERT_EQ("This is a test", test);

  test = "This is a test";
  CommonUtil::replaceAll(test, "is", "is");
  ASSERT_EQ("This is a test", test);

  test = "This is a test";
  CommonUtil::replaceAll(test, "is", "is");
  ASSERT_EQ("This is a test", test);

  test = "This is a test";
  CommonUtil::replaceAll(test, "is", "is");
  ASSERT_EQ("This is a test", test);
}

TEST(CommonUtilTest, TestGetLastFragment) {
  ASSERT_EQ("test", CommonUtil::getLastFragment("This is a test", " "));
  ASSERT_EQ("test", CommonUtil::getLastFragment("This_is_a_test", "_"));
  ASSERT_EQ("", CommonUtil::getLastFragment("This.is.a.", "."));
  ASSERT_EQ("This is a test", CommonUtil::getLastFragment("This is a test", "This is a test "));
}

TEST(CommonUtilTest, TestTrim) {
  ASSERT_EQ("test", CommonUtil::trim("  test  "));
  ASSERT_EQ("This is a test", CommonUtil::trim("This is a test  "));
  ASSERT_EQ("String with whitespaces only at the front", CommonUtil::trim("  String with whitespaces only at the front"));
  ASSERT_EQ("test", CommonUtil::trim("test"));
  ASSERT_EQ("  ", CommonUtil::trim("  "));
  ASSERT_EQ("", CommonUtil::trim(""));
}

TEST(CommonUtilTest, TestSplit) {
  ASSERT_EQ(std::vector<std::string>({"test", "test"}), CommonUtil::split("test test"));
  ASSERT_EQ(std::vector<std::string>({"This", "is", "a", "test"}), CommonUtil::split("This is a test"));
  ASSERT_EQ(std::vector<std::string>({"String", "with", "whitespaces", "at", "the", "front"}),
            CommonUtil::split("  String with whitespaces at the front"));
  ASSERT_EQ(std::vector<std::string>({"test"}), CommonUtil::split("test"));
  ASSERT_EQ(std::vector<std::string>({"", ""}), CommonUtil::split("  "));
  ASSERT_EQ(std::vector<std::string>({}), CommonUtil::split(""));
}

} // namespace spice::testing