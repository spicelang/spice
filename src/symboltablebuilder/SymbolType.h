// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <vector>

#include <util/CommonUtil.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolTable;
class ASTNode;
class Scope;

// Constants
const char *const STROBJ_NAME = "String";
const int ARRAY_SIZE_UNKNOWN = 0;
const int ARRAY_SIZE_DYNAMIC = -1;

enum SymbolSuperType {
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
      default:
        return true;
      }
    }

    // Public members
    SymbolSuperType superType = TY_DYN;
    std::string subType;
    TypeChainElementData data = {.arraySize = 0};
    std::vector<SymbolType> templateTypes;
    llvm::Value *dynamicArraySize = nullptr;

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
  [[nodiscard]] SymbolType toPointer(const ASTNode *node, llvm::Value *dynamicSize = nullptr) const;
  [[nodiscard]] SymbolType toReference(const ASTNode *node) const;
  [[nodiscard]] SymbolType toArray(const ASTNode *node, long size = 0) const;
  [[nodiscard]] SymbolType getContainedTy() const;
  [[nodiscard]] SymbolType replaceBaseSubType(const std::string &newSubType) const;
  [[nodiscard]] SymbolType replaceBaseType(const SymbolType &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, SymbolTable *accessScope) const;
  [[nodiscard]] bool isPointer() const;
  [[nodiscard]] bool isPointerOf(SymbolSuperType superType) const;
  [[nodiscard]] bool isReference() const;
  [[nodiscard]] bool isReferenceOf(SymbolSuperType superType) const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool isArrayOf(SymbolSuperType superType) const;
  [[nodiscard]] bool isArrayOf(const SymbolType &symbolType) const;
  [[nodiscard]] bool is(SymbolSuperType superType) const;
  [[nodiscard]] bool is(SymbolSuperType superType, const std::string &subType) const;
  [[nodiscard]] bool isPrimitive() const;
  [[nodiscard]] bool isBaseType(SymbolSuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::vector<SymbolSuperType> &superTypes) const;
  [[nodiscard]] SymbolSuperType getSuperType() const;
  [[nodiscard]] std::string getSubType() const;
  [[nodiscard]] SymbolType getBaseType() const;
  void setTemplateTypes(const std::vector<SymbolType> &templateTypes);
  [[nodiscard]] const std::vector<SymbolType> &getTemplateTypes() const;
  [[nodiscard]] std::string getName(bool withSize = false, bool mangledName = false) const;
  [[nodiscard]] long getArraySize() const;
  void setSigned(bool value = true);
  [[nodiscard]] bool isSigned() const;
  void setStructBodyScope(Scope *bodyScope);
  [[nodiscard]] Scope *getStructBodyScope() const;
  [[nodiscard]] llvm::Value *getDynamicArraySize() const;
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