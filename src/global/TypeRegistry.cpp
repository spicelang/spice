// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeRegistry.h"

#include <ranges>
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
  const auto [iter, inserted] = types.emplace(hash, std::make_unique<Type>(type));
  return iter->second.get();
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
  std::vector<std::string> typeStrings;
  typeStrings.reserve(types.size());
  for (const std::unique_ptr<Type>& type : types | std::views::values)
    typeStrings.push_back(type->getName());
  // Sort to ensure deterministic output
  std::ranges::sort(typeStrings);
  // Serialize type registry
  std::stringstream typeRegistryString;
  for (const std::string &typeString : typeStrings)
    typeRegistryString << typeString << "\n";
  return typeRegistryString.str();
}

/**
 * Clear the type registry
 */
void TypeRegistry::clear() { types.clear(); }

} // namespace spice::compiler
