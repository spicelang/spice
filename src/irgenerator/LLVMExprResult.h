// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/SymbolTableEntry.h>

#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class SymbolTableEntry;
class ExprNode;

// For routing through multiple LLVM values at once
struct LLVMExprResult {
  llvm::Value *value = nullptr;
  llvm::Constant *constant = nullptr;
  llvm::Value *ptr = nullptr;
  llvm::Value *refPtr = nullptr;
  SymbolTableEntry *entry = nullptr;
  const ExprNode *node = nullptr;

  [[nodiscard]] bool isTemporary() const { return entry == nullptr || entry->anonymous; }
};

} // namespace spice::compiler