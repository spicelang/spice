// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <symboltablebuilder/TypeSpecifiers.h>
#include <util/GlobalDefinitions.h>

#include <llvm/IR/Type.h>

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
const char *const TIOBJ_NAME = "TypeInfo";
const char *const IITERATOR_NAME = "IIterator";
const long ARRAY_SIZE_UNKNOWN = 0;
const uint64_t TYPE_ID_ITERATOR_INTERFACE = 255;

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
  TY_IMPORT,
};

class SymbolType {
public:
  // Unions
  union TypeChainElementData {
    unsigned int arraySize;     // TY_ARRAY
    Scope *bodyScope = nullptr; // TY_STRUCT, TY_INTERFACE, TY_ENUM
    bool hasCaptures;           // TY_FUNCTION, TY_PROCEDURE (lambdas)

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElementData, arraySize)
  };

  // Structs
  struct TypeChainElement {
  public:
    // Constructors
    TypeChainElement() = default;
    explicit TypeChainElement(SymbolSuperType superType) : superType(superType), typeId(superType){};
    TypeChainElement(SymbolSuperType superType, std::string subType)
        : superType(superType), subType(std::move(subType)), typeId(superType){};
    TypeChainElement(SymbolSuperType superType, TypeChainElementData data)
        : superType(superType), typeId(superType), data(data){};
    TypeChainElement(SymbolSuperType superType, std::string subType, uint64_t typeId, TypeChainElementData data,
                     const std::vector<SymbolType> &templateTypes)
        : superType(superType), subType(std::move(subType)), typeId(typeId), data(data), templateTypes(templateTypes){};

    // Overloaded operators
    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs);
    friend bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs);
    [[nodiscard]] std::string getName(bool withSize) const;

    // Public members
    SymbolSuperType superType = TY_INVALID;
    std::string subType;
    uint64_t typeId = TY_INVALID;
    TypeChainElementData data = {.arraySize = 0};
    std::vector<SymbolType> templateTypes;
    std::vector<SymbolType> paramTypes; // First type is the return type

    // Json serializer/deserializer
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeChainElement, superType, subType, typeId, data, templateTypes, paramTypes)
  };

  // Make sure we have no unexpected increases in memory consumption
  static_assert(sizeof(TypeChainElement) == 104);

  // Typedefs
  using TypeChain = std::vector<TypeChainElement>;

  // Constructors
  SymbolType() = default;
  explicit SymbolType(SymbolSuperType superType);
  SymbolType(SymbolSuperType superType, const std::string &subType);
  SymbolType(SymbolSuperType superType, const std::string &subType, uint64_t typeId, const TypeChainElementData &data,
             const std::vector<SymbolType> &templateTypes);
  explicit SymbolType(const TypeChain &types);
  SymbolType(TypeChain types, TypeSpecifiers specifiers);

  // Public methods
  [[nodiscard]] SymbolType toPointer(const ASTNode *node) const;
  [[nodiscard]] SymbolType toReference(const ASTNode *node) const;
  [[nodiscard]] SymbolType toConstReference(const ASTNode *node) const;
  [[nodiscard]] SymbolType toArray(const ASTNode *node, unsigned int size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] SymbolType getContainedTy() const;
  [[nodiscard]] SymbolType replaceBaseType(const SymbolType &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;
  [[nodiscard]] ALWAYS_INLINE bool isPtr() const { return getSuperType() == TY_PTR; }
  [[nodiscard]] ALWAYS_INLINE bool isPtrOf(SymbolSuperType superType) const { return isPtr() && getContainedTy().is(superType); }
  [[nodiscard]] ALWAYS_INLINE bool isRef() const { return getSuperType() == TY_REF; }
  [[nodiscard]] ALWAYS_INLINE bool isConstRef() const { return getSuperType() == TY_REF && isConst(); }
  [[nodiscard]] [[maybe_unused]] ALWAYS_INLINE bool isRefOf(SymbolSuperType superType) const {
    return isRef() && getContainedTy().is(superType);
  }
  [[nodiscard]] ALWAYS_INLINE bool isArray() const { return getSuperType() == TY_ARRAY; }
  [[nodiscard]] [[maybe_unused]] ALWAYS_INLINE bool isArrayOf(SymbolSuperType superType) const {
    return isArray() && getContainedTy().is(superType);
  }
  [[nodiscard]] ALWAYS_INLINE bool is(SymbolSuperType superType) const { return getSuperType() == superType; }
  [[nodiscard]] ALWAYS_INLINE bool is(SymbolSuperType superType, const std::string &subType) const {
    return getSuperType() == superType && getSubType() == subType;
  }
  [[nodiscard]] ALWAYS_INLINE bool isPrimitive() const {
    return isOneOf({TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL});
  }
  [[nodiscard]] bool isIterator(const ASTNode *node) const;
  [[nodiscard]] bool isStringObj() const;
  [[nodiscard]] bool isErrorObj() const;
  [[nodiscard]] bool implements(const SymbolType &symbolType, const ASTNode *node) const;
  [[nodiscard]] bool isBaseType(SymbolSuperType superType) const;
  [[nodiscard]] ALWAYS_INLINE bool isOneOf(const std::initializer_list<SymbolSuperType> &superTypes) const {
    const SymbolSuperType superType = getSuperType();
    return std::ranges::any_of(superTypes, [&superType](SymbolSuperType type) { return type == superType; });
  }
  [[nodiscard]] bool isSameContainerTypeAs(const SymbolType &otherType) const;
  [[nodiscard]] ALWAYS_INLINE SymbolSuperType getSuperType() const {
    assert(!typeChain.empty());
    return typeChain.back().superType;
  }
  [[nodiscard]] ALWAYS_INLINE const std::string &getSubType() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
    return typeChain.back().subType;
  }
  [[nodiscard]] ALWAYS_INLINE SymbolType removeReferenceWrapper() const { return isRef() ? getContainedTy() : *this; }
  [[nodiscard]] SymbolType getBaseType() const {
    assert(!typeChain.empty());
    return SymbolType({typeChain.front()}, specifiers);
  }
  [[nodiscard]] bool hasAnyGenericParts() const;
  void setTemplateTypes(const std::vector<SymbolType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes);
  [[nodiscard]] const std::vector<SymbolType> &getTemplateTypes() const;
  [[nodiscard]] bool isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const;
  [[nodiscard]] std::string getName(bool withSize = false, bool ignorePublic = false) const;
  [[nodiscard]] ALWAYS_INLINE unsigned int getArraySize() const {
    assert(getSuperType() == TY_ARRAY);
    return typeChain.back().data.arraySize;
  }
  [[nodiscard]] ALWAYS_INLINE bool isConst() const { return specifiers.isConst; }
  [[nodiscard]] ALWAYS_INLINE bool isSigned() const {
    assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
    return specifiers.isSigned;
  }
  [[nodiscard]] ALWAYS_INLINE bool isInline() const {
    assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
    return specifiers.isInline;
  }
  [[nodiscard]] ALWAYS_INLINE bool isPublic() const {
    assert(isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
    return specifiers.isPublic;
  }
  [[nodiscard]] ALWAYS_INLINE bool isHeap() const { return specifiers.isHeap; }
  ALWAYS_INLINE void setBodyScope(Scope *bodyScope) {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
    typeChain.back().data.bodyScope = bodyScope;
  }
  [[nodiscard]] ALWAYS_INLINE Scope *getBodyScope() const {
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
  [[nodiscard]] bool canBind(const SymbolType &otherType, bool isTemporary) const;

  // Static util methods
  static void unwrapBoth(SymbolType &typeA, SymbolType &typeB);

  // Public members
  TypeChain typeChain;
  TypeSpecifiers specifiers;

public:
  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SymbolType, typeChain);
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(SymbolType) == 32);

} // namespace spice::compiler