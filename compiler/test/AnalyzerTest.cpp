// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <stdexcept>

#include <gtest/gtest.h>

#include <antlr4-runtime.h>

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <analyzer/AnalyzerVisitor.h>
#include "exception/AntlrThrowingErrorListener.h"
#include "exception/LexerParserError.h"
#include "exception/SemanticError.h"

#include "TestUtil.h"

struct AnalyzerTestCase {
    const std::string testName;
    const std::string testPath;
};

typedef std::vector<AnalyzerTestCase> AnalyzerTestSuite;

std::vector<AnalyzerTestCase> detectAnalyzerTestCases(const std::string& suitePath) {
    std::vector<std::string> subDirs = TestUtil::getSubdirs(suitePath);

    std::vector<AnalyzerTestCase> testCases;
    testCases.reserve(subDirs.size());
    for (std::string& dirName : subDirs) {
        // Save test suite
        testCases.push_back({dirName, suitePath + "/" + dirName});
    }

    return testCases;
}

std::vector<AnalyzerTestSuite> detectAnalyzerTestSuites(const std::string& testFilesPath) {
    std::vector<std::string> subDirs = TestUtil::getSubdirs(testFilesPath);

    std::vector<AnalyzerTestSuite> testSuites;
    testSuites.reserve(subDirs.size());
    for (std::string& dirName : subDirs)
        testSuites.push_back(detectAnalyzerTestCases(testFilesPath + "/" + dirName));

    return testSuites;
}

void executeTest(const AnalyzerTestCase& testCase) {
    // Check if disabled
    std::string disabledFile = testCase.testPath + "/disabled";
    if (TestUtil::fileExists(disabledFile)) GTEST_SKIP();

    // Read source file
    std::string sourceFile = testCase.testPath + "/source.spice";
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

        // Prepare global LLVM assets
        std::shared_ptr<llvm::LLVMContext> context = std::make_shared<llvm::LLVMContext>();
        std::shared_ptr<llvm::IRBuilder<>> builder = std::make_shared<llvm::IRBuilder<>>(*context);

        // Execute semantic analysis
        AnalyzerVisitor analyzer = AnalyzerVisitor(
                context,
                builder,
                sourceFile,
                "",
                "",
                "",
                ".",
                false,
                0,
                true,
                false
        );
        SymbolTable* symbolTable = analyzer.visit(tree).as<SymbolTable*>();

        // Fail if an error was expected
        if (TestUtil::fileExists(testCase.testPath + "/exception.out"))
            FAIL() << "Expected error, but got no error";  // GCOV_EXCL_LINE

        // Check if the AST matches the expected output
        /*std::string astFileName = testCase.testPath + "/syntax-tree.ast";
        if (fileExists(astFileName)) {
            std::string expectedSymbolTable = getFileContent(astFileName);
            EXPECT_EQ(expectedSymbolTable, tree->toStringTree(true));
        }*/

        // Check if the symbol table matches the expected output
        std::string symbolTableFileName = testCase.testPath + "/symbol-table.json";
        if (TestUtil::fileExists(symbolTableFileName)) {
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(symbolTableFileName, symbolTable->toJSON().dump(2)); // GCOV_EXCL_LINE
            } else {
                std::string expectedSymbolTable = TestUtil::getFileContent(symbolTableFileName);
                EXPECT_EQ(expectedSymbolTable, symbolTable->toJSON().dump(2));
            }
        }

        SUCCEED();
    } catch (LexerParserError& error) {
        // Check if the exception message matches the expected output
        std::string exceptionFile = testCase.testPath + "/exception.out";
        if (TestUtil::fileExists(exceptionFile)) {
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(exceptionFile, error.what()); // GCOV_EXCL_LINE
            } else {
                std::string expectedException = TestUtil::getFileContent(exceptionFile);
                EXPECT_EQ(std::string(error.what()), expectedException);
            }
        } else {
            FAIL() << "Expected no error, but got '" << error.what() << "'"; // GCOV_EXCL_LINE
        }
    } catch (SemanticError& error) {
        // Check if the exception message matches the expected output
        std::string exceptionFile = testCase.testPath + "/exception.out";
        if (TestUtil::fileExists(exceptionFile)) {
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(exceptionFile, error.what()); // GCOV_EXCL_LINE
            } else {
                std::string expectedException = TestUtil::getFileContent(exceptionFile);
                EXPECT_EQ(std::string(error.what()), expectedException);
            }
        } else {
            FAIL() << "Expected no error, but got '" << error.what() << "'"; // GCOV_EXCL_LINE
        }
    }
}

// Test classes

class AnalyzerArbitraryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerArrayTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerBuiltinTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerComingSoonTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerForEachLoopTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionCallTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerFunctionTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerIfStatementTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerImportTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerLexerTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerLoopCtlInstTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerMethodTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerOperatorTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerParserTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerProcedureTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerStructTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerTernaryTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
class AnalyzerTheadTests : public ::testing::TestWithParam<AnalyzerTestCase> {};
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

TEST_P(AnalyzerComingSoonTests, ComingSoonTests) {
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

TEST_P(AnalyzerLexerTests, LexerTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerLoopCtlInstTests, LoopCtlInstTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerMethodTests, MethodTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerOperatorTests, OperatorTests) {
    executeTest(GetParam());
}

TEST_P(AnalyzerParserTests, ParserTests) {
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

TEST_P(AnalyzerTheadTests, TheadTests) {
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

struct NameResolver {
    template <class AnalyzerTestCase>
    std::string operator()(const ::testing::TestParamInfo<AnalyzerTestCase>& info) const {
        auto testCase = static_cast<AnalyzerTestCase>(info.param);
        return TestUtil::toCamelCase(testCase.testName);
    }
};

// Instantiations

const std::vector<AnalyzerTestSuite> testSuites = detectAnalyzerTestSuites("./test-files/analyzer");

INSTANTIATE_TEST_SUITE_P(
        AnalyzerArbitraryTests,
        AnalyzerArbitraryTests,
        ::testing::ValuesIn(testSuites[0]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerArrayTests,
        AnalyzerArrayTests,
        ::testing::ValuesIn(testSuites[1]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerBuiltinTests,
        AnalyzerBuiltinTests,
        ::testing::ValuesIn(testSuites[2]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerComingSoonTests,
        AnalyzerComingSoonTests,
        ::testing::ValuesIn(testSuites[3]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerForLoopTests,
        AnalyzerForLoopTests,
        ::testing::ValuesIn(testSuites[4]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerForEachLoopTests,
        AnalyzerForEachLoopTests,
        ::testing::ValuesIn(testSuites[5]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerFunctionCallTests,
        AnalyzerFunctionCallTests,
        ::testing::ValuesIn(testSuites[6]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerFunctionTests,
        AnalyzerFunctionTests,
        ::testing::ValuesIn(testSuites[7]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerIfStatementTests,
        AnalyzerIfStatementTests,
        ::testing::ValuesIn(testSuites[8]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerImportTests,
        AnalyzerImportTests,
        ::testing::ValuesIn(testSuites[9]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerLexerTests,
        AnalyzerLexerTests,
        ::testing::ValuesIn(testSuites[10]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerLoopCtlInstTests,
        AnalyzerLoopCtlInstTests,
        ::testing::ValuesIn(testSuites[11]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerMethodTests,
        AnalyzerMethodTests,
        ::testing::ValuesIn(testSuites[12]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerOperatorTests,
        AnalyzerOperatorTests,
        ::testing::ValuesIn(testSuites[13]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerParserTests,
        AnalyzerParserTests,
        ::testing::ValuesIn(testSuites[14]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerProcedureTests,
        AnalyzerProcedureTests,
        ::testing::ValuesIn(testSuites[15]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerStructTests,
        AnalyzerStructTests,
        ::testing::ValuesIn(testSuites[16]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTernaryTests,
        AnalyzerTernaryTests,
        ::testing::ValuesIn(testSuites[17]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTheadTests,
        AnalyzerTheadTests,
        ::testing::ValuesIn(testSuites[18]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTypeSystemTests,
        AnalyzerTypeSystemTests,
        ::testing::ValuesIn(testSuites[19]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerVariableTests,
        AnalyzerVariableTests,
        ::testing::ValuesIn(testSuites[20]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        AnalyzerWhileLoopTests,
        AnalyzerWhileLoopTests,
        ::testing::ValuesIn(testSuites[21]),
        NameResolver());
