// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <vector>

#include <util/CommonUtil.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class SymbolTable;
class ASTNode;
class Scope;

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
  TY_GENERIC,
  TY_STRUCT,
  TY_INTERFACE,
  TY_ENUM,
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
    long arraySize = 0;     // TY_ARRAY
    bool numericSigned;     // TY_INT, TY_SHORT, TY_LONG
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
        const std::string lhsSubTypeSuffix = CommonUtil::getLastFragment(lhs.subType, "::");
        const std::string rhsSubTypeSuffix = CommonUtil::getLastFragment(rhs.subType, "::");
        return lhsSubTypeSuffix == rhsSubTypeSuffix && lhs.templateTypes == rhs.templateTypes &&
               lhs.data.structBodyScope == rhs.data.structBodyScope;
      }
      case TY_INTERFACE:
      case TY_ENUM: {
        assert(lhs.data.structBodyScope != nullptr && rhs.data.structBodyScope != nullptr);
        const std::string lhsSubTypeSuffix = CommonUtil::getLastFragment(lhs.subType, "::");
        const std::string rhsSubTypeSuffix = CommonUtil::getLastFragment(rhs.subType, "::");
        return lhsSubTypeSuffix == rhsSubTypeSuffix && lhs.data.structBodyScope == rhs.data.structBodyScope;
      }
      case TY_GENERIC:
        return lhs.subType == rhs.subType;
      default:
        return true;
      }
    }

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
  explicit SymbolType(SymbolSuperType superType) : typeChain({TypeChainElement{superType}}) {}
  SymbolType(SymbolSuperType superType, const std::string &subType) : typeChain({TypeChainElement{superType, subType}}) {}
  SymbolType(SymbolSuperType superType, const std::string &subType, const TypeChainElementData &data,
             const std::vector<SymbolType> &templateTypes)
      : typeChain({TypeChainElement{superType, subType, data, templateTypes}}) {}
  explicit SymbolType(TypeChain types) : typeChain(std::move(types)) {}

  // Public methods
  [[nodiscard]] SymbolType toPointer(const ASTNode *node) const;
  [[nodiscard]] SymbolType toReference(const ASTNode *node) const;
  [[nodiscard]] SymbolType toArray(const ASTNode *node, long size = 0) const;
  [[nodiscard]] SymbolType getContainedTy() const;
  [[nodiscard]] SymbolType replaceBaseSubType(const std::string &newSubType) const;
  [[nodiscard]] SymbolType replaceBaseType(const SymbolType &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;
  [[nodiscard]] inline bool isPointer() const { return getSuperType() == TY_PTR; }
  [[nodiscard]] inline bool isPointerOf(SymbolSuperType superType) const { return isPointer() && getContainedTy().is(superType); }
  [[nodiscard]] inline bool isReference() const { return getSuperType() == TY_REF; }
  [[nodiscard]] inline bool isReferenceOf(SymbolSuperType superType) const {
    return isReference() && getContainedTy().is(superType);
  }
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
  [[nodiscard]] SymbolType getBaseType() const { return SymbolType({typeChain.front()}); }
  void setTemplateTypes(const std::vector<SymbolType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes);
  [[nodiscard]] inline const std::vector<SymbolType> &getTemplateTypes() const { return typeChain.back().templateTypes; }
  [[nodiscard]] std::string getName(bool withSize = false, bool mangledName = false) const;
  [[nodiscard]] long getArraySize() const;
  void setSigned(bool value = true);
  [[nodiscard]] bool isSigned() const;
  void setStructBodyScope(Scope *bodyScope);
  [[nodiscard]] Scope *getStructBodyScope() const;
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs);
  friend bool operator!=(const SymbolType &lhs, const SymbolType &rhs);

  // Public members
  TypeChain typeChain;

protected:
  // Protected methods
  void setSubType(const std::string &newSubType);

private:
  // Private methods
  [[nodiscard]] static std::string getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, bool mangledName);

public:
  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SymbolType, typeChain);
};

} // namespace spice::compiler