/* llvm-c/Target.h helper functions wrappers.
 *
 * The LLVMInitializeAll* functions and friends are defined `static inline`, so
 * we can't bind directly to them (the function body is generated via macro),
 * so here are some wrappers.
 */
#include <llvm-c/Target.h>

/* The LLVMInitializeAll* functions of llvm-c/Target.h initialize every target of the LLVM build (see
 * llvm/Config/Targets.def), but these bindings only link the libraries of the AArch64, WebAssembly and X86 targets (see
 * linker-flags.spice). With an LLVM that was built with more targets, they reference libraries that are not linked, so
 * the "All" wrappers initialize exactly the linked targets instead. Keep them in sync with linker-flags.spice. */

/* Declared here as well, because llvm-c/Target.h only declares them for the targets of the LLVM build */
#define SPICE_DECLARE_TARGET(Name)                                                                                               \
  LLVM_C_ABI void LLVMInitialize##Name##TargetInfo(void);                                                                        \
  LLVM_C_ABI void LLVMInitialize##Name##Target(void);                                                                            \
  LLVM_C_ABI void LLVMInitialize##Name##TargetMC(void);                                                                          \
  LLVM_C_ABI void LLVMInitialize##Name##AsmPrinter(void);                                                                        \
  LLVM_C_ABI void LLVMInitialize##Name##AsmParser(void);                                                                         \
  LLVM_C_ABI void LLVMInitialize##Name##Disassembler(void);
SPICE_DECLARE_TARGET(AArch64)
SPICE_DECLARE_TARGET(WebAssembly)
SPICE_DECLARE_TARGET(X86)
#undef SPICE_DECLARE_TARGET

void LLVM_InitializeAllTargetInfos(void) {
  LLVMInitializeAArch64TargetInfo();
  LLVMInitializeWebAssemblyTargetInfo();
  LLVMInitializeX86TargetInfo();
}

void LLVM_InitializeAllTargets(void) {
  LLVMInitializeAArch64Target();
  LLVMInitializeWebAssemblyTarget();
  LLVMInitializeX86Target();
}

void LLVM_InitializeAllTargetMCs(void) {
  LLVMInitializeAArch64TargetMC();
  LLVMInitializeWebAssemblyTargetMC();
  LLVMInitializeX86TargetMC();
}

void LLVM_InitializeAllAsmPrinters(void) {
  LLVMInitializeAArch64AsmPrinter();
  LLVMInitializeWebAssemblyAsmPrinter();
  LLVMInitializeX86AsmPrinter();
}

void LLVM_InitializeAllAsmParsers(void) {
  LLVMInitializeAArch64AsmParser();
  LLVMInitializeWebAssemblyAsmParser();
  LLVMInitializeX86AsmParser();
}

void LLVM_InitializeAllDisassemblers(void) {
  LLVMInitializeAArch64Disassembler();
  LLVMInitializeWebAssemblyDisassembler();
  LLVMInitializeX86Disassembler();
}

/* These functions return true on failure. */
LLVMBool LLVM_InitializeNativeTarget(void) { return LLVMInitializeNativeTarget(); }

LLVMBool LLVM_InitializeNativeAsmParser(void) { return LLVMInitializeNativeAsmParser(); }

LLVMBool LLVM_InitializeNativeAsmPrinter(void) { return LLVMInitializeNativeAsmPrinter(); }

LLVMBool LLVM_InitializeNativeDisassembler(void) { return LLVMInitializeNativeDisassembler(); }
