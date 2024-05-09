// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeRegistry.h"

#include <symboltablebuilder/Type.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<std::string, std::unique_ptr<Type>> TypeRegistry::types = {};

const Type *TypeRegistry::get(const std::string &name) {
  const auto it = types.find(name);
  return it != types.end() ? it->second.get() : nullptr;
}

const Type *TypeRegistry::getOrInsert(Type type) {
  const std::string name = type.getName();

  // Check if type already exists
  const auto it = types.find(name);
  if (it != types.end())
    return it->second.get();

  // Create new type
  const auto insertedElement = types.emplace(name, std::make_unique<Type>(std::move(type)));
  return insertedElement.first->second.get();
}

const Type *TypeRegistry::getOrInsert(SuperType superType) { return getOrInsert(Type(superType)); }

} // namespace spice::compiler
