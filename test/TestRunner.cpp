// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <exception/CompilerError.h>
#include <exception/LexerError.h>
#include <exception/LinkerError.h>
#include <exception/ParserError.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <symboltablebuilder/SymbolTable.h>
#include <util/FileUtil.h>

#include "util/TestUtil.h"

using namespace spice::compiler;

namespace spice::testing {

void execTestCase(const TestCase &testCase) {
  // Check if test is disabled
  if (TestUtil::isDisabled(testCase, skipNonGitHubTests))
    GTEST_SKIP();

  // Create fake cli options
  const std::filesystem::path sourceFilePath = testCase.testPath / REF_NAME_SOURCE;
  const llvm::Triple targetTriple(llvm::Triple::normalize(llvm::sys::getDefaultTargetTriple()));
  CliOptions cliOptions = {
      /* mainSourceFile= */ sourceFilePath,
      /* targetTriple= */ targetTriple.getTriple(),
      /* targetArch= */ std::string(targetTriple.getArchName()),
      /* targetVendor= */ std::string(targetTriple.getVendorName()),
      /* targetOs= */ std::string(targetTriple.getOSName()),
      /* isNativeTarget= */ true,
      /* useCPUFeatures*/ false, // Disabled because it makes the refs differ on different machines
      /* execute= */ false,      // If we set this to 'true', the compiler will not emit object files
      /* cacheDir= */ "./cache",
      /* outputDir= */ "./",
      /* outputPath= */ "",
      /* buildMode= */ DEBUG,
      /* compileJobCount= */ 0,
      /* ignoreCache */ true,
      /* llvmArgs= */ "",
      /* printDebugOutput= */ false,
      CliOptions::DumpSettings{
          /* dumpCST= */ false,
          /* dumpAST= */ false,
          /* dumpSymbolTables= */ false,
          /* dumpTypes= */ false,
          /* dumpCacheStats= */ false,
          /* dumpDependencyGraph= */ false,
          /* dumpIR= */ false,
          /* dumpAssembly= */ false,
          /* dumpObjectFile= */ false,
          /* dumpToFiles= */ false,
          /* abortAfterDump */ false,
      },
      /* namesForIRValues= */ true,
      /* useLifetimeMarkers= */ false,
      /* optLevel= */ O0,
      /* useLTO= */ exists(testCase.testPath / CTL_LTO),
      /* noEntryFct= */ exists(testCase.testPath / CTL_RUN_BUILTIN_TESTS),
      /* generateTestMain= */ exists(testCase.testPath / CTL_RUN_BUILTIN_TESTS),
      /* staticLinking= */ false,
      /* debugInfo= */ exists(testCase.testPath / CTL_DEBUG_INFO),
      /* disableVerifier= */ false,
      /* testMode= */ true,
  };
  static_assert(sizeof(CliOptions::DumpSettings) == 11, "CliOptions::DumpSettings struct size changed");
  static_assert(sizeof(CliOptions) == 360, "CliOptions struct size changed");

  // Instantiate GlobalResourceManager
  GlobalResourceManager resourceManager(cliOptions);

  try {
    // Create source file instance for main source file
    SourceFile *mainSourceFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, cliOptions.mainSourceFile, false);

    // Run Lexer and Parser
    mainSourceFile->runLexer();
    mainSourceFile->runParser();

    // Check CST
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_PARSE_TREE, [&] {
      mainSourceFile->runCSTVisualizer();
      return mainSourceFile->compilerOutput.cstString;
    });

    // Build and optimize AST
    mainSourceFile->runASTBuilder();

    // Check AST
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_SYNTAX_TREE, [&] {
      mainSourceFile->runASTVisualizer();
      return mainSourceFile->compilerOutput.astString;
    });

    // Execute import collector and semantic analysis stages
    mainSourceFile->runImportCollector();
    mainSourceFile->runSymbolTableBuilder();
    mainSourceFile->runMiddleEnd(); // TypeChecker pre + post

    // Check symbol table output (check happens here to include updates from type checker)
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_SYMBOL_TABLE,
                            [&] { return mainSourceFile->globalScope->getSymbolTableJSON().dump(/*indent=*/2); });

    // Fail if an error was expected
    if (exists(testCase.testPath / REF_NAME_ERROR_OUTPUT))
      FAIL() << "Expected error, but got no error";

    // Check dependency graph
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_DEP_GRAPH, [&] {
      mainSourceFile->runDependencyGraphVisualizer();
      return mainSourceFile->compilerOutput.depGraphString;
    });

    // Run backend for all dependencies
    for (SourceFile *sourceFile : mainSourceFile->dependencies | std::views::values)
      sourceFile->runBackEnd();

    // Execute IR generator in normal or debug mode
    mainSourceFile->runIRGenerator();

    // Check unoptimized IR code
    TestUtil::checkRefMatch(
        testCase.testPath / REF_NAME_IR, [&] { return mainSourceFile->compilerOutput.irString; },
        [&](std::string &expectedOutput, std::string &actualOutput) {
          if (cliOptions.generateDebugInfo) {
            // Remove the lines, containing paths on the local file system
            TestUtil::eraseLinesBySubstring(expectedOutput, " = !DIFile(filename:");
            TestUtil::eraseLinesBySubstring(actualOutput, " = !DIFile(filename:");
          }
        });

    // Check optimized IR code
    for (uint8_t i = 1; i <= 5; i++) {
      TestUtil::checkRefMatch(testCase.testPath / REF_NAME_OPT_IR[i - 1], [&] {
        cliOptions.optLevel = static_cast<OptLevel>(i);

        if (cliOptions.useLTO) {
          mainSourceFile->runPreLinkIROptimizer();
          mainSourceFile->runBitcodeLinker();
          mainSourceFile->runPostLinkIROptimizer();
        } else {
          mainSourceFile->runDefaultIROptimizer();
        }

        return mainSourceFile->compilerOutput.irOptString;
      });
    }

    // Link the bitcode if not happened yet
    if (cliOptions.useLTO && cliOptions.optLevel == O0)
      mainSourceFile->runBitcodeLinker();

    // Check assembly code
    bool objectFilesEmitted = false;
    if (!skipNonGitHubTests) {
      TestUtil::checkRefMatch(testCase.testPath / REF_NAME_ASM, [&] {
        mainSourceFile->runObjectEmitter();
        objectFilesEmitted = true;

        return mainSourceFile->compilerOutput.asmString;
      });
    }

    // Check warnings
    mainSourceFile->collectAndPrintWarnings();
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_WARNING_OUTPUT, [&] {
      std::stringstream actualWarningString;
      for (const CompilerWarning &warning : mainSourceFile->compilerOutput.warnings)
        actualWarningString << warning.warningMessage << "\n";
      return actualWarningString.str();
    });

    // Do linking and conclude compilation
    const bool needsNormalRun = exists(testCase.testPath / REF_NAME_EXECUTION_OUTPUT);
    const bool needsDebuggerRun = exists(testCase.testPath / REF_NAME_GDB_OUTPUT);
    if (needsNormalRun || needsDebuggerRun) {
      // Prepare linker
      resourceManager.linker.outputPath = TestUtil::getDefaultExecutableName();

      // Parse linker flags
      const std::filesystem::path linkerFlagsFile = testCase.testPath / INPUT_NAME_LINKER_FLAGS;
      if (exists(linkerFlagsFile))
        for (const std::string &linkerFlag : TestUtil::getFileContentLinesVector(linkerFlagsFile))
          resourceManager.linker.addLinkerFlag(linkerFlag);

      // Emit main source file object if not done already
      if (!objectFilesEmitted)
        mainSourceFile->runObjectEmitter();

      // Conclude the compilation
      mainSourceFile->concludeCompilation();

      // Prepare and run linker
      resourceManager.linker.prepare();
      resourceManager.linker.link();
    }

    // Check type registry output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_TYPE_REGISTRY, [&] { return TypeRegistry::dump(); });

    // Check cache stats output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_CACHE_STATS, [&] {
      std::stringstream cacheStats;
      cacheStats << FunctionManager::dumpLookupCacheStatistics() << std::endl;
      cacheStats << StructManager::dumpLookupCacheStatistics() << std::endl;
      cacheStats << InterfaceManager::dumpLookupCacheStatistics() << std::endl;
      return cacheStats.str();
    });

    // Check if the execution output matches the expected output
    TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXECUTION_OUTPUT, [&] {
      const std::filesystem::path cliFlagsFile = testCase.testPath / INPUT_NAME_CLI_FLAGS;
      // Execute binary
      std::stringstream cmd;
      if (enableLeakDetection)
        cmd << "valgrind -q --leak-check=full --num-callers=100 --error-exitcode=1 ";
      cmd << TestUtil::getDefaultExecutableName();
      if (exists(cliFlagsFile))
        cmd << " " << TestUtil::getFileContentLinesVector(cliFlagsFile).at(0);
      const auto [output, exitCode] = FileUtil::exec(cmd.str(), true);

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
      // Check if the exit code matches the expected one
      // If no exit code ref file exists, check against 0
      if (TestUtil::checkRefMatch(testCase.testPath / REF_NAME_EXIT_CODE, [&] { return std::to_string(exitCode); })) {
        EXPECT_NE(0, exitCode) << "Program exited with zero exit code, but expected erronous exit code";
      } else {
        EXPECT_EQ(0, exitCode) << "Program exited with non-zero exit code";
      }
#endif

      return output;
    });

    // Check if the debugger output matches the expected output
    if (!skipNonGitHubTests) { // GDB tests are currently not support on GH actions
      TestUtil::checkRefMatch(
          testCase.testPath / REF_NAME_GDB_OUTPUT,
          [&] {
            // Execute debugger script
            std::filesystem::path gdbScriptPath = testCase.testPath / CTL_DEBUG_SCRIPT;
            EXPECT_TRUE(std::filesystem::exists(gdbScriptPath)) << "Debug output requested, but debug script not found";
            gdbScriptPath.make_preferred();
            const std::string cmd = "gdb -x " + gdbScriptPath.string() + " " + TestUtil::getDefaultExecutableName();
            const auto [output, exitCode] = FileUtil::exec(cmd);

#if not OS_WINDOWS // Windows does not give us the exit code, so we cannot check it on Windows
            EXPECT_EQ(0, exitCode) << "GDB exited with non-zero exit code when running debug script";
#endif

            return output;
          },
          [&](std::string &expectedOutput, std::string &actualOutput) {
            // Do not compare against the GDB header
            TestUtil::eraseGDBHeader(expectedOutput);
            TestUtil::eraseGDBHeader(actualOutput);
          });
    }
  } catch (LexerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (ParserError &error) {
    TestUtil::handleError(testCase, error);
  } catch (SemanticError &error) {
    TestUtil::handleError(testCase, error);
  } catch (CompilerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (LinkerError &error) {
    TestUtil::handleError(testCase, error);
  } catch (std::exception &error) {
    TestUtil::handleError(testCase, error);
  }

  SUCCEED();
}

class CommonTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(CommonTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, CommonTests, ::testing::ValuesIn(TestUtil::collectTestCases("common", false)),
                         TestUtil::NameResolver());

class LexerTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(LexerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, LexerTests, ::testing::ValuesIn(TestUtil::collectTestCases("lexer", false)), TestUtil::NameResolver());

class ParserTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(ParserTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ParserTests, ::testing::ValuesIn(TestUtil::collectTestCases("parser", false)),
                         TestUtil::NameResolver());

class SymbolTableBuilderTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(SymbolTableBuilderTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, SymbolTableBuilderTests, ::testing::ValuesIn(TestUtil::collectTestCases("symboltablebuilder", true)),
                         TestUtil::NameResolver());

class TypeCheckerTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(TypeCheckerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, TypeCheckerTests, ::testing::ValuesIn(TestUtil::collectTestCases("typechecker", true)),
                         TestUtil::NameResolver());

class IRGeneratorTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(IRGeneratorTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, IRGeneratorTests, ::testing::ValuesIn(TestUtil::collectTestCases("irgenerator", true)),
                         TestUtil::NameResolver());

class StdTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(StdTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, StdTests, ::testing::ValuesIn(TestUtil::collectTestCases("std", true)), TestUtil::NameResolver());

class BenchmarkTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(BenchmarkTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BenchmarkTests, ::testing::ValuesIn(TestUtil::collectTestCases("benchmark", false)),
                         TestUtil::NameResolver());

class ExampleTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(ExampleTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, ExampleTests, ::testing::ValuesIn(TestUtil::collectTestCases("examples", false)),
                         TestUtil::NameResolver());

class BootstrapCompilerTests : public ::testing::TestWithParam<TestCase> {};
TEST_P(BootstrapCompilerTests, ) { execTestCase(GetParam()); }
INSTANTIATE_TEST_SUITE_P(, BootstrapCompilerTests, ::testing::ValuesIn(TestUtil::collectTestCases("bootstrap-compiler", false)),
                         TestUtil::NameResolver());

} // namespace spice::testing

// GCOV_EXCL_STOP