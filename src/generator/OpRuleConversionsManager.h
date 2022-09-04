// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/SymbolType.h>

#include <llvm/IR/Value.h>

// Forward declarations
class GeneratorVisitor;
class StdFunctionManager;
struct CodeLoc;

#define COMB(en1, en2) ((en1) | ((en2) << 16))

class OpRuleConversionsManager {
public:
  // Constructors
  explicit OpRuleConversionsManager(GeneratorVisitor *generator);

  // Public methods
  llvm::Value *getPlusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                                SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getMinusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                                 SymbolTable *accessScope);
  llvm::Value *getMulEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getDivEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHLEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getSHREqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getAndEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getOrEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getXorEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseAndInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseOrInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getBitwiseXorInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                            const CodeLoc &codeLoc);
  llvm::Value *getNotEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                               const CodeLoc &codeLoc);
  llvm::Value *getLessInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getLessEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getGreaterEqualInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getShiftLeftInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getShiftRightInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getPlusInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                           SymbolTable *accessScope, const CodeLoc &codeLoc);
  llvm::Value *getMinusInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                            SymbolTable *accessScope);
  llvm::Value *getMulInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy,
                          const CodeLoc &codeLoc);
  llvm::Value *getDivInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getRemInst(llvm::Value *lhs, llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy);
  llvm::Value *getPrefixMinusInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixPlusPlusInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixMinusMinusInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixNotInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPrefixBitwiseNotInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPostfixPlusPlusInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getPostfixMinusMinusInst(llvm::Value *lhs, const SymbolType &lhsTy);
  llvm::Value *getCastInst(llvm::Value *rhs, const SymbolType &lhsTy, const SymbolType &rhsTy, SymbolTable *accessScope);

private:
  // Members
  GeneratorVisitor *generator;
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  const StdFunctionManager *stdFunctionManager;

  // Private methods
  llvm::Value *propagateValueToStringObject(const SymbolType &symbolType, llvm::Value *operandValue);
};