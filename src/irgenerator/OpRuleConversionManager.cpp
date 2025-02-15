// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "OpRuleConversionManager.h"

#include <llvm/IR/Module.h>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <irgenerator/IRGenerator.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

OpRuleConversionManager::OpRuleConversionManager(SourceFile *sourceFile, IRGenerator *irGenerator)
    : context(irGenerator->cliOptions.useLTO ? irGenerator->resourceManager.ltoContext : sourceFile->context),
      builder(sourceFile->builder), irGenerator(irGenerator), stdFunctionManager(irGenerator->stdFunctionManager) {}

LLVMExprResult OpRuleConversionManager::getPlusEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                         LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFAdd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG): {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    llvm::Value *rhsVExt = builder.CreateIntCast(rhsV(), builder.getInt64Ty(), rhsSTy.isSigned());
    return {.value = builder.CreateGEP(elementTy, lhsV(), rhsVExt)};
  }
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: +="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getMinusEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                          LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFSub(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG): {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    llvm::Value *rhsVExt = builder.CreateIntCast(rhsV(), builder.getInt64Ty(), rhsSTy.isSigned());
    llvm::Value *rhsVNeg = builder.CreateNeg(rhsVExt);
    return {.value = builder.CreateGEP(elementTy, lhsV(), rhsVNeg)};
  }
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getMulEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFMul(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsShort, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: *="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getDivEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFDiv(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = generateDiv(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getRemEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSRem(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsShort)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSRem(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getSHLEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateShl(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  default:                                                              // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getSHREqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateLShr(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                              // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getAndEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAnd(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAnd(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getOrEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                       LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateOr(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateOr(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getXorEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateXor(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateXor(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getBitwiseOrInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                         LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):   // fallthrough
  case COMB(TY_BOOL, TY_BOOL):
    return {.value = builder.CreateOr(lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: |"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getBitwiseXorInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                          LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):   // fallthrough
  case COMB(TY_BOOL, TY_BOOL):
    return {.value = builder.CreateXor(lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ^"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getBitwiseAndInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                          LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):     // fallthrough
  case COMB(TY_SHORT, TY_SHORT): // fallthrough
  case COMB(TY_LONG, TY_LONG):   // fallthrough
  case COMB(TY_BYTE, TY_BYTE):   // fallthrough
  case COMB(TY_BOOL, TY_BOOL):
    return {.value = builder.CreateAnd(lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: &"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                     LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};

  // Check if lhs is of type pointer and rhs is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }

  // Check if one value is a string and the other one is a char*
  if ((lhsSTy.is(TY_STRING) && rhsSTy.isPtrTo(TY_CHAR)) || (lhsSTy.isPtrTo(TY_CHAR) && rhsSTy.is(TY_STRING))) {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    return {.value = result};
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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), lhsT);
    return {.value = builder.CreateFCmpOEQ(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOEQ(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateICmpEQ(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsV(), rhsLong)};
  }
  case COMB(TY_BYTE, TY_INT): // fallthrough
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsInt, rhsV())};
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpEQ(lhsShort, rhsV())};
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
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
  case COMB(TY_PROCEDURE, TY_PROCEDURE): {
    const uint64_t typeSize = irGenerator->module->getDataLayout().getTypeSizeInBits(lhsT) / 8;
    llvm::Function *memcmpFct = stdFunctionManager.getMemcmpFct();
    llvm::Value *memcmpResult = builder.CreateCall(memcmpFct, {lhsP(), rhsP(), builder.getInt64(typeSize)});
    return {.value = builder.CreateICmpEQ(memcmpResult, llvm::ConstantInt::get(context, llvm::APInt(32, 0)))};
  }
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: =="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getNotEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                        LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  // Check if both values are of type pointer
  if (lhsSTy.isPtr() && rhsSTy.isPtr())
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};

  // Check if lhs is of type pointer and rhs is of type int
  if (lhsT->isPointerTy() && rhsT->isIntegerTy(32)) {
    llvm::Value *lhsInt = builder.CreatePtrToInt(lhsV(), rhsT);
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }

  // Check if one value is a string and the other one is a char*
  if ((lhsSTy.is(TY_STRING) && rhsSTy.isPtrTo(TY_CHAR)) || (lhsSTy.isPtrTo(TY_CHAR) && rhsSTy.is(TY_STRING))) {
    // Generate call to the function isRawEqual(string, string) of the string std
    llvm::Function *opFct = stdFunctionManager.getStringIsRawEqualStringStringFct();
    llvm::Value *result = builder.CreateCall(opFct, {lhsV(), rhsV()});
    // Negate the result
    return {.value = builder.CreateNot(result)};
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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsLong, rhsV())};
  }
  case COMB(TY_INT, TY_BYTE): // fallthrough
  case COMB(TY_INT, TY_CHAR): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpONE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_BYTE): // fallthrough
  case COMB(TY_SHORT, TY_CHAR): {
    llvm::Value *rhsShort = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsV(), rhsShort)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpONE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateICmpNE(lhsV(), rhsV())};
  case COMB(TY_LONG, TY_BYTE): // fallthrough
  case COMB(TY_LONG, TY_CHAR): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsV(), rhsLong)};
  }
  case COMB(TY_BYTE, TY_INT):
  case COMB(TY_CHAR, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsInt, rhsV())};
  }
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_SHORT): {
    llvm::Value *lhsShort = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateICmpNE(lhsShort, rhsV())};
  }
  case COMB(TY_BYTE, TY_LONG): // fallthrough
  case COMB(TY_CHAR, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
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
  case COMB(TY_PROCEDURE, TY_PROCEDURE): {
    const uint64_t typeSize = irGenerator->module->getDataLayout().getTypeSizeInBits(lhsT) / 8;
    llvm::Function *memcmpFct = stdFunctionManager.getMemcmpFct();
    llvm::Value *memcmpResult = builder.CreateCall(memcmpFct, {lhsP(), rhsP(), builder.getInt64(typeSize)});
    return {.value = builder.CreateICmpNE(memcmpResult, llvm::ConstantInt::get(context, llvm::APInt(32, 0)))};
  }
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getLessInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                    LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLT(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLT(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLT(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateLT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getGreaterInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                       LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGT(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGT(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGT(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGT(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateGT(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getLessEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                         LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLE(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateLE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOLE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateLE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getGreaterEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                            LLVMExprResult &rhs, QualType rhsSTy) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGE(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGE(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateGE(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFCmpOGE(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = generateGE(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >="); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getShiftLeftInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                         LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateShl(lhsV(), rhsInt)};
  }
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateShl(lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: <<"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getShiftRightInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                          LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<2>(node, {lhsV, lhsP, rhsV, rhsP}, opIdx);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_INT, TY_INT):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): // fallthrough
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = generateSHR(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: >>"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                    LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_INT, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFAdd(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsInt, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFAdd(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateAdd(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_LONG, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateAdd(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG): {
    llvm::Value *rhsExt = builder.CreateIntCast(rhsV(), builder.getInt64Ty(), rhsSTy.isSigned());
    return {.value = builder.CreateGEP(lhsSTy.getContained().toLLVMType(irGenerator->sourceFile), lhsV(), rhsExt)};
  }
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: +"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                     LLVMExprResult &rhs, QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_INT, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFSub(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsInt, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFSub(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSub(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_LONG, TY_PTR): {
    llvm::Value *lhsExt = builder.CreateIntCast(lhsV(), builder.getInt64Ty(), lhsSTy.isSigned());
    return {.value = builder.CreateGEP(rhsSTy.getContained().toLLVMType(irGenerator->sourceFile), rhsV(), lhsExt)};
  }
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateSub(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_PTR, TY_INT):   // fallthrough
  case COMB(TY_PTR, TY_SHORT): // fallthrough
  case COMB(TY_PTR, TY_LONG): {
    llvm::Value *rhsExt = builder.CreateIntCast(rhsV(), builder.getInt64Ty(), rhsSTy.isSigned());
    return {.value = builder.CreateGEP(lhsSTy.getContained().toLLVMType(irGenerator->sourceFile), lhsV(), rhsExt)};
  }
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getMulInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                                   QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsInt, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFMul(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsInt, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsLong, rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFMul(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateMul(lhsV(), rhsLong, "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE):
    return {.value = builder.CreateMul(lhsV(), rhsV(), "", false, lhsSTy.isSigned() && rhsSTy.isSigned())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: *"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getDivInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                                   QualType rhsSTy, size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  ResolverFct rhsP = [&] { return irGenerator->resolveAddress(rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

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
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSDiv(lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSDiv(lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFDiv(lhsFP, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSDiv(lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = builder.CreateSDiv(lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_DOUBLE): {
    llvm::Value *lhsFP = generateIToFp(lhsSTy, lhsV(), rhsT);
    return {.value = builder.CreateFDiv(lhsFP, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = builder.CreateSDiv(lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG): // fallthrough
  case COMB(TY_BYTE, TY_BYTE): // fallthrough
  case COMB(TY_CHAR, TY_CHAR):
    return {.value = builder.CreateSDiv(lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: /"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getRemInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                                   QualType rhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);
  llvm::Type *rhsT = rhsSTy.toLLVMType(irGenerator->sourceFile);

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE):
    return {.value = builder.CreateFRem(lhsV(), rhsV())};
  case COMB(TY_INT, TY_INT):
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_INT, TY_SHORT): {
    llvm::Value *rhsInt = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsInt)};
  }
  case COMB(TY_INT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_SHORT, TY_INT): {
    llvm::Value *lhsInt = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsInt, rhsV())};
  }
  case COMB(TY_SHORT, TY_SHORT):
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsV())};
  case COMB(TY_SHORT, TY_LONG): {
    llvm::Value *lhsLong = builder.CreateIntCast(lhsV(), rhsT, rhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsLong, rhsV())};
  }
  case COMB(TY_LONG, TY_INT): // fallthrough
  case COMB(TY_LONG, TY_SHORT): {
    llvm::Value *rhsLong = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned());
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsLong)};
  }
  case COMB(TY_LONG, TY_LONG):
    return {.value = generateRem(lhsSTy, rhsSTy, lhsV(), rhsV())};
  default:                                                            // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: %"); // GCOV_EXCL_LINE
  }
}

LLVMExprResult OpRuleConversionManager::getPrefixMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_DOUBLE:
    return {.value = builder.CreateFNeg(lhsV())};
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return {.value = builder.CreateNeg(lhsV(), "")};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt32(1), "", false, lhsSTy.isSigned())};
  case TY_SHORT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt16(1), "", false, lhsSTy.isSigned())};
  case TY_LONG:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt64(1), "", false, lhsSTy.isSigned())};
  case TY_PTR: {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    return {.value = builder.CreateGEP(elementTy, lhsV(), builder.getInt64(1))};
  }
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (prefix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt32(1), "", false, lhsSTy.isSigned())};
  case TY_SHORT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt16(1), "", false, lhsSTy.isSigned())};
  case TY_LONG:
    return {.value = builder.CreateSub(lhsV(), builder.getInt64(1), "", false, lhsSTy.isSigned())};
  case TY_PTR: {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    return {.value = builder.CreateGEP(elementTy, lhsV(), builder.getInt64(-1))};
  }
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (prefix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixNotInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_BOOL:
    return {.value = builder.CreateNot(lhsV())};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: !"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPrefixBitwiseNotInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy) const {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  switch (lhsSTy.getSuperType()) {
  case TY_INT:   // fallthrough
  case TY_SHORT: // fallthrough
  case TY_LONG:
    return {.value = builder.CreateNot(lhsV())};
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ~"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPostfixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                               size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<1>(node, {lhsV, lhsP}, opIdx);

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt32(1), "", false, lhsSTy.isSigned())};
  case TY_SHORT:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt16(1), "", false, lhsSTy.isSigned())};
  case TY_LONG:
    return {.value = builder.CreateAdd(lhsV(), builder.getInt64(1), "", false, lhsSTy.isSigned())};
  case TY_PTR: {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    return {.value = builder.CreateGEP(elementTy, lhsV(), builder.getInt64(1))};
  }
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: ++ (postfix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getPostfixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy,
                                                                 size_t opIdx) {
  ResolverFct lhsV = [&] { return irGenerator->resolveValue(lhsSTy, lhs); };
  ResolverFct lhsP = [&] { return irGenerator->resolveAddress(lhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();

  // Handle operator overloads
  if (callsOverloadedOpFct(node, opIdx))
    return callOperatorOverloadFct<1>(node, {lhsV, lhsP}, opIdx);

  switch (lhsSTy.getSuperType()) {
  case TY_INT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt32(1), "", false, lhsSTy.isSigned())};
  case TY_SHORT:
    return {.value = builder.CreateSub(lhsV(), builder.getInt16(1), "", false, lhsSTy.isSigned())};
  case TY_LONG:
    return {.value = builder.CreateSub(lhsV(), builder.getInt64(1), "", false, lhsSTy.isSigned())};
  case TY_PTR: {
    llvm::Type *elementTy = lhsSTy.getContained().toLLVMType(irGenerator->sourceFile);
    return {.value = builder.CreateGEP(elementTy, lhsV(), builder.getInt64(-1))};
  }
  default:
    break;
  }
  throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: -- (postfix)"); // GCOV_EXCL_LINE
}

LLVMExprResult OpRuleConversionManager::getCastInst(const ASTNode *node, QualType lhsSTy, LLVMExprResult &rhs,
                                                    QualType rhsSTy) const {
  ResolverFct rhsV = [&] { return irGenerator->resolveValue(rhsSTy, rhs); };
  lhsSTy = lhsSTy.removeReferenceWrapper();
  rhsSTy = rhsSTy.removeReferenceWrapper();
  llvm::Type *lhsT = lhsSTy.toLLVMType(irGenerator->sourceFile);

  // Handle identity casts
  if (lhsSTy.matches(rhsSTy, false, true, false))
    return rhs;

  switch (getTypeCombination(lhsSTy, rhsSTy)) {
  case COMB(TY_DOUBLE, TY_DOUBLE): // fallthrough
  case COMB(TY_INT, TY_INT):       // fallthrough
  case COMB(TY_SHORT, TY_SHORT):   // fallthrough
  case COMB(TY_LONG, TY_LONG):     // fallthrough
  case COMB(TY_BYTE, TY_BYTE):     // fallthrough
  case COMB(TY_CHAR, TY_CHAR):     // fallthrough
  case COMB(TY_STRING, TY_STRING): // fallthrough
  case COMB(TY_BOOL, TY_BOOL):     // fallthrough
  case COMB(TY_PTR, TY_PTR):
    return rhs; // Identity cast
  case COMB(TY_DOUBLE, TY_INT):
  case COMB(TY_DOUBLE, TY_SHORT):
  case COMB(TY_DOUBLE, TY_LONG):
    return {.value = rhsSTy.isSigned() ? builder.CreateSIToFP(rhsV(), lhsT) : builder.CreateUIToFP(rhsV(), lhsT)};
  case COMB(TY_INT, TY_DOUBLE):
    return {.value = lhsSTy.isSigned() ? builder.CreateFPToSI(rhsV(), lhsT) : builder.CreateFPToUI(rhsV(), lhsT)};
  case COMB(TY_INT, TY_SHORT): // fallthrough
  case COMB(TY_INT, TY_LONG):  // fallthrough
  case COMB(TY_INT, TY_BYTE):  // fallthrough
  case COMB(TY_INT, TY_CHAR):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned())};
  case COMB(TY_SHORT, TY_DOUBLE):
    return {.value = lhsSTy.isSigned() ? builder.CreateFPToSI(rhsV(), lhsT) : builder.CreateFPToUI(rhsV(), lhsT)};
  case COMB(TY_SHORT, TY_INT): // fallthrough
  case COMB(TY_SHORT, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned())};
  case COMB(TY_LONG, TY_DOUBLE):
    return {.value = lhsSTy.isSigned() ? builder.CreateFPToSI(rhsV(), lhsT) : builder.CreateFPToUI(rhsV(), lhsT)};
  case COMB(TY_LONG, TY_INT):   // fallthrough
  case COMB(TY_LONG, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_INT):   // fallthrough
  case COMB(TY_BYTE, TY_SHORT): // fallthrough
  case COMB(TY_BYTE, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned())};
  case COMB(TY_BYTE, TY_CHAR):
    return {.value = rhsV()};
  case COMB(TY_CHAR, TY_INT):   // fallthrough
  case COMB(TY_CHAR, TY_SHORT): // fallthrough
  case COMB(TY_CHAR, TY_LONG):
    return {.value = builder.CreateIntCast(rhsV(), lhsT, lhsSTy.isSigned())};
  case COMB(TY_CHAR, TY_BYTE):  // fallthrough
  case COMB(TY_STRING, TY_PTR): // fallthrough
  case COMB(TY_PTR, TY_STRING):
    return {.value = rhsV()};
  default:                                                                 // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Operator fallthrough: (cast)"); // GCOV_EXCL_LINE
  }
}

bool OpRuleConversionManager::callsOverloadedOpFct(const ASTNode *node, size_t opIdx) const {
  const std::vector<const Function *> &opFctList = irGenerator->getOpFctPointers(node);
  return opFctList.size() > opIdx && opFctList.at(opIdx) != nullptr;
}

template <size_t N>
LLVMExprResult OpRuleConversionManager::callOperatorOverloadFct(const ASTNode *node, const std::array<ResolverFct, N * 2> &opV,
                                                                size_t opIdx) {
  static_assert(N == 1 || N == 2, "Only unary and binary operators are overloadable");
  const size_t manIdx = irGenerator->manIdx;
  const std::vector<std::vector<const Function *>> *opFctPointers = node->getOpFctPointers();
  assert(!opFctPointers->empty() && opFctPointers->size() > manIdx);
  assert(!opFctPointers->at(manIdx).empty() && opFctPointers->at(manIdx).size() > opIdx);
  const Function *opFct = opFctPointers->at(manIdx).at(opIdx);
  assert(opFct != nullptr);

  const std::string mangledName = opFct->getMangledName();
  assert(opFct->entry->scope != nullptr);

  // Get arg values
  const QualTypeList &paramTypes = opFct->getParamTypes();
  assert(paramTypes.size() == N);
  llvm::Value *argValues[N];
  argValues[0] = paramTypes[0].isRef() ? opV[1]() : opV[0]();
  if constexpr (N == 2)
    argValues[1] = paramTypes[1].isRef() ? opV[3]() : opV[2]();

  // Function is not defined in the current module -> declare it
  if (!irGenerator->module->getFunction(mangledName)) {
    // Get returnType
    llvm::Type *returnType = builder.getVoidTy();
    if (!opFct->returnType.is(TY_DYN))
      returnType = opFct->returnType.toLLVMType(irGenerator->sourceFile);

    // Get arg types
    std::vector<llvm::Type *> argTypes;
    for (const QualType &paramType : opFct->getParamTypes())
      argTypes.push_back(paramType.toLLVMType(irGenerator->sourceFile));

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

  // Attach address to anonymous symbol to keep track of de-allocation
  SymbolTableEntry *anonymousSymbol = nullptr;
  llvm::Value *resultPtr = nullptr;
  if (opFct->returnType.is(TY_STRUCT)) {
    anonymousSymbol = irGenerator->currentScope->symbolTable.lookupAnonymous(node->codeLoc, opIdx);
    if (anonymousSymbol != nullptr) {
      resultPtr = irGenerator->insertAlloca(result->getType());
      irGenerator->insertStore(result, resultPtr);
      anonymousSymbol->updateAddress(resultPtr);
    }
  }

  // If the return type is reference, return the result value as refPtr
  if (opFct->returnType.isRef())
    return {.ptr = result, .refPtr = resultPtr, .entry = anonymousSymbol};

  // Otherwise as value
  return {.value = result, .ptr = resultPtr, .entry = anonymousSymbol};
}

llvm::Value *OpRuleConversionManager::generateIToFp(const QualType &srcSTy, llvm::Value *srcV, llvm::Type *tgtT) const {
  if (srcSTy.isSigned())
    return builder.CreateSIToFP(srcV, tgtT);
  else
    return builder.CreateUIToFP(srcV, tgtT);
}

llvm::Value *OpRuleConversionManager::generateSHR(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                  llvm::Value *rhsV) const {
  if (lhsSTy.isSigned())
    return builder.CreateAShr(lhsV, rhsV);
  else
    return builder.CreateLShr(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateLT(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSLT(lhsV, rhsV);
  else
    return builder.CreateICmpULT(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateLE(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSLE(lhsV, rhsV);
  else
    return builder.CreateICmpULE(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateGT(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSGT(lhsV, rhsV);
  else
    return builder.CreateICmpUGT(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateGE(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                 llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateICmpSGE(lhsV, rhsV);
  else
    return builder.CreateICmpUGE(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateDiv(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                  llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateSDiv(lhsV, rhsV);
  else
    return builder.CreateUDiv(lhsV, rhsV);
}

llvm::Value *OpRuleConversionManager::generateRem(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                                  llvm::Value *rhsV) const {
  if (lhsSTy.isSigned() && rhsSTy.isSigned())
    return builder.CreateSRem(lhsV, rhsV);
  else
    return builder.CreateURem(lhsV, rhsV);
}

} // namespace spice::compiler