// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "OpRuleConversionsManager.h"

#include <stdexcept>

#include "symboltablebuilder/SymbolTable.h"
#include <irgenerator/IRGenerator.h>
#include <util/CodeLoc.h>

OpRuleConversionsManager::OpRuleConversionsManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator)
    : context(resourceManager.context), builder(resourceManager.builder), irGenerator(irGenerator),
      stdFunctionManager(irGenerator->stdFunctionManager) {}

PtrAndValue OpRuleConversionsManager::getPlusEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy, SymbolTable *accessScope,
                                                       const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Value *lhsV = lhsData.value;
  llvm::Value *lhsP = lhsData.ptr;
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV, rhsV)};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_STROBJ, TY_CHAR): {
    // Generate call to the method append(char) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringAppendCharFct();
    builder.CreateCall(opFct, {lhsP, rhsV});
    return {.ptr = lhsP};
  }
  case COMB(TY_STROBJ, TY_STRING): {
    // Generate call to the method append(string) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringAppendStringFct();
    builder.CreateCall(opFct, {lhsP, rhsV});
    return {.ptr = lhsP};
  }
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    llvm::Type *elementTy = lhsSTy.getContainedTy().toLLVMType(context, accessScope);
    return {.value = builder.CreateGEP(elementTy, lhsV, rhsV)};
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMinusEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy, SymbolTable *accessScope) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFSub(lhsV, rhsV);
  case COMB(TY_INT, TY_INT):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    llvm::Type *elementType = lhsSTy.getContainedTy().toLLVMType(context, accessScope);
    return builder.CreateGEP(elementType, lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -="); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionsManager::getMulEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                      const SymbolType &rhsSTy, SymbolTable *accessScope,
                                                      const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Value *lhsV = lhsData.value;
  llvm::Value *lhsP = lhsData.ptr;
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV, rhsV)};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateMul(lhsV, rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateMul(lhsV, rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateMul(lhsV, rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateMul(lhsV, rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_STROBJ, TY_INT): {
    // Generate call to the method append(char) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringMulOpIntFct();
    builder.CreateCall(opFct, {lhsP, rhsV});
    return {.ptr = lhsP};
  }
  case COMB(TY_STROBJ, TY_LONG): {
    // Generate call to the method append(char) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringMulOpLongFct();
    builder.CreateCall(opFct, {lhsP, rhsV});
    return {.ptr = lhsP};
  }
  case COMB(TY_STROBJ, TY_SHORT): {
    // Generate call to the method append(char) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringMulOpShortFct();
    builder.CreateCall(opFct, {lhsP, rhsV});
    return {.ptr = lhsP};
  }
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: *="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getDivEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFDiv(lhsV, rhsV);
  case COMB(TY_INT, TY_INT):
    return builder.CreateSDiv(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSDiv(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSDiv(lhsV, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSDiv(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSDiv(lhsV, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSDiv(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSDiv(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: /="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getRemEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFRem(lhsV, rhsV);
  case COMB(TY_INT, TY_INT):
    return builder.CreateSRem(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSRem(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSRem(lhsV, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSRem(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSRem(lhsV, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSRem(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSRem(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: %="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getSHLEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateShl(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateShl(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateShl(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateShl(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateShl(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <<="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getSHREqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateLShr(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateLShr(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateLShr(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateLShr(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateLShr(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >>="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getAndEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateAnd(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateAnd(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateAnd(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateAnd(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateAnd(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: &="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getOrEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                      const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateOr(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateOr(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateOr(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateOr(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateOr(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: |="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getXorEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateXor(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateXor(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateXor(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateXor(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateXor(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ^="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseAndInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateAnd(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: &"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseOrInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateOr(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: |"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseXorInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateXor(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ^"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                    const SymbolType &rhsSTy, const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Value *lhsV = lhsData.value;
  llvm::Value *lhsP = lhsData.ptr;
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsVTy = rhsV->getType();

  // Check if both values are of type pointer
  if (lhsSTy.isPointer() && rhsSTy.isPointer())
    return builder.CreateICmpEQ(lhsV, rhsV);

  // Check if one value is of type pointer and one is of type int
  if (lhsVTy->isPointerTy() && rhsVTy->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV, rhsVTy);
    return builder.CreateICmpEQ(lhsInt, rhsV);
  }

  // Check for primitive type combinations
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOEQ(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpOEQ(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOEQ(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpEQ(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpEQ(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsLong, rhsV);
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpEQ(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOEQ(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpEQ(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpEQ(lhsV, rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOEQ(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpEQ(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateICmpEQ(lhsV, rhsV);
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpEQ(lhsV, rhsLong);
  }
  case COMB(TY_BYTE, TY_INT): // fallthrough
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsInt, rhsV);
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsShort, rhsV);
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpEQ(lhsLong, rhsV);
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpEQ(lhsV, rhsV);
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV, rhsV});
    return result;
  }
  case COMB(TY_STROBJ, TY_STROBJ): {
    // Generate call to the function isEqual(strobj) of the string_rt std
    llvm::Function *opFct = stdFunctionManager.getStringIsEqualStrobjFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsP, rhsV});
    return result;
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder.CreateICmpEQ(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: =="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getNotEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy, const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Value *lhsV = lhsData.value;
  llvm::Value *lhsP = lhsData.ptr;
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsVTy = rhsV->getType();

  // Check if both values are of type pointer
  if (lhsSTy.isPointer() && rhsSTy.isPointer())
    return builder.CreateICmpNE(lhsV, rhsV);

  // Check if one value is of type pointer and one is of type int
  if (lhsVTy->isPointerTy() && rhsVTy->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV, rhsVTy);
    return builder.CreateICmpNE(lhsInt, rhsV);
  }

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpONE(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpONE(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpONE(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpNE(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpNE(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsLong, rhsV);
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpNE(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpONE(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpNE(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpNE(lhsV, rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpONE(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpNE(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateICmpNE(lhsV, rhsV);
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpNE(lhsV, rhsLong);
  }
  case COMB(TY_BYTE, TY_INT):
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsInt, rhsV);
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsShort, rhsV);
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpNE(lhsLong, rhsV);
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpNE(lhsV, rhsV);
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV, rhsV});
    // Negate the result
    return builder.CreateNot(result);
  }
  case COMB(TY_STROBJ, TY_STROBJ): {
    // Generate call to the function isEqual(strobj) of the string_rt std
    llvm::Function *opFct = stdFunctionManager.getStringIsEqualStrobjFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsP, rhsV});
    // Negate the result
    return builder.CreateNot(result);
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder.CreateICmpNE(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: !="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getLessInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                   const SymbolType &rhsSTy) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();

  // Unpack rhsV
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOLT(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpOLT(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLT(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSLT(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSLT(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLT(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLT(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLT(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSLT(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLT(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLT(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSLT(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSLT(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getGreaterInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                      const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOGT(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpOGT(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGT(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSGT(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSGT(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGT(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGT(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGT(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSGT(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGT(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGT(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSGT(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSGT(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getLessEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOLE(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpOLE(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLE(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSLE(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSLE(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLE(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLE(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLE(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSLE(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSLE(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOLE(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSLE(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSLE(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getGreaterEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                           const SymbolType &rhsSTy) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhsV
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOGE(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFCmpOGE(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGE(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSGE(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSGE(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGE(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGE(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGE(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSGE(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateICmpSGE(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFCmpOGE(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateICmpSGE(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSGE(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getShiftLeftInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateShl(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateShl(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateShl(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateShl(lhsV, rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateShl(lhsV, rhsV);
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateShl(lhsV, rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateShl(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <<"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getShiftRightInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateLShr(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateLShr(lhsV, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateLShr(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateLShr(lhsV, rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateLShr(lhsV, rhsV);
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateLShr(lhsV, rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateLShr(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >>"); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionsManager::getPlusInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy, SymbolTable *accessScope, const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV, rhsV)};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return {.value = builder.CreateFAdd(lhsV, rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return {.value = builder.CreateFAdd(lhsFP, rhsV)};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return {.value = builder.CreateAdd(lhsLong, rhsV)};
  }
  case COMB(TY_INT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV)};
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return {.value = builder.CreateFAdd(lhsFP, rhsV)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return {.value = builder.CreateAdd(lhsInt, rhsV)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return {.value = builder.CreateAdd(lhsLong, rhsV)};
  }
  case COMB(TY_SHORT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV)};
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return {.value = builder.CreateFAdd(lhsFP, rhsV)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return {.value = builder.CreateAdd(lhsV, rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_LONG, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV)};
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateAdd(lhsV, rhsV)};
  case COMB(TY_STROBJ, TY_STRING): {
    // Generate call to the constructor ctor(strobj, string) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjStringFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, lhsV, rhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_STROBJ, TY_STROBJ): {
    // Generate call to the constructor ctor(strobj, strobj) of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, lhsV, rhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return {.value = builder.CreateGEP(lhsSTy.getContainedTy().toLLVMType(context, accessScope), lhsV, rhsV)};
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMinusInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                    const SymbolType &rhsSTy, SymbolTable *accessScope) {
  // Unpack lhsV
  llvm::Type *lhsVTy = lhsV->getType();
  // Unpack rhsV
  llvm::Type *rhsVTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFSub(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsVTy);
    return builder.CreateFSub(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFSub(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateSub(lhsLong, rhsV);
  }
  case COMB(TY_INT, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV);
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFSub(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateSub(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsVTy, true);
    return builder.CreateSub(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV);
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsVTy);
    return builder.CreateFSub(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsVTy, true);
    return builder.CreateSub(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_LONG, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV, lhsV);
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateSub(lhsV, rhsV);
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder.CreateGEP(lhsSTy.getContainedTy().toLLVMType(context, accessScope), lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -"); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionsManager::getMulInst(const PtrAndValue &lhsData, const PtrAndValue &rhsData, const SymbolType &lhsSTy,
                                                 const SymbolType &rhsSTy, SymbolTable *accessScope, const CodeLoc &codeLoc) {
  // Unpack lhs
  llvm::Value *lhsV = lhsData.value;
  llvm::Value *lhsP = lhsData.ptr;
  llvm::Type *lhsTy = lhsV->getType();
  // Unpack rhs
  llvm::Value *rhsV = rhsData.value;
  llvm::Value *rhsP = rhsData.ptr;
  llvm::Type *rhsTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV, rhsV)};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsTy);
    return {.value = builder.CreateFMul(lhsV, rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return {.value = builder.CreateFMul(lhsFP, rhsV)};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsTy, true);
    return {.value = builder.CreateMul(lhsV, rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return {.value = builder.CreateMul(lhsLong, rhsV)};
  }
  case COMB(TY_INT, TY_STROBJ): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Generate call to the method opMul(int) of the String struct
    opFct = stdFunctionManager.getStringMulOpIntFct();
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return {.value = builder.CreateFMul(lhsFP, rhsV)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsTy, true);
    return {.value = builder.CreateMul(lhsInt, rhsV)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return {.value = builder.CreateMul(lhsLong, rhsV)};
  }
  case COMB(TY_SHORT, TY_STROBJ): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Generate call to the method opMul(short) of the String struct
    opFct = stdFunctionManager.getStringMulOpShortFct();
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return {.value = builder.CreateFMul(lhsFP, rhsV)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsTy, true);
    return {.value = builder.CreateMul(lhsV, rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_LONG, TY_STROBJ): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Generate call to the method opMul(long) of the String struct
    opFct = stdFunctionManager.getStringMulOpLongFct();
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV, rhsV)};
  case COMB(TY_STROBJ, TY_INT): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Generate call to the method opMul(int) of the String struct
    opFct = stdFunctionManager.getStringMulOpIntFct();
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_STROBJ, TY_SHORT): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Generate call to the method opMul(short) of the String struct
    opFct = stdFunctionManager.getStringMulOpShortFct();
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  case COMB(TY_STROBJ, TY_LONG): {
    // Generate call to the copy constructor of the String struct
    llvm::Function *opFct = stdFunctionManager.getStringCtorStrobjFct();
    llvm::Value *thisPtr = irGenerator->insertAlloca(StdFunctionManager::getStrobjType(context));
    builder.CreateCall(opFct, {thisPtr, lhsV});
    // Generate call to the method opMul(long) of the String struct
    opFct = stdFunctionManager.getStringMulOpLongFct();
    builder.CreateCall(opFct, {thisPtr, rhsV});
    // Update mem address of anonymous symbol
    SymbolTableEntry *anonEntry = accessScope->lookupAnonymous(codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisPtr);
    return {.ptr = thisPtr};
  }
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: *"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getDivInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFDiv(lhsV, rhsV);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV, lhsTy);
    return builder.CreateFDiv(lhsV, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return builder.CreateFDiv(lhsFP, rhsV);
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateSDiv(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsTy, true);
    return builder.CreateSDiv(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSDiv(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return builder.CreateFDiv(lhsFP, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSDiv(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSDiv(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSDiv(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV, rhsTy);
    return builder.CreateFDiv(lhsFP, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsTy, true);
    return builder.CreateSDiv(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateSDiv(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: /"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getRemInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy) {
  // Unpack lhs
  llvm::Type *lhsTy = lhsV->getType();
  // Unpack rhs
  llvm::Type *rhsTy = rhsV->getType();

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFRem(lhsV, rhsV);
  case COMB(TY_INT, TY_INT):
    return builder.CreateSRem(lhsV, rhsV);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV, lhsTy, true);
    return builder.CreateSRem(lhsV, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSRem(lhsLong, rhsV);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSRem(lhsInt, rhsV);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSRem(lhsV, rhsV);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV, rhsTy, true);
    return builder.CreateSRem(lhsLong, rhsV);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV, lhsTy, true);
    return builder.CreateSRem(lhsV, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateSRem(lhsV, rhsV);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: %"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixMinusInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_DOUBLE:
    return builder.CreateFMul(lhsV, llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(double(-1))));
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder.CreateNeg(lhsV);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixPlusPlusInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateAdd(lhsV, builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateAdd(lhsV, builder.getInt16(1));
  case TY_LONG:
    return builder.CreateAdd(lhsV, builder.getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ++ (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixMinusMinusInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateSub(lhsV, builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateSub(lhsV, builder.getInt16(1));
  case TY_LONG:
    return builder.CreateSub(lhsV, builder.getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -- (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixNotInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_BOOL:
    return builder.CreateNot(lhsV);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: !"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixBitwiseNotInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder.CreateNeg(lhsV);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ~"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPostfixPlusPlusInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateAdd(lhsV, builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateAdd(lhsV, builder.getInt16(1));
  case TY_LONG:
    return builder.CreateAdd(lhsV, builder.getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ++ (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPostfixMinusMinusInst(llvm::Value *lhsV, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateSub(lhsV, builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateSub(lhsV, builder.getInt16(1));
  case TY_LONG:
    return builder.CreateSub(lhsV, builder.getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -- (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getCastInst(llvm::Value *rhsV, const SymbolType &lhsSTy, const SymbolType &rhsSTy,
                                                   SymbolTable *accessScope) {
  // Unpack lhs
  llvm::Type *lhsTy = lhsSTy.toLLVMType(context, accessScope);

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return rhsV;
  case COMB(TY_INT, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV, lhsTy);
  case COMB(TY_INT, TY_INT):
    return rhsV;
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_INT, TY_BYTE):  // fallthrough
  case COMB(TY_INT, TY_CHAR):
    return builder.CreateIntCast(rhsV, lhsTy, true);
  case COMB(TY_SHORT, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV, lhsTy);
  case COMB(TY_SHORT, TY_INT):
    return builder.CreateIntCast(rhsV, lhsTy, true);
  case COMB(TY_SHORT, TY_SHORT):
    return rhsV;
  case COMB(TY_SHORT, TY_LONG):
    return builder.CreateIntCast(rhsV, lhsTy, true);
  case COMB(TY_LONG, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV, lhsTy);
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT):
    return builder.CreateIntCast(rhsV, lhsTy, true);
  case COMB(TY_LONG, TY_LONG):
    return rhsV;
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG):
    return builder.CreateIntCast(rhsV, lhsTy, false);
  case COMB(TY_BYTE, TY_CHAR): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return rhsV;
  case COMB(TY_CHAR, TY_INT):   // fallthrough
  case COMB(TY_CHAR, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_LONG):
    return builder.CreateIntCast(rhsV, lhsTy, false);
  case COMB(TY_CHAR, TY_BYTE):     // fallthrough
  case COMB(TY_CHAR, TY_CHAR):     // fallthrough
  case COMB(TY_STRING, TY_STRING): // fallthrough
  case COMB(TY_STRING, TY_PTR):    // fallthrough
  case COMB(TY_STROBJ, TY_STROBJ): // fallthrough
  case COMB(TY_BOOL, TY_BOOL):     // fallthrough
  case COMB(TY_PTR, TY_STRING):
    return rhsV;
  case COMB(TY_PTR, TY_PTR):
    return lhsSTy.getContainedTy() == rhsSTy.getContainedTy() ? rhsV : builder.CreatePointerCast(rhsV, lhsTy);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: (cast)"); // GCOV_EXCL_LINE
}