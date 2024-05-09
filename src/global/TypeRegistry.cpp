// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeRegistry.h"

#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, std::unique_ptr<Type>> TypeRegistry::types = {};

const Type *TypeRegistry::getOrInsert(const Type &&type) {
  const uint64_t hash = std::hash<Type>{}(type);

  // Check if type already exists
  const auto it = types.find(hash);
  if (it != types.end())
    return it->second.get();

  // Create new type
  const auto insertedElement = types.emplace(hash, std::make_unique<Type>(type));
  return insertedElement.first->second.get();
}

const Type *TypeRegistry::getOrInsert(SuperType superType) { return getOrInsert(Type(superType)); }

const Type *TypeRegistry::getOrInsert(SuperType superType, const std::string &subType) {
  return getOrInsert(Type(superType, subType));
}

const Type *TypeRegistry::getOrInsert(SuperType superType, const std::string &subType, uint64_t typeId,
                                      const TypeChainElementData &data, const QualTypeList &templateTypes) {
  return getOrInsert(Type(superType, subType, typeId, data, templateTypes));
}

const Type *TypeRegistry::getOrInsert(const TypeChain &typeChain) { return getOrInsert(Type(typeChain)); }

} // namespace spice::compiler
