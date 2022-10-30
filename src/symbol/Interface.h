// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symbol/Function.h>
#include <symbol/SymbolSpecifiers.h>

#include "../../lib/json/json.hpp"

// Forward declaration
class SymbolTable;
class SymbolType;
class ASTNode;
struct CodeLoc;

class Interface {
public:
  // Constructors
  Interface(std::string name, SymbolSpecifiers specifiers, std::vector<Function *> methods, const ASTNode *declNode)
      : name(std::move(name)), specifiers(specifiers), methods(std::move(methods)), declNode(declNode) {}

  // Public members
  std::string name;
  SymbolSpecifiers specifiers;
  std::vector<Function *> methods;
  SymbolTable *interfaceScope = nullptr;
  const ASTNode *declNode;
  bool isUsed = false;
};