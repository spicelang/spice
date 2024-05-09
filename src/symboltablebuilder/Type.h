// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <symboltablebuilder/QualType.h>
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
    TypeChainElement(SuperType superType, TypeChainElementData data) : superType(superType), typeId(superType), data(data){};
    TypeChainElement(SuperType superType, std::string subType, uint64_t typeId, TypeChainElementData data,
                     const QualTypeList &templateTypes)
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
    QualTypeList templateTypes;
    QualTypeList paramTypes; // First type is the return type
  };

  // Make sure we have no unexpected increases in memory consumption
  static_assert(sizeof(TypeChainElement) == 104);

  // Typedefs
  using TypeChain = std::vector<TypeChainElement>;

  // Constructors
  explicit Type(SuperType superType);
  explicit Type(TypeChain types);
  Type(SuperType superType, const std::string &subType);
  Type(SuperType superType, const std::string &subType, uint64_t typeId, const TypeChainElementData &data,
       const QualTypeList &templateTypes);

  // Getters and setters on type parts
  [[nodiscard]] SuperType getSuperType() const;
  [[nodiscard]] const std::string &getSubType() const;
  [[nodiscard]] unsigned int getArraySize() const;
  [[nodiscard]] Scope *getBodyScope() const;
  void setBodyScope(Scope *bodyScope);
  [[nodiscard]] const QualType &getFunctionReturnType() const;
  void setFunctionReturnType(const QualType &returnType);
  [[nodiscard]] QualTypeList getFunctionParamTypes() const;
  void setFunctionParamTypes(const QualTypeList &paramTypes);
  [[nodiscard]] const QualTypeList &getFunctionParamAndReturnTypes() const;
  void setFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes);
  [[nodiscard]] bool hasLambdaCaptures() const;
  void setHasLambdaCaptures(bool hasCaptures);
  [[nodiscard]] const QualTypeList &getTemplateTypes() const;
  void setTemplateTypes(const QualTypeList &templateTypes);
  void setBaseTemplateTypes(const QualTypeList &templateTypes);

  // Queries on the type
  [[nodiscard]] bool is(SuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::initializer_list<SuperType> &superTypes) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] bool isPrimitive() const;
  [[nodiscard]] bool isPtr() const;
  [[nodiscard]] bool isRef() const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool hasAnyGenericParts() const;

  // Complex queries on the type
  [[nodiscard]] bool isSameContainerTypeAs(const Type &other) const;
  [[nodiscard]] bool matches(const Type &otherType, bool ignoreArraySize) const;

  // Serialization
  void getName(std::stringstream &name, bool withSize = false) const;
  [[nodiscard]] std::string getName(bool withSize = false) const;

  // LLVM helpers
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;

  // Get new type, based on this one
  [[nodiscard]] [[deprecated]] Type toPointer(const ASTNode *node) const;
  [[nodiscard]] const Type *toPtr(const ASTNode *node) const;
  [[nodiscard]] [[deprecated]] Type toReference(const ASTNode *node) const;
  [[nodiscard]] const Type *toRef(const ASTNode *node) const;
  [[nodiscard]] [[deprecated]] Type toArray(const ASTNode *node, unsigned int size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] const Type *toArr(const ASTNode *node, unsigned int size = 0, bool skipDynCheck = false) const;
  [[nodiscard]] [[deprecated]] Type getContainedTy() const;
  [[nodiscard]] const Type *getContained() const;
  [[nodiscard]] [[deprecated]] Type replaceBaseType(const Type &newBaseType) const;
  [[nodiscard]] const Type *replaceBase(const Type &newBaseType) const;
  [[nodiscard]] Type removeReferenceWrapper() const;
  [[nodiscard]] Type getBase() const;

  // Overloaded operators
  friend bool operator==(const Type &lhs, const Type &rhs);
  friend bool operator!=(const Type &lhs, const Type &rhs);

  // Public static methods
  static void unwrapBoth(Type &typeA, Type &typeB);

  // Public members
  TypeChain typeChain;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(Type) == 24);

} // namespace spice::compiler