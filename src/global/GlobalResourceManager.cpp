// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <SourceFile.h>
#include <global/TypeRegistry.h>
#include <llvm/IR/InlineAsm.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/StructManager.h>
#include <util/FileUtil.h>

#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(cliOptions), cacheManager(cliOptions.cacheDir), runtimeModuleManager(*this) {
  // Initialize the required LLVM targets
  if (cliOptions.isNativeTarget) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
  } else { // GCOV_EXCL_START
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
  } // GCOV_EXCL_STOP

  // Create cpu name and features strings
  cpuName = "generic";
  std::stringstream featureString;
  if (cliOptions.isNativeTarget && cliOptions.useCPUFeatures) {
    // Retrieve native CPU name and the supported CPU features
    cpuName = llvm::sys::getHostCPUName();
    for (const llvm::StringMapEntry<bool> &feature : llvm::sys::getHostCPUFeatures()) {
      if (featureString.rdbuf()->in_avail() > 0)
        featureString << ',';
      featureString << (feature.second ? '+' : '-') << feature.first().str();
    }
  }
  cpuFeatures = featureString.str();

  // Create lto module
  if (cliOptions.useLTO)
    ltoModule = std::make_unique<llvm::Module>(LTO_FILE_NAME, ltoContext);
}

GlobalResourceManager::~GlobalResourceManager() {
  // Cleanup all statics
  TypeRegistry::clear();
  FunctionManager::clear();
  StructManager::clear();
  InterfaceManager::clear();
  // Cleanup all LLVM statics
  llvm::llvm_shutdown();
}

SourceFile *GlobalResourceManager::createSourceFile(SourceFile *parent, const std::string &depName,
                                                    const std::filesystem::path &path, bool isStdFile) {
  // Check if the source file was already added (e.g. by another source file that imports it)
  const std::string filePathStr = weakly_canonical(absolute(path)).string();

  // Create the new source file if it does not exist yet
  if (!sourceFiles.contains(filePathStr))
    sourceFiles[filePathStr] = std::make_unique<SourceFile>(*this, parent, depName, path, isStdFile);

  return sourceFiles.at(filePathStr).get();
}

/**
 * Bring the source files into an order, so that a source file is only visited, as soon as all its dependencies are visited.
 * We use Kahn's Algorithm for that, which is a graph algorithm that works on directed acyclic graphs (DAGs) and performs a
 * BFS on the graph, while iteratively pushes source files onto a stack, that have no incoming edges and subsequently removing
 * them from the graph.
 */
void GlobalResourceManager::enqueueSourceFilesForTypeChecking() {
  assert(!sourceFiles.empty());
  sourceFileVisitQueue.clear();

  // Create map for in degree for each source file
  std::unordered_map<SourceFile *, unsigned int> inDegreeMap;
  inDegreeMap.reserve(sourceFiles.size());
  for (const std::unique_ptr<SourceFile> &file : sourceFiles | std::views::values)
    inDegreeMap[file.get()] = file->dependants.size();

  // Push the root source file to the queue, because we start iterating there
  SourceFile *rootSourceFile = sourceFiles.begin()->second->getRootSourceFile();
  assert(rootSourceFile != nullptr);
  std::queue<SourceFile *> inDegreeZeroQueue;
  inDegreeZeroQueue.push(rootSourceFile);

  // Process the graph
  while (!inDegreeZeroQueue.empty()) {
    // Pick the next source file from the zero in degree queue
    SourceFile *sourceFile = inDegreeZeroQueue.front();
    inDegreeZeroQueue.pop();

    // Push the current source file to the front of the queue
    sourceFileVisitQueue.push_front(sourceFile);

    // For all neighbors, decrease their in degree
    for (SourceFile *dependency : sourceFile->dependencies | std::views::values)
      if (--inDegreeMap.at(dependency) == 0)
        inDegreeZeroQueue.push(dependency);
  }

  assert(sourceFileVisitQueue.size() == sourceFiles.size());
}

void GlobalResourceManager::requestRevisitOf(SourceFile *sourceFile) {
  if (!std::ranges::contains(sourceFileVisitQueue, sourceFile))
    sourceFileVisitQueue.push_back(sourceFile);
}

uint64_t GlobalResourceManager::getNextCustomTypeId() { return nextCustomTypeId++; }

size_t GlobalResourceManager::getTotalLineCount() const {
  const auto acc = [](size_t sum, const auto &sourceFile) { return sum + FileUtil::getLineCount(sourceFile.second->filePath); };
  return std::accumulate(sourceFiles.begin(), sourceFiles.end(), 0, acc);
}

} // namespace spice::compiler