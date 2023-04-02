// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <ast/ASTBuilder.h>
#include <symboltablebuilder/TypeSpecifiers.h>
#include <util/CommonUtil.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class SymbolTable;
class ASTNode;
class Scope;
class GenericType;

// Constants
const char *const STROBJ_NAME = "String";
const long ARRAY_SIZE_UNKNOWN = 0;

enum SymbolSuperType : uint16_t {
  TY_INVALID,
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
  TY_IMPORT
};

class SymbolType {
public:
  // Unions
  union TypeChainElementData {
    // Union fields
    size_t arraySize = 0;   // TY_ARRAY
    Scope *structBodyScope; // TY_STRUCT, TY_INTERFACE, TY_ENUM

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElementData, arraySize)
  };

  // Structs
  struct TypeChainElement {
  public:
    // Overloaded operators
    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs) {
      // Check super type
      if (lhs.superType != rhs.superType)
        return false;
      // Check data
      switch (lhs.superType) {
      case TY_ARRAY:
        return lhs.data.arraySize == rhs.data.arraySize;
      case TY_STRUCT: {
        assert(lhs.data.structBodyScope != nullptr && rhs.data.structBodyScope != nullptr);
        const std::string lhsSubTypeSuffix = CommonUtil::getLastFragment(lhs.subType, SCOPE_ACCESS_TOKEN);
        const std::string rhsSubTypeSuffix = CommonUtil::getLastFragment(rhs.subType, SCOPE_ACCESS_TOKEN);
        return lhsSubTypeSuffix == rhsSubTypeSuffix && lhs.templateTypes == rhs.templateTypes;
      }
      case TY_INTERFACE: {
        const std::string lhsSubTypeSuffix = CommonUtil::getLastFragment(lhs.subType, SCOPE_ACCESS_TOKEN);
        const std::string rhsSubTypeSuffix = CommonUtil::getLastFragment(rhs.subType, SCOPE_ACCESS_TOKEN);
        return lhsSubTypeSuffix == rhsSubTypeSuffix;
      }
      case TY_ENUM: {
        assert(lhs.data.structBodyScope != nullptr && rhs.data.structBodyScope != nullptr);
        const std::string lhsSubTypeSuffix = CommonUtil::getLastFragment(lhs.subType, SCOPE_ACCESS_TOKEN);
        const std::string rhsSubTypeSuffix = CommonUtil::getLastFragment(rhs.subType, SCOPE_ACCESS_TOKEN);
        return lhsSubTypeSuffix == rhsSubTypeSuffix && lhs.data.structBodyScope == rhs.data.structBodyScope;
      }
      case TY_GENERIC:
        return lhs.subType == rhs.subType;
      default:
        return true;
      }
    }
    friend bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs) { return !(lhs == rhs); }

    // Public members
    SymbolSuperType superType = TY_DYN;
    std::string subType;
    TypeChainElementData data = {.arraySize = 0};
    std::vector<SymbolType> templateTypes;

    // Json serializer/deserializer
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElement, superType, subType, data, templateTypes)
  };

  // Typedefs
  using TypeChain = std::vector<TypeChainElement>;

  // Constructors
  SymbolType() = default;
  explicit SymbolType(SymbolSuperType superType)
      : typeChain({TypeChainElement{superType}}), specifiers(TypeSpecifiers::of(superType)) {}
  SymbolType(SymbolSuperType superType, const std::string &subType)
      : typeChain({TypeChainElement{superType, subType}}), specifiers(TypeSpecifiers::of(superType)) {}
  SymbolType(SymbolSuperType superType, const std::string &subType, const TypeChainElementData &data,
             const std::vector<SymbolType> &templateTypes)
      : typeChain({TypeChainElement{superType, subType, data, templateTypes}}), specifiers(TypeSpecifiers::of(superType)) {}
  explicit SymbolType(const TypeChain &types) : typeChain(types), specifiers(TypeSpecifiers::of(types.front().superType)) {}
  SymbolType(TypeChain types, TypeSpecifiers specifiers) : typeChain(std::move(types)), specifiers(specifiers) {}

  // Public methods
  [[nodiscard]] SymbolType toPointer(const ASTNode *node) const;
  [[nodiscard]] SymbolType toReference(const ASTNode *node) const;
  [[nodiscard]] SymbolType toArray(const ASTNode *node, size_t size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] SymbolType getContainedTy() const;
  [[nodiscard]] SymbolType replaceBaseSubType(const std::string &newSubType) const;
  [[nodiscard]] SymbolType replaceBaseType(const SymbolType &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;
  [[nodiscard]] inline bool isPtr() const { return getSuperType() == TY_PTR; }
  [[nodiscard]] inline bool isPtrOf(SymbolSuperType superType) const { return isPtr() && getContainedTy().is(superType); }
  [[nodiscard]] inline bool isRef() const { return getSuperType() == TY_REF; }
  [[nodiscard]] inline bool isRefOf(SymbolSuperType superType) const { return isRef() && getContainedTy().is(superType); }
  [[nodiscard]] inline bool isArray() const { return getSuperType() == TY_ARRAY; }
  [[nodiscard]] inline bool isArrayOf(SymbolSuperType superType) const { return isArray() && getContainedTy().is(superType); }
  [[nodiscard]] inline bool isArrayOf(const SymbolType &symbolType) const { return isArray() && getContainedTy() == symbolType; }
  [[nodiscard]] inline bool is(SymbolSuperType superType) const { return getSuperType() == superType; }
  [[nodiscard]] inline bool is(SymbolSuperType superType, const std::string &subType) const {
    return getSuperType() == superType && getSubType() == subType;
  }
  [[nodiscard]] inline bool isPrimitive() const {
    return isOneOf({TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL});
  }
  [[nodiscard]] bool isBaseType(SymbolSuperType superType) const;
  [[nodiscard]] inline bool isOneOf(const std::vector<SymbolSuperType> &superTypes) const {
    const SymbolSuperType superType = getSuperType();
    return std::any_of(superTypes.begin(), superTypes.end(), [&superType](int type) { return type == superType; });
  }
  [[nodiscard]] bool isSameContainerTypeAs(const SymbolType &otherType) const;
  [[nodiscard]] inline SymbolSuperType getSuperType() const {
    assert(!typeChain.empty());
    return typeChain.back().superType;
  }
  [[nodiscard]] inline const std::string &getSubType() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
    return typeChain.back().subType;
  }
  [[nodiscard]] inline std::string getOriginalSubType() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
    return CommonUtil::getLastFragment(typeChain.back().subType, SCOPE_ACCESS_TOKEN);
  }
  [[nodiscard]] inline TypeChain getTypeChainWithoutReferences() const {
    assert(!typeChain.empty());
    TypeChain typeChainCopy = typeChain;
    while (typeChainCopy.back().superType == TY_REF)
      typeChainCopy.pop_back();
    return typeChainCopy;
  }
  [[nodiscard]] inline SymbolType removeReferenceWrappers() const { return SymbolType(getTypeChainWithoutReferences()); }
  [[nodiscard]] SymbolType getBaseType() const {
    assert(!typeChain.empty());
    return SymbolType({typeChain.front()});
  }
  [[nodiscard]] bool hasAnyGenericParts() const;
  void setTemplateTypes(const std::vector<SymbolType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes);
  [[nodiscard]] inline const std::vector<SymbolType> &getTemplateTypes() const { return typeChain.back().templateTypes; }
  [[nodiscard]] bool isCoveredByGenericTypeList(const std::vector<GenericType> &genericTypeList) const;
  [[nodiscard]] std::string getName(bool withSize = false, bool mangledName = false) const;
  [[nodiscard]] size_t getArraySize() const;
  [[nodiscard]] bool isConst() const;
  [[nodiscard]] bool isSigned() const;
  [[nodiscard]] bool isInline() const;
  [[nodiscard]] bool isPublic() const;
  [[nodiscard]] bool isHeap() const;
  void setStructBodyScope(Scope *bodyScope);
  [[nodiscard]] Scope *getStructBodyScope() const;
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs);
  friend bool operator!=(const SymbolType &lhs, const SymbolType &rhs);
  [[nodiscard]] bool matches(const SymbolType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;

  // Public members
  TypeChain typeChain;
  TypeSpecifiers specifiers;

private:
  // Private methods
  [[nodiscard]] static std::string getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, bool mangledName);

public:
  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SymbolType, typeChain);
};

} // namespace spice::compiler