// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolType.h>

#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class IRGenerator;
class StdFunctionManager;
class SymbolTableEntry;
struct CodeLoc;

#define COMB(en1, en2) (en1 | (en2 << 16))

/**
 * Helper struct for passing the pointer to the value and the value itself in parallel.
 * If only one is set, the other must be derived by the processing code unit.
 */
struct PtrAndValue {
  llvm::Value *ptr = nullptr;
  llvm::Value *value = nullptr;
};

// For routing through the symbol type as well as the current variable entry
struct ExprResult {
  llvm::Value *ptr = nullptr;
  llvm::Value *value = nullptr;
  llvm::Constant *constant = nullptr;
  SymbolTableEntry *entry = nullptr;
  const ASTNode *node = nullptr;
};

/**
 * Helper class for the IRGenerator to resolve operator instructions for each valid operator/type combination
 */
class OpRuleConversionManager {
public:
  // Constructors
  OpRuleConversionManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator);

  // Public methods
  ExprResult getPlusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getMinusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getMulEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getDivEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getRemEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getSHLEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getSHREqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getAndEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getOrEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                            const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getXorEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getBitwiseAndInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getBitwiseOrInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getBitwiseXorInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                          const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getNotEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                             const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getLessInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                         const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getGreaterInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                            const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getLessEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getGreaterEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getShiftLeftInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getShiftRightInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                         const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                          const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getMulInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs, const SymbolType &rhsSTy,
                        Scope *accessScope, size_t opIdx);
  ExprResult getDivInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs, const SymbolType &rhsSTy,
                        Scope *accessScope, size_t opIdx);
  ExprResult getRemInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs, const SymbolType &rhsSTy,
                        Scope *accessScope, size_t opIdx);
  ExprResult getPrefixMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getPrefixPlusPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                   size_t opIdx);
  ExprResult getPrefixMinusMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                     size_t opIdx);
  ExprResult getPrefixNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getPrefixBitwiseNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                     size_t opIdx);
  ExprResult getPostfixPlusPlusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getPostfixMinusMinusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy, Scope *accessScope, size_t opIdx);
  ExprResult getCastInst(const ASTNode *node, const SymbolType &lhsSTy, ExprResult &rhs, const SymbolType &rhsSTy,
                         Scope *accessScope, size_t opIdx);

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;

  // Private methods
  bool callsOverloadedOpFct(const ASTNode *node, size_t opIdx) const;
  ExprResult callBinaryOperatorOverloadFct(const ASTNode *node, auto &lhsV, auto &rhsV, auto &lhsP, auto &rhsP, size_t opIdx = 0);
  [[nodiscard]] llvm::Value *generateIToFpCast(const SymbolType &srcType, llvm::Value *srcValue, llvm::Type *targetType) const;
  [[nodiscard]] static inline uint32_t getTypeCombination(const SymbolType &lhsTy, const SymbolType &rhsTy) {
    return COMB(lhsTy.getSuperType(), rhsTy.getSuperType());
  }
};

} // namespace spice::compiler