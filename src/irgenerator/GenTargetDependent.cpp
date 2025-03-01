// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <driver/Driver.h>

namespace spice::compiler {

std::string IRGenerator::getSysCallAsmString(uint8_t numRegs) const {
  // For each architecture we have a mapping of operand index -> register.
  std::stringstream asmString;
  if (cliOptions.targetArch == "x86_64" || cliOptions.targetArch == "amd64") {
    static constexpr const char *regs[] = {"%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
    for (uint8_t i = 0; i < numRegs; ++i)
      asmString << "movq $" << std::to_string(i) << ", " << regs[i] << "\n";
    asmString << "syscall\n";
  } else if (cliOptions.targetArch == "x86" || cliOptions.targetArch == "i386") {
    // Note: Using movl for 32-bit registers.
    static constexpr const char *regs[] = {"%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "%ebp"};
    for (uint8_t i = 0; i < numRegs; ++i)
      asmString << "movl $" << std::to_string(i) << ", " << regs[i] << "\n";
    asmString << "syscall\n";
  } else if (cliOptions.targetArch == "aarch64" || cliOptions.targetArch == "arm64") {
    // Mapping: operand 0 -> x8, then operands 1..6 -> x0,x1,...,x5.
    static constexpr const char *regs[] = {"x8", "x0", "x1", "x2", "x3", "x4", "x5"};
    for (uint8_t i = 0; i < numRegs; ++i)
      asmString << "mov " << regs[i] << ", $" << std::to_string(i) << "\n";
    asmString << "svc 0\n";
  } else {                                                 // LCOV_EXCL_LINE
    assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE
  return asmString.str();
}

std::string IRGenerator::getSysCallConstraintString(uint8_t numRegs) const {
  std::stringstream constraints;

  // Generate a comma-separated constraint string: first the operand constraints,
  // then the corresponding clobbers, then extra clobbers.
  if (cliOptions.targetArch == "x86_64" || cliOptions.targetArch == "amd64") {
    // Mapping of operand i to its clobber.
    static constexpr const char *clobbers[] = {"~{rax}", "~{rdi}", "~{rsi}", "~{rdx}", "~{r10}", "~{r8}", "~{r9}"};
    // Operand constraints: "r" for each operand.
    for (uint8_t i = 0; i < numRegs; i++) {
      constraints << "r";
      if (i != numRegs - 1)
        constraints << ",";
    }
    // Append corresponding clobbers.
    for (uint8_t i = 0; i < numRegs; i++)
      constraints << "," << clobbers[i];
    // Append extra clobbers.
    constraints << ",~{dirflag},~{fpsr},~{flags}";
  } else if (cliOptions.targetArch == "x86" || cliOptions.targetArch == "i386") {
    static constexpr const char *clobbers[] = {"~{eax}", "~{ebx}", "~{ecx}", "~{edx}", "~{esi}", "~{edi}", "~{ebp}"};
    for (uint8_t i = 0; i < numRegs; i++) {
      constraints << "r";
      if (i != numRegs - 1)
        constraints << ",";
    }
    for (uint8_t i = 0; i < numRegs; i++)
      constraints << "," << clobbers[i];
    constraints << ",~{dirflag},~{fpsr},~{flags}";
  } else if (cliOptions.targetArch == "aarch64" || cliOptions.targetArch == "arm64") {
    static constexpr const char *clobbers[] = {"~{x8}", "~{x0}", "~{x1}", "~{x2}", "~{x3}", "~{x4}", "~{x5}"};
    for (uint8_t i = 0; i < numRegs; i++) {
      constraints << "r";
      if (i != numRegs - 1)
        constraints << ",";
    }
    for (uint8_t i = 0; i < numRegs; i++)
      constraints << "," << clobbers[i];
    constraints << ",~{dirflag},~{fpsr},~{flags}";
  } else {                                                 // LCOV_EXCL_LINE
    assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE
  return constraints.str();
}

} // namespace spice::compiler
