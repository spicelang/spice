/* C API wrapper around the TPDE LLVM back end (tpde-llvm/LLVMCompiler.hpp).
 *
 * TPDE only offers a C++ API, so Spice can't bind to it directly. These functions expose it in the style of the
 * LLVM C API: they take the LLVM C handles, return true on failure and hand out error messages, that have to be
 * released with LLVMDisposeMessage.
 *
 * TPDE emits ELF objects for x86_64 and aarch64 only. Where its headers are not available (e.g. on macOS and Windows,
 * or when TPDE_INCLUDE_DIRS is not set), the functions are stubs, that report TPDE as unavailable.
 */
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <llvm-c/Core.h>

#if __has_include(<tpde-llvm/LLVMCompiler.hpp>)
#define SPICE_TPDE_AVAILABLE 1
#include <llvm/IR/Module.h>
#include <llvm/TargetParser/Triple.h>
#include <tpde-llvm/LLVMCompiler.hpp>
#else
#define SPICE_TPDE_AVAILABLE 0
#endif

extern "C" {

typedef struct TPDEOpaqueCompiler *TPDECompilerRef;

/* Check if this build of the bindings is backed by TPDE */
LLVMBool TPDEIsAvailable(void) { return SPICE_TPDE_AVAILABLE; }

#if SPICE_TPDE_AVAILABLE

/* Create a compiler for the given target triple. Returns null if TPDE does not support the triple */
TPDECompilerRef TPDECreateCompiler(const char *triple) {
  std::unique_ptr<tpde_llvm::LLVMCompiler> compiler = tpde_llvm::LLVMCompiler::create(llvm::Triple(triple));
  return reinterpret_cast<TPDECompilerRef>(compiler.release());
}

void TPDEDisposeCompiler(TPDECompilerRef compiler) { delete reinterpret_cast<tpde_llvm::LLVMCompiler *>(compiler); }

static bool compileModule(TPDECompilerRef compiler, LLVMModuleRef module, std::vector<uint8_t> &buffer, char **errorMessage) {
  llvm::Module *mod = llvm::unwrap(module);
  if (!reinterpret_cast<tpde_llvm::LLVMCompiler *>(compiler)->compile_to_elf(*mod, buffer)) {
    const std::string message = "TPDE failed to compile module '" + mod->getName().str() + "'";
    *errorMessage = LLVMCreateMessage(message.c_str());
    return false;
  }
  return true;
}

/* Compile the module to an ELF object file. The module might be modified during compilation */
LLVMBool TPDECompileToObjectFile(TPDECompilerRef compiler, LLVMModuleRef module, const char *filename, char **errorMessage) {
  std::vector<uint8_t> buffer;
  if (!compileModule(compiler, module, buffer, errorMessage))
    return 1;
  FILE *file = std::fopen(filename, "wb");
  if (file == nullptr) {
    const std::string message = "File '" + std::string(filename) + "' could not be opened";
    *errorMessage = LLVMCreateMessage(message.c_str());
    return 1;
  }
  const size_t written = std::fwrite(buffer.data(), 1, buffer.size(), file);
  const bool closeFailed = std::fclose(file) != 0;
  if (written != buffer.size() || closeFailed) {
    const std::string message = "File '" + std::string(filename) + "' could not be written";
    *errorMessage = LLVMCreateMessage(message.c_str());
    return 1;
  }
  return 0;
}

/* Compile the module to an ELF object in memory. The module might be modified during compilation */
LLVMBool TPDECompileToMemoryBuffer(TPDECompilerRef compiler, LLVMModuleRef module, LLVMMemoryBufferRef *outMemBuf,
                                   char **errorMessage) {
  std::vector<uint8_t> buffer;
  if (!compileModule(compiler, module, buffer, errorMessage))
    return 1;
  const char *data = reinterpret_cast<const char *>(buffer.data());
  *outMemBuf = LLVMCreateMemoryBufferWithMemoryRangeCopy(data, buffer.size(), "tpde-object");
  return 0;
}

#else

TPDECompilerRef TPDECreateCompiler(const char *) { return nullptr; }

void TPDEDisposeCompiler(TPDECompilerRef) {}

static LLVMBool reportUnavailable(char **errorMessage) {
  *errorMessage = LLVMCreateMessage("The TPDE bindings were built without TPDE");
  return 1;
}

LLVMBool TPDECompileToObjectFile(TPDECompilerRef, LLVMModuleRef, const char *, char **errorMessage) {
  return reportUnavailable(errorMessage);
}

LLVMBool TPDECompileToMemoryBuffer(TPDECompilerRef, LLVMModuleRef, LLVMMemoryBufferRef *outMemBuf, char **errorMessage) {
  *outMemBuf = nullptr;
  return reportUnavailable(errorMessage);
}

#endif

} // extern "C"
