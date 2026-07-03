// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>

// Forward declarations
namespace llvm {
  class Module;
} // namespace llvm

namespace spice::compiler {

/**
 * Thin functional entry-point that compiles an LLVM module to an ELF object via TPDE.
 *
 * Kept intentionally free of Spice-specific headers so the translation unit can be compiled
 * with -fno-rtti (TPDE's compile-option ABI), while the rest of the compiler keeps RTTI enabled.
 *
 * Only compiled when SPICE_ENABLE_TPDE is defined.
 */
namespace tpde_backend {

/// Compile @p module to an ELF object at @p objectPath.
/// Throws spice::compiler::CompilerError on failure.
void emitObjectFile(llvm::Module &module, const std::filesystem::path &objectPath);

/// Return a placeholder assembly listing. TPDE emits object bytes directly and does not
/// expose an assembly stage; @p output is set to a human-readable note.
void getAssemblyString(std::string &output);

} // namespace tpde_backend

} // namespace spice::compiler
