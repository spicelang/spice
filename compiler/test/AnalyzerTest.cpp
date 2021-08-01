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
        {
            "error-assignment-same-type",
            "Wrong data type for operator: Cannot apply the assign operator to different data types"
        },
        {
            "error-if-condition-bool",
            "Condition must be bool: If condition must be of type bool"
        },
        {
            "error-while-condition-bool",
            "Condition must be bool: While loop condition must be of type bool"
        },
        {
            "error-for-condition-bool",
            "Condition must be bool: For loop condition must be of type bool"
        },
        {
            "error-ternary-condition-bool",
            "Wrong data type for operator: Condition operand in ternary must be a bool"
        },
        {
            "error-ternary-types-match",
            "Wrong data type for operator: True operand and false operand in ternary must be from same data type"
        },
        {
            "error-variable-declared-before-referenced",
            "Referenced undefined variable: Variable test was referenced before initialized"
        },
        {
            "error-variable-declared-only-once",
            "Multiple declarations of the same variable: The variable 'i' was declared more than once"
        },
        {
            "error-variable-const-not-modified",
            "Cannot re-assign constant variable: Not re-assignable variable 'variable'"
        },
        /*{
            "error-functions-defined-before-called",
            ""
        },*/
        {
            "error-cannot-call-variables",
            "Can only call function or procedure: Object 'variable' is not of type function or procedure"
        },
        /*{
            "error-function-params-match-declaration",
            "Parameter types do not match: Type of parameter 'param1' does not match the declaration"
        },*/
        {
            "error-function-param-types-match-declaration",
            "Parameter types do not match: Type of parameter 'param1' does not match the declaration"
        },
        {
            "error-return-only-inside-function",
            "Return statement outside function: Cannot assign return statement to a function"
        },
        {
            "error-return-type-matches-def",
            "Wrong data type for operator: Passed wrong data type to return statement"
        },
        {
            "error-logical-operators-are-booleans",
            "Wrong data type for operator: Can only apply logical or to booleans"
        },
        {
            "error-bitwise-operators-are-booleans-or-integers",
            "Wrong data type for operator: Can only apply bitwise or to booleans and integers"
        },
        {
            "error-equality-operators-some-combinations",
            "Wrong data type for operator: Can only compare some type combinations with an equality operator"
        },
        {
            "error-relational-operators-are-doubles-or-integers",
            "Wrong data type for operator: Can only compare doubles or ints with one another with a relational operator"
        },
        {
            "error-additive-operators-some-combinations",
            "Wrong data type for operator: Incompatible operands string and string for additive operator"
        },
        {
            "error-multiplicative-operators-some-combinations",
            "Wrong data type for operator: Incompatible operands string and string for multiplicative operator"
        },
        {
            "error-prefix-unary-only-integer-identifiers",
            "Wrong data type for operator: Prefix '++' or '--' only can be applied to an identifier of type integer"
        },
        {
            "error-postfix-unary-only-integer-identifiers",
            "Wrong data type for operator: Postfix '++' or '--' only can be applied to an identifier of type integer"
        },
        {
            "error-must-contain-main-function",
            "Spice programs must contain a main function: No main function found."
        },
        {
            "error-function-arg-decl-type-dyn",
            "Parameter type dyn not valid in function/procedure definition without default value: Type of parameter 'arg2' is invalid"
        },
        {
            "error-dyn-return-types-not-matching",
            "Wrong data type for operator: Passed wrong data type to return statement"
        },
        // Successful tests
        {
            "success-fibonacci",
            ""
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {};

TEST_P(AnalyzerTests, TestAnalyzerWithValidAndInvalidTestFiles) {
    AnalyzerParams param = GetParam();

    // Read source file
    std::ifstream sourceStream;
    sourceStream.open("./test-files/" + param.testCaseName + "/source.spice");
    antlr4::ANTLRInputStream input(sourceStream);

    // Parse input to AST
    SpiceLexer lexer(&input);
    antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);
    SpiceParser parser(&tokens);

    // Execute syntactical analysis
    antlr4::tree::ParseTree *tree = parser.entry();
    try {
        // Execute semantic analysis
        SymbolTable* symbolTable = AnalyzerVisitor().visit(tree).as<SymbolTable*>();

        // Fail if an error was expected
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";

        // Check if the symbol table matches the expected output
        std::ifstream symbolTableStream;
        symbolTableStream.open("./test-files/" + param.testCaseName + "/symbol-table.txt");
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