// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <driver/Driver.h>

namespace spice::compiler {

std::string IRGenerator::getSysCallAsmString(uint8_t numRegs) const {
  const llvm::Triple &targetTriple = cliOptions.targetTriple;
  std::stringstream asmString;

  // For each OS/architecture we have a mapping of operand index -> register.
  // Operand $0 is the result, so the inputs start at $1.
  if (targetTriple.isOSDarwin()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *regs[] = {"%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
      for (uint8_t i = 0; i < numRegs; i++) {
        asmString << "movq $" << std::to_string(i + 1) << ", " << regs[i] << "\n";
        // macOS x86_64 syscall numbers are offset by 0x2000000 (BSD syscall class). "$$" emits a literal '$'.
        if (i == 0)
          asmString << "addq $$" << std::to_string(0x2000000) << ", %rax\n";
      }
      asmString << "syscall\n";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *regs[] = {"x16", "x0", "x1", "x2", "x3", "x4", "x5"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "mov " << regs[i] << ", $" << std::to_string(i + 1) << "\n";
      asmString << "svc #0x80\n";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported macOS target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else if (targetTriple.isOSLinux()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *regs[] = {"%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "movq $" << std::to_string(i + 1) << ", " << regs[i] << "\n";
      asmString << "syscall\n";
    } else if (targetTriple.getArch() == llvm::Triple::ArchType::x86) {
      // Note: Using movl for 32-bit registers.
      static constexpr const char *regs[] = {"%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "%ebp"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "movl $" << std::to_string(i + 1) << ", " << regs[i] << "\n";
      asmString << "int $0x80\n";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *regs[] = {"x8", "x0", "x1", "x2", "x3", "x4", "x5"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "mov " << regs[i] << ", $" << std::to_string(i + 1) << "\n";
      asmString << "svc 0\n";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported Linux target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else {                                                 // LCOV_EXCL_LINE
    assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  return asmString.str();
}

std::string IRGenerator::getSysCallConstraintString(uint8_t numRegs) const {
  const llvm::Triple &targetTriple = cliOptions.targetTriple;

  // For each OS/architecture we have a mapping of operand index -> register, plus the register holding the result.
  const char *const *regs = nullptr;
  const char *resultReg = nullptr;
  if (targetTriple.isOSDarwin()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *darwinX86_64Regs[] = {"rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"};
      regs = darwinX86_64Regs;
      resultReg = "rax";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *darwinAArch64Regs[] = {"x16", "x0", "x1", "x2", "x3", "x4", "x5"};
      regs = darwinAArch64Regs;
      resultReg = "x0";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported macOS target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else if (targetTriple.isOSLinux()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *linuxX86_64Regs[] = {"rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"};
      regs = linuxX86_64Regs;
      resultReg = "rax";
    } else if (targetTriple.getArch() == llvm::Triple::ArchType::x86) {
      static constexpr const char *linuxX86Regs[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"};
      regs = linuxX86Regs;
      resultReg = "eax";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *linuxAArch64Regs[] = {"x8", "x0", "x1", "x2", "x3", "x4", "x5"};
      regs = linuxAArch64Regs;
      resultReg = "x0";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported Linux target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else {                                                 // LCOV_EXCL_LINE
    assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  // Generate a comma-separated constraint string: first the result, then the operand constraints,
  // then the clobbers of the used registers, then extra clobbers.
  // The result is early-clobber, because the asm writes the result register before all inputs are read.
  std::stringstream constraints;
  constraints << "=&{" << resultReg << "}";
  for (uint8_t i = 0; i < numRegs; i++)
    constraints << ",r";
  for (uint8_t i = 0; i < numRegs; i++)
    if (std::string_view(regs[i]) != resultReg)
      constraints << ",~{" << regs[i] << "}";
  constraints << ",~{dirflag},~{fpsr},~{flags}";
  return constraints.str();
}

} // namespace spice::compiler
