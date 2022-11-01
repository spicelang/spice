// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <global/GlobalResourceManager.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/Value.h>

// Forward declarations
class IRGenerator;
class StdFunctionManager;
class SymbolTable;
struct CodeLoc;

#define COMB(en1, en2) ((en1) | ((en2) << 16))

/**
 * Helper struct for passing the pointer to the value and the value itself in parallel.
 * The caller always has to provide both. If the PtrAndValue struct gets passed back, it either contains both data or only one
 * and the other one is nullptr.
 */
struct PtrAndValue {
  llvm::Value *ptr = nullptr;
  llvm::Value *value = nullptr;
};

/**
 * Helper class for the AnalyzerVisitor to check if type combinations for operators are valid and to retrieve the resulting type
 */
class OpRuleConversionsManager {
public:
  // Constructors
  OpRuleConversionsManager(GlobalResourceManager &resourceManager, IRGenerator *irGenerator);

  // Public methods
  PtrAndValue getPlusEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                               SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getMinusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                                 SymbolTable *accessScope);
  PtrAndValue getMulEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                              SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getDivEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHLEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHREqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getAndEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getOrEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getXorEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseAndInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseOrInst(llvm::Value *lhsV, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseXorInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                            const CodeLoc &codeLoc);
  llvm::Value *getNotEqualInst(const PtrAndValue &lhsData, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                               const CodeLoc &codeLoc);
  llvm::Value *getLessInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getLessEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterEqualInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getShiftLeftInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getShiftRightInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  PtrAndValue getPlusInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                          SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getMinusInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy,
                            SymbolTable *accessScope);
  PtrAndValue getMulInst(const PtrAndValue &lhsData, const PtrAndValue &rhsData, const SymbolType &lhsTy, const SymbolType &rhsTy,
                         SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getDivInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemInst(llvm::Value *lhsV, llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getPrefixMinusInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPrefixPlusPlusInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPrefixMinusMinusInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPrefixNotInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPrefixBitwiseNotInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPostfixPlusPlusInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getPostfixMinusMinusInst(llvm::Value *lhsV, const SymbolType &lhsTy);
  llvm::Value *getCastInst(llvm::Value *rhsV, const SymbolType &lhsTy, const SymbolType &rhsTy, SymbolTable *accessScope);

  // Util methods
  /*llvm::Value *propagateValueToStringObject(SymbolTable *accessScope, const SymbolType &symbolType, llvm::Value *operandPtr,
                                            llvm::Value *operandValue, const CodeLoc &codeLoc);*/

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  IRGenerator *irGenerator;
  const StdFunctionManager &stdFunctionManager;
};