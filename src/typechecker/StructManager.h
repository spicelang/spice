// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
class TypeChecker;
class Struct;
class Scope;
class SymbolType;
class ASTNode;

using StructSubstantiationList = std::unordered_map</*mangledName=*/std::string, /*structObject=*/Struct>;
using StructCollection = std::unordered_map</*codeLoc=*/std::string, /*manifestations=*/StructSubstantiationList>;

class StructManager {
public:
  // Constructors
  explicit StructManager(const TypeChecker *typeChecker);

  // Public methods
  [[nodiscard]] Struct *insertStruct(const Struct &spiceStruct) const;
  [[nodiscard]] Struct *matchStruct(const std::string &name, const std::vector<SymbolType> &templateTypes,
                                    const ASTNode *node) const;

private:
  // Private members
  Scope *currentScope = nullptr;

  // Private methods
};