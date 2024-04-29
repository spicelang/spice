// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace spice::compiler {

// Forward declarations
class Type;
enum SuperType : uint8_t;

class TypeRegistry {
public:
  // Constructors
  TypeRegistry() = delete;
  TypeRegistry(const TypeRegistry &) = delete;

  // Public methods
  static const Type *get(const std::string &name);
  static const Type *getOrInsert(Type type);
  static const Type *getOrInsert(SuperType superType);

private:
  // Private members
  static std::unordered_map<std::string, std::unique_ptr<Type>> types;
};

} // namespace spice::compiler
