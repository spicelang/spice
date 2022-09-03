// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <vector>

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

// Forward declarations
class SymbolTable;
struct CodeLoc;

enum SymbolSuperType {
  TY_INVALID,
  TY_DOUBLE,
  TY_INT,
  TY_SHORT,
  TY_LONG,
  TY_BYTE,
  TY_CHAR,
  TY_STRING,
  TY_BOOL,
  TY_GENERIC,
  TY_STRUCT,
  TY_ENUM,
  TY_DYN,
  TY_PTR,
  TY_ARRAY,
  TY_FUNCTION,
  TY_PROCEDURE,
  TY_IMPORT
};

union TypeChainElementData {
  bool isStringObject; // TY_STRING
  int arraySize;       // TY_ARRAY
};

class SymbolType {
public:
  // Structs
  struct TypeChainElement {
    SymbolSuperType superType;
    std::string subType;
    TypeChainElementData data;
    std::vector<SymbolType> templateTypes;
    llvm::Value *dynamicArraySize;

    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs) {
      // Check super type, subtype and template types
      if (!equalsIgnoreArraySize(lhs, rhs))
        return false;
      // Check data
      switch (lhs.superType) {
      case TY_STRING:
        return lhs.data.isStringObject == rhs.data.isStringObject;
      case TY_ARRAY:
        return lhs.data.arraySize == rhs.data.arraySize;
      default:
        return true;
      }
    }
    friend bool equalsIgnoreArraySize(const TypeChainElement &lhs, const TypeChainElement &rhs) {
      // Check super type, subtype and template types
      return lhs.superType == rhs.superType && lhs.subType == rhs.subType && lhs.templateTypes == rhs.templateTypes;
    }
  };

  // Type defs
  typedef std::stack<TypeChainElement> TypeChain;

  // Constructors
  explicit SymbolType(SymbolSuperType superType) : typeChain({{superType, "", {.arraySize = 0}, {}, nullptr}}) {}
  explicit SymbolType(SymbolSuperType superType, const std::string &subType)
      : typeChain({{superType, subType, {.arraySize = 0}, {}, nullptr}}) {}
  explicit SymbolType(SymbolSuperType superType, const std::string &subType, const TypeChainElementData &data,
                      const std::vector<SymbolType> &templateTypes)
      : typeChain({{superType, subType, data, templateTypes, nullptr}}) {}
  explicit SymbolType(TypeChain types) : typeChain(std::move(types)) {}
  SymbolType() = default;
  virtual ~SymbolType() = default;

  // Public methods
  [[nodiscard]] TypeChain getTypeChain() const;
  SymbolType toPointer(const CodeLoc &codeLoc, llvm::Value *dynamicSize = nullptr) const;
  SymbolType toArray(const CodeLoc &codeLoc, int size = 0) const;
  [[nodiscard]] SymbolType getContainedTy() const;
  [[nodiscard]] SymbolType replaceBaseSubType(const std::string &newSubType) const;
  [[nodiscard]] SymbolType replaceBaseType(const SymbolType &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, SymbolTable *accessScope) const;
  [[nodiscard]] bool isPointer() const;
  [[nodiscard]] bool isPointerOf(SymbolSuperType superType) const;
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
  void setTemplateTypes(std::vector<SymbolType> templateTypes);
  [[nodiscard]] std::vector<SymbolType> getTemplateTypes() const;
  void setSigned(bool isSigned);
  [[nodiscard]] bool isSigned() const;
  [[nodiscard]] std::string getName(bool withSize = false, bool mangledName = false) const;
  [[nodiscard]] int getArraySize() const;
  [[nodiscard]] llvm::Value *getDynamicArraySize() const;
  friend bool equalsIgnoreArraySizes(SymbolType lhs, SymbolType rhs);
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs);
  friend bool operator!=(const SymbolType &lhs, const SymbolType &rhs);

protected:
  // Members
  TypeChain typeChain;
  bool isBaseTypeSigned = true;

  // Protected methods
  void setSubType(const std::string &newSubType);

private:
  // Private methods
  [[nodiscard]] static std::string getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, bool mangledName);
};