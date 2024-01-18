// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <global/GlobalResourceManager.h>
#include <irgenerator/LLVMExprResult.h>
#include <symboltablebuilder/SymbolType.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class IRGenerator;
class StdFunctionManager;
class SymbolTableEntry;
struct CodeLoc;

// Typedefs
using ResolverFct = const std::function<llvm::Value *()>;

#define COMB(en1, en2) (en1 | (en2 << 16))

/**
 * Helper class for the IRGenerator to resolve operator instructions for each valid operator/type combination
 */
class OpRuleConversionManager {
public:
  // Constructors
  OpRuleConversionManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator);

  // Public methods
  LLVMExprResult getPlusEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                  SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getMinusEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                   SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getMulEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getDivEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getRemEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getSHLEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getSHREqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getAndEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getOrEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getXorEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getBitwiseOrInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                  SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getBitwiseXorInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                   SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getBitwiseAndInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                   SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                              Scope *accessScope, size_t opIdx);
  LLVMExprResult getNotEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                 SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getLessInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                             Scope *accessScope);
  LLVMExprResult getGreaterInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getLessEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                  SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getGreaterEqualInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                     SymbolType rhsSTy, Scope *accessScope);
  LLVMExprResult getShiftLeftInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                  SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getShiftRightInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs,
                                   SymbolType rhsSTy, Scope *accessScope, size_t opIdx);
  LLVMExprResult getPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                             Scope *accessScope, size_t opIdx);
  LLVMExprResult getMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                              Scope *accessScope, size_t opIdx);
  LLVMExprResult getMulInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                            Scope *accessScope, size_t opIdx);
  LLVMExprResult getDivInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                            Scope *accessScope, size_t opIdx);
  LLVMExprResult getRemInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy,
                            Scope *accessScope);
  LLVMExprResult getPrefixMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope);
  LLVMExprResult getPrefixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope);
  LLVMExprResult getPrefixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope);
  LLVMExprResult getPrefixNotInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope);
  LLVMExprResult getPrefixBitwiseNotInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope);
  LLVMExprResult getPostfixPlusPlusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope,
                                        size_t opIdx);
  LLVMExprResult getPostfixMinusMinusInst(const ASTNode *node, LLVMExprResult &lhs, SymbolType lhsSTy, Scope *accessScope,
                                          size_t opIdx);
  LLVMExprResult getCastInst(const ASTNode *node, SymbolType lhsSTy, LLVMExprResult &rhs, SymbolType rhsSTy, Scope *accessScope);

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
  [[nodiscard]] llvm::Value *generateIToFp(const SymbolType &srcSTy, llvm::Value *srcV, llvm::Type *tgtT) const;
  [[nodiscard]] llvm::Value *generateSHR(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateLT(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateGT(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateLE(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateGE(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                        llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateDiv(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] llvm::Value *generateRem(const SymbolType &lhsSTy, const SymbolType &rhsSTy, llvm::Value *lhsV,
                                         llvm::Value *rhsV) const;
  [[nodiscard]] static inline uint32_t getTypeCombination(const SymbolType &lhsTy, const SymbolType &rhsTy) {
    return COMB(lhsTy.getSuperType(), rhsTy.getSuperType());
  }
};

} // namespace spice::compiler