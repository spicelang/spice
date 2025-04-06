// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <symboltablebuilder/Type.h>

namespace spice::compiler {

class TypeRegistry {
public:
  // Constructors
  TypeRegistry() = delete;
  TypeRegistry(const TypeRegistry &) = delete;

  // Public methods
  static uint64_t getTypeHash(const Type& type);
  static const Type *getOrInsert(SuperType superType);
  static const Type *getOrInsert(SuperType superType, const std::string &subType);
  static const Type *getOrInsert(SuperType superType, const std::string &subType, uint64_t typeId,
                                 const TypeChainElementData &data, const QualTypeList &templateTypes);
  static const Type *getOrInsert(const TypeChain &typeChain);
  static size_t getTypeCount();
  static std::string dump();
  static void clear();

private:
  // Private members
  static std::unordered_map<uint64_t, std::unique_ptr<Type>> types;

  // Private methods
  static const Type *getOrInsert(const Type &&type);
};

} // namespace spice::compiler
