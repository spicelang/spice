// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <vector>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

// Forward declarations
class ErrorFactory;
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
  TY_DYN,
  TY_PTR,
  TY_ARRAY,
  TY_FUNCTION,
  TY_PROCEDURE,
  TY_IMPORT
};

class SymbolType {
public:
  // Structs
  struct TypeChainElement {
    SymbolSuperType superType;
    std::string subType;
    std::vector<SymbolType> templateTypes;
    llvm::Value *dynamicArraySize;

    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs) {
      return lhs.superType == rhs.superType && lhs.subType == rhs.subType && lhs.templateTypes == rhs.templateTypes &&
             lhs.dynamicArraySize == rhs.dynamicArraySize;
    }
  };

  // Type defs
  typedef std::stack<TypeChainElement> TypeChain;

  // Constructors
  explicit SymbolType(SymbolSuperType superType) : typeChain({{superType, "", {}, nullptr}}) {}
  explicit SymbolType(SymbolSuperType superType, const std::string &subType) : typeChain({{superType, subType, {}, nullptr}}) {}
  explicit SymbolType(SymbolSuperType superType, const std::string &subType, const std::vector<SymbolType> &templateTypes)
      : typeChain({{superType, subType, templateTypes, nullptr}}) {}
  explicit SymbolType(TypeChain types) : typeChain(std::move(types)) {}
  SymbolType() = default;
  virtual ~SymbolType() = default;

  // Public methods
  [[nodiscard]] TypeChain getTypeChain() const;
  SymbolType toPointer(const ErrorFactory *err, const CodeLoc &codeLoc, llvm::Value *dynamicSize = nullptr) const;
  SymbolType toArray(const ErrorFactory *err, const CodeLoc &codeLoc, int size = 0) const;
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
  [[nodiscard]] SymbolType setDynamicArraySize(llvm::Value *dynamicArraySize) const;
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