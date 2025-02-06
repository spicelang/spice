// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <symboltablebuilder/QualType.h>

namespace spice::compiler {

// Forward declarations
class Scope;

// Constants
static constexpr long ARRAY_SIZE_UNKNOWN = 0;

enum SuperType : uint8_t {
  TY_INVALID,
  TY_UNRESOLVED,
  TY_DOUBLE,
  TY_INT,
  TY_SHORT,
  TY_LONG,
  TY_BYTE,
  TY_CHAR,
  TY_STRING, // Alias for 'const char*'
  TY_BOOL,
  TY_STRUCT,
  TY_INTERFACE,
  TY_ENUM,
  TY_GENERIC,
  TY_ALIAS,
  TY_DYN,
  TY_PTR,
  TY_REF,
  TY_ARRAY,
  TY_FUNCTION,
  TY_PROCEDURE,
  TY_IMPORT,
};

union TypeChainElementData {
  unsigned int arraySize;     // TY_ARRAY
  Scope *bodyScope = nullptr; // TY_STRUCT, TY_INTERFACE, TY_ENUM
};

// Structs
struct TypeChainElement {
  // Constructors
  TypeChainElement() = default;
  explicit TypeChainElement(SuperType superType) : superType(superType), typeId(superType) {}
  TypeChainElement(SuperType superType, std::string subType)
      : superType(superType), subType(std::move(subType)), typeId(superType) {}
  TypeChainElement(SuperType superType, TypeChainElementData data) : superType(superType), typeId(superType), data(data) {}
  TypeChainElement(SuperType superType, std::string subType, uint64_t typeId, TypeChainElementData data,
                   QualTypeList templateTypes)
      : superType(superType), subType(std::move(subType)), typeId(typeId), data(data), templateTypes(std::move(templateTypes)) {}

  // Overloaded operators
  friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs);
  friend bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs);
  void getName(std::stringstream &name, bool withSize, bool ignorePublic) const;
  [[nodiscard]] std::string getName(bool withSize, bool ignorePublic) const;

  // Public members
  SuperType superType = TY_INVALID;
  std::string subType;
  uint64_t typeId = TY_INVALID;
  TypeChainElementData data = {.arraySize = 0};
  QualTypeList templateTypes;
  QualTypeList paramTypes; // First type is the return type
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(TypeChainElement) == 104);

// Typedefs
using TypeChain = std::vector<TypeChainElement>;

} // namespace spice::compiler
