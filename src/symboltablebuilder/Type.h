// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/TypeSpecifiers.h>
#include <util/GlobalDefinitions.h>

#include <llvm/IR/Type.h>

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
const char *const RESULTOBJ_NAME = "Result";
const char *const ERROBJ_NAME = "Error";
const char *const TIOBJ_NAME = "TypeInfo";
const char *const IITERATOR_NAME = "IIterator";
const char *const ARRAY_ITERATOR_NAME = "ArrayIterator";
const long ARRAY_SIZE_UNKNOWN = 0;
const uint64_t TYPE_ID_ITERATOR_INTERFACE = 255;
const uint64_t TYPE_ID_ITERABLE_INTERFACE = 256;

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

class Type {
public:
  // Unions
  union TypeChainElementData {
    unsigned int arraySize;     // TY_ARRAY
    Scope *bodyScope = nullptr; // TY_STRUCT, TY_INTERFACE, TY_ENUM
    bool hasCaptures;           // TY_FUNCTION, TY_PROCEDURE (lambdas)
  };

  // Structs
  struct TypeChainElement {
  public:
    // Constructors
    TypeChainElement() = default;
    explicit TypeChainElement(SuperType superType) : superType(superType), typeId(superType){};
    TypeChainElement(SuperType superType, std::string subType)
        : superType(superType), subType(std::move(subType)), typeId(superType){};
    TypeChainElement(SuperType superType, TypeChainElementData data)
        : superType(superType), typeId(superType), data(data){};
    TypeChainElement(SuperType superType, std::string subType, uint64_t typeId, TypeChainElementData data,
                     const std::vector<QualType> &templateTypes)
        : superType(superType), subType(std::move(subType)), typeId(typeId), data(data), templateTypes(templateTypes){};

    // Overloaded operators
    friend bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs);
    friend bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs);
    void getName(std::stringstream &name, bool withSize) const;
    [[nodiscard]] std::string getName(bool withSize) const;

    // Public members
    SuperType superType = TY_INVALID;
    std::string subType;
    uint64_t typeId = TY_INVALID;
    TypeChainElementData data = {.arraySize = 0};
    std::vector<QualType> templateTypes;
    std::vector<QualType> paramTypes; // First type is the return type
  };

  // Make sure we have no unexpected increases in memory consumption
  static_assert(sizeof(TypeChainElement) == 104);

  // Typedefs
  using TypeChain = std::vector<TypeChainElement>;

  // Constructors
  Type() = default;
  explicit Type(SuperType superType);
  [[deprecated]] Type(const QualType &qualType); // ToDo: Remove
  Type(SuperType superType, const std::string &subType);
  Type(SuperType superType, const std::string &subType, uint64_t typeId, const TypeChainElementData &data,
             const std::vector<QualType> &templateTypes);
  explicit Type(const TypeChain &types);
  Type(TypeChain types, TypeSpecifiers specifiers);

  // Public methods
  [[nodiscard]] [[deprecated]] Type toPointer(const ASTNode *node) const;
  [[nodiscard]] const Type *toPtr(const ASTNode *node) const;
  [[nodiscard]] [[deprecated]] Type toReference(const ASTNode *node) const;
  [[nodiscard]] const Type *toRef(const ASTNode *node) const;
  [[nodiscard]] Type toConstReference(const ASTNode *node) const;
  [[nodiscard]] [[deprecated]] Type toArray(const ASTNode *node, unsigned int size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] const Type *toArr(const ASTNode *node, unsigned int size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] [[deprecated]] Type getContainedTy() const;
  [[nodiscard]] const Type *getContained() const;
  [[nodiscard]] [[deprecated]] Type replaceBaseType(const Type &newBaseType) const;
  [[nodiscard]] const Type *replaceBase(const Type &newBaseType) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;
  [[nodiscard]] ALWAYS_INLINE bool isPtr() const { return getSuperType() == TY_PTR; }
  [[nodiscard]] ALWAYS_INLINE bool isRef() const { return getSuperType() == TY_REF; }
  [[nodiscard]] ALWAYS_INLINE bool isConstRef() const { return getSuperType() == TY_REF && isConst(); }
  [[nodiscard]] ALWAYS_INLINE bool isArray() const { return getSuperType() == TY_ARRAY; }
  [[nodiscard]] ALWAYS_INLINE bool is(SuperType superType) const { return getSuperType() == superType; }
  [[nodiscard]] ALWAYS_INLINE bool isPrimitive() const {
    return isOneOf({TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL});
  }
  [[nodiscard]] bool isIterator(const ASTNode *node) const;
  [[nodiscard]] bool isIterable(const ASTNode *node) const;
  [[nodiscard]] bool isStringObj() const;
  [[nodiscard]] bool isErrorObj() const;
  [[nodiscard]] bool implements(const Type &symbolType, const ASTNode *node) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] ALWAYS_INLINE bool isOneOf(const std::initializer_list<SuperType> &superTypes) const {
    const SuperType superType = getSuperType();
    return std::ranges::any_of(superTypes, [&superType](SuperType type) { return type == superType; });
  }
  [[nodiscard]] bool isSameContainerTypeAs(const Type &other) const;
  [[nodiscard]] ALWAYS_INLINE SuperType getSuperType() const {
    assert(!typeChain.empty());
    return typeChain.back().superType;
  }
  [[nodiscard]] ALWAYS_INLINE const std::string &getSubType() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
    return typeChain.back().subType;
  }
  [[nodiscard]] ALWAYS_INLINE Type removeReferenceWrapper() const { return isRef() ? getContainedTy() : *this; }
  [[nodiscard]] Type getBase() const;
  [[nodiscard]] bool hasAnyGenericParts() const;
  void setTemplateTypes(const std::vector<QualType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<QualType> &templateTypes);
  [[nodiscard]] const std::vector<QualType> &getTemplateTypes() const;
  [[nodiscard]] bool isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const;
  void getName(std::stringstream &name, bool withSize = false, bool ignorePublic = false) const;
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
  [[nodiscard]] ALWAYS_INLINE bool isPublic() const {
    assert(isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
    return specifiers.isPublic;
  }
  ALWAYS_INLINE void setBodyScope(Scope *bodyScope) {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
    typeChain.back().data.bodyScope = bodyScope;
  }
  [[nodiscard]] ALWAYS_INLINE Scope *getBodyScope() const {
    assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
    return typeChain.back().data.bodyScope;
  }
  void setFunctionReturnType(const QualType &returnType);
  [[nodiscard]] const QualType &getFunctionReturnType() const;
  void setFunctionParamTypes(const std::vector<QualType> &paramTypes);
  [[nodiscard]] std::vector<QualType> getFunctionParamTypes() const;
  void setFunctionParamAndReturnTypes(const std::vector<QualType> &paramAndReturnTypes);
  [[nodiscard]] const std::vector<QualType> &getFunctionParamAndReturnTypes() const;
  void setHasLambdaCaptures(bool hasCaptures);
  [[nodiscard]] bool hasLambdaCaptures() const;
  Struct *getStruct(const ASTNode *node) const;
  [[nodiscard]] Interface *getInterface(const ASTNode *node) const;
  friend bool operator==(const Type &lhs, const Type &rhs);
  friend bool operator!=(const Type &lhs, const Type &rhs);
  [[nodiscard]] bool matches(const Type &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;
  [[nodiscard]] bool matchesInterfaceImplementedByStruct(const Type &otherType) const;
  [[nodiscard]] bool canBind(const Type &otherType, bool isTemporary) const;

  // Static util methods
  static void unwrapBoth(Type &typeA, Type &typeB);

  // Public members
  TypeChain typeChain;
  TypeSpecifiers specifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(Type) == 32);

} // namespace spice::compiler