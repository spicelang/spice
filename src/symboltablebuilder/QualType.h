// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <utility>

#include <llvm/IR/Type.h>

#include <symboltablebuilder/TypeSpecifiers.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

// Forward declarations
class Type;
class ASTNode;
class Scope;
class Struct;
class Interface;
class GenericType;
class QualType;
enum SuperType : uint8_t;

// Constants
const char *const STROBJ_NAME = "String";
const char *const RESULTOBJ_NAME = "Result";
const char *const ERROBJ_NAME = "Error";
const char *const TIOBJ_NAME = "TypeInfo";
const char *const IITERATOR_NAME = "IIterator";
const char *const ARRAY_ITERATOR_NAME = "ArrayIterator";
const uint64_t TYPE_ID_ITERATOR_INTERFACE = 255;
const uint64_t TYPE_ID_ITERABLE_INTERFACE = 256;

// Typedefs
using QualTypeList = std::vector<QualType>;

class QualType {
public:
  // Constructors
  QualType() = default;
  explicit QualType(SuperType superType);
  QualType(SuperType superType, const std::string &subType);
  QualType(const Type *type, TypeSpecifiers specifiers);

  // Getters and setters on type
  [[nodiscard]] const Type *getType() const { return type; }
  void setType(const Type *newType);

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
  Struct *getStruct(const ASTNode *node) const;
  [[nodiscard]] Interface *getInterface(const ASTNode *node) const;

  // Queries on the type
  [[nodiscard]] bool is(SuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::initializer_list<SuperType> &superTypes) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] bool isPrimitive() const;
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
  [[nodiscard]] bool doesImplement(const QualType &symbolType, const ASTNode *node) const;
  [[nodiscard]] bool canBind(const QualType &otherType, bool isTemporary) const;
  [[nodiscard]] bool matches(const QualType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;
  [[nodiscard]] bool matchesInterfaceImplementedByStruct(const QualType &otherType) const;
  [[nodiscard]] bool isSameContainerTypeAs(const QualType &other) const;
  [[nodiscard]] bool isSelfReferencingStructType(const QualType *typeToCompareWith = nullptr) const;
  [[nodiscard]] bool isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const;

  // Serialization
  void getName(std::stringstream &name, bool withSize = false, bool ignorePublic = false) const;
  [[nodiscard]] std::string getName(bool withSize = false, bool ignorePublic = false) const;

  // LLVM helpers
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;

  // Get new type, based on this one
  [[nodiscard]] QualType toPtr(const ASTNode *node) const;
  [[nodiscard]] QualType toRef(const ASTNode *node) const;
  [[nodiscard]] QualType toConstRef(const ASTNode *node) const;
  [[nodiscard]] QualType toArray(const ASTNode *node, size_t size, bool skipDynCheck = false) const;
  [[nodiscard]] QualType toNonConst() const;
  [[nodiscard]] QualType getContained() const;
  [[nodiscard]] QualType getBase() const;
  [[nodiscard]] QualType removeReferenceWrapper() const;
  [[nodiscard]] QualType replaceBaseType(const QualType &newBaseType) const;
  [[nodiscard]] QualType getWithLambdaCaptures(bool enabled = true) const;
  [[nodiscard]] QualType getWithBodyScope(Scope *bodyScope) const;
  [[nodiscard]] QualType getWithTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] QualType getWithBaseTemplateTypes(const QualTypeList &templateTypes) const;
  [[nodiscard]] QualType getWithFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes) const;
  [[nodiscard]] QualType getWithFunctionParamAndReturnTypes(const QualType &returnType, const QualTypeList &paramTypes) const;

  // Getters and setters on specifiers
  [[nodiscard]] TypeSpecifiers &getSpecifiers() { return specifiers; }
  [[nodiscard]] const TypeSpecifiers &getSpecifiers() const { return specifiers; }
  void setSpecifiers(TypeSpecifiers newSpecifiers) { specifiers = newSpecifiers; }

  // Getters and setters on specifier parts
  [[nodiscard]] bool isConst() const;
  [[nodiscard]] bool isSigned() const;
  [[nodiscard]] bool isUnsigned() const;
  [[nodiscard]] bool isInline() const;
  [[nodiscard]] bool isPublic() const;
  [[nodiscard]] bool isHeap() const;
  [[nodiscard]] bool isComposition() const;
  void makeConst(bool isConst = true);
  void makeSigned(bool isSigned = true);
  void makeUnsigned(bool isUnsigned = true);
  void makeInline(bool isInline = true);
  void makePublic(bool isPublic = true);
  void makeHeap(bool isHeap = true);
  void makeComposition(bool isComposition = true);

  // Overloaded operators
  friend bool operator==(const QualType &lhs, const QualType &rhs);
  friend bool operator!=(const QualType &lhs, const QualType &rhs);

  // Public static methods
  static void unwrapBoth(QualType &typeA, QualType &typeB);

private:
  // Private members
  const Type *type = nullptr;
  TypeSpecifiers specifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(QualType) == 16);

} // namespace spice::compiler
