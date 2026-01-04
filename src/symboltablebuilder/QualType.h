// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <llvm/IR/Type.h>

#include <symboltablebuilder/TypeQualifiers.h>

namespace spice::compiler {

// Forward declarations
class SourceFile;
class Type;
class ASTNode;
class Scope;
class Struct;
class Interface;
class GenericType;
class QualType;
class SymbolTableEntry;
enum SuperType : uint8_t;

// Constants
constexpr const char *const STROBJ_NAME = "String";
constexpr const char *const RESULTOBJ_NAME = "Result";
constexpr const char *const ERROBJ_NAME = "Error";
constexpr const char *const TIOBJ_NAME = "TypeInfo";
constexpr const char *const IITERATOR_NAME = "IIterator";
constexpr const char *const ARRAY_ITERATOR_NAME = "ArrayIterator";
static constexpr const char *const RESERVED_TYPE_NAMES[] = {
    STROBJ_NAME, RESULTOBJ_NAME, ERROBJ_NAME, TIOBJ_NAME, IITERATOR_NAME, ARRAY_ITERATOR_NAME,
};
static constexpr uint64_t TYPE_ID_ITERATOR_INTERFACE = 255;
static constexpr uint64_t TYPE_ID_ITERABLE_INTERFACE = 256;

// Typedefs
using QualTypeList = std::vector<QualType>;

class QualType {
public:
  // Constructors
  QualType() = default;
  explicit QualType(SuperType superType);
  QualType(SuperType superType, const std::string &subType);
  QualType(const Type *type, TypeQualifiers qualifiers);

  // Getters and setters on type
  [[nodiscard]] const Type *getType() const { return type; }

  // Getters on type parts
  [[nodiscard]] SuperType getSuperType() const;
  [[nodiscard]] const std::string &getSubType() const;
  [[nodiscard]] unsigned int getArraySize() const;
  [[nodiscard]] Scope *getBodyScope() const;
  [[nodiscard]] const QualType &getFunctionReturnType() const;
  [[nodiscard]] QualTypeList getFunctionParamTypes() const;
  [[nodiscard]] const QualTypeList &getFunctionParamAndReturnTypes() const;
  [[nodiscard]] bool hasLambdaCaptures() const;
  [[nodiscard]] const QualTypeList &getTemplateTypes() const;
  [[nodiscard]] Struct *getStruct(const ASTNode *node, const QualTypeList &templateTypes) const;
  [[nodiscard]] Struct *getStruct(const ASTNode *node) const;
  [[nodiscard]] Struct *getStructAndAdjustType(const ASTNode *node, const QualTypeList &templateTypes);
  [[nodiscard]] Struct *getStructAndAdjustType(const ASTNode *node);
  [[nodiscard]] Interface *getInterface(const ASTNode *node, const QualTypeList &templateTypes) const;
  [[nodiscard]] Interface *getInterface(const ASTNode *node) const;

  // Queries on the type
  [[nodiscard]] bool is(SuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::initializer_list<SuperType> &superTypes) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] bool isPrimitive() const;
  [[nodiscard]] bool isExtendedPrimitive() const;
  [[nodiscard]] bool isPtr() const;
  [[nodiscard]] bool isPtrTo(SuperType superType) const;
  [[nodiscard]] bool isRef() const;
  [[nodiscard]] bool isRefTo(SuperType superType) const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool isArrayOf(SuperType superType) const;
  [[nodiscard]] bool isConstRef() const;
  [[nodiscard]] bool isIterator(const ASTNode *node) const;
  [[nodiscard]] bool isIterable(const ASTNode *node) const;
  [[nodiscard]] bool isStringObj() const;
  [[nodiscard]] bool isErrorObj() const;
  [[nodiscard]] bool hasAnyGenericParts() const;

  // Complex queries on the type
  [[nodiscard]] bool isTriviallyConstructible(const ASTNode *node) const;
  [[nodiscard]] bool isTriviallyCopyable(const ASTNode *node) const;
  [[nodiscard]] bool isTriviallyDestructible(const ASTNode *node) const;
  [[nodiscard]] bool doesImplement(const QualType &implementedInterfaceType, const ASTNode *node) const;
  [[nodiscard]] bool canBind(const QualType &inputType, bool isTemporary) const;
  [[nodiscard]] bool matches(const QualType &otherType, bool ignoreArraySize, bool ignoreQualifiers, bool allowConstify) const;
  [[nodiscard]] bool matchesInterfaceImplementedByStruct(const QualType &structType) const;
  [[nodiscard]] bool isSameContainerTypeAs(const QualType &other) const;
  [[nodiscard]] bool isSelfReferencingStructType(const QualType *typeToCompareWith = nullptr) const;
  [[nodiscard]] bool isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const;
  [[nodiscard]] bool needsDeAllocation() const;

  // Serialization
  void getName(std::stringstream &name, bool withSize = false, bool ignorePublic = false, bool withAliases = true) const;
  [[nodiscard]] std::string getName(bool withSize = false, bool ignorePublic = false, bool withAliases = true) const;

  // LLVM helpers
  [[nodiscard]] llvm::Type *toLLVMType(SourceFile *sourceFile) const;

  // Get new type, based on this one
  [[nodiscard]] QualType toPtr(const ASTNode *node) const;
  [[nodiscard]] QualType toRef(const ASTNode *node) const;
  [[nodiscard]] QualType toConstRef(const ASTNode *node) const;
  [[nodiscard]] QualType toArr(const ASTNode *node, size_t size, bool skipDynCheck = false) const;
  [[nodiscard]] QualType toNonConst() const;
  [[nodiscard]] QualType getContained() const;
  [[nodiscard]] QualType getBase() const;
  [[nodiscard]] QualType getAliased(const SymbolTableEntry *aliasEntry) const;
  [[nodiscard]] QualType removeReferenceWrapper() const;
  [[nodiscard]] QualType autoDeReference() const;
  [[nodiscard]] QualType replaceBaseType(const QualType &newBaseType) const;
  [[nodiscard]] QualType getWithLambdaCaptures(bool enabled = true) const;
  [[nodiscard]] QualType getWithBodyScope(Scope *bodyScope) const;
  [[nodiscard]] QualType getWithTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] QualType getWithBaseTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] QualType getWithFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes) const;
  [[nodiscard]] QualType getWithFunctionParamAndReturnTypes(const QualType &returnType, const QualTypeList &paramTypes) const;

  // Getters and setters on qualifiers
  [[nodiscard]] TypeQualifiers &getQualifiers() { return qualifiers; }
  [[nodiscard]] const TypeQualifiers &getQualifiers() const { return qualifiers; }
  void setQualifiers(TypeQualifiers newQualifiers) { qualifiers = newQualifiers; }

  // Getters and setters on qualifier parts
  [[nodiscard]] bool isConst() const;
  [[nodiscard]] bool isSigned() const;
  [[nodiscard]] bool isUnsigned() const;
  [[nodiscard]] bool isInline() const;
  [[nodiscard]] bool isPublic() const;
  [[nodiscard]] bool isHeap() const;
  [[nodiscard]] bool isComposition() const;
  void makeConst(bool isConst = true);
  void makeUnsigned(bool isUnsigned = true);
  void makePublic(bool isPublic = true);
  void makeHeap(bool isHeap = true);

  // Overloaded operators
  friend bool operator==(const QualType &lhs, const QualType &rhs);
  friend bool operator!=(const QualType &lhs, const QualType &rhs);

  // Public static methods
  static void unwrapBoth(QualType &typeA, QualType &typeB);
  static void unwrapBothWithRefWrappers(QualType &typeA, QualType &typeB);

private:
  // Private members
  const Type *type = nullptr;
  TypeQualifiers qualifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(QualType) == 16);

} // namespace spice::compiler
