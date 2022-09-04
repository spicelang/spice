// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "OpRuleConversionsManager.h"

#include <stdexcept>

#include <exception/IRError.h>
#include <generator/GeneratorVisitor.h>
#include <generator/StdFunctionManager.h>
#include <util/CodeLoc.h>

OpRuleConversionsManager::OpRuleConversionsManager(GeneratorVisitor *generator) : generator(generator) {
  builder = generator->builder.get();
  context = generator->context.get();
  stdFunctionManager = generator->stdFunctionManager.get();
}

llvm::Value *OpRuleConversionsManager::getPlusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy, SymbolTable *accessScope,
                                                        const CodeLoc &codeLoc) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFAdd(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_STRING, TY_CHAR): {
    // Convert lhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(lhsSTy, lhs);
    // Generate call to the method append(char) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringAppendCharFct();
    builder->CreateCall(opFct, {thisPtr, rhs});
    return thisPtr;
  }
  case COMB(TY_STRING, TY_STRING): {
    // Convert lhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(lhsSTy, lhs);
    // Generate call to the method append(string) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringAppendStringFct();
    builder->CreateCall(opFct, {thisPtr, rhs});
    return thisPtr;
  }
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder->CreateGEP(lhsSTy.getContainedTy().toLLVMType(*context, accessScope), lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMinusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy, SymbolTable *accessScope) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFSub(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder->CreateGEP(lhsSTy.getContainedTy().toLLVMType(*context, accessScope), lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMulEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFMul(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateMul(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: *="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getDivEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFDiv(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateSDiv(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSDiv(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateSDiv(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: /="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getRemEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFRem(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateSRem(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSRem(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateSRem(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: %="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getSHLEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateShl(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateShl(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateShl(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateShl(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateShl(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <<="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getSHREqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateLShr(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateLShr(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateLShr(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateLShr(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateLShr(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >>="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getAndEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateAnd(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAnd(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateAnd(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAnd(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateAnd(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: &="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getOrEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                      const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateOr(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateOr(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateOr(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateOr(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateOr(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: |="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getXorEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateXor(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateXor(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateXor(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateXor(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateXor(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ^="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseAndInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateAnd(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: &"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseOrInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateOr(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: |"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getBitwiseXorInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateXor(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ^"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                    const SymbolType &rhsSTy, const CodeLoc &codeLoc) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();

  // Check if both values are of type pointer
  if (lhsTy->isPointerTy() && rhsTy->isPointerTy())
    return builder->CreateICmpEQ(lhs, rhs);

  // Check if one value is of type pointer and one is of type byte
  if (lhsTy->isPointerTy() && rhsTy->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder->CreatePtrToInt(lhs, rhsTy);
    return builder->CreateICmpEQ(lhsInt, rhs);
  }

  // Check for primitive type combinations
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpOEQ(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpOEQ(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOEQ(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpEQ(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpEQ(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsLong, rhs);
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpEQ(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOEQ(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpEQ(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpEQ(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOEQ(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpEQ(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateICmpEQ(lhs, rhs);
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpEQ(lhs, rhsLong);
  }
  case COMB(TY_BYTE, TY_INT): // fallthrough
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsInt, rhs);
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsShort, rhs);
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpEQ(lhsLong, rhs);
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpEQ(lhs, rhs);
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager->getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder->CreateCall(opFct, {lhs, rhs});
    return result;
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder->CreateICmpEQ(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: =="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getNotEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                       const SymbolType &rhsSTy, const CodeLoc &codeLoc) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();

  // Check if both values are of type pointer
  if (lhsTy->isPointerTy() && rhsTy->isPointerTy())
    return builder->CreateICmpNE(lhs, rhs);

  // Check if one value is of type pointer and one is of type byte
  if (lhsTy->isPointerTy() && rhsTy->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder->CreatePtrToInt(lhs, rhsTy);
    return builder->CreateICmpNE(lhsInt, rhs);
  }

  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpONE(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpONE(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpONE(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpNE(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpNE(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsLong, rhs);
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpNE(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpONE(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpNE(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpNE(lhs, rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpONE(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpNE(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateICmpNE(lhs, rhs);
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpNE(lhs, rhsLong);
  }
  case COMB(TY_BYTE, TY_INT):
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsInt, rhs);
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsShort, rhs);
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpNE(lhsLong, rhs);
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpNE(lhs, rhs);
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager->getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder->CreateCall(opFct, {lhs, rhs});
    // Negate the result
    return builder->CreateNot(result);
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder->CreateICmpNE(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: !="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getLessInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                   const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpOLT(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpOLT(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLT(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpSLT(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSLT(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLT(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLT(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLT(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpSLT(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLT(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLT(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSLT(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpSLT(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getGreaterInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                      const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpOGT(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpOGT(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGT(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpSGT(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSGT(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGT(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGT(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGT(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpSGT(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGT(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGT(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSGT(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpSGT(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getLessEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpOLE(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpOLE(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLE(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpSLE(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSLE(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLE(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLE(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLE(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpSLE(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSLE(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOLE(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSLE(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpSLE(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getGreaterEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                           const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFCmpOGE(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFCmpOGE(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGE(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateICmpSGE(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSGE(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGE(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGE(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGE(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateICmpSGE(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateICmpSGE(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFCmpOGE(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateICmpSGE(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateICmpSGE(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getShiftLeftInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                        const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateShl(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateShl(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateShl(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateShl(lhs, rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateShl(lhs, rhs);
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateShl(lhs, rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateShl(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: <<"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getShiftRightInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                         const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_INT, TY_INT):
    return builder->CreateLShr(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateLShr(lhs, rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateLShr(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateLShr(lhs, rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateLShr(lhs, rhs);
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateLShr(lhs, rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateLShr(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: >>"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPlusInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                   const SymbolType &rhsSTy, SymbolTable *accessScope, const CodeLoc &codeLoc) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFAdd(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFAdd(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFAdd(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateAdd(lhsLong, rhs);
  }
  case COMB(TY_INT, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFAdd(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateAdd(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateAdd(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFAdd(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateAdd(lhsInt, rhs);
  }
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateAdd(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_LONG, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateAdd(lhs, rhs);
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the constructor ctor(string, string) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringCtorStringStringFct();
    llvm::Value *thisPtr = generator->insertAlloca(stdFunctionManager->getStringStructType());
    builder->CreateCall(opFct, {thisPtr, lhs, rhs});
    return thisPtr;
  }
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder->CreateGEP(lhsSTy.getContainedTy().toLLVMType(*context, accessScope), lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMinusInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                    const SymbolType &rhsSTy, SymbolTable *accessScope) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFSub(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFSub(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFSub(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSub(lhsLong, rhs);
  }
  case COMB(TY_INT, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFSub(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSub(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSub(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFSub(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSub(lhsInt, rhs);
  }
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSub(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_LONG, TY_PTR):
    return builder->CreateGEP(rhsSTy.getContainedTy().toLLVMType(*context, accessScope), rhs, lhs);
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateSub(lhs, rhs);
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder->CreateGEP(lhsSTy.getContainedTy().toLLVMType(*context, accessScope), lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getMulInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy, const CodeLoc &codeLoc) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFMul(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFMul(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFMul(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateMul(lhsLong, rhs);
  }
  case COMB(TY_INT, TY_CHAR): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=int and rhs=char yet");
  }
  case COMB(TY_INT, TY_STRING): {
    // Convert rhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(rhsSTy, rhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpIntFct();
    builder->CreateCall(opFct, {thisPtr, lhs});
    return thisPtr;
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFMul(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateMul(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateMul(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_CHAR): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=short and rhs=char yet");
  }
  case COMB(TY_SHORT, TY_STRING): {
    // Convert rhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(rhsSTy, rhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpShortFct();
    builder->CreateCall(opFct, {thisPtr, lhs});
    return thisPtr;
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFMul(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateMul(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_LONG, TY_CHAR): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=long and rhs=char yet");
  }
  case COMB(TY_LONG, TY_STRING): {
    // Convert rhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(rhsSTy, rhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpLongFct();
    builder->CreateCall(opFct, {thisPtr, lhs});
    return thisPtr;
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder->CreateMul(lhs, rhs);
  case COMB(TY_CHAR, TY_INT): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=int yet");
  }
  case COMB(TY_CHAR, TY_SHORT): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=short yet");
  }
  case COMB(TY_CHAR, TY_LONG): {
    // ToDo(@marcauberer): Insert call to opMul in the runtime lib
    throw IRError(codeLoc, COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=long yet");
  }
  case COMB(TY_STRING, TY_INT): {
    // Convert lhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(lhsSTy, lhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpIntFct();
    builder->CreateCall(opFct, {thisPtr, rhs});
    return thisPtr;
  }
  case COMB(TY_STRING, TY_SHORT): {
    // Convert lhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(lhsSTy, lhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpShortFct();
    builder->CreateCall(opFct, {thisPtr, rhs});
    return thisPtr;
  }
  case COMB(TY_STRING, TY_LONG): {
    // Convert lhs literal to string object if required
    llvm::Value *thisPtr = propagateStringToStringObject(lhsSTy, lhs);
    // Generate call to the method opMul(int) of the String struct
    llvm::Function *opFct = stdFunctionManager->getStringMulOpLongFct();
    builder->CreateCall(opFct, {thisPtr, rhs});
    return thisPtr;
  }
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: *"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getDivInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFDiv(lhs, rhs);
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder->CreateSIToFP(rhs, lhsTy);
    return builder->CreateFDiv(lhs, rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFDiv(lhsFP, rhs);
  }
  case COMB(TY_INT, TY_INT):
    return builder->CreateSDiv(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSDiv(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFDiv(lhsFP, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSDiv(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSDiv(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSDiv(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder->CreateSIToFP(lhs, rhsTy);
    return builder->CreateFDiv(lhsFP, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSDiv(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder->CreateSDiv(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: /"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getRemInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsSTy,
                                                  const SymbolType &rhsSTy) {
  llvm::Type *lhsTy = lhs->getType();
  llvm::Type *rhsTy = rhs->getType();
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder->CreateFRem(lhs, rhs);
  case COMB(TY_INT, TY_INT):
    return builder->CreateSRem(lhs, rhs);
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSRem(lhsLong, rhs);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSRem(lhsInt, rhs);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder->CreateSRem(lhs, rhs);
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
    return builder->CreateSRem(lhsLong, rhs);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
    return builder->CreateSRem(lhs, rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder->CreateSRem(lhs, rhs);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: %"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixMinusInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_DOUBLE:
    return builder->CreateFMul(lhs, llvm::ConstantFP::get(builder->getContext(), llvm::APFloat(double(-1))));
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder->CreateNeg(lhs);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: +"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixPlusPlusInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder->CreateAdd(lhs, builder->getInt32(1));
  case TY_SHORT:
    return builder->CreateAdd(lhs, builder->getInt16(1));
  case TY_LONG:
    return builder->CreateAdd(lhs, builder->getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ++ (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixMinusMinusInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder->CreateSub(lhs, builder->getInt32(1));
  case TY_SHORT:
    return builder->CreateSub(lhs, builder->getInt16(1));
  case TY_LONG:
    return builder->CreateSub(lhs, builder->getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -- (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixNotInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_BOOL:
    return builder->CreateNot(lhs);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: !"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPrefixBitwiseNotInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder->CreateNeg(lhs);
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ~"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPostfixPlusPlusInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder->CreateAdd(lhs, builder->getInt32(1));
  case TY_SHORT:
    return builder->CreateAdd(lhs, builder->getInt16(1));
  case TY_LONG:
    return builder->CreateAdd(lhs, builder->getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: ++ (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getPostfixMinusMinusInst(llvm::Value *lhs, const SymbolType &lhsSTy) {
  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder->CreateSub(lhs, builder->getInt32(1));
  case TY_SHORT:
    return builder->CreateSub(lhs, builder->getInt16(1));
  case TY_LONG:
    return builder->CreateSub(lhs, builder->getInt64(1));
  default:
    break;
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: -- (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::getCastInst(llvm::Value *rhs, const SymbolType &lhsSTy, const SymbolType &rhsSTy,
                                                   SymbolTable *accessScope) {
  llvm::Type *lhsTy = lhsSTy.toLLVMType(*context, accessScope);
  switch (COMB(lhsSTy.getSuperType(), rhsSTy.getSuperType())) {
  case COMB(TY_DOUBLE, TY_DOUBLE): // fallthrough
  case COMB(TY_INT, TY_INT):
    return rhs;
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_INT, TY_BYTE):  // fallthrough
  case COMB(TY_INT, TY_CHAR):  // fallthrough
  case COMB(TY_SHORT, TY_INT):
    return builder->CreateIntCast(rhs, lhsTy, true);
  case COMB(TY_SHORT, TY_SHORT):
    return rhs;
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT):
    return builder->CreateIntCast(rhs, lhsTy, true);
  case COMB(TY_LONG, TY_LONG):
    return rhs;
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG):
    return builder->CreateIntCast(rhs, lhsTy, false);
  case COMB(TY_BYTE, TY_BYTE):
    return rhs;
  case COMB(TY_CHAR, TY_INT):   // fallthrough
  case COMB(TY_CHAR, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_LONG):
    return builder->CreateIntCast(rhs, lhsTy, false);
  case COMB(TY_CHAR, TY_CHAR): // fallthrough
  case COMB(TY_STRING, TY_STRING):
    return rhs;
  case COMB(TY_STRING, TY_PTR):
    return builder->CreatePointerCast(rhs, lhsTy);
  case COMB(TY_BOOL, TY_BOOL):
    return rhs;
  case COMB(TY_PTR, TY_STRING): // fallthrough (string corresponds to byte* or char*)
  case COMB(TY_PTR, TY_PTR):
    return builder->CreatePointerCast(rhs, lhsTy);
  }
  throw std::runtime_error("Internal compiler error: Operator fallthrough: (cast)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionsManager::propagateStringToStringObject(const SymbolType &symbolType, llvm::Value *operandValue) {
  if (symbolType.isStringStruct())
    return operandValue;
  // Convert rhs literal to string object
  llvm::Function *opFct = stdFunctionManager->getStringCtorStringFct();
  llvm::Value *thisPtr = generator->insertAlloca(stdFunctionManager->getStringStructType());
  builder->CreateCall(opFct, {thisPtr, operandValue});
  return thisPtr;
}