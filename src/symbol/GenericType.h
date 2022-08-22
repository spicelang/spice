// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symbol/SymbolType.h>

class GenericType : public SymbolType {
public:
  // Constructors
  explicit GenericType(const SymbolType &type);
  explicit GenericType(const std::string &name, const std::vector<SymbolType> &typeConditions);
  explicit GenericType(const std::string &name);

  // Public methods
  [[nodiscard]] bool checkConditionsOf(const SymbolType &symbolType) const;

private:
  // Members
  std::vector<SymbolType> typeConditions = {SymbolType(TY_DYN)};

  // Private methods
  [[nodiscard]] bool checkTypeConditionOf(const SymbolType &symbolType) const;
};