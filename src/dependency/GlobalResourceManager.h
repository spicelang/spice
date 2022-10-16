// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <dependency/CacheManager.h>
#include <dependency/RuntimeModuleManager.h>
#include <linker/LinkerInterface.h>
#include <util/ThreadFactory.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

// Forward declarations
struct CliOptions;

/**
 * The GlobalResourceManager is instantiated at startup of the compiler and serves as distribution point for globally used assets.
 * Other components of the compiler can request the required global resources from the GlobalResourceManager.
 */
class GlobalResourceManager {
public:
  // Constructors
  explicit GlobalResourceManager(CliOptions &cliOptions)
      : cliOptions(cliOptions), linker(threadFactory, cliOptions), cacheManager(cliOptions.cacheDir) {}

  // Public members
  CliOptions &cliOptions;
  ThreadFactory threadFactory;
  LinkerInterface linker;
  CacheManager cacheManager;
  RuntimeModuleManager runtimeModuleManager;
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder = llvm::IRBuilder<>(context);
};