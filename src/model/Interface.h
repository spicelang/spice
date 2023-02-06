// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <model/Function.h>
#include <symboltablebuilder/SymbolSpecifiers.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declaration
class Scope;
class SymbolType;
class ASTNode;
struct CodeLoc;

class Interface {
public:
  // Constructors
  Interface(std::string name, std::vector<Function *> methods, ASTNode *declNode)
      : name(std::move(name)), methods(std::move(methods)), declNode(declNode) {}

  // Public members
  std::string name;
  std::vector<Function *> methods;
  Scope *interfaceScope = nullptr;
  ASTNode *declNode;
  bool used = false;
};

} // namespace spice::compiler