// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <irgenerator/LLVMExprResult.h>
#include <symboltablebuilder/Type.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class SourceFile;
class IRGenerator;
class StdFunctionManager;
class SymbolTableEntry;
struct CodeLoc;

// Typedefs
using ResolverFct = const std::function<llvm::Value *()>;

#define COMB(en1, en2) ((en1) | ((en2) << 16))

/**
 * Helper class for the IRGenerator to resolve operator instructions for each valid operator/type combination
 */
class OpRuleConversionManager {
public:
  // Constructors
  OpRuleConversionManager(SourceFile *sourceFile, IRGenerator *irGenerator);

  // Public methods
  LLVMExprResult getPlusEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                  size_t opIdx);
  LLVMExprResult getMinusEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                   QualType rhsSTy, size_t opIdx);
  LLVMExprResult getMulEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                 size_t opIdx);
  LLVMExprResult getDivEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                 size_t opIdx);
  LLVMExprResult getRemEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getSHLEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getSHREqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getAndEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getOrEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getXorEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getBitwiseOrInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                  size_t opIdx);
  LLVMExprResult getBitwiseXorInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                   QualType rhsSTy);
  LLVMExprResult getBitwiseAndInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                   QualType rhsSTy, size_t opIdx);
  LLVMExprResult getEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                              size_t opIdx);
  LLVMExprResult getNotEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                 size_t opIdx);
  LLVMExprResult getLessInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getGreaterInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getLessEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                  QualType rhsSTy);
  LLVMExprResult getGreaterEqualInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                     QualType rhsSTy);
  LLVMExprResult getShiftLeftInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                                  size_t opIdx);
  LLVMExprResult getShiftRightInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs,
                                   QualType rhsSTy, size_t opIdx);
  LLVMExprResult getPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                             size_t opIdx);
  LLVMExprResult getMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                              size_t opIdx);
  LLVMExprResult getMulInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                            size_t opIdx);
  LLVMExprResult getDivInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy,
                            size_t opIdx);
  LLVMExprResult getRemInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);
  LLVMExprResult getPrefixMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy);
  LLVMExprResult getPrefixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy);
  LLVMExprResult getPrefixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy);
  LLVMExprResult getPrefixNotInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy);
  LLVMExprResult getPrefixBitwiseNotInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy);
  LLVMExprResult getPostfixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, size_t opIdx);
  LLVMExprResult getPostfixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, QualType lhsSTy, size_t opIdx);
  LLVMExprResult getCastInst(const ASTNode *node, QualType lhsSTy, LLVMExprResult &rhs, QualType rhsSTy);

  // Util methods
  bool callsOverloadedOpFct(const ASTNode *node, size_t opIdx) const;

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;

  // Private methods
  template <size_t N>
  LLVMExprResult callOperatorOverloadFct(const ASTNode *node, const std::array<ResolverFct, N * 2> &opV, size_t opIdx);
  [[nodiscard]] llvm::Value *generateIToFp(const QualType &srcSTy, llvm::Value *srcV, llvm::Type *tgtT) const;
  [[nodiscard]] llvm::Value *generateSHR(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateLT(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateGT(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateLE(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateGE(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateDiv(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateRem(const QualType &lhsSTy, const QualType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] static uint32_t getTypeCombination(const QualType &lhsTy, const QualType &rhsTy) {
    return COMB(lhsTy.getSuperType(), rhsTy.getSuperType());
  }
};

} // namespace spice::compiler