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
#include <llvm/IR/Attributes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
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

// Count the general purpose and floating point registers, that are needed to return a value of the given type
bool countReturnRegs(llvm::Type *type, unsigned &gpRegs, unsigned &fpRegs) {
  if (type->isIntegerTy()) {
    gpRegs += (type->getIntegerBitWidth() + 63) / 64;
    return true;
  }
  if (type->isPointerTy()) {
    gpRegs++;
    return true;
  }
  if (type->isFloatTy() || type->isDoubleTy() || type->isHalfTy() || type->isBFloatTy()) {
    fpRegs++;
    return true;
  }
  if (const auto structType = llvm::dyn_cast<llvm::StructType>(type)) {
    for (llvm::Type *elementType : structType->elements())
      if (!countReturnRegs(elementType, gpRegs, fpRegs))
        return false;
    return true;
  }
  if (const auto arrayType = llvm::dyn_cast<llvm::ArrayType>(type)) {
    for (uint64_t i = 0; i < arrayType->getNumElements(); i++)
      if (!countReturnRegs(arrayType->getElementType(), gpRegs, fpRegs))
        return false;
    return true;
  }
  return false;
}

// Check if TPDE can't return a value of the given type in registers
bool needsSRet(llvm::Type *returnType, const llvm::Triple &triple) {
  if (!returnType->isAggregateType())
    return false;
  unsigned gpRegs = 0;
  unsigned fpRegs = 0;
  if (!countReturnRegs(returnType, gpRegs, fpRegs))
    return true;
  const unsigned maxRegs = triple.isAArch64() ? 8 : 2;
  return gpRegs > maxRegs || fpRegs > maxRegs;
}

llvm::FunctionType *getSRetFunctionType(llvm::FunctionType *fctType) {
  std::vector<llvm::Type *> paramTypes = {llvm::PointerType::get(fctType->getContext(), 0)};
  paramTypes.insert(paramTypes.end(), fctType->param_begin(), fctType->param_end());
  return llvm::FunctionType::get(llvm::Type::getVoidTy(fctType->getContext()), paramTypes, fctType->isVarArg());
}

llvm::AttributeList getSRetAttributes(llvm::LLVMContext &ctx, const llvm::AttributeList &attrs, llvm::Type *returnType,
                                      unsigned numParams) {
  std::vector<llvm::AttributeSet> paramAttrs;
  paramAttrs.push_back(llvm::AttributeSet::get(ctx, {llvm::Attribute::getWithStructRetType(ctx, returnType)}));
  for (unsigned i = 0; i < numParams; i++)
    paramAttrs.push_back(attrs.getParamAttrs(i));
  return llvm::AttributeList::get(ctx, attrs.getFnAttrs(), llvm::AttributeSet(), paramAttrs);
}

// Return aggregates, that do not fit into the return registers, via an explicit sret pointer, like LLVM CodeGen does it
// implicitly. TPDE does not support such return values
void demoteLargeAggregateReturns(llvm::Module &module, const llvm::Triple &triple) {
  llvm::LLVMContext &ctx = module.getContext();
  const llvm::DataLayout &dataLayout = module.getDataLayout();

  // Rewrite all calls
  std::vector<llvm::CallBase *> calls;
  for (llvm::Function &fct : module)
    for (llvm::BasicBlock &block : fct)
      for (llvm::Instruction &inst : block)
        if (auto call = llvm::dyn_cast<llvm::CallBase>(&inst); call && needsSRet(call->getType(), triple))
          calls.push_back(call);
  for (llvm::CallBase *call : calls) {
    llvm::Type *returnType = call->getType();
    llvm::FunctionType *newFctType = getSRetFunctionType(call->getFunctionType());
    llvm::Function *caller = call->getFunction();
    llvm::IRBuilder<> builder(&*caller->getEntryBlock().getFirstInsertionPt());
    llvm::AllocaInst *sretPtr = builder.CreateAlloca(returnType, nullptr, "sret");
    sretPtr->setAlignment(dataLayout.getPrefTypeAlign(returnType));
    std::vector<llvm::Value *> args = {sretPtr};
    args.insert(args.end(), call->arg_begin(), call->arg_end());
    llvm::SmallVector<llvm::OperandBundleDef> bundles;
    call->getOperandBundlesAsDefs(bundles);
    llvm::CallBase *newCall;
    llvm::Instruction *loadInsertPt;
    if (auto invoke = llvm::dyn_cast<llvm::InvokeInst>(call)) {
      // Load the return value on an own edge to the normal destination, which might have other predecessors
      llvm::BasicBlock *normalDest = invoke->getNormalDest();
      llvm::BasicBlock *loadBlock = llvm::BasicBlock::Create(ctx, "sret.load", caller, normalDest);
      loadInsertPt = llvm::UncondBrInst::Create(normalDest, loadBlock);
      normalDest->replacePhiUsesWith(invoke->getParent(), loadBlock);
      newCall = llvm::InvokeInst::Create(newFctType, invoke->getCalledOperand(), loadBlock, invoke->getUnwindDest(), args,
                                         bundles, "", invoke->getIterator());
    } else {
      newCall = llvm::CallInst::Create(newFctType, call->getCalledOperand(), args, bundles, "", call->getIterator());
      llvm::cast<llvm::CallInst>(newCall)->setTailCallKind(llvm::CallInst::TCK_None);
      loadInsertPt = call->getNextNode();
    }
    newCall->setCallingConv(call->getCallingConv());
    newCall->setAttributes(getSRetAttributes(ctx, call->getAttributes(), returnType, call->arg_size()));
    newCall->setDebugLoc(call->getDebugLoc());
    llvm::LoadInst *result = new llvm::LoadInst(returnType, sretPtr, "", loadInsertPt->getIterator());
    result->setDebugLoc(call->getDebugLoc());
    call->replaceAllUsesWith(result);
    call->eraseFromParent();
  }

  // Rewrite all function declarations and definitions
  std::vector<llvm::Function *> fcts;
  for (llvm::Function &fct : module)
    if (!fct.isIntrinsic() && needsSRet(fct.getReturnType(), triple))
      fcts.push_back(&fct);
  for (llvm::Function *fct : fcts) {
    llvm::Type *returnType = fct->getReturnType();
    llvm::Function *newFct = llvm::Function::Create(getSRetFunctionType(fct->getFunctionType()), fct->getLinkage(),
                                                    fct->getAddressSpace(), "", &module);
    newFct->copyAttributesFrom(fct);
    newFct->setAttributes(getSRetAttributes(ctx, fct->getAttributes(), returnType, fct->arg_size()));
    newFct->setComdat(fct->getComdat());
    newFct->copyMetadata(fct, 0);
    newFct->takeName(fct);
    newFct->splice(newFct->begin(), fct);
    llvm::Argument *sretArg = newFct->getArg(0);
    for (unsigned i = 0; i < fct->arg_size(); i++) {
      newFct->getArg(i + 1)->takeName(fct->getArg(i));
      fct->getArg(i)->replaceAllUsesWith(newFct->getArg(i + 1));
    }
    for (llvm::BasicBlock &block : *newFct) {
      auto ret = llvm::dyn_cast<llvm::ReturnInst>(block.getTerminator());
      if (!ret)
        continue;
      new llvm::StoreInst(ret->getReturnValue(), sretArg, ret->getIterator());
      llvm::ReturnInst::Create(ctx, nullptr, ret->getIterator())->setDebugLoc(ret->getDebugLoc());
      ret->eraseFromParent();
    }
    fct->replaceAllUsesWith(newFct);
    fct->eraseFromParent();
  }
}

// Compile the module to an ELF object in the given buffer. Returns true on success
bool compileModule(TPDECompilerRef compiler, LLVMModuleRef module, std::vector<uint8_t> &buffer, char **errorMessage) {
  llvm::Module *mod = llvm::unwrap(module);
  demoteLargeAggregateReturns(*mod, llvm::Triple(mod->getTargetTriple()));
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
