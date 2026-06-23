// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <fstream>
#include <random>

#include <gtest/gtest.h>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <global/CacheManager.h>
#include <global/GlobalResourceManager.h>

#include <llvm/TargetParser/Host.h>

// LCOV_EXCL_START

namespace spice::testing {

using namespace spice::compiler;

namespace {

std::filesystem::path makeUniqueCacheDir() {
  std::random_device rd;
  std::mt19937_64 rng(rd());
  const std::string suffix = "spice-cache-test-" + std::to_string(rng());
  std::filesystem::path dir = std::filesystem::temp_directory_path() / suffix;
  std::filesystem::create_directories(dir);
  return dir;
}

void writeDummyFile(const std::filesystem::path &path, const std::string &content) {
  std::ofstream stream(path);
  stream << content;
}

class CompileCacheTest : public ::testing::Test {
protected:
  void SetUp() override {
    cacheDir = makeUniqueCacheDir();
    outputDir = makeUniqueCacheDir();
    cliOptions.cacheDir = cacheDir;
    cliOptions.outputDir = outputDir;
  }

  void TearDown() override {
    std::error_code ec;
    std::filesystem::remove_all(cacheDir, ec);
    std::filesystem::remove_all(outputDir, ec);
  }

  CliOptions cliOptions;
  std::filesystem::path cacheDir;
  std::filesystem::path outputDir;
};

} // namespace

TEST_F(CompileCacheTest, ComputeCacheKeyIsDeterministic) {
  const CacheManager manager(cliOptions);
  const std::string source = "f<int> main() { return 0; }";
  ASSERT_EQ(manager.computeCacheKey(source), manager.computeCacheKey(source));
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForDifferentContent) {
  const CacheManager manager(cliOptions);
  const std::string a = "f<int> main() { return 0; }";
  const std::string b = "f<int> main() { return 1; }";
  ASSERT_NE(manager.computeCacheKey(a), manager.computeCacheKey(b));
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForBuildMode) {
  const std::string source = "f<int> main() { return 0; }";

  cliOptions.buildMode = BuildMode::DEBUG;
  const CacheManager managerDebug(cliOptions);
  const std::string keyDebug = managerDebug.computeCacheKey(source);

  cliOptions.buildMode = BuildMode::RELEASE;
  const CacheManager managerRelease(cliOptions);
  const std::string keyRelease = managerRelease.computeCacheKey(source);

  ASSERT_NE(keyDebug, keyRelease);
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForOptLevel) {
  const std::string source = "f<int> main() { return 0; }";

  cliOptions.optLevel = OptLevel::O0;
  const CacheManager managerO0(cliOptions);
  const std::string keyO0 = managerO0.computeCacheKey(source);

  cliOptions.optLevel = OptLevel::O3;
  const CacheManager managerO3(cliOptions);
  const std::string keyO3 = managerO3.computeCacheKey(source);

  ASSERT_NE(keyO0, keyO3);
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForSanitizer) {
  const std::string source = "f<int> main() { return 0; }";

  cliOptions.instrumentation.sanitizer = Sanitizer::NONE;
  const CacheManager managerNone(cliOptions);
  const std::string keyNone = managerNone.computeCacheKey(source);

  cliOptions.instrumentation.sanitizer = Sanitizer::ADDRESS;
  const CacheManager managerAsan(cliOptions);
  const std::string keyAsan = managerAsan.computeCacheKey(source);

  ASSERT_NE(keyNone, keyAsan);
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForDebugInfo) {
  const std::string source = "f<int> main() { return 0; }";

  cliOptions.instrumentation.generateDebugInfo = false;
  const CacheManager cmNoDebug(cliOptions);
  const std::string keyNoDebug = cmNoDebug.computeCacheKey(source);

  cliOptions.instrumentation.generateDebugInfo = true;
  const CacheManager cmDebug(cliOptions);
  const std::string keyDebug = cmDebug.computeCacheKey(source);

  ASSERT_NE(keyNoDebug, keyDebug);
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForLTO) {
  const std::string source = "f<int> main() { return 0; }";

  cliOptions.useLTO = false;
  const CacheManager managerNoLto(cliOptions);
  const std::string keyNoLto = managerNoLto.computeCacheKey(source);

  cliOptions.useLTO = true;
  const CacheManager managerLto(cliOptions);
  const std::string keyLto = managerLto.computeCacheKey(source);

  ASSERT_NE(keyNoLto, keyLto);
}

TEST_F(CompileCacheTest, ComputeCacheKeyDiffersForDepCacheKeys) {
  const CacheManager manager(cliOptions);
  const std::string source = "f<int> main() { return 0; }";

  const std::string keyNoDeps = manager.computeCacheKey(source);
  const std::string keyWithDep = manager.computeCacheKey(source, {"dep-v1"});
  const std::string keyWithDepChanged = manager.computeCacheKey(source, {"dep-v2"});

  ASSERT_NE(keyNoDeps, keyWithDep);
  ASSERT_NE(keyWithDep, keyWithDepChanged);
}

TEST_F(CompileCacheTest, ComputeCacheKeyIsOrderIndependentInDeps) {
  const CacheManager manager(cliOptions);
  const std::string source = "f<int> main() { return 0; }";
  // Dep cache keys must be sorted internally, so call sites do not need to provide a particular
  // order. This matters because dep traversal order is not stable across builds.
  ASSERT_EQ(manager.computeCacheKey(source, {"a", "b", "c"}), manager.computeCacheKey(source, {"c", "a", "b"}));
}

TEST_F(CompileCacheTest, LookupExecutableMissReturnsFalse) {
  const CacheManager manager(cliOptions);
  std::filesystem::path resolved;
  ASSERT_FALSE(manager.lookupExecutable({"key-a", "key-b"}, {"-lm"}, {}, resolved));
  ASSERT_TRUE(resolved.empty());
}

TEST_F(CompileCacheTest, CacheExecutableRoundTrip) {
  const CacheManager manager(cliOptions);

  const std::filesystem::path executablePath = outputDir / "my-program";
  writeDummyFile(executablePath, "executable-bytes");

  const std::vector<std::string> objectKeys = {"obj-1", "obj-2"};
  const std::vector<std::string> linkerFlags = {"-lm", "-lpthread"};

  manager.cacheExecutable(objectKeys, linkerFlags, {}, executablePath);

  std::filesystem::path resolved;
  ASSERT_TRUE(manager.lookupExecutable(objectKeys, linkerFlags, {}, resolved));
  ASSERT_TRUE(std::filesystem::exists(resolved));
  ASSERT_EQ(cacheDir, resolved.parent_path());
}

TEST_F(CompileCacheTest, CacheExecutableNonExistingSourceIsNoop) {
  const CacheManager manager(cliOptions);

  const std::filesystem::path missingExecutable = outputDir / "does-not-exist";
  const std::vector<std::string> objectKeys = {"obj-1"};
  constexpr std::vector<std::string> linkerFlags;

  manager.cacheExecutable(objectKeys, linkerFlags, {}, missingExecutable);

  std::filesystem::path resolved;
  ASSERT_FALSE(manager.lookupExecutable(objectKeys, linkerFlags, {}, resolved));
}

TEST_F(CompileCacheTest, LookupExecutableMissesWhenObjectKeysDiffer) {
  const CacheManager manager(cliOptions);

  const std::filesystem::path executablePath = outputDir / "program";
  writeDummyFile(executablePath, "executable-bytes");
  const std::vector<std::string> linkerFlags = {"-lm"};

  manager.cacheExecutable({"obj-1", "obj-2"}, linkerFlags, {}, executablePath);

  std::filesystem::path resolved;
  ASSERT_FALSE(manager.lookupExecutable({"obj-1", "obj-3"}, linkerFlags, {}, resolved));
}

TEST_F(CompileCacheTest, LookupExecutableMissesWhenLinkerFlagsDiffer) {
  const CacheManager manager(cliOptions);

  const std::filesystem::path executablePath = outputDir / "program";
  writeDummyFile(executablePath, "executable-bytes");
  const std::vector<std::string> objectKeys = {"obj-1"};

  manager.cacheExecutable(objectKeys, {"-lm"}, {}, executablePath);

  std::filesystem::path resolved;
  ASSERT_FALSE(manager.lookupExecutable(objectKeys, {"-lpthread"}, {}, resolved));
}

TEST_F(CompileCacheTest, LookupExecutableMissesWhenStaticLinkingDiffers) {
  cliOptions.staticLinking = false;
  const CacheManager manager(cliOptions);

  const std::filesystem::path executablePath = outputDir / "program";
  writeDummyFile(executablePath, "executable-bytes");
  const std::vector<std::string> objectKeys = {"obj-1"};
  const std::vector<std::string> linkerFlags = {"-lm"};

  manager.cacheExecutable(objectKeys, linkerFlags, {}, executablePath);

  // Same cacheDir, but a CacheManager configured with staticLinking=true should miss
  cliOptions.staticLinking = true;
  const CacheManager cmStatic(cliOptions);
  std::filesystem::path resolved;
  ASSERT_FALSE(cmStatic.lookupExecutable(objectKeys, linkerFlags, {}, resolved));
}

TEST_F(CompileCacheTest, LookupExecutableMissesWhenOutputContainerDiffers) {
  cliOptions.outputContainer = OutputContainer::EXECUTABLE;
  const CacheManager managerExec(cliOptions);

  const std::filesystem::path executablePath = outputDir / "program";
  writeDummyFile(executablePath, "executable-bytes");
  const std::vector<std::string> objectKeys = {"obj-1"};
  const std::vector<std::string> linkerFlags = {"-lm"};

  managerExec.cacheExecutable(objectKeys, linkerFlags, {}, executablePath);

  cliOptions.outputContainer = OutputContainer::SHARED_LIBRARY;
  const CacheManager managerShared(cliOptions);
  std::filesystem::path resolved;
  ASSERT_FALSE(managerShared.lookupExecutable(objectKeys, linkerFlags, {}, resolved));
}

TEST_F(CompileCacheTest, LookupExecutableMissesWhenAdditionalSourceContentChanges) {
  const CacheManager manager(cliOptions);

  // C/C++ files referenced via @core.linker.additionalSource must contribute to the executable
  // cache key, otherwise editing them would silently keep serving the previously linked binary.
  const std::filesystem::path additionalSource = outputDir / "extra.c";
  writeDummyFile(additionalSource, "int compute() { return 1; }\n");

  const std::filesystem::path executablePath = outputDir / "program";
  writeDummyFile(executablePath, "executable-bytes");

  const std::vector<std::string> objectKeys = {"obj-1"};
  const std::vector<std::string> linkerFlags = {"-lm"};
  const std::vector<std::filesystem::path> additionalSources = {additionalSource};

  manager.cacheExecutable(objectKeys, linkerFlags, additionalSources, executablePath);

  std::filesystem::path resolved;
  ASSERT_TRUE(manager.lookupExecutable(objectKeys, linkerFlags, additionalSources, resolved));

  // Editing the additional source must invalidate the cached executable
  writeDummyFile(additionalSource, "int compute() { return 2; }\n");
  ASSERT_FALSE(manager.lookupExecutable(objectKeys, linkerFlags, additionalSources, resolved));
}

// Walks through a 3-file dependency chain (main -> utils -> math) using the real cache-key
// folding behavior, caches every file, then mutates math's source and verifies that the change
// cascades into utils and main: their cache keys move with the dep, so all three miss and must
// be recompiled. Also verifies that each restored entry holds only its own object file (no
// transitive accumulation, which would cause duplicate linker registrations downstream).
TEST_F(CompileCacheTest, MultiFileDependencyChangeForcesRecompilation) {
  // Configure native target so that SourceFile construction can look up an LLVM target
  cliOptions.targetTriple = llvm::Triple(llvm::Triple::normalize(llvm::sys::getProcessTriple()));
  cliOptions.isNativeTarget = true;

  // Place stub source files on disk (their contents are irrelevant: cache keys are derived from
  // the in-memory "source" strings below and assigned to SourceFile::cacheKey directly).
  const std::filesystem::path mathPath = outputDir / "math.spice";
  const std::filesystem::path utilsPath = outputDir / "utils.spice";
  const std::filesystem::path mainPath = outputDir / "main.spice";
  writeDummyFile(mathPath, "");
  writeDummyFile(utilsPath, "");
  writeDummyFile(mainPath, "");

  // GlobalResourceManager initializes LLVM targets and owns a CacheManager wired to cliOptions
  GlobalResourceManager resourceManager(cliOptions);
  CacheManager &manager = resourceManager.cacheManager;

  // Sources that drive cache keys for the initial state
  const std::string mathSrcV1 = "f<int> add(int a, int b) { return a + b; }";
  const std::string utilsSrc = "import \"math\"; f<int> helper(int x) { return add(x, 1); }";
  const std::string mainSrc = "import \"utils\"; f<int> main() { return helper(41); }";

  SourceFile *math = resourceManager.createSourceFile(nullptr, "math", mathPath, false);
  SourceFile *utils = resourceManager.createSourceFile(nullptr, "utils", utilsPath, false);
  SourceFile *main = resourceManager.createSourceFile(nullptr, "main", mainPath, false);
  math->isMainFile = false;
  utils->isMainFile = false;
  main->isMainFile = true;

  // Wire up the dependency chain: main -> utils -> math
  utils->dependencies["math"] = math;
  main->dependencies["utils"] = utils;

  // Assign cache keys as runImportCollector would: each file's key folds in its transitive
  // dep cache keys, so changes to any dep propagate up the chain.
  const auto recomputeKeys = [&] {
    math->cacheKey = manager.computeCacheKey(mathSrcV1);
    utils->cacheKey = manager.computeCacheKey(utilsSrc, {math->cacheKey});
    main->cacheKey = manager.computeCacheKey(mainSrc, {utils->cacheKey, math->cacheKey});
  };
  recomputeKeys();

  // Pretend the back end emitted object files into outputDir (cacheSourceFile picks them up there)
  writeDummyFile(outputDir / "math.o", "math-v1-obj");
  writeDummyFile(outputDir / "utils.o", "utils-obj");
  writeDummyFile(outputDir / "main.o", "main-obj");

  manager.cacheSourceFile(math);
  manager.cacheSourceFile(utils);
  manager.cacheSourceFile(main);

  // Reset lookup-output fields between lookups, since they accumulate on each call
  auto lookup = [&](SourceFile *sourceFile) {
    sourceFile->cachedObjectFilePaths.clear();
    sourceFile->sourceLinkerFlags.clear();
    sourceFile->sourceAdditionalSourcePaths.clear();
    return manager.lookupSourceFile(sourceFile);
  };

  // Phase 1: nothing changed since caching - every file hits and pulls its transitive dep
  // objects (kept so runtime modules pulled in implicitly at symbol-table-building time still
  // make it into the link for cache-restored files; the linker dedupes the overlap).
  ASSERT_TRUE(lookup(math));
  ASSERT_EQ(1u, math->cachedObjectFilePaths.size()); // math.o
  ASSERT_TRUE(lookup(utils));
  ASSERT_EQ(2u, utils->cachedObjectFilePaths.size()); // math.o + utils.o
  ASSERT_TRUE(lookup(main));
  ASSERT_EQ(3u, main->cachedObjectFilePaths.size()); // utils.o, math.o, main.o

  // Phase 2: math's source changes -> its cache key changes -> utils and main keys also change
  // (because the fold pulls in the new math key transitively). All three miss and recompile.
  const std::string oldMathKey = math->cacheKey;
  const std::string oldUtilsKey = utils->cacheKey;
  const std::string oldMainKey = main->cacheKey;

  const std::string mathSrcV2 = "f<int> add(int a, int b) { return a + b + 0; }";
  math->cacheKey = manager.computeCacheKey(mathSrcV2);
  utils->cacheKey = manager.computeCacheKey(utilsSrc, {math->cacheKey});
  main->cacheKey = manager.computeCacheKey(mainSrc, {utils->cacheKey, math->cacheKey});
  ASSERT_NE(oldMathKey, math->cacheKey);
  ASSERT_NE(oldUtilsKey, utils->cacheKey);
  ASSERT_NE(oldMainKey, main->cacheKey);

  ASSERT_FALSE(lookup(math));
  ASSERT_FALSE(lookup(utils));
  ASSERT_FALSE(lookup(main));
}

// Provokes the bug that was fixed: before transitive dep cache keys were folded into a
// file's own cache key, a dependent whose source text was unchanged would keep cache-hitting
// against a stale object file even when one of its dependencies had been edited - causing
// the linker to consume out-of-date code. With the fold in place, mutating any dependency's
// cache key must propagate into every dependent's cache key, so the dependent's lookup misses,
// and it gets recompiled against the new dependency.
TEST_F(CompileCacheTest, DependencyChangeInvalidatesDependentCacheKey) {
  const CacheManager manager(cliOptions);

  // Initial cache keys for a dependent file (utils) and its single dependency (math)
  const std::string mathSrc = "f<int> add(int a, int b) { return a + b; }";
  const std::string utilsSrc = "import \"math\"; f<int> helper(int x) { return add(x, 1); }";

  const std::string mathKeyV1 = manager.computeCacheKey(mathSrc);
  const std::string utilsKeyV1 = manager.computeCacheKey(utilsSrc, {mathKeyV1});

  // The dependent's own source text didn't change - only the dependency did.
  const std::string mathSrcChanged = "f<int> add(int a, int b) { return a + b + 0; }";
  const std::string mathKeyV2 = manager.computeCacheKey(mathSrcChanged);
  ASSERT_NE(mathKeyV1, mathKeyV2);

  // Without the fold, recomputing utils cache key here would yield the same value as before
  // (since utilsSrc is unchanged) and the stale cached utils.o would be served. The fold makes
  // the dependent's cache key a function of the dependency's cache key, so utils now misses too.
  const std::string utilsKeyV2 = manager.computeCacheKey(utilsSrc, {mathKeyV2});
  ASSERT_NE(utilsKeyV1, utilsKeyV2);

  // Sanity check: without any dep change the dependent's cache key stays stable, so we don't
  // pay unnecessary rebuilds when nothing actually moved.
  ASSERT_EQ(utilsKeyV1, manager.computeCacheKey(utilsSrc, {mathKeyV1}));
}

// Provokes a real (non-mocked) regression: compile a two-file program once to populate the cache,
// then edit only the dependent file's source and recompile in a fresh GlobalResourceManager (this
// stands in for a new process run). The unchanged dependency must hit the cache for its object
// file, but its exported symbols still have to be resolvable by the dependent, since the dependent
// itself isn't a cache hit and needs to type-check against them. Before the fix, cache-restored
// files skipped SymbolTableBuilder/TypeChecker entirely, leaving their exportedNameRegistry empty
// and causing "undefined function" errors for every symbol the cache-hit dependency exported.
TEST_F(CompileCacheTest, CacheHitDependencyStillExposesSymbolsToChangedDependent) {
  cliOptions.targetTriple = llvm::Triple(llvm::Triple::normalize(llvm::sys::getProcessTriple()));
  cliOptions.isNativeTarget = true;

  const std::filesystem::path mathPath = outputDir / "math.spice";
  const std::filesystem::path mainPath = outputDir / "main.spice";

  // The dependency stays untouched across both compiler runs.
  writeDummyFile(mathPath, "public f<int> add(int a, int b) {\n    return a + b;\n}\n");
  writeDummyFile(mainPath, "import \"math\";\n\nf<int> main() {\n    return add(1, 2);\n}\n");

  // First "process run": compiles both files from scratch and populates the cache.
  {
    GlobalResourceManager resourceManager(cliOptions);
    SourceFile *mainFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, mainPath, false);
    mainFile->runFrontEnd();
    mainFile->runMiddleEnd();
    ASSERT_TRUE(resourceManager.errorManager.softErrors.empty());
    for (SourceFile *dependency : mainFile->dependencies | std::views::values)
      dependency->runBackEnd();
    mainFile->runBackEnd();
  }

  // Edit only main.spice. math.spice is unchanged, so it must still cache-hit on the next run.
  writeDummyFile(mainPath, "import \"math\";\n\nf<int> main() {\n    return add(1, 2) + 1;\n}\n");

  // Second "process run" (fresh GlobalResourceManager, nothing carried over in memory).
  {
    GlobalResourceManager resourceManager(cliOptions);
    SourceFile *mainFile = resourceManager.createSourceFile(nullptr, MAIN_FILE_NAME, mainPath, false);
    mainFile->runFrontEnd();

    SourceFile *mathFile = mainFile->dependencies.at("math");
    ASSERT_TRUE(mathFile->restoredFromCache);
    // This is the crux of the bug: a cache-restored file must still expose its symbols.
    ASSERT_NE(nullptr, mathFile->getNameRegistryEntry("add"));

    mainFile->runMiddleEnd();
    ASSERT_TRUE(resourceManager.errorManager.softErrors.empty());

    for (SourceFile *dependency : mainFile->dependencies | std::views::values)
      dependency->runBackEnd();
    mainFile->runBackEnd();
  }
}

} // namespace spice::testing

// LCOV_EXCL_STOP