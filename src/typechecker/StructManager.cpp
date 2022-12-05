// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "StructManager.h"

#include <ast/ASTNodes.h>
#include <scope/Scope.h>
#include <typechecker/TypeChecker.h>

StructManager::StructManager(const TypeChecker *typeChecker) : currentScope(typeChecker->currentScope) {}

Struct *StructManager::insertStruct(const Struct &spiceStruct) const {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the struct
  currentScope->structs.insert({spiceStruct.declNode->codeLoc.toString(), std::unordered_map<std::string, Struct>()});
}

Struct *StructManager::matchStruct(const std::string &name, const std::vector<SymbolType> &templateTypes,
                                   const ASTNode *node) const {}