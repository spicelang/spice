// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

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
            "Semantic error at 2:5: Wrong data type for operator: Cannot apply '=' operator on types int and string"
        },
        {
            "error-if-condition-bool",
            "Semantic error at 2:8: Condition must be bool: If condition must be of type bool"
        },
        {
            "error-else-if-condition-bool",
            "Semantic error at 4:15: Condition must be bool: If condition must be of type bool"
        },
        {
            "error-while-condition-bool",
            "Semantic error at 2:11: Condition must be bool: While loop condition must be of type bool"
        },
        {
            "error-for-condition-bool",
            "Semantic error at 2:20: Condition must be bool: For loop condition must be of type bool"
        },
        {
            "error-ternary-condition-bool",
            "Semantic error at 2:25: Wrong data type for operator: Condition operand in ternary must be a bool"
        },
        {
            "error-ternary-types-match",
            "Semantic error at 2:25: Wrong data type for operator: True and false operands in ternary must be of same data type"
        },
        {
            "error-variable-declared-before-referenced",
            "Semantic error at 2:26: Referenced undefined variable: Variable 'test' was referenced before declared"
        },
        {
            "error-variable-declared-only-once",
            "Semantic error at 3:5: Multiple declarations of the same variable: The variable 'i' was declared more than once"
        },
        {
            "error-variable-const-not-modified",
            "Semantic error at 2:5: Cannot re-assign constant variable: Not re-assignable variable 'variable'"
        },
        {
            "error-functions-defined-before-called",
            "Semantic error at 7:25: Referenced undefined function: Function/Procedure 'testFunction(int)' could not be found"
        },
        {
            "error-cannot-call-variables",
            "Semantic error at 3:22: Referenced undefined function: Function/Procedure 'variable()' could not be found"
        },
        {
            "error-function-params-match-declaration",
            "Semantic error at 6:5: Referenced undefined function: Function/Procedure 'testFunction()' could not be found"
        },
        {
            "error-function-param-types-match-declaration",
            "Semantic error at 6:5: Referenced undefined function: Function/Procedure 'testFunction(string)' could not be found"
        },
        {
            "error-return-type-matches-def",
            "Semantic error at 2:12: Wrong data type for operator: Passed wrong data type to return statement. Expected string but got int"
        },
        {
            "error-logical-operators-are-booleans",
            "Semantic error at 2:9: Wrong data type for operator: Cannot apply '||' operator on types string and bool"
        },
        {
            "error-bitwise-operators-are-booleans-or-integers",
            "Semantic error at 2:22: Wrong data type for operator: Cannot apply '|' operator on types string and int"
        },
        {
            "error-equality-operators-some-combinations",
            "Semantic error at 2:9: Wrong data type for operator: Cannot apply '==' operator on types string and double"
        },
        {
            "error-relational-operators-are-doubles-or-integers",
            "Semantic error at 3:12: Wrong data type for operator: Cannot apply '>' operator on types double and string"
        },
        {
            "error-additive-operators-some-combinations",
            "Semantic error at 2:29: Wrong data type for operator: Cannot apply '+' operator on types bool and string"
        },
        {
            "error-multiplicative-operators-some-combinations",
            "Semantic error at 2:29: Wrong data type for operator: Cannot apply '*' operator on types bool and string"
        },
        {
            "error-prefix-unary-only-integer-identifiers",
            "Semantic error at 2:7: Wrong data type for operator: Cannot apply '--' operator on type string"
        },
        {
            "error-postfix-unary-only-integer-identifiers",
            "Semantic error at 2:5: Wrong data type for operator: Cannot apply '++' operator on type string"
        },
        {
            "error-must-contain-main-function",
            "Semantic error at 1:1: Spice programs must contain a main function: No main function found"
        },
        {
            "error-function-arg-decl-type-dyn",
            "Semantic error at 1:51: Parameter type dyn not valid in function/procedure definition without default value: Type of parameter 'arg2' is invalid"
        },
        {
            "error-dyn-return-types-not-matching",
            "Semantic error at 3:12: Wrong data type for operator: Passed wrong data type to return statement. Expected string but got double"
        },
        {
            "error-printf-type-incompatibility",
            "Semantic error at 6:36: Types of printf call not matching: Template string expects char, but got string"
        },
        {
            "error-printf-arg-number-matches",
            "Semantic error at 2:5: Types of printf call not matching: Number of placeholders does not match the number of passed arguments"
        },
        {
            "error-break-count-valid",
            "Semantic error at 7:23: Invalid number of break calls: Break count must be >= 1, you provided -10"
        },
        {
            "error-break-count-not-too-high",
            "Semantic error at 7:23: Invalid number of break calls: We can only break 2 time(s) here"
        },
        {
            "error-continue-count-valid",
            "Semantic error at 7:26: Invalid number of continue calls: Continue count must be >= 1, you provided -10"
        },
        {
            "error-continue-count-not-too-high",
            "Semantic error at 7:26: Invalid number of continue calls: We can only continue 2 time(s) here"
        },
        {
            "error-imported-file-not-existing",
            "Semantic error at 1:8: Imported source file not existing: The source file 'source2.spice' does not exist"
        },
        {
            "error-imported-std-not-existing",
            "Semantic error at 1:8: Imported source file not existing: The source file 'std/non-existing/foo.spice' was not found in standard library"
        },
        {
            "error-circular-import",
            "Semantic error - Circular import detected: './test-files/analyzer/error-circular-import/source1.spice'"
        },
        {
            "error-duplicate-struct-def",
            "Semantic error at 7:1: Multiple declarations of a struct with the same name: Duplicate struct 'Person'"
        },
        {
            "error-duplicate-function-def",
            "Semantic error at 5:1: Multiple declarations of a function with the same name: Function 'exampleFunc()' is declared twice"
        },
        {
            "error-duplicate-main-function-def",
            "Semantic error at 5:1: Multiple declarations of a function with the same name: Main function is declared twice"
        },
        {
            "error-duplicate-procedure-def",
            "Semantic error at 5:1: Multiple declarations of a procedure with the same name: Procedure 'exampleProcedure()' is declared twice"
        },
        {
            "error-custom-type-unknown",
            "Semantic error at 8:5: Unknown datatype: Unknown datatype 'NonExisting'"
        },
        {
            "error-struct-defined-before-used",
            "Semantic error at 2:5: Unknown datatype: Unknown datatype 'Test'"
        },
        {
            "error-struct-fields-match-declaration",
            "Semantic error at 8:41: The type of a field value does not match the declaration: Expected type double* for the field 'dbl', but got double"
        },
        {
            "error-struct-passed-too-many-less-values",
            "Semantic error at 9:41: Number of struct fields not matching declaration: You've passed too less/many field values"
        },
        {
            "error-struct-types-not-matching",
            "Semantic error at 13:26: Referenced undefined variable: Variable 'dbl' was referenced before declared"
        },
        {
            "error-return-without-value-result",
            "Semantic error at 4:9: Return without initialization of result variable: Return without value, but result variable is not initialized yet"
        },
        {
            "error-foreach-wrong-array-type",
            "Semantic error at 3:13: Wrong data type for operator: Can only apply foreach loop on an array type. You provided bool"
        },
        {
            "error-foreach-wrong-item-type",
            "Semantic error at 3:13: Wrong data type for operator: Foreach loop item type does not match array type. Expected int[], provided double"
        },
        {
            "error-foreach-wrong-index-type",
            "Semantic error at 3:13: Array index not of type int: Index in foreach loop must be of type int. You provided double"
        },
        {
            "error-method-not-existing",
            "Semantic error at 12:19: Referenced undefined function: Function/Procedure 'getInt()' could not be found"
        },
        // Successful tests
        {
            "success-fibonacci",
            ""
        },
        {
            "success-function-overloading",
            ""
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {};

TEST_P(AnalyzerTests, TestAnalyzerWithValidAndInvalidTestFiles) {
    AnalyzerParams param = GetParam();

    std::string sourceFile = "./test-files/analyzer/" + param.testCaseName + "/source.spice";

    // Read source file
    std::ifstream sourceStream;
    sourceStream.open(sourceFile);
    if (!sourceStream) throw std::runtime_error("Test file '" + sourceFile + "' does not exist");
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
                sourceFile,
                "",
                "",
                "",
                ".",
                false,
                3,
                true,
                false
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