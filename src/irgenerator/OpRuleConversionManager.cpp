// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "OpRuleConversionManager.h"

#include <stdexcept>

#include <ast/ASTNodes.h>
#include <irgenerator/IRGenerator.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

OpRuleConversionManager::OpRuleConversionManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator)
    : context(resourceManager.context), builder(resourceManager.builder), irGenerator(irGenerator),
      stdFunctionManager(irGenerator->stdFunctionManager) {}

LLVMExprResult OpRuleConversionManager::getPlusEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                         LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                         size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

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

LLVMExprResult OpRuleConversionManager::getMinusEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                          LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                          size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFSub(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    llvm::Type *elementType = lhsSTy.getContainedTy().toLLVMType(context, accessScope);
    return {.value = builder.CreateGEP(elementType, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getMulEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

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

LLVMExprResult OpRuleConversionManager::getDivEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFDiv(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getRemEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getSHLEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateShl(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getSHREqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateLShr(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateLShr(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getAndEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAnd(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateAnd(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getOrEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                       LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateOr(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateOr(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getXorEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateXor(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateXor(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getBitwiseAndInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                          LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                          size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getBitwiseOrInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                         LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                         size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getBitwiseXorInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                          LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                          size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                     LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};

  // Check if one value is of type pointer and one is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }

  // Check for primitive type combinations
  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpOEQ(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpOEQ(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOEQ(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpEQ(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpEQ(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), lhsT);
    return {.value = builder.CreateFCmpOEQ(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpEQ(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOEQ(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpEQ(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpEQ(lhsV(), rhsLong)};
  }
  case COMB(TY_BYTE, TY_INT): // fallthrough
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsShort, rhsV())};
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpEQ(lhsLong, rhsV())};
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    return {.value = result};
  }
  case COMB(TY_BOOL, TY_BOOL):         // fallthrough
  case COMB(TY_FUNCTION, TY_FUNCTION): // fallthrough
  case COMB(TY_PROCEDURE, TY_PROCEDURE):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: =="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getNotEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                        LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                        size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};

  // Check if one value is of type pointer and one is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpONE(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpONE(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpONE(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpNE(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpNE(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpONE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpNE(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpONE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpNE(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateICmpNE(lhsV(), rhsLong)};
  }
  case COMB(TY_BYTE, TY_INT):
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsShort, rhsV())};
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateICmpNE(lhsLong, rhsV())};
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_STRING, TY_STRING): {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    // Negate the result
    return {.value = builder.CreateNot(result)};
  }
  case COMB(TY_BOOL, TY_BOOL):         // fallthrough
  case COMB(TY_FUNCTION, TY_FUNCTION): // fallthrough
  case COMB(TY_PROCEDURE, TY_PROCEDURE):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getLessInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                    LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpOLT(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpOLT(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLT(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLT(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLT(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLT(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getGreaterInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                       LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpOGT(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpOGT(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGT(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGT(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGT(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGT(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getLessEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                         LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                         size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpOLE(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpOLE(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLE(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLE(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateLE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getGreaterEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                            LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                            size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFCmpOGE(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFCmpOGE(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGE(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGE(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = generateGE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >="); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getShiftLeftInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                         LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                         size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getShiftRightInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                          LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope,
                                                          size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateLShr(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateLShr(lhsV(), rhsInt)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateLShr(lhsV(), rhsInt)};
  }
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                    LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFAdd(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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

LLVMExprResult OpRuleConversionManager::getMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                     LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFSub(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFSub(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFSub(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSub(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFSub(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSub(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSub(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFSub(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSub(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_PTR):
    return {.value = builder.CreateGEP(rhsSTy.getContainedTy().toLLVMType(context, accessScope), rhsV(), lhsV())};
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateSub(lhsV(), rhsV())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG):
    return {.value = builder.CreateGEP(lhsSTy.getContainedTy().toLLVMType(context, accessScope), lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getMulInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                   LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFMul(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
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

LLVMExprResult OpRuleConversionManager::getDivInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                   LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&]() { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFDiv(lhsV(), rhsV())};
  case COMB(TY_DOUBLE, TY_INT):   // fallthrough
  case COMB(TY_DOUBLE, TY_SHORT): // fallthrough
  case COMB(TY_DOUBLE, TY_LONG): {
    llvm::Value *rhsFP = generateIToFp(rhsSTy, rhsV(), lhsT);
    return {.value = builder.CreateFDiv(lhsV(), rhsFP)};
  }
  case COMB(TY_INT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFDiv(lhsFP, rhsV())};
  }
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSDiv(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFDiv(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSDiv(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSDiv(lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFDiv(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSDiv(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getRemInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                   LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);
  llvm::Type *rhsT = rhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSRem(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSRem(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, true);
    return {.value = builder.CreateSRem(lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, true);
    return {.value = builder.CreateSRem(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                           Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_DOUBLE:
    return {.value = builder.CreateFMul(lhsV(), llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(double(-1))))};
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return {.value = builder.CreateNeg(lhsV())};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                              Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt32(1))};
  case TY_SHORT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt16(1))};
  case TY_LONG:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt64(1))};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (prefix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                                Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt32(1))};
  case TY_SHORT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt16(1))};
  case TY_LONG:
    return {.value = builder.CreateSub(lhsV(), builder.getInt64(1))};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (prefix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixNotInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                         Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_BOOL:
    return {.value = builder.CreateNot(lhsV())};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixBitwiseNotInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                                Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return {.value = builder.CreateNeg(lhsV())};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ~"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPostfixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                               Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<1>(node, {lhsV, lhsP}, opIdx);

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt32(1))};
  case TY_SHORT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt16(1))};
  case TY_LONG:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt64(1))};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (postfix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPostfixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy,
                                                                 Scope *accessScope, size_t opIdx) {
  ResolverFct lhsV = [&]() { return irGenerator->resolveValue(lhsSTy, lhs, accessScope); };
  ResolverFct lhsP = [&]() { return irGenerator->resolveAddress(lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<1>(node, {lhsV, lhsP}, opIdx);

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt32(1))};
  case TY_SHORT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt16(1))};
  case TY_LONG:
    return {.value = builder.CreateSub(lhsV(), builder.getInt64(1))};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (postfix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getCastInst(const ASTNode *node, SymbolType lhsSTy, LLVMExprResult &rhs,
                                                    SymbolType rhsSTy, Scope *accessScope, size_t opIdx) {
  ResolverFct rhsV = [&]() { return irGenerator->resolveValue(rhsSTy, rhs, accessScope); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(context, accessScope);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = rhsV()};
  case COMB(TY_INT, TY_DOUBLE):
    return {.value = builder.CreateFPToSI(rhsV(), lhsT)};
  case COMB(TY_INT, TY_INT):
    return {.value = rhsV()};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_INT, TY_BYTE):  // fallthrough
  case COMB(TY_INT, TY_CHAR):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, true)};
  case COMB(TY_SHORT, TY_DOUBLE):
    return {.value = builder.CreateFPToSI(rhsV(), lhsT)};
  case COMB(TY_SHORT, TY_INT):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, true)};
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = rhsV()};
  case COMB(TY_SHORT, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, true)};
  case COMB(TY_LONG, TY_DOUBLE):
    return {.value = builder.CreateFPToSI(rhsV(), lhsT)};
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, true)};
  case COMB(TY_LONG, TY_LONG):
    return {.value = rhsV()};
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, false)};
  case COMB(TY_BYTE, TY_CHAR): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = rhsV()};
  case COMB(TY_CHAR, TY_INT):   // fallthrough
  case COMB(TY_CHAR, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, false)};
  case COMB(TY_CHAR, TY_BYTE):     // fallthrough
  case COMB(TY_CHAR, TY_CHAR):     // fallthrough
  case COMB(TY_STRING, TY_STRING): // fallthrough
  case COMB(TY_STRING, TY_PTR):    // fallthrough
  case COMB(TY_BOOL, TY_BOOL):     // fallthrough
  case COMB(TY_PTR, TY_STRING):
    return {.value = rhsV()};
  case COMB(TY_PTR, TY_PTR):
    return {.value = lhsSTy.getContainedTy() == rhsSTy.getContainedTy() ? rhsV() : builder.CreatePointerCast(rhsV(), lhsT)};
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: (cast)"); // GCOV_EXCL_LINE
}

bool OpRuleConversionManager::callsOverloadedOpFct(const ASTNode *node, size_t opIdx) const {
  const std::vector<const Function *> &opFctList = irGenerator->getOpFctPointers(node);
  return opFctList.size() > opIdx && opFctList.at(opIdx) != nullptr;
}

template <size_t N>
LLVMExprResult OpRuleConversionManager::callOperatorOverloadFct(const ASTNode *node, const std::array<ResolverFct, N * 2> &opV,
                                                                size_t opIdx) {
  const size_t manIdx = irGenerator->manIdx;
  assert(!node->opFct.empty() && node->opFct.size() > manIdx);
  assert(!node->opFct.at(manIdx).empty() && node->opFct.at(manIdx).size() > opIdx);
  const Function *opFct = node->opFct.at(manIdx).at(opIdx);
  assert(opFct != nullptr);

  const std::string mangledName = NameMangling::mangleFunction(*opFct);
  Scope *accessScope = opFct->entry->scope;
  assert(accessScope != nullptr);

  // Get arg values
  const std::vector<SymbolType> &paramTypes = opFct->getParamTypes();
  assert(paramTypes.size() == N);
  llvm::Value *argValues[N];
  argValues[0] = paramTypes[0].isRef() ? opV[1]() : opV[0]();
  if constexpr (N == 2)
    argValues[1] = paramTypes[1].isRef() ? opV[3]() : opV[2]();

  // Obtain information about the call
  const bool isImported = accessScope->isImportedBy(irGenerator->rootScope);
  const CodeLoc &callLoc = node->codeLoc;
  const CodeLoc &defLoc = opFct->entry->getDeclCodeLoc();

  // Function is not defined in the current module -> declare it
  if (!irGenerator->module->getFunction(mangledName)) {
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
  llvm::Value *result = builder.CreateCall(callee, argValues);

  // If this is a procedure, return true
  if (opFct->isProcedure())
    return {.constant = builder.getTrue()};

  // Attach address to anonymous symbol to keep track of deallocation
  SymbolTableEntry *anonymousSymbol = nullptr;
  llvm::Value *resultPtr = nullptr;
  if (opFct->returnType.is(TY_STRUCT)) {
    anonymousSymbol = irGenerator->currentScope->symbolTable.lookupAnonymous(node->codeLoc, opIdx);
    if (anonymousSymbol != nullptr) {
      resultPtr = irGenerator->insertAlloca(result->getType());
      builder.CreateStore(result, resultPtr);
      anonymousSymbol->updateAddress(resultPtr);
    }
  }

  // If the return type is reference, return the result value as refPtr
  if (opFct->returnType.isRef())
    return {.ptr = result, .refPtr = resultPtr, .entry = anonymousSymbol};

  // Otherwise as value
  return {.value = result, .ptr = resultPtr, .entry = anonymousSymbol};
}

llvm::Value *OpRuleConversionManager::generateIToFp(const SymbolType &srcSTy, llvm::Value *srcV, llvm::Type *tgtT) const {
  if (srcSTy.isSigned())
    return builder.CreateSIToFP(srcV, tgtT);
  else
    return builder.CreateUIToFP(srcV, tgtT);
}

llvm::Value *OpRuleConversionManager::generateLT(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSLT(lhsV, rhsV);
  else
    return builder.CreateICmpULT(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateLE(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSLE(lhsV, rhsV);
  else
    return builder.CreateICmpULE(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateGT(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSGT(lhsV, rhsV);
  else
    return builder.CreateICmpUGT(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateGE(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSGE(lhsV, rhsV);
  else
    return builder.CreateICmpUGE(lhsV, rhsV);
}

} // namespace spice::compiler