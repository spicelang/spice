// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TPDEObjectEmitter.h"

#include <fstream>
#include <memory>
#include <vector>

#include <exception/CompilerError.h>

#include <llvm/IR/Attributes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/TargetParser/Triple.h>

#include <tpde-llvm/LLVMCompiler.hpp>

namespace spice::compiler {

namespace {

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

} // namespace

TPDEObjectEmitter::TPDEObjectEmitter(llvm::Module &module) : module(module) {}

void TPDEObjectEmitter::emit(const std::filesystem::path &objectPath) const {
  const std::string objectPathString = objectPath.string();

  // Create a TPDE compiler for the module's target triple
  const llvm::Triple triple(module.getTargetTriple());
  const std::unique_ptr<tpde_llvm::LLVMCompiler> compiler = tpde_llvm::LLVMCompiler::create(triple);
  if (!compiler)
    throw CompilerError(WRONG_OUTPUT_TYPE,
                        "The TPDE backend does not support target triple '" + triple.str() + "'"); // GCOV_EXCL_LINE

  demoteLargeAggregateReturns(module, triple);

  // Compile to an in-memory ELF object
  std::vector<uint8_t> objBytes;
  if (!compiler->compile_to_elf(module, objBytes))
    throw CompilerError(WRONG_OUTPUT_TYPE, "TPDE failed to compile module '" + module.getName().str() + "'");

  // Write to the requested path
  std::ofstream out(objectPathString, std::ios::binary);
  if (!out)
    throw CompilerError(CANT_OPEN_OUTPUT_FILE, "File '" + objectPathString + "' could not be opened"); // GCOV_EXCL_LINE
  out.write(reinterpret_cast<const char *>(objBytes.data()), static_cast<std::streamsize>(objBytes.size()));
  out.flush();
}

void TPDEObjectEmitter::getASMString(std::string &output) const {
  // TPDE emits ELF bytes directly and does not expose an assembly listing.
  output = "; Assembly listing is not available under the TPDE backend.\n"
           "; Use --backend=llvm to obtain assembly output.\n";
}

} // namespace spice::compiler
