// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <stdexcept>

#include <gtest/gtest.h>

#include <antlr4-runtime.h>

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <analyzer/AnalyzerVisitor.h>
#include <generator/GeneratorVisitor.h>

#include "exception/AntlrThrowingErrorListener.h"
#include "exception/LexerParserError.h"
#include "exception/SemanticError.h"

#include "TestUtil.h"

const unsigned int IR_FILE_SKIP_LINES = 4;

struct StdTestCase {
    const std::string testName;
    const std::string testPath;
};

typedef std::vector<StdTestCase> StdTestSuite;

std::vector<StdTestCase> detectStdTestCases(const std::string& suitePath) {
    std::vector<std::string> subDirs = TestUtil::getSubdirs(suitePath);

    std::vector<StdTestCase> testCases;
    testCases.reserve(subDirs.size());
    for (std::string& dirName : subDirs) {
        // Save test suite
        testCases.push_back({dirName, suitePath + "/" + dirName});
    }

    return testCases;
}

std::vector<StdTestSuite> detectStdTestSuites(const std::string& testFilesPath) {
    std::vector<std::string> subDirs = TestUtil::getSubdirs(testFilesPath);

    std::vector<StdTestSuite> testSuites;
    testSuites.reserve(subDirs.size());
    for (std::string& dirName : subDirs)
        testSuites.push_back(detectStdTestCases(testFilesPath + "/" + dirName));

    return testSuites;
}

void executeTest(const StdTestCase& testCase) {
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
        if (TestUtil::fileExists(testCase.testPath + "/exception.out")) // GCOV_EXCL_LINE
            FAIL() << "Expected error, but got no error";                      // GCOV_EXCL_LINE

        // Check if the symbol table matches the expected output
        std::string symbolTableFileName = testCase.testPath + "/symbol-table.json";
        if (TestUtil::fileExists(symbolTableFileName)) {
            std::string actualSymbolTable = symbolTable->toJSON().dump(2);
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(symbolTableFileName, actualSymbolTable); // GCOV_EXCL_LINE
            } else {
                std::string expectedSymbolTable = TestUtil::getFileContent(symbolTableFileName);
                EXPECT_EQ(expectedSymbolTable, actualSymbolTable);
            }
        }

        // Determine the expected opt level
        std::string irCodeO1FileName = testCase.testPath + "/ir-code-O1.ll";
        std::string irCodeO2FileName = testCase.testPath + "/ir-code-O2.ll";
        std::string irCodeO3FileName = testCase.testPath + "/ir-code-O3.ll";
        std::string irCodeOptFileName;
        std::string expectedOptIR;
        int expectedOptLevel = 0;
        if (TestUtil::fileExists(irCodeO1FileName)) {
            expectedOptLevel = 1;
            irCodeOptFileName = irCodeO1FileName;
            expectedOptIR = TestUtil::getFileContent(irCodeO1FileName);
        } else if (TestUtil::fileExists(irCodeO2FileName)) {
            expectedOptLevel = 2;
            irCodeOptFileName = irCodeO2FileName;
            expectedOptIR = TestUtil::getFileContent(irCodeO2FileName);
        } else if (TestUtil::fileExists(irCodeO3FileName)) {
            expectedOptLevel = 3;
            irCodeOptFileName = irCodeO3FileName;
            expectedOptIR = TestUtil::getFileContent(irCodeO3FileName);
        }

        // Execute generator
        GeneratorVisitor generator = GeneratorVisitor(
                context,
                builder,
                symbolTable,
                sourceFile,
                "",
                "",
                "",
                "./source.spice.o",
                false,
                expectedOptLevel,
                true
        );
        generator.init(); // Initialize code generation
        generator.visit(tree); // Generate IR code

        // Check if the ir code matches the expected output
        std::string irCodeFileName = testCase.testPath + "/ir-code.ll";
        if (TestUtil::fileExists(irCodeFileName)) {
            std::string actualIR = generator.getIRString();
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(irCodeFileName, actualIR); // GCOV_EXCL_LINE
            } else {
                std::string expectedIR = TestUtil::getFileContent(irCodeFileName);
                // Cut of first n lines to have a target independent
                for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
                    expectedIR.erase(0, expectedIR.find('\n') + 1);
                    actualIR.erase(0, actualIR.find('\n') + 1);
                }
                EXPECT_EQ(expectedIR, actualIR);
            }
        }

        // Check if the optimized ir code matches the expected output
        if (expectedOptLevel > 0) {
            generator.optimize();
            std::string actualOptimizedIR = generator.getIRString();
            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(irCodeOptFileName, actualOptimizedIR); // GCOV_EXCL_LINE
            } else {
                // Cut of first n lines to have a target independent
                for (int i = 0; i < IR_FILE_SKIP_LINES; i++) {
                    expectedOptIR.erase(0, expectedOptIR.find('\n') + 1);
                    actualOptimizedIR.erase(0, actualOptimizedIR.find('\n') + 1);
                }
                EXPECT_EQ(expectedOptIR, actualOptimizedIR);
            }
        }

        // Check if the execution output matches the expected output
        std::string outputFileName = testCase.testPath + "/cout.out";
        if (TestUtil::fileExists(outputFileName)) {
            // Emit the object file
            generator.emit(); // Emit object file for specified platform

            // Link
            TestUtil::exec("gcc -no-pie -o source source.spice.o");

            // Execute the program and get the output
            std::string actualOutput = TestUtil::exec(TestUtil::getDefaultExecutableName());

            if (TestUtil::isUpdateRefsEnabled()) {
                // Update ref
                TestUtil::setFileContent(outputFileName, actualOutput); // GCOV_EXCL_LINE
            } else {
                std::string expectedOutput = TestUtil::getFileContent(outputFileName);

                // Check if the outputs are matching
                EXPECT_EQ(expectedOutput, actualOutput);
            }
        }

        SUCCEED();
    } catch (LexerParserError& error) { // GCOV_EXCL_START
        FAIL() << "Hit lexer/parser error: " << error.what();
    } catch (SemanticError& error) {
        // Check if the exception message matches the expected output
        std::string exceptionFile = testCase.testPath + "/exception.out";
        if (TestUtil::fileExists(exceptionFile)) {
            std::string expectedException = TestUtil::getFileContent(exceptionFile);
            EXPECT_EQ(std::string(error.what()), expectedException);
        } else {
            FAIL() << "Expected no error, but got '" << error.what() << "'";
        }
    } // GCOV_EXCL_STOP
}

// Test classes

class StdIOTests : public ::testing::TestWithParam<StdTestCase> {};
class StdOSTests : public ::testing::TestWithParam<StdTestCase> {};

// Test macros

TEST_P(StdIOTests, IOTests) {
    executeTest(GetParam());
}

TEST_P(StdOSTests, OSTests) {
    executeTest(GetParam());
}

// Name resolver

struct NameResolver {
    template <class StdTestCase>
    std::string operator()(const ::testing::TestParamInfo<StdTestCase>& info) const {
        auto testCase = static_cast<StdTestCase>(info.param);
        return TestUtil::toCamelCase(testCase.testName);
    }
};

// Instantiations

const std::vector<StdTestSuite> testSuites = detectStdTestSuites("./test-files/std");

INSTANTIATE_TEST_SUITE_P(
        StdIOTests,
        StdIOTests,
        ::testing::ValuesIn(testSuites[0]),
        NameResolver());

INSTANTIATE_TEST_SUITE_P(
        StdOSTests,
        StdOSTests,
        ::testing::ValuesIn(testSuites[1]),
        NameResolver());
