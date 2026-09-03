// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CoverageWorkingDirFileSystem.h"

#include <llvm/Support/VirtualFileSystem.h>

namespace spice::compiler {

namespace {
// LCOV_EXCL_START
class FixedWorkingDirFileSystem final : public llvm::vfs::ProxyFileSystem {
public:
  FixedWorkingDirFileSystem(llvm::IntrusiveRefCntPtr<FileSystem> fs, std::string workingDir)
      : ProxyFileSystem(std::move(fs)), workingDir(std::move(workingDir)) {}

  llvm::ErrorOr<std::string> getCurrentWorkingDirectory() const override { return workingDir; }

private:
  std::string workingDir;
};
// LCOV_EXCL_STOP
} // namespace

llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> createFixedWorkingDirFileSystem(std::string workingDir) {
  return new FixedWorkingDirFileSystem(llvm::vfs::getRealFileSystem(), std::move(workingDir)); // GCOV_EXCL_LINE
}

} // namespace spice::compiler
