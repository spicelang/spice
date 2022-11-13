// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <tuple>
#include <vector>

// Forward declarations
class SymbolTable;

class ScopePath {
public:
  explicit ScopePath() = default;
  void pushFragment(const std::string &fragment, SymbolTable *symbolTable);
  [[nodiscard]] bool isEmpty() const;
  void clear();
  [[nodiscard]] SymbolTable *getCurrentScope() const;
  [[nodiscard]] std::string getScopePrefix(bool includeLast = false) const;
  [[nodiscard]] std::string getLastScopeName() const;

private:
  std::vector<std::pair<std::string, SymbolTable *>> fragments;
};