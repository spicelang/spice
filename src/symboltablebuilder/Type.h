// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/TypeChain.h>

// LLVM forward declarations
namespace llvm {
class Type;
} // namespace llvm

namespace spice::compiler {

// Forward declarations
class SymbolTable;
class ASTNode;
class Scope;
class GenericType;
class Struct;
class Interface;

class Type {
public:
  // Constructors
  explicit Type(SuperType superType);
  Type(SuperType superType, const std::string &subType);
  Type(SuperType superType, const std::string &subType, uint64_t typeId, const TypeChainElementData &data,
       const QualTypeList &templateTypes);
  explicit Type(TypeChain typeChain);

  // Getters and setters on type parts
  [[nodiscard]] SuperType getSuperType() const;
  [[nodiscard]] const std::string &getSubType() const;
  [[nodiscard]] unsigned int getArraySize() const;
  [[nodiscard]] Scope *getBodyScope() const;
  [[nodiscard]] const QualType &getFunctionReturnType() const;
  [[nodiscard]] QualTypeList getFunctionParamTypes() const;
  [[nodiscard]] const QualTypeList &getFunctionParamAndReturnTypes() const;
  [[nodiscard]] bool hasLambdaCaptures() const;
  [[nodiscard]] const QualTypeList &getTemplateTypes() const;

  // Queries on the type
  [[nodiscard]] bool is(SuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::initializer_list<SuperType> &superTypes) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] bool isPrimitive() const;
  [[nodiscard]] bool isExtendedPrimitive() const;
  [[nodiscard]] bool isPtr() const;
  [[nodiscard]] bool isRef() const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool hasAnyGenericParts() const;

  // Complex queries on the type
  [[nodiscard]] bool isSameContainerTypeAs(const Type *other) const;
  [[nodiscard]] bool matches(const Type *otherType, bool ignoreArraySize) const;

  // Serialization
  void getName(std::stringstream &name, bool withSize, bool ignorePublic, bool withAliases) const;
  [[nodiscard]] std::string getName(bool withSize, bool ignorePublic, bool withAliases) const;

  // Get new type, based on this one
  [[nodiscard]] const Type *toPtr(const ASTNode *node) const;
  [[nodiscard]] const Type *toRef(const ASTNode *node) const;
  [[nodiscard]] const Type *toArr(const ASTNode *node, unsigned int size, bool skipDynCheck) const;
  [[nodiscard]] const Type *getContained() const;
  [[nodiscard]] const Type *replaceBase(const Type *newBaseType) const;
  [[nodiscard]] const Type *removeReferenceWrapper() const;
  [[nodiscard]] const Type *getBase() const;
  [[nodiscard]] const Type *getWithLambdaCaptures(bool enabled) const;
  [[nodiscard]] const Type *getWithBodyScope(Scope *bodyScope) const;
  [[nodiscard]] const Type *getWithTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] const Type *getWithBaseTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] const Type *getWithFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes) const;

  // LLVM helpers
  [[nodiscard]] llvm::Type *toLLVMType(SourceFile *sourceFile) const;

  // Public static methods
  static void unwrapBoth(const Type *&typeA, const Type *&typeB);
  static bool hasSameTypeChainDepth(const Type *typeA, const Type *typeB);

  // Public members
  TypeChain typeChain;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(Type) == 24);

} // namespace spice::compiler
