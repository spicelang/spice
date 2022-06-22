// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <symbol/SymbolTable.h>

class ScopePath {
public:
  explicit ScopePath() = default;
  void pushFragment(const std::string &fragment, SymbolTable *symbolTable);
  void clear();
  [[nodiscard]] SymbolTable *getCurrentScope() const;
  [[nodiscard]] std::string getScopeName() const;
  [[nodiscard]] std::string getScopePrefix(bool includeLast = false) const;

private:
  std::vector<std::pair<std::string, SymbolTable *>> fragments;
};