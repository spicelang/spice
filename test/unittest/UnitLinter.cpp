// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <fstream>
#include <random>

#include <gtest/gtest.h>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <linter/LintFinding.h>
#include <linter/LintPass.h>

#include <llvm/TargetParser/Host.h>

// LCOV_EXCL_START

namespace spice::testing {

using namespace spice::compiler;

namespace {

std::filesystem::path makeUniqueDir() {
  std::random_device rd;
  std::mt19937_64 rng(rd());
  const std::string suffix = "spice-linter-test-" + std::to_string(rng());
  std::filesystem::path dir = std::filesystem::temp_directory_path() / suffix;
  std::filesystem::create_directories(dir);
  return dir;
}

void writeFile(const std::filesystem::path &path, const std::string &content) {
  std::ofstream stream(path);
  stream << content;
}

class LinterTest : public ::testing::Test {
protected:
  void SetUp() override {
    cacheDir = makeUniqueDir();
    outputDir = makeUniqueDir();
    cliOptions.cacheDir = cacheDir;
    cliOptions.outputDir = outputDir;
    // GlobalResourceManager construction looks up an LLVM target for the source file, so a valid
    // native target triple is required even though these tests never reach code generation.
    cliOptions.targetTriple = llvm::Triple(llvm::Triple::normalize(llvm::sys::getProcessTriple()));
    cliOptions.isNativeTarget = true;
  }

  void TearDown() override {
    std::error_code ec;
    std::filesystem::remove_all(cacheDir, ec);
    std::filesystem::remove_all(outputDir, ec);
  }

  // Runs the front end and type checker for the given source, then runs the LintPass over the
  // resulting AST - mirrors what `spice lint` does, minus the CLI/print plumbing.
  std::vector<LintFinding> lintSource(const std::string &source) {
    const std::filesystem::path sourcePath = outputDir / "source.spice";
    writeFile(sourcePath, source);

    GlobalResourceManager resourceManager(cliOptions);
    SourceFile *mainFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, sourcePath, false);
    mainFile->runFrontEnd();
    mainFile->runMiddleEnd();

    LintPass lintPass(resourceManager, mainFile);
    return lintPass.lint(mainFile->ast);
  }

  CliOptions cliOptions;
  std::filesystem::path cacheDir;
  std::filesystem::path outputDir;
};

} // namespace

TEST_F(LinterTest, NamingConventionFlagsSnakeCaseFunctionAndStructNames) {
  const std::string source = R"(
type My_Point struct {
    double x
    double y
}

f<int> bad_function_name(int a, int b) {
    return a + b;
}

f<int> main() {
    printf("%d\n", bad_function_name(1, 2));
    return 0;
}
)";
  const std::vector<LintFinding> findings = lintSource(source);

  ASSERT_EQ(2u, findings.size());
  for (const LintFinding &finding : findings)
    EXPECT_EQ("naming-convention", finding.ruleId);
  EXPECT_NE(std::string::npos, findings[0].findingMessage.find("My_Point"));
  EXPECT_NE(std::string::npos, findings[0].findingMessage.find("PascalCase"));
  EXPECT_NE(std::string::npos, findings[1].findingMessage.find("bad_function_name"));
  EXPECT_NE(std::string::npos, findings[1].findingMessage.find("camelCase"));
}

TEST_F(LinterTest, NamingConventionAcceptsConformingNames) {
  const std::string source = R"(
type Point struct {
    double x
    double y
}

f<int> addTwo(int a, int b) {
    return a + b;
}

f<int> main() {
    printf("%d\n", addTwo(1, 2));
    return 0;
}
)";
  const std::vector<LintFinding> findings = lintSource(source);

  EXPECT_TRUE(findings.empty());
}

// Operator overloads and constructors follow language-mandated naming ("op.<name>", "ctor"), not
// the camelCase convention, so the rule must not flag them.
TEST_F(LinterTest, NamingConventionIgnoresOperatorOverloadsAndConstructors) {
  const std::string source = R"(
type Point struct {
    int x
    int y
}

public p Point.ctor(int x, int y) {
    this.x = x;
    this.y = y;
}

public f<bool> operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

f<int> main() {
    Point a = Point(1, 2);
    Point b = Point(1, 2);
    return a == b ? 0 : 1;
}
)";
  const std::vector<LintFinding> findings = lintSource(source);

  EXPECT_TRUE(findings.empty());
}

} // namespace spice::testing

// LCOV_EXCL_STOP
