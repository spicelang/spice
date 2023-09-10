// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

llvm::Value *IRGenerator::doImplicitCast(llvm::Value *src, SymbolType dstSTy, SymbolType srcSTy) {
  assert(srcSTy != dstSTy); // We only need to cast implicitly, if the types do not match exactly

  // Unpack the pointers until a pointer of another type is met
  size_t loadCounter = 0;
  while (srcSTy.isPtr()) {
    src = builder.CreateLoad(srcSTy.toLLVMType(context, currentScope), src);
    srcSTy = srcSTy.getContainedTy();
    dstSTy = dstSTy.getContainedTy();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstSTy.isArray() && srcSTy.isArray()) { // Special case that is used for passing arrays as pointer to functions
    llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
    src = builder.CreateInBoundsGEP(srcSTy.toLLVMType(context, currentScope), src, indices);
  } else {
    src = builder.CreateLoad(srcSTy.toLLVMType(context, currentScope), src);
    src = builder.CreateBitCast(src, dstSTy.toLLVMType(context, currentScope));
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(src->getType());
    builder.CreateStore(src, newActualArg);
    src = newActualArg;
  }
  return src;
}

void IRGenerator::generateScopeCleanup(const StmtLstNode *node) const {
  // Do not clean up if the block is already terminated
  if (blockAlreadyTerminated)
    return;

  // Call all dtor functions
  for (auto [entry, dtor] : node->dtorFunctions.at(manIdx))
    generateDtorCall(entry, dtor);
}

void IRGenerator::generateDtorCall(SymbolTableEntry *entry, Function *dtor) const {
  assert(dtor != nullptr);

  // Retrieve metadata for the function
  const std::string mangledName = NameMangling::mangleFunction(*dtor);

  // Function is not defined in the current module -> declare it
  if (!module->getFunction(mangledName)) {
    llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), builder.getPtrTy(), false);
    module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = module->getFunction(mangledName);
  assert(callee != nullptr);

  // Retrieve address of the struct variable. For fields this is the 'this' variable, otherwise use the normal address
  llvm::Value *structPtr;
  if (entry->isField()) {
    // Take 'this' var as base pointer
    const SymbolTableEntry *thisVar = currentScope->lookupStrict(THIS_VARIABLE_NAME);
    assert(thisVar != nullptr);
    assert(thisVar->getType().isPtr() && thisVar->getType().getContainedTy().is(TY_STRUCT));
    llvm::Type *thisType = thisVar->getType().getContainedTy().toLLVMType(context, currentScope);
    llvm::Value *thisPtr = builder.CreateLoad(builder.getPtrTy(), thisVar->getAddress());
    // Add field offset
    structPtr = builder.CreateInBoundsGEP(thisType, thisPtr, {builder.getInt32(0), builder.getInt32(entry->orderIndex)});
  } else {
    structPtr = entry->getAddress();
  }
  assert(structPtr != nullptr);

  // Generate function call
  builder.CreateCall(callee, structPtr);
}

} // namespace spice::compiler