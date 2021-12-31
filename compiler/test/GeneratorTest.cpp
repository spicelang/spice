// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <gtest/gtest.h>
#include <iostream>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include <generator/GeneratorVisitor.h>

struct GeneratorParams {
    const std::string testCaseName;
    const std::string errorMessage; // Empty error message for testing for no error
};

const GeneratorParams GENERATOR_TEST_PARAMETERS[] = {
        // Succeeding tests
        {
            "success-fibonacci",
            ""
        },
        {
            "success-faculty",
            ""
        },
        {
            "success-decl-default-value",
            ""
        },
        {
            "success-operators",
            ""
        },
        {
            "success-if-stmt",
            ""
        },
        {
            "success-for-loop",
            ""
        },
        {
            "success-foreach-loop",
            ""
        },
        {
            "success-while-loop",
            ""
        },
        {
            "success-modules",
            ""
        },
        {
            "success-pointer",
            ""
        },
        {
            "success-pointer-functions",
            ""
        },
        {
            "success-struct",
            ""
        },
        {
            "success-global-variables",
            ""
        },
        {
            "success-arrays",
            ""
        },
        {
            "success-modules-std",
            ""
        },
        {
            "success-methods",
            ""
        }
};

class GeneratorTests : public ::testing::TestWithParam<GeneratorParams> {};

TEST_P(GeneratorTests, TestGeneratorWithValidAndInvalidTestFiles) {
    GeneratorParams param = GetParam();

    std::string sourceFile = "./test-files/generator/" + param.testCaseName + "/source.spice";

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
    SymbolTable* symbolTable;
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
        symbolTable = analyzer.visit(tree).as<SymbolTable*>();
    } catch (SemanticError &error) {
        FAIL() << "Error thrown in semantic analysis while testing the generator: " << error.what();
    }

    // Execute generator
    try {
        GeneratorVisitor generator = GeneratorVisitor(
                symbolTable,
                sourceFile,
                "",
                "",
                "",
                ".",
                false,
                3,
                true
        );
        generator.init(); // Initialize code generation
        generator.visit(tree); // Generate IR code

        // Fail if an error was expected
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";

        // Check if the symbol table matches the expected output
        std::string irCodeFile = "./test-files/generator/" + param.testCaseName + "/ir-code.ll";
        std::ifstream irCodeStream;
        irCodeStream.open(irCodeFile);
        if (!sourceStream) throw std::runtime_error("Test file '" + irCodeFile + "' does not exist");
        std::ostringstream stringStream;
        stringStream << irCodeStream.rdbuf();
        std::string expectedIR = stringStream.str();
        EXPECT_EQ(expectedIR, generator.getIRString());

        SUCCEED();
    } catch (IRError& error) {
        if (param.errorMessage.length() == 0)
            FAIL() << "Expected no error, but got '" + std::string(error.what()) + "'";
        EXPECT_EQ(std::string(error.what()), param.errorMessage);
    }
}

INSTANTIATE_TEST_SUITE_P(
        GeneratorTests,
        GeneratorTests,
        ::testing::ValuesIn(GENERATOR_TEST_PARAMETERS)
);