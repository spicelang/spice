// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "GlobalResourceManager.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/TypeRegistry.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/StructManager.h>
#include <util/FileUtil.h>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

namespace spice::compiler {

GlobalResourceManager::GlobalResourceManager(const CliOptions &cliOptions)
    : cliOptions(cliOptions), linker(cliOptions), cacheManager(cliOptions.cacheDir), runtimeModuleManager(*this) {
  // Initialize the required LLVM targets
  if (cliOptions.isNativeTarget) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
  } else { // GCOV_EXCL_START
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
  } // GCOV_EXCL_STOP

  // Search after selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw CompilerError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  // Create target machine for LLVM
  llvm::TargetOptions opt;
  opt.MCOptions.AsmVerbose = true;
  opt.MCOptions.PreserveAsmComments = true;
  std::string cpuName = "generic";
  std::stringstream featureString;
  if (cliOptions.isNativeTarget && cliOptions.useCPUFeatures) {
    // Retrieve native CPU name and the supported CPU features
    cpuName = llvm::sys::getHostCPUName();
    llvm::StringMap<bool> hostFeatures;
    llvm::sys::getHostCPUFeatures(hostFeatures);
    for (const llvm::StringMapEntry<bool> &feature : hostFeatures) {
      if (featureString.rdbuf()->in_avail() > 0)
        featureString << ",";
      featureString << (feature.second ? "+" : "-") << feature.first().str();
    }
  }

  // Create target machine
  const std::string features = featureString.str();
  const std::string &targetTriple = cliOptions.targetTriple;
  llvm::TargetMachine *targetMachineRaw = target->createTargetMachine(targetTriple, cpuName, features, opt, llvm::Reloc::PIC_);
  targetMachine = std::unique_ptr<llvm::TargetMachine>(targetMachineRaw);

  // Create lto module
  if (cliOptions.useLTO)
    ltoModule = std::make_unique<llvm::Module>(LTO_FILE_NAME, context);
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
  const std::string filePathStr = std::filesystem::weakly_canonical(std::filesystem::absolute(path)).string();

  // Create the new source file if it does not exist yet
  if (!sourceFiles.contains(filePathStr))
    sourceFiles.insert({filePathStr, std::make_unique<SourceFile>(*this, parent, depName, path, isStdFile)});

  return sourceFiles.at(filePathStr).get();
}

uint64_t GlobalResourceManager::getNextCustomTypeId() { return nextCustomTypeId++; }

size_t GlobalResourceManager::getTotalLineCount() const {
  const auto acc = [](size_t sum, const auto &sourceFile) { return sum + FileUtil::getLineCount(sourceFile.second->filePath); };
  return std::accumulate(sourceFiles.begin(), sourceFiles.end(), 0, acc);
}

} // namespace spice::compiler