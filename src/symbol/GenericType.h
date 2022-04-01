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
  explicit GenericType(const std::string &name);

private:
  // Members
  std::string name;
};