// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>

#include <objectemitter/AbstractObjectEmitter.h>

// Forward declarations
namespace llvm {
  class Module;
} // namespace llvm

namespace spice::compiler {

/**
 * Object emitter backed by the experimental TPDE compiler back-end framework.
 *
 * Consumes an llvm::Module directly and emits an ELF object at roughly -O0 code quality but
 * significantly faster than LLVM CodeGen at -O0. Only compiled when SPICE_ENABLE_TPDE is defined
 * (gated by the SPICE_ENABLE_TPDE CMake option). Supported targets: ELF on x86_64 or aarch64;
 * small code model; PIC relocation only; no LTO.
 *
 * Kept deliberately free of Spice-internal headers other than AbstractObjectEmitter so this class
 * (and its TU) can be compiled with -fno-rtti to match TPDE's ABI without dragging in RTTI-heavy
 * Spice code.
 */
class TPDEObjectEmitter final : public AbstractObjectEmitter {
public:
  // Constructors
  explicit TPDEObjectEmitter(llvm::Module &module);

  // Public methods
  void emit(const std::filesystem::path &objectPath) const override;
  void getASMString(std::string &output) const override;

private:
  // Private members
  llvm::Module &module;
};

} // namespace spice::compiler
