// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolType.h>

#include <llvm/IR/Value.h>

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
  PtrAndValue getPlusEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getMinusEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  PtrAndValue getMulEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getDivEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getRemEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getSHLEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getSHREqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getAndEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getOrEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getXorEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getBitwiseAndInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getBitwiseOrInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getBitwiseXorInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getNotEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getLessInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getGreaterInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getLessEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getGreaterEqualInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN,
                                   Scope *accessScope);
  llvm::Value *getShiftLeftInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getShiftRightInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  PtrAndValue getPlusInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getMinusInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  PtrAndValue getMulInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getDivInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getRemInst(ExprResult &lhs, const ASTNode *lhsN, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);
  llvm::Value *getPrefixMinusInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPrefixPlusPlusInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPrefixMinusMinusInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPrefixNotInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPrefixBitwiseNotInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPostfixPlusPlusInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getPostfixMinusMinusInst(ExprResult &lhs, const ASTNode *lhsN, Scope *accessScope);
  llvm::Value *getCastInst(const SymbolType &lhsSTy, ExprResult &rhs, const ASTNode *rhsN, Scope *accessScope);

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;

  // Private methods
  [[nodiscard]] static inline uint32_t getTypeCombination(const SymbolType &lhsTy, const SymbolType &rhsTy) {
    return COMB(lhsTy.getSuperType(), rhsTy.getSuperType());
  }
};