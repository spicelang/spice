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
  PtrAndValue getPlusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getMinusEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  PtrAndValue getMulEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getDivEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getRemEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getSHLEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getSHREqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getAndEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getOrEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getXorEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getBitwiseAndInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getBitwiseOrInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getBitwiseXorInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                            const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getNotEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                               const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getLessInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                           const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getGreaterInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                              const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getLessEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getGreaterEqualInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                   const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getShiftLeftInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getShiftRightInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                                 const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  PtrAndValue getPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                          const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                            const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  PtrAndValue getMulInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                         const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getDivInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                          const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getRemInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, ExprResult &rhs,
                          const SymbolType &rhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getPrefixMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                  size_t opIdx);
  llvm::Value *getPrefixPlusPlusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                     size_t opIdx);
  llvm::Value *getPrefixMinusMinusInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                       size_t opIdx);
  llvm::Value *getPrefixNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getPrefixBitwiseNotInst(const ASTNode *node, ExprResult &lhs, const SymbolType &lhsSTy, Scope *accessScope,
                                       size_t opIdx);
  llvm::Value *getPostfixPlusPlusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy, Scope *accessScope, size_t opIdx);
  llvm::Value *getPostfixMinusMinusInst(const ASTNode *node, ExprResult &lhs, SymbolType lhsSTy, Scope *accessScope,
                                        size_t opIdx);
  llvm::Value *getCastInst(const ASTNode *node, const SymbolType &lhsSTy, ExprResult &rhs, const SymbolType &rhsSTy,
                           Scope *accessScope, size_t opIdx);

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;

  // Private methods
  bool callsOverloadedOpFct(const ASTNode *node, size_t opIdx) const;
  PtrAndValue callBinaryOperatorOverloadFct(const ASTNode *node, auto &lhs, auto &rhs, size_t opIdx = 0);
  [[nodiscard]] static inline uint32_t getTypeCombination(const SymbolType &lhsTy, const SymbolType &rhsTy) {
    return COMB(lhsTy.getSuperType(), rhsTy.getSuperType());
  }
};

} // namespace spice::compiler