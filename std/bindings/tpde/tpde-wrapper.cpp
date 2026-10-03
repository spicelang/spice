/* C API wrapper around the TPDE LLVM back end (tpde-llvm/LLVMCompiler.hpp).
 *
 * TPDE only offers a C++ API, so Spice can't bind to it directly. These functions expose it in the style of the
 * LLVM C API: they take the LLVM C handles, return true on failure and hand out error messages, that have to be
 * released with LLVMDisposeMessage.
 *
 * TPDE emits ELF objects for x86_64 and aarch64 only, so it is only used on Linux. The TPDE headers and libraries both
 * come from the TPDE_FLAGS environment variable (see linker-flags.spice), so the headers are only visible if the libraries
 * are linked as well. Otherwise, the functions are stubs, that report TPDE as unavailable.
 */
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <llvm-c/Core.h>

#if defined(__linux__) && __has_include(<tpde-llvm/LLVMCompiler.hpp>)
#define SPICE_TPDE_AVAILABLE 1
#include <llvm/IR/Module.h>
#include <llvm/TargetParser/Triple.h>
#include <tpde-llvm/LLVMCompiler.hpp>
#else
#define SPICE_TPDE_AVAILABLE 0
#endif

extern "C" {
typedef struct TPDEOpaqueCompiler *TPDECompilerRef;
}

namespace {

#if SPICE_TPDE_AVAILABLE
// Compile the module to an ELF object in the given buffer. Returns true on success
bool compileModule(TPDECompilerRef compiler, LLVMModuleRef module, std::vector<uint8_t> &buffer, char **errorMessage) {
  llvm::Module *mod = llvm::unwrap(module);
  if (!reinterpret_cast<tpde_llvm::LLVMCompiler *>(compiler)->compile_to_elf(*mod, buffer)) {
    const std::string message = "TPDE failed to compile module '" + mod->getName().str() + "'";
    *errorMessage = LLVMCreateMessage(message.c_str());
    return false;
  }
  return true;
}
#else
// Report, that the bindings are not backed by TPDE. Returns true, like the failing API functions
LLVMBool reportUnavailable(char **errorMessage) {
  *errorMessage = LLVMCreateMessage("The TPDE bindings were built without TPDE");
  return 1;
}
#endif

} // namespace

extern "C" {

/* Check if this build of the bindings is backed by TPDE */
LLVMBool TPDEIsAvailable(void) { return SPICE_TPDE_AVAILABLE; }

#if SPICE_TPDE_AVAILABLE

/* Create a compiler for the given target triple. Returns null if TPDE does not support the triple */
TPDECompilerRef TPDECreateCompiler(const char *triple) {
  std::unique_ptr<tpde_llvm::LLVMCompiler> compiler = tpde_llvm::LLVMCompiler::create(llvm::Triple(triple));
  return reinterpret_cast<TPDECompilerRef>(compiler.release());
}

/* Dispose a compiler, that was created with TPDECreateCompiler */
void TPDEDisposeCompiler(TPDECompilerRef compiler) { delete reinterpret_cast<tpde_llvm::LLVMCompiler *>(compiler); }

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

/* Stub: TPDE is not available, so there is no compiler for any triple */
TPDECompilerRef TPDECreateCompiler(const char *) { return nullptr; }

/* Stub: there is no compiler to dispose */
void TPDEDisposeCompiler(TPDECompilerRef) {}

/* Stub: report, that TPDE is not available */
LLVMBool TPDECompileToObjectFile(TPDECompilerRef, LLVMModuleRef, const char *, char **errorMessage) {
  return reportUnavailable(errorMessage);
}

/* Stub: report, that TPDE is not available */
LLVMBool TPDECompileToMemoryBuffer(TPDECompilerRef, LLVMModuleRef, LLVMMemoryBufferRef *outMemBuf, char **errorMessage) {
  *outMemBuf = nullptr;
  return reportUnavailable(errorMessage);
}

#endif

} // extern "C"
