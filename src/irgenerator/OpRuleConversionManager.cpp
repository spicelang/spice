// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "OpRuleConversionManager.h"

#include <stdexcept>

#include <ast/ASTNodes.h>
#include <irgenerator/IRGenerator.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

OpRuleConversionManager::OpRuleConversionManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator)
    : context(resourceManager.context), builder(resourceManager.builder), irGenerator(irGenerator),
      stdFunctionManager(irGenerator->stdFunctionManager) {}

PtrAndValue OpRuleConversionManager::getPlusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsP = [&]() { return irGenerator->resolveAddress(lhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    llvm::Type *elementTy = lhsSTy.getContainedTy().toLLVMType(context, accessScope);
    return {.value = builder.CreateGEP(elementTy, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: +="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getMinusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                        ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFSub(lhsV(), rhsV());
  case COMB(TY_INT, TY_INT):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    llvm::Type *elementType = lhsSTy.getContainedTy().toLLVMType(context, accessScope);
    return builder.CreateGEP(elementType, lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -="); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionManager::getMulEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                     ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                     size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsP = [&]() { return irGenerator->resolveAddress(lhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: *="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getDivEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFDiv(lhsV(), rhsV());
  case COMB(TY_INT, TY_INT):
    return builder.CreateSDiv(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSDiv(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSDiv(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getRemEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFRem(lhsV(), rhsV());
  case COMB(TY_INT, TY_INT):
    return builder.CreateSRem(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsShort);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSRem(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsShort);
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateSRem(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getSHLEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateShl(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateShl(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateShl(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateShl(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateShl(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getSHREqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateLShr(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateLShr(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateLShr(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateLShr(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateLShr(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getAndEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateAnd(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateAnd(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateAnd(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateAnd(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateAnd(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getOrEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                     ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                     size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateOr(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateOr(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateOr(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateOr(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateOr(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getXorEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateXor(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateXor(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateXor(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateXor(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateXor(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getBitwiseAndInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                        ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateAnd(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getBitwiseOrInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                       ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                       size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateOr(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getBitwiseXorInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                        ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateXor(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                   ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return builder.CreateICmpEQ(lhsV(), rhsV());

  // Check if one value is of type pointer and one is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return builder.CreateICmpEQ(lhsInt, rhsV());
  }

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  // Check for primitive type combinations
  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOEQ(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpOEQ(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOEQ(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpEQ(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpEQ(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsLong, rhsV());
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpEQ(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOEQ(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpEQ(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpEQ(lhsV(), rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOEQ(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpEQ(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateICmpEQ(lhsV(), rhsV());
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpEQ(lhsV(), rhsLong);
  }
  case COMB(TY_BYTE, TY_INT): // fallthrough
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsInt, rhsV());
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsShort, rhsV());
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpEQ(lhsLong, rhsV());
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpEQ(lhsV(), rhsV());
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    return result;
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder.CreateICmpEQ(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: =="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getNotEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                      ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                      size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return builder.CreateICmpNE(lhsV(), rhsV());

  // Check if one value is of type pointer and one is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return builder.CreateICmpNE(lhsInt, rhsV());
  }

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpONE(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpONE(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpONE(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpNE(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpNE(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsLong, rhsV());
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpNE(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpONE(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpNE(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpNE(lhsV(), rhsShort);
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpONE(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpNE(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateICmpNE(lhsV(), rhsV());
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpNE(lhsV(), rhsLong);
  }
  case COMB(TY_BYTE, TY_INT):
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsInt, rhsV());
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsShort, rhsV());
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpNE(lhsLong, rhsV());
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpNE(lhsV(), rhsV());
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    // Negate the result
    return builder.CreateNot(result);
  }
  case COMB(TY_BOOL, TY_BOOL):
    return builder.CreateICmpNE(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getLessInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                                  const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOLT(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpOLT(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLT(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSLT(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSLT(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLT(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLT(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLT(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSLT(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLT(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLT(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSLT(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSLT(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getGreaterInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                     ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                     size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOGT(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpOGT(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGT(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSGT(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSGT(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGT(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGT(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGT(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSGT(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGT(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGT(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSGT(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSGT(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getLessEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                       ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                       size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOLE(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpOLE(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLE(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSLE(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSLE(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLE(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLE(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLE(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSLE(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSLE(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOLE(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSLE(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSLE(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getGreaterEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                          ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                          size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFCmpOGE(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFCmpOGE(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGE(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateICmpSGE(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSGE(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGE(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGE(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGE(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateICmpSGE(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateICmpSGE(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFCmpOGE(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateICmpSGE(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateICmpSGE(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >="); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getShiftLeftInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                       ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                       size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateShl(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateShl(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateShl(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateShl(lhsV(), rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateShl(lhsV(), rhsV());
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateShl(lhsV(), rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateShl(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getShiftRightInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                        ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return builder.CreateLShr(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateLShr(lhsV(), rhsInt);
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateLShr(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateLShr(lhsV(), rhsInt);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateLShr(lhsV(), rhsV());
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateLShr(lhsV(), rhsInt);
  }
  case COMB(TY_BYTE, TY_BYTE):
    return builder.CreateLShr(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>"); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionManager::getPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return {.value = builder.CreateFAdd(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFAdd(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateAdd(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFAdd(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateAdd(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateAdd(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFAdd(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAdd(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateAdd(lhsV(), rhsV())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return {.value = builder.CreateGEP(lhsSTy.getContainedTy().toLLVMType(context, accessScope), lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: +"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                   ExprResult &rhs, const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFSub(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFSub(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFSub(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSub(lhsLong, rhsV());
  }
  case COMB(TY_INT, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV());
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFSub(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSub(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSub(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV());
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFSub(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSub(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_LONG, TY_PTR):
    return builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV());
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateSub(lhsV(), rhsV());
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return builder.CreateGEP(lhsSTy.getContainedTy().toLLVMType(context, accessScope), lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionManager::getMulInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                                const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return {.value = builder.CreateFMul(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFMul(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateMul(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFMul(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateMul(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateMul(lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return {.value = builder.CreateFMul(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateMul(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: *"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getDivInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (lhsSTy.is(TY_STRUCT) || rhsSTy.is(TY_STRUCT))
    return callBinaryOperatorOverloadFct(node, lhsV, rhsV, opIdx).value;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFDiv(lhsV(), rhsV());
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = builder.CreateSIToFP(rhsV(), lhsT);
    return builder.CreateFDiv(lhsV(), rhsFP);
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFDiv(lhsFP, rhsV());
  }
  case COMB(TY_INT, TY_INT):
    return builder.CreateSDiv(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSDiv(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFDiv(lhsFP, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSDiv(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSDiv(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSDiv(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = builder.CreateSIToFP(lhsV(), rhsT);
    return builder.CreateFDiv(lhsFP, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSDiv(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return builder.CreateSDiv(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getRemInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);
  const auto rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return builder.CreateFRem(lhsV(), rhsV());
  case COMB(TY_INT, TY_INT):
    return builder.CreateSRem(lhsV(), rhsV());
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsInt);
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSRem(lhsLong, rhsV());
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSRem(lhsInt, rhsV());
  }
  case COMB(TY_SHORT, TY_SHORT):
    return builder.CreateSRem(lhsV(), rhsV());
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return builder.CreateSRem(lhsLong, rhsV());
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return builder.CreateSRem(lhsV(), rhsLong);
  }
  case COMB(TY_LONG, TY_LONG):
    return builder.CreateSRem(lhsV(), rhsV());
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPrefixMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                         Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_DOUBLE:
    return builder.CreateFMul(lhsV(), llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(double(-1))));
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder.CreateNeg(lhsV());
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPrefixPlusPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                            Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateAdd(lhsV(), builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateAdd(lhsV(), builder.getInt16(1));
  case TY_LONG:
    return builder.CreateAdd(lhsV(), builder.getInt64(1));
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPrefixMinusMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                              Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateSub(lhsV(), builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateSub(lhsV(), builder.getInt16(1));
  case TY_LONG:
    return builder.CreateSub(lhsV(), builder.getInt64(1));
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (prefix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPrefixNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                       Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_BOOL:
    return builder.CreateNot(lhsV());
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPrefixBitwiseNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy,
                                                              Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return builder.CreateNeg(lhsV());
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ~"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPostfixPlusPlusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy,
                                                             Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateAdd(lhsV(), builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateAdd(lhsV(), builder.getInt16(1));
  case TY_LONG:
    return builder.CreateAdd(lhsV(), builder.getInt64(1));
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getPostfixMinusMinusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy,
                                                               Scope *accessScope, size_t opIdx) {
  const auto lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return builder.CreateSub(lhsV(), builder.getInt32(1));
  case TY_SHORT:
    return builder.CreateSub(lhsV(), builder.getInt16(1));
  case TY_LONG:
    return builder.CreateSub(lhsV(), builder.getInt64(1));
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (postfix)"); // GCOV_EXCL_LINE
}

llvm::Value *OpRuleConversionManager::getCastInst(const ASTNode *node, const SymbolType &lhsSTy, ExprResult &rhs,
                                                  const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx) {
  const auto rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  const auto lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return rhsV();
  case COMB(TY_INT, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV(), lhsT);
  case COMB(TY_INT, TY_INT):
    return rhsV();
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_INT, TY_BYTE):  // fallthrough
  case COMB(TY_INT, TY_CHAR):
    return builder.CreateIntCast(rhsV(), lhsT, true);
  case COMB(TY_SHORT, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV(), lhsT);
  case COMB(TY_SHORT, TY_INT):
    return builder.CreateIntCast(rhsV(), lhsT, true);
  case COMB(TY_SHORT, TY_SHORT):
    return rhsV();
  case COMB(TY_SHORT, TY_LONG):
    return builder.CreateIntCast(rhsV(), lhsT, true);
  case COMB(TY_LONG, TY_DOUBLE):
    return builder.CreateFPToSI(rhsV(), lhsT);
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT):
    return builder.CreateIntCast(rhsV(), lhsT, true);
  case COMB(TY_LONG, TY_LONG):
    return rhsV();
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG):
    return builder.CreateIntCast(rhsV(), lhsT, false);
  case COMB(TY_BYTE, TY_CHAR): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return rhsV();
  case COMB(TY_CHAR, TY_INT):   // fallthrough
  case COMB(TY_CHAR, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_LONG):
    return builder.CreateIntCast(rhsV(), lhsT, false);
  case COMB(TY_CHAR, TY_BYTE):     // fallthrough
  case COMB(TY_CHAR, TY_CHAR):     // fallthrough
  case COMB(TY_STRING, TY_STRING): // fallthrough
  case COMB(TY_STRING, TY_PTR):    // fallthrough
  case COMB(TY_BOOL, TY_BOOL):     // fallthrough
  case COMB(TY_PTR, TY_STRING):
    return rhsV();
  case COMB(TY_PTR, TY_PTR):
    return lhsSTy.getContainedTy() == rhsSTy.getContainedTy() ? rhsV() : builder.CreatePointerCast(rhsV(), lhsT);
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: (cast)"); // GCOV_EXCL_LINE
}

PtrAndValue OpRuleConversionManager::callBinaryOperatorOverloadFct(const ASTNode *node, auto &lhs, auto &rhs, size_t opIdx) {
  const size_t manIdx = irGenerator->manIdx;
  assert(!node->opFct.empty() && node->opFct.size() > manIdx);
  assert(!node->opFct.at(manIdx).empty() && node->opFct.at(manIdx).size() > opIdx);
  const Function *opFct = node->opFct.at(manIdx).at(opIdx);
  assert(opFct != nullptr);

  const std::string mangledName = opFct->getMangledName();
  Scope *accessScope = opFct->entry->scope;
  assert(accessScope != nullptr);

  // Get arg values
  llvm::Value *argValues[2] = {lhs(), rhs()};

  // Obtain information about the call
  const bool isImported = accessScope->isImportedBy(irGenerator->rootScope);
  const CodeLoc &callLoc = node->codeLoc;
  const CodeLoc &defLoc = opFct->entry->getDeclCodeLoc();
  const bool isDownCall = defLoc.line > callLoc.line || (defLoc.line == callLoc.line && defLoc.col > callLoc.col);

  // Function is not defined in the current module -> declare it
  // This can happen when:
  // 1) If this is an imported source file
  // 2) This is a down-call to a function, which is defined later in the same file
  if (isImported || isDownCall) {
    // Get returnType
    llvm::Type *returnType = builder.getVoidTy();
    if (!opFct->returnType.is(TY_DYN))
      returnType = opFct->returnType.toLLVMType(context, accessScope);

    // Get arg types
    std::vector<llvm::Type *> argTypes;
    for (const SymbolType &paramType : opFct->getParamTypes())
      argTypes.push_back(paramType.toLLVMType(context, accessScope));

    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
    irGenerator->module->getOrInsertFunction(mangledName, fctType);
  }

  // Get callee function
  llvm::Function *callee = irGenerator->module->getFunction(mangledName);
  assert(callee != nullptr);

  // Generate function call
  llvm::Value *resultValue = builder.CreateCall(callee, argValues);

  return {.value = resultValue};
}

} // namespace spice::compiler