// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeRegistry.h"

#include <sstream>

#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, std::unique_ptr<Type>> TypeRegistry::types = {};

/**
 * Get or insert a type into the type registry
 *
 * @param type The type to insert
 * @return The inserted type
 */
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

/**
 * Get or insert a type into the type registry
 *
 * @param superType The super type of the type
 * @return The inserted type
 */
const Type *TypeRegistry::getOrInsert(SuperType superType) { return getOrInsert(Type(superType)); }

/**
 * Get or insert a type into the type registry
 *
 * @param superType The super type of the type
 * @param subType The sub type of the type
 * @return The inserted type
 */
const Type *TypeRegistry::getOrInsert(SuperType superType, const std::string &subType) {
  return getOrInsert(Type(superType, subType));
}

/**
 * Get or insert a type into the type registry
 *
 * @param superType The super type of the type
 * @param subType The sub type of the type
 * @param typeId The type ID of the type
 * @param data The data of the type
 * @param templateTypes The template types of the type
 * @return The inserted type
 */
const Type *TypeRegistry::getOrInsert(SuperType superType, const std::string &subType, uint64_t typeId,
                                      const TypeChainElementData &data, const QualTypeList &templateTypes) {
  return getOrInsert(Type(superType, subType, typeId, data, templateTypes));
}

/**
 * Get or insert a type into the type registry
 *
 * @param typeChain The type chain of the type
 * @return The inserted type
 */
const Type *TypeRegistry::getOrInsert(const TypeChain &typeChain) { return getOrInsert(Type(typeChain)); }

/**
 * Get the number of types in the type registry
 *
 * @return The number of types in the type registry
 */
size_t TypeRegistry::getTypeCount() { return types.size(); }

/**
 * Dump all types in the type registry
 */
std::string TypeRegistry::dump() {
  // Sort the keys of the unordered map to get a deterministic output
  std::vector<uint64_t> keys;
  keys.reserve(types.size());
  for (const auto &it : types)
    keys.push_back(it.first);
  std::sort(keys.begin(), keys.end());
  // Serialize type registry
  std::stringstream typeRegistryString;
  for (const uint64_t &key : keys) {
    types.at(key)->getName(typeRegistryString);
    typeRegistryString << "\n";
  }
  return typeRegistryString.str();
}

/**
 * Clear the type registry
 */
void TypeRegistry::clear() { types.clear(); }

} // namespace spice::compiler
