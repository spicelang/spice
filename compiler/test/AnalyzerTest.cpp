// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <gtest/gtest.h>
#include <analyzer/AnalyzerVisitor.h>

struct AnalyzerParams {
    const std::string testDirectory;
    const std::string errorMessage; // Empty error message for testing for no error
};

const AnalyzerParams ANALYZER_TEST_PARAMETERS[] = {
        {
            "",
            ""
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {};

TEST_P(AnalyzerTests, TestAnalyzerWithValidAndInvalidTestFiles) {
    AnalyzerParams param = GetParam();
}

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTests,
        AnalyzerTests,
        ::testing::ValuesIn(ANALYZER_TEST_PARAMETERS)
);