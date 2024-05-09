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
enum SuperType : uint8_t;

class QualType {
public:
  // Constructors
  QualType() = default;
  explicit QualType(SuperType superType);
  QualType(SuperType superType, const std::string &subType);
  QualType(const Type &type, TypeSpecifiers specifiers);

  // ToDo: Remove those later on
  QualType(const QualType &other);
  QualType &operator=(const QualType &other);

  // Getters and setters on type
  [[nodiscard]] Type &getType() { return *type; }
  [[nodiscard]] const Type &getType() const { return *type; }
  void setType(const Type &newType);

  // Getters on type parts
  [[nodiscard]] SuperType getSuperType() const;
  [[nodiscard]] const std::string &getSubType() const;
  [[nodiscard]] unsigned int getArraySize() const;
  [[nodiscard]] Scope *getBodyScope() const;
  void setBodyScope(Scope *newBodyScope);
  [[nodiscard]] const QualType &getFunctionReturnType() const;
  void setFunctionReturnType(const QualType &returnType);
  [[nodiscard]] std::vector<QualType> getFunctionParamTypes() const;
  void setFunctionParamTypes(const std::vector<QualType> &paramTypes);
  [[nodiscard]] const std::vector<QualType> &getFunctionParamAndReturnTypes() const;
  void setFunctionParamAndReturnTypes(const std::vector<QualType> &paramAndReturnTypes);
  [[nodiscard]] bool hasLambdaCaptures() const;
  void setHasLambdaCaptures(bool hasCaptures);
  [[nodiscard]] const std::vector<QualType> &getTemplateTypes() const;
  void setTemplateTypes(const std::vector<QualType> &templateTypes);
  void setBaseTemplateTypes(const std::vector<QualType> &templateTypes);
  [[nodiscard]] Struct *getStruct(const ASTNode *node) const;
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

  // Getters on specifiers
  [[nodiscard]] TypeSpecifiers &getSpecifiers() { return specifiers; }
  [[nodiscard]] const TypeSpecifiers &getSpecifiers() const { return specifiers; }

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
  std::unique_ptr<Type> type;
  TypeSpecifiers specifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(QualType) == 16);

} // namespace spice::compiler
