// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

// Forward declarations
class Function;
class Struct;
class Interface;

class NameMangling {
public:
  // Public methods
  [[nodiscard]] static std::string mangleFunction(const Function &spiceFunc);
  [[nodiscard]] static std::string mangleStruct(const Struct &spiceStruct);
  [[nodiscard]] static std::string mangleType(const SymbolType &type);

private:
  // Typedefs
  using TypeChainElement = SymbolType::TypeChainElement;

  // Private methods
  static void mangleType(std::stringstream &out, const SymbolType &type);
  static void mangleTypeChainElement(std::stringstream &out, const TypeChainElement &chainElement, bool signedness);
};

} // namespace spice::compiler