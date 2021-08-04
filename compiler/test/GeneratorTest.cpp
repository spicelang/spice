// Copyright (c) 2021 ChilliBits. All rights reserved.

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
        { // 0
            "success-fibonacci",
            "Wrong data type for operator: Cannot apply the assign operator to different data types"
        }
};

class GeneratorTests : public ::testing::TestWithParam<GeneratorParams> {};

TEST_P(GeneratorTests, TestGeneratorWithValidAndInvalidTestFiles) {
    GeneratorParams param = GetParam();

    // Read source file
    std::ifstream sourceStream;
    sourceStream.open("./test-files/generator/" + param.testCaseName + "/source.spice");
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
        symbolTable = AnalyzerVisitor().visit(tree).as<SymbolTable*>();
    } catch (SemanticError &error) {
        FAIL() << "Error thrown in semantic analysis while testing the generator: " << error.what();
    }

    // Execute generator
    try {
        GeneratorVisitor generator = GeneratorVisitor(symbolTable);
        generator.init(); // Initialize code generation
        generator.visit(tree); // Generate IR code

        // Check if the symbol table matches the expected output
        std::ifstream symbolTableStream;
        symbolTableStream.open("./test-files/generator/" + param.testCaseName + "/ir-code.ll");
        std::ostringstream stringStream;
        stringStream << symbolTableStream.rdbuf();
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