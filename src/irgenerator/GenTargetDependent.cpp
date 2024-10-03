// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <driver/Driver.h>

namespace spice::compiler {

const char *IRGenerator::getSysCallAsmString() const {
  if (cliOptions.targetArch == "x86_64" || cliOptions.targetArch == "amd64")
    return "movq $0, %rax\n"
           "movq $1, %rdi\n"
           "movq $2, %rsi\n"
           "movq $3, %rdx\n"
           "movq $4, %r10\n"
           "movq $5, %r8\n"
           "movq $6, %r9\n"
           "syscall\n";

  if (cliOptions.targetArch == "x86" || cliOptions.targetArch == "i386") // ToDo: Test this on an actual 32 bit x86 CPU
    return "movq $0, %eax\n"
           "movq $1, %ebx\n"
           "movq $2, %ecx\n"
           "movq $3, %edx\n"
           "movq $4, %esi\n"
           "movq $5, %edi\n"
           "movq $6, %ebp\n"
           "syscall\n";

  if (cliOptions.targetArch == "aarch64" || cliOptions.targetArch == "arm64") // ToDo: Test this on an actual AArch64 CPU
    return "movq $0, %rax\n"
           "movq $1, %rdi\n"
           "movq $2, %rsi\n"
           "movq $3, %rdx\n"
           "movq $4, %r10\n"
           "movq $5, %r8\n"
           "movq $6, %r9\n"
           "syscall\n";

  assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  return nullptr;                                        // LCOV_EXCL_LINE
}

const char *IRGenerator::getSysCallConstraintString() const {
  if (cliOptions.targetArch == "x86_64" || cliOptions.targetArch == "amd64")
    return "r,r,r,r,r,r,r,~{rax},~{rdi},~{rsi},~{rdx},~{r10},~{r8},~{r9},~{dirflag},~{fpsr},~{flags}";
  if (cliOptions.targetArch == "x86" || cliOptions.targetArch == "i386")
    return "r,r,r,r,r,r,r,~{eax},~{ebx},~{ecx},~{edx},~{esi},~{edi},~{ebp},~{dirflag},~{fpsr},~{flags}";
  if (cliOptions.targetArch == "aarch64" || cliOptions.targetArch == "arm64")
    return "r,r,r,r,r,r,r,~{x8},~{x0},~{x1},~{x2},~{x3},~{x4},~{x5},~{dirflag},~{fpsr},~{flags}";
  assert_fail("Unsupported target for inline assembly"); // LCOV_EXCL_LINE
  return nullptr;                                        // LCOV_EXCL_LINE
}

} // namespace spice::compiler
