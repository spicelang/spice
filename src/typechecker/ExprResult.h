// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/SymbolTableEntry.h>

namespace spice::compiler {

struct ExprResult {
  QualType type;
  SymbolTableEntry *entry = nullptr;

  [[nodiscard]] bool isTemporary() const { return entry == nullptr || entry->anonymous; }
};

} // namespace spice::compiler