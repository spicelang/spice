// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <llvm/ADT/IntrusiveRefCntPtr.h>

namespace llvm::vfs {
class FileSystem;
} // namespace llvm::vfs

namespace spice::compiler {

/**
 * GCOVProfilerPass always resolves the .gcno/.gcda output location as "current working directory + source file
 * basename" (see llvm/lib/Transforms/Instrumentation/GCOVProfiling.cpp: GCOVProfiler::mangleName()), ignoring the
 * requested object file output directory entirely. This creates a filesystem view that reports the given directory
 * as the current working directory instead of the process' real one, so coverage files land next to the rest of the
 * compiler output regardless of where the compiler process was started from - without touching the real,
 * process-wide working directory, which would affect unrelated, concurrently running code.
 *
 * @param workingDir Directory to report as the current working directory
 * @return A filesystem view wrapping the real filesystem, with the working directory pinned to workingDir
 */
llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> createFixedWorkingDirFileSystem(std::string workingDir);

} // namespace spice::compiler
