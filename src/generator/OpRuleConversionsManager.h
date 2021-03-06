// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <Token.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

// Forward declarations
class ErrorFactory;
struct CodeLoc;

enum OpRuleType { P_TY_DOUBLE, P_TY_INT, P_TY_SHORT, P_TY_LONG, P_TY_BYTE_OR_CHAR, P_TY_STRING, P_TY_BOOL, P_TY_PTR };

#define COMB(en1, en2) ((en1) | ((en2) << 16))

class OpRuleConversionsManager {
public:
  explicit OpRuleConversionsManager(std::shared_ptr<llvm::IRBuilder<>> builder, const ErrorFactory *errorFactory)
      : err(errorFactory), builder(std::move(builder)) {}

  // Public methods
  llvm::Value *getPlusEqualInst(llvm::Value *lhs, llvm::Value *rhs, const CodeLoc &codeLoc);
  llvm::Value *getMinusEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getMulEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getDivEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getRemEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getSHLEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getSHREqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getAndEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getOrEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getXorEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getBitwiseAndInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getBitwiseOrInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getBitwiseXorInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getEqualInst(llvm::Value *lhs, llvm::Value *rhs, const CodeLoc &codeLoc);
  llvm::Value *getNotEqualInst(llvm::Value *lhs, llvm::Value *rhs, const CodeLoc &codeLoc);
  llvm::Value *getLessInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getGreaterInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getLessEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getGreaterEqualInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getShiftLeftInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getShiftRightInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getPlusInst(llvm::Value *lhs, llvm::Value *rhs, const CodeLoc &codeLoc);
  llvm::Value *getMinusInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getMulInst(llvm::Value *lhs, llvm::Value *rhs, const CodeLoc &codeLoc);
  llvm::Value *getDivInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getRemInst(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *getPrefixMinusInst(llvm::Value *lhs);
  llvm::Value *getPrefixPlusPlusInst(llvm::Value *lhs);
  llvm::Value *getPrefixMinusMinusInst(llvm::Value *lhs);
  llvm::Value *getPrefixNotInst(llvm::Value *lhs);
  llvm::Value *getPrefixBitwiseNotInst(llvm::Value *lhs);
  llvm::Value *getPostfixPlusPlusInst(llvm::Value *lhs);
  llvm::Value *getPostfixMinusMinusInst(llvm::Value *lhs);
  llvm::Value *getCastInst(llvm::Type *, llvm::Value *lhs);
  static bool isDouble(llvm::Type *llvmType);
  static bool isInt(llvm::Type *llvmType);
  static bool isShort(llvm::Type *llvmType);
  static bool isLong(llvm::Type *llvmType);
  static bool isByteOrChar(llvm::Type *llvmType);
  static bool isString(llvm::Type *llvmType);
  static bool isBool(llvm::Type *llvmType);
  static bool isPtr(llvm::Type *ty);

private:
  // Members
  const ErrorFactory *err;
  std::shared_ptr<llvm::IRBuilder<>> builder;

  // Private methods
  static OpRuleType getSymbolTypeFromLLVMType(llvm::Type *ty);
};