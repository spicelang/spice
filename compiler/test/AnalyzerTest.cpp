// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <gtest/gtest.h>
#include <iostream>
#include <dirent.h>
#include "antlr4-runtime.h"

#include "SpiceLexer.h"
#include "SpiceParser.h"
#include <analyzer/AnalyzerVisitor.h>

struct AnalyzerTestCase {
    const std::string testName;
    const std::string testPath;
};

typedef std::vector<AnalyzerTestCase> AnalyzerTestSuite;

std::vector<std::string> getSubdirs(const std::string& basePath) {
    std::vector<std::string> subdirs;
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(basePath.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                subdirs.emplace_back(ent->d_name);
        }
        closedir(dir);
    }
    return subdirs;
}

std::vector<AnalyzerTestCase> detectTestCases(const std::string& suitePath) {
    std::vector<std::string> subDirs = getSubdirs(suitePath);

    std::vector<AnalyzerTestCase> testCases;
    testCases.reserve(subDirs.size());
    for (std::string& dirName : subDirs) {
        // Save test suite
        testCases.push_back({
            dirName,
            suitePath + "/" + dirName
        });
    }

    return testCases;
}

std::vector<AnalyzerTestSuite> detectTestSuites() {
    std::string testFilesPath = "./test-files/analyzer";
    std::vector<std::string> subDirs = getSubdirs(testFilesPath);

    std::vector<AnalyzerTestSuite> testSuites;
    testSuites.reserve(subDirs.size());
    for (std::string& dirName : subDirs)
        testSuites.push_back(detectTestCases(testFilesPath + "/" + dirName));

    return testSuites;
}

std::string getFileContent(const std::string& filePath) {
    std::ifstream symbolTableStream;
    symbolTableStream.open(filePath);
    std::ostringstream stringStream;
    stringStream << symbolTableStream.rdbuf();
    return stringStream.str();
}

inline bool fileExists(const std::string& filePath) {
    return std::ifstream(filePath.c_str()).good();
}

void executeTest(const AnalyzerTestCase& testCase) {
    std::string sourceFile = testCase.testPath + "/source.spice";

    // Read source file
    std::ifstream sourceStream;
    sourceStream.open(sourceFile);
    if (!sourceStream) throw std::runtime_error("Test file '" + sourceFile + "' does not exist");
    antlr4::ANTLRInputStream input(sourceStream);

    // Create error handlers for lexer and parser
    AntlrThrowingErrorListener lexerErrorHandler = AntlrThrowingErrorListener(LEXER);
    AntlrThrowingErrorListener parserErrorHandler = AntlrThrowingErrorListener(PARSER);

    try {
        // Tokenize input
        SpiceLexer lexer(&input);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&lexerErrorHandler);
        antlr4::CommonTokenStream tokens((antlr4::TokenSource*) &lexer);

        // Parse input to AST
        SpiceParser parser(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&parserErrorHandler);
        antlr4::tree::ParseTree* tree = parser.entry();

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
        if (fileExists(testCase.testPath + "/exception.out"))
            FAIL() << "Expected error, but got no error";

        // Check if the AST matches the expected output
        /*std::string astFileName = testCase.testPath + "/syntax-tree.ast";
        if (fileExists(astFileName)) {
            std::string expectedSymbolTable = getFileContent(astFileName);
            EXPECT_EQ(expectedSymbolTable, tree->toStringTree(true));
        }*/

        // Check if the symbol table matches the expected output
        std::string symbolTableFileName = testCase.testPath + "/symbol-table.txt";
        if (fileExists(symbolTableFileName)) {
            std::string expectedSymbolTable = getFileContent(symbolTableFileName);
            EXPECT_EQ(expectedSymbolTable, symbolTable->toString());
        }

        SUCCEED();
    } catch (LexerParserError& error) {
        FAIL() << "Hit lexer/parser error: " << error.what();
    } catch (SemanticError& error) {
        // Check if the exception message matches the expected output
        std::string exceptionFile = testCase.testPath + "/exception.out";
        if (fileExists(exceptionFile)) {
            std::string expectedException = getFileContent(exceptionFile);
            EXPECT_EQ(std::string(error.what()), expectedException);
        } else {
            FAIL() << "Expected no error, but got '" << error.what() << "'";
        }
    }
}

// Test classes

class AnalyzerArbitraryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerArrayTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerBuiltinTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForEachLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionCallTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerIfStatementTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerImportTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerLoopControlInstructionTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerMethodTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerOperatorTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerProcedureTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerStructTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerTernaryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerTypeSystemTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerVariableTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerWhileLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};

// Test macros

TEST_P(AnalyzerArbitraryTests, ArbitraryTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerArrayTests, ArrayTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerBuiltinTests, BuiltinTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerForLoopTests, ForLoopTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerForEachLoopTests, ForEachLoopTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerFunctionCallTests, FunctionCallTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerFunctionTests, FunctionTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerIfStatementTests, IfStatementTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerImportTests, ImportTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerLoopControlInstructionTests, LoopControlInstructionTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerMethodTests, MethodTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerOperatorTests, OperatorTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerProcedureTests, ProcedureTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerStructTests, StructTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerTernaryTests, TernaryTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerTypeSystemTests, TypeSystemTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerVariableTests, VariableTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerWhileLoopTests, WhileLoopTests) {
    executeTest(GetParam());
}

// Name resolver

std::string toCamelCase(std::string text) {
    for (auto it = text.begin(); it != text.end(); it++) {
        if (*it == '-' || *it == '_') {
            it = text.erase(it);
            *it = toupper(*it);
        }
    }
    return text;
}

struct NameResolver {
    template <class AnalyzerTestCase>
    std::string operator()(const ::testing::TestParamInfo<AnalyzerTestCase>& info) const {
        auto testCase = static_cast<AnalyzerTestCase>(info.param);
        return toCamelCase(testCase.testName);
    }
};

// Instantiations

INSTANTIATE_TEST_SUITE_P(
        AnalyzerArbitraryTests,
        AnalyzerArbitraryTests,
        ::testing::ValuesIn(detectTestSuites()[0]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerArrayTests,
        AnalyzerArrayTests,
        ::testing::ValuesIn(detectTestSuites()[1]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerBuiltinTests,
        AnalyzerBuiltinTests,
        ::testing::ValuesIn(detectTestSuites()[2]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerForLoopTests,
        AnalyzerForLoopTests,
        ::testing::ValuesIn(detectTestSuites()[3]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerForEachLoopTests,
        AnalyzerForEachLoopTests,
        ::testing::ValuesIn(detectTestSuites()[4]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerFunctionCallTests,
        AnalyzerFunctionCallTests,
        ::testing::ValuesIn(detectTestSuites()[5]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerFunctionTests,
        AnalyzerFunctionTests,
        ::testing::ValuesIn(detectTestSuites()[6]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerIfStatementTests,
        AnalyzerIfStatementTests,
        ::testing::ValuesIn(detectTestSuites()[7]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerImportTests,
        AnalyzerImportTests,
        ::testing::ValuesIn(detectTestSuites()[8]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerLoopControlInstructionTests,
        AnalyzerLoopControlInstructionTests,
        ::testing::ValuesIn(detectTestSuites()[9]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerMethodTests,
        AnalyzerMethodTests,
        ::testing::ValuesIn(detectTestSuites()[10]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerOperatorTests,
        AnalyzerOperatorTests,
        ::testing::ValuesIn(detectTestSuites()[11]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerProcedureTests,
        AnalyzerProcedureTests,
        ::testing::ValuesIn(detectTestSuites()[12]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerStructTests,
        AnalyzerStructTests,
        ::testing::ValuesIn(detectTestSuites()[13]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTernaryTests,
        AnalyzerTernaryTests,
        ::testing::ValuesIn(detectTestSuites()[14]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTypeSystemTests,
        AnalyzerTypeSystemTests,
        ::testing::ValuesIn(detectTestSuites()[15]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerVariableTests,
        AnalyzerVariableTests,
        ::testing::ValuesIn(detectTestSuites()[16]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerWhileLoopTests,
        AnalyzerWhileLoopTests,
        ::testing::ValuesIn(detectTestSuites()[17]),
        NameResolver());
