// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <gtest/gtest.h>
#include <iostream>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include <analyzer/AnalyzerVisitor.h>

struct AnalyzerParams {
    const std::string testCaseName;
    const std::string errorMessage; // Empty error message for testing for no error
};

const AnalyzerParams ANALYZER_TEST_PARAMETERS[] = {
        // Failing tests
        { // 0
            "error-assignment-same-type",
            "Semantic error at 2:14: Wrong data type for operator: Cannot apply the assign operator to different data types"
        },
        { // 1
            "error-if-condition-bool",
            "Semantic error at 2:8: Condition must be bool: If condition must be of type bool"
        },
        { // 2
            "error-while-condition-bool",
            "Semantic error at 2:11: Condition must be bool: While loop condition must be of type bool"
        },
        { // 3
            "error-for-condition-bool",
            "Semantic error at 2:20: Condition must be bool: For loop condition must be of type bool"
        },
        { // 4
            "error-ternary-condition-bool",
            "Semantic error at 2:25: Wrong data type for operator: Condition operand in ternary must be a bool"
        },
        { // 5
            "error-ternary-types-match",
            "Semantic error at 2:25: Wrong data type for operator: True operand and false operand in ternary must be from same data type"
        },
        { // 6
            "error-variable-declared-before-referenced",
            "Semantic error at 2:26: Referenced undefined variable: Variable test was referenced before initialized"
        },
        { // 7
            "error-variable-declared-only-once",
            "Semantic error at 3:5: Multiple declarations of the same variable: The variable 'i' was declared more than once"
        },
        { // 8
            "error-variable-const-not-modified",
            "Semantic error - Cannot re-assign constant variable: Not re-assignable variable 'variable'"
        },
        { // 9
            "error-functions-defined-before-called",
            "Semantic error at 7:25: Referenced undefined function: Function/Procedure 'testFunction(int)' could not be found"
        },
        { // 10
            "error-cannot-call-variables",
            "Semantic error at 3:22: Referenced undefined function: Function/Procedure 'variable()' could not be found"
        },
        { // 11
            "error-function-params-match-declaration",
            "Semantic error at 6:5: Referenced undefined function: Function/Procedure 'testFunction()' could not be found"
        },
        { // 12
            "error-function-param-types-match-declaration",
            "Semantic error at 6:5: Referenced undefined function: Function/Procedure 'testFunction(string)' could not be found"
        },
        { // 13
            "error-return-only-inside-function",
            "Semantic error at 1:1: Return statement outside function: Cannot assign return statement to a function"
        },
        { // 14
            "error-return-type-matches-def",
            "Semantic error at 2:12: Wrong data type for operator: Passed wrong data type to return statement"
        },
        { // 15
            "error-logical-operators-are-booleans",
            "Semantic error at 2:9: Wrong data type for operator: Can only apply logical or to booleans"
        },
        { // 16
            "error-bitwise-operators-are-booleans-or-integers",
            "Semantic error at 2:22: Wrong data type for operator: Can only apply bitwise or to booleans and integers"
        },
        { // 17
            "error-equality-operators-some-combinations",
            "Semantic error at 2:9: Wrong data type for operator: Can only compare some type combinations with an equality operator"
        },
        { // 18
            "error-relational-operators-are-doubles-or-integers",
            "Semantic error at 3:12: Wrong data type for operator: Can only compare doubles or ints with one another with a relational operator"
        },
        { // 19
            "error-additive-operators-some-combinations",
            "Semantic error at 2:29: Wrong data type for operator: Incompatible operands string and string for additive operator"
        },
        { // 20
            "error-multiplicative-operators-some-combinations",
            "Semantic error at 2:29: Wrong data type for operator: Incompatible operands string and string for multiplicative operator"
        },
        { // 21
            "error-prefix-unary-only-integer-identifiers",
            "Semantic error at 2:7: Wrong data type for operator: Prefix '++' or '--' only can be applied to an identifier of type integer"
        },
        { // 22
            "error-postfix-unary-only-integer-identifiers",
            "Semantic error at 2:5: Wrong data type for operator: Postfix '++' or '--' only can be applied to an identifier of type integer"
        },
        { // 23
            "error-must-contain-main-function",
            "Semantic error at 1:1: Spice programs must contain a main function: No main function found."
        },
        { // 24
            "error-function-arg-decl-type-dyn",
            "Semantic error at 1:51: Parameter type dyn not valid in function/procedure definition without default value: Type of parameter 'arg2' is invalid"
        },
        { // 25
            "error-dyn-return-types-not-matching",
            "Semantic error at 3:12: Wrong data type for operator: Passed wrong data type to return statement"
        },
        { // 26
            "error-printf-type-incompatibility",
            "Semantic error at 4:36: Types of printf call not matching: Template string expects an int or a bool here"
        },
        { // 27
            "error-break-count-valid",
            "Semantic error at 7:23: Invalid number of break calls: Break count must be >= 1: -10"
        },
        { // 28
            "error-break-count-not-too-high",
            "Semantic error at 7:23: Invalid number of break calls: We only can break 2 time(s) here"
        },
        { // 29
            "error-continue-count-valid",
            "Semantic error at 7:26: Invalid number of continue calls: Continue count must be >= 1: -10"
        },
        { // 30
            "error-continue-count-not-too-high",
            "Semantic error at 7:26: Invalid number of continue calls: We only can continue 2 time(s) here"
        },
        // Successful tests
        { // 31
            "success-fibonacci",
            ""
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {};

TEST_P(AnalyzerTests, TestAnalyzerWithValidAndInvalidTestFiles) {
    AnalyzerParams param = GetParam();

    // Read source file
    std::ifstream sourceStream;
    sourceStream.open("./test-files/analyzer/" + param.testCaseName + "/source.spice");
    antlr4::ANTLRInputStream input(sourceStream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens);

    // Execute syntactical analysis
    antlr4::tree::ParseTree *tree = parser.entry();
    try {
        // Execute semantic analysis
        AnalyzerVisitor analyzer = AnalyzerVisitor(
                "source.spice",
                "",
                ".",
                true,
                3,
                true
        );
        SymbolTable* symbolTable = analyzer.visit(tree).as<SymbolTable*>();

        // Fail if an error was expected
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";

        // Check if the symbol table matches the expected output
        std::ifstream symbolTableStream;
        symbolTableStream.open("./test-files/analyzer/" + param.testCaseName + "/symbol-table.txt");
        std::ostringstream stringStream;
        stringStream << symbolTableStream.rdbuf();
        std::string expectedSymbolTable = stringStream.str();
        EXPECT_EQ(expectedSymbolTable, symbolTable->toString());

        SUCCEED();
    } catch (SemanticError &error) {
        if (param.errorMessage.length() == 0)
            FAIL() << "Expected no error, but got '" + std::string(error.what()) + "'";
        EXPECT_EQ(std::string(error.what()), param.errorMessage);
    }
}

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTests,
        AnalyzerTests,
        ::testing::ValuesIn(ANALYZER_TEST_PARAMETERS)
);