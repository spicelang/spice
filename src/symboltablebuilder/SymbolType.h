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
class Struct;
class Interface;

// Constants
const char *const STROBJ_NAME = "String";
const char *const ERROBJ_NAME = "Error";
const long ARRAY_SIZE_UNKNOWN = 0;

enum SymbolSuperType : uint8_t {
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
  TY_IMPORT
};

class SymbolType {
public:
  // Unions
  union TypeChainElementData {
    size_t arraySize = 0; // TY_ARRAY
    Scope *bodyScope;     // TY_STRUCT, TY_INTERFACE, TY_ENUM
    bool hasCaptures;     // TY_FUNCTION, TY_PROCEDURE (lambdas)

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElementData, arraySize)
  };

  // Structs
  struct TypeChainElement {
  public:
    // Overloaded operators
    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs);
    friend bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs);
    [[nodiscard]] std::string getName(bool withSize) const;

    // Public members
    SymbolSuperType superType = TY_DYN;
    std::string subType;
    TypeChainElementData data = {.arraySize = 0};
    std::vector<SymbolType> templateTypes;
    std::vector<SymbolType> paramTypes; // First type is the return type

    // Json serializer/deserializer
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElement, superType, subType, data, templateTypes, paramTypes)
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
  [[nodiscard]] SymbolType toConstReference(const ASTNode *node) const;
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
  [[nodiscard]] bool isIterator(const ASTNode *node) const;
  [[nodiscard]] bool isStringObj() const;
  [[nodiscard]] bool isErrorObj() const;
  [[nodiscard]] bool implements(const SymbolType &symbolType, const ASTNode *node) const;
  [[nodiscard]] bool isBaseType(SymbolSuperType superType) const;
  [[nodiscard]] inline bool isOneOf(const std::vector<SymbolSuperType> &superTypes) const {
    const SymbolSuperType superType = getSuperType();
    return std::ranges::any_of(superTypes, [&superType](int type) { return type == superType; });
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
  [[nodiscard]] inline SymbolType removeReferenceWrapper() const { return isRef() ? getContainedTy() : *this; }
  [[nodiscard]] SymbolType getBaseType() const {
    assert(!typeChain.empty());
    return SymbolType({typeChain.front()});
  }
  [[nodiscard]] bool hasAnyGenericParts() const;
  void setTemplateTypes(const std::vector<SymbolType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes);
  [[nodiscard]] const std::vector<SymbolType> &getTemplateTypes() const;
  [[nodiscard]] bool isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const;
  [[nodiscard]] std::string getName(bool withSize = false) const;
  [[nodiscard]] inline size_t getArraySize() const {
    assert(getSuperType() == TY_ARRAY);
    return typeChain.back().data.arraySize;
  }
  [[nodiscard]] inline bool isConst() const { return typeChain.size() == 1 && specifiers.isConst; }
  [[nodiscard]] inline bool isSigned() const {
    assert(isOneOf({TY_INT, TY_SHORT, TY_LONG}));
    return specifiers.isSigned;
  }
  [[nodiscard]] inline bool isInline() const {
    assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
    return specifiers.isInline;
  }
  [[nodiscard]] inline bool isPublic() const {
    assert(isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
    return specifiers.isPublic;
  }
  [[nodiscard]] inline bool isHeap() const { return specifiers.isHeap; }
  inline void setBodyScope(Scope *bodyScope) {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
    typeChain.back().data.bodyScope = bodyScope;
  }
  [[nodiscard]] inline Scope *getBodyScope() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
    return typeChain.back().data.bodyScope;
  }
  void setFunctionReturnType(const SymbolType &returnType);
  [[nodiscard]] const SymbolType &getFunctionReturnType() const;
  void setFunctionParamTypes(const std::vector<SymbolType> &paramTypes);
  [[nodiscard]] std::vector<SymbolType> getFunctionParamTypes() const;
  void setFunctionParamAndReturnTypes(const std::vector<SymbolType> &paramAndReturnTypes);
  [[nodiscard]] const std::vector<SymbolType> &getFunctionParamAndReturnTypes() const;
  void setHasLambdaCaptures(bool hasCaptures);
  [[nodiscard]] bool hasLambdaCaptures() const;
  [[nodiscard]] Struct *getStruct(const ASTNode *node) const;
  [[nodiscard]] Interface *getInterface(const ASTNode *node) const;
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs);
  friend bool operator!=(const SymbolType &lhs, const SymbolType &rhs);
  [[nodiscard]] bool matches(const SymbolType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;

  // Public members
  TypeChain typeChain;
  TypeSpecifiers specifiers;

public:
  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SymbolType, typeChain);
};

} // namespace spice::compiler