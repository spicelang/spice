// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <driver/Driver.h>

namespace spice::compiler {

std::string IRGenerator::getSysCallAsmString(uint8_t numRegs) const {
  const llvm::Triple &targetTriple = cliOptions.targetTriple;
  std::stringstream asmString;

  // For each OS/architecture we have a mapping of operand index -> register.
  if (targetTriple.isOSDarwin()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *regs[] = {"%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
      for (uint8_t i = 0; i < numRegs; i++) {
        // macOS syscall numbers are offset by 0x2000000
        if (i == 0)
          asmString << "movq $(" << std::to_string(0x2000000) << " + " << std::to_string(i) << "), " << regs[i] << "\n";
        else
          asmString << "movq $" << std::to_string(i) << ", " << regs[i] << "\n";
      }
      asmString << "syscall\n";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *regs[] = {"x16", "x0", "x1", "x2", "x3", "x4", "x5"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "mov " << regs[i] << ", $" << std::to_string(i) << "\n";
      asmString << "svc #0x80\n";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported macOS target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else if (targetTriple.isOSLinux()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *regs[] = {"%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "movq $" << std::to_string(i) << ", " << regs[i] << "\n";
      asmString << "syscall\n";
    } else if (targetTriple.getArch() == llvm::Triple::ArchType::x86) {
      // Note: Using movl for 32-bit registers.
      static constexpr const char *regs[] = {"%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "%ebp"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "movl $" << std::to_string(i) << ", " << regs[i] << "\n";
      asmString << "int $0x80\n";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *regs[] = {"x8", "x0", "x1", "x2", "x3", "x4", "x5"};
      for (uint8_t i = 0; i < numRegs; ++i)
        asmString << "mov " << regs[i] << ", $" << std::to_string(i) << "\n";
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
  std::stringstream constraints;

  // Generate a comma-separated constraint string: first the operand constraints,
  // then the corresponding clobbers, then extra clobbers.
  if (targetTriple.isOSDarwin()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *clobbers[] = {"~{rax}", "~{rdi}", "~{rsi}", "~{rdx}", "~{r10}", "~{r8}", "~{r9}"};
      for (uint8_t i = 0; i < numRegs; i++) {
        constraints << "r";
        if (i != numRegs - 1)
          constraints << ",";
      }
      for (uint8_t i = 0; i < numRegs; i++)
        constraints << "," << clobbers[i];
      constraints << ",~{dirflag},~{fpsr},~{flags}";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *clobbers[] = {"~{x16}", "~{x0}", "~{x1}", "~{x2}", "~{x3}", "~{x4}", "~{x5}"};
      for (uint8_t i = 0; i < numRegs; i++) {
        constraints << "r";
        if (i != numRegs - 1)
          constraints << ",";
      }
      for (uint8_t i = 0; i < numRegs; i++)
        constraints << "," << clobbers[i];
      constraints << ",~{dirflag},~{fpsr},~{flags}";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported macOS target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else if (targetTriple.isOSLinux()) {
    if (targetTriple.getArch() == llvm::Triple::ArchType::x86_64) {
      static constexpr const char *clobbers[] = {"~{rax}", "~{rdi}", "~{rsi}", "~{rdx}", "~{r10}", "~{r8}", "~{r9}"};
      for (uint8_t i = 0; i < numRegs; i++) {
        constraints << "r";
        if (i != numRegs - 1)
          constraints << ",";
      }
      for (uint8_t i = 0; i < numRegs; i++)
        constraints << "," << clobbers[i];
      constraints << ",~{dirflag},~{fpsr},~{flags}";
    } else if (targetTriple.getArch() == llvm::Triple::ArchType::x86) {
      static constexpr const char *clobbers[] = {"~{eax}", "~{ebx}", "~{ecx}", "~{edx}", "~{esi}", "~{edi}", "~{ebp}"};
      for (uint8_t i = 0; i < numRegs; i++) {
        constraints << "r";
        if (i != numRegs - 1)
          constraints << ",";
      }
      for (uint8_t i = 0; i < numRegs; i++)
        constraints << "," << clobbers[i];
      constraints << ",~{dirflag},~{fpsr},~{flags}";
    } else if (targetTriple.isAArch64()) {
      static constexpr const char *clobbers[] = {"~{x8}", "~{x0}", "~{x1}", "~{x2}", "~{x3}", "~{x4}", "~{x5}"};
      for (uint8_t i = 0; i < numRegs; i++) {
        constraints << "r";
        if (i != numRegs - 1)
          constraints << ",";
      }
      for (uint8_t i = 0; i < numRegs; i++)
        constraints << "," << clobbers[i];
      constraints << ",~{dirflag},~{fpsr},~{flags}";
    } else {                                                       // LCOV_EXCL_LINE
      assert_fail("Unsupported Linux target for inline assembly"); // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  } else {                                                 // LCOV_EXCL_LINE
    assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  return constraints.str();
}

} // namespace spice::compiler
