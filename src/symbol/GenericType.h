// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symbol/SymbolType.h>

// Typedefs
typedef std::pair<std::string, SymbolType> GenericTypeReplacement; // <generic-type-name, replacement-type>

class GenericType : public SymbolType {
public:
  // Constructors
  explicit GenericType(const std::string &name, const SymbolType &type);
  explicit GenericType(const std::string &name, const std::vector<SymbolType> &typeConditions);
  explicit GenericType(const std::string &name);

  // Public methods
  [[nodiscard]] bool meetsConditions(const SymbolType &symbolType) const;

private:
  // Members
  std::string name;
  std::vector<SymbolType> typeConditions = {SymbolType(TY_DYN)};

  // Private methods
  [[nodiscard]] bool meetsTypeConditions(const SymbolType &symbolType) const;
};