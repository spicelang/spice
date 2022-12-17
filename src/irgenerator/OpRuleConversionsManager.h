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

const uint32_t TY_STROBJ = SHRT_MAX; // Max of 16 bit value in 32 bit variable

#define COMB(en1, en2) ((en1) | ((en2) << 16))

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
};

/**
 * Helper class for the IRGenerator to resolve operator instructions for each valid operator/type combination
 */
class OpRuleConversionsManager {
public:
  // Constructors
  OpRuleConversionsManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator);

  // Public methods
  PtrAndValue getPlusEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsSTy, const SymbolType &rhsSTy,
                               Scope *accessScope);
  llvm::Value *getMinusEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                                 Scope *accessScope);
  PtrAndValue getMulEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsSTy, const SymbolType &rhsSTy);
  llvm::Value *getDivEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHLEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHREqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getAndEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getOrEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getXorEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseAndInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseOrInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseXorInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getNotEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                               const CodeLoc &codeLoc);
  llvm::Value *getLessInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getLessEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterEqualInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy,
                                   const SymbolType &rhsTy);
  llvm::Value *getShiftLeftInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getShiftRightInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  PtrAndValue getPlusInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                          Scope *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getMinusInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                            Scope *accessScope);
  PtrAndValue getMulInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                         Scope *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getDivInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getPrefixMinusInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixPlusPlusInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixMinusMinusInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixNotInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixBitwiseNotInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPostfixPlusPlusInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getPostfixMinusMinusInst(const ExprResult &lhs, const SymbolType &lhsTy);
  llvm::Value *getCastInst(const ExprResult &lhs, const ExprResult &rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                           Scope *accessScope);

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;

  // Private methods
  [[nodiscard]] static inline uint32_t getTypeCombination(const SymbolType &lhsTy, const SymbolType &rhsTy) {
    const uint32_t lhsSuperType = lhsTy.is(TY_STRUCT, STROBJ_NAME) ? TY_STROBJ : lhsTy.getSuperType();
    const uint32_t rhsSuperType = rhsTy.is(TY_STRUCT, STROBJ_NAME) ? TY_STROBJ : rhsTy.getSuperType();
    return COMB(lhsSuperType, rhsSuperType);
  }
};