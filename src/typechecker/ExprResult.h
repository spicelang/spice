// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/Type.h>

namespace spice::compiler {

// Forward declarations
class SymbolTableEntry;

struct ExprResult {
  QualType type;
  SymbolTableEntry *entry = nullptr;

  [[nodiscard]] bool isTemporary() const { return entry == nullptr; }
};

} // namespace spice::compiler