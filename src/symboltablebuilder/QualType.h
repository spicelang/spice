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
enum SuperType : uint8_t;

class QualType {
public:
  // Constructors
  QualType() = default;
  [[deprecated]] /*ToDo: explicit*/ QualType(Type type);
  explicit QualType(SuperType superType);
  QualType(SuperType superType, const std::string &subType);
  QualType(Type type, TypeSpecifiers specifiers);

  // ToDo: Remove those later on
  QualType(const QualType &other);
  QualType &operator=(const QualType &other);

  // Public methods
  void getName(std::stringstream &name, bool withSize = false, bool ignorePublic = false) const;
  [[nodiscard]] std::string getName(bool withSize = false, bool ignorePublic = false) const;
  [[nodiscard]] bool is(SuperType superType) const;
  [[nodiscard]] bool isOneOf(const std::initializer_list<SuperType> &superTypes) const;
  [[nodiscard]] bool isBase(SuperType superType) const;
  [[nodiscard]] QualType getBase() const;

  // Getters and setters on type
  [[nodiscard]] Type &getType() { return *type; }
  [[nodiscard]] const Type &getType() const { return *type; }
  void setType(const Type &newType);

  // Getters and setters on specifiers
  [[nodiscard]] bool isConst() const;
  void makeConst(bool isConst = true);
  [[nodiscard]] bool isSigned() const;
  void makeSigned(bool isSigned = true);
  [[nodiscard]] bool isInline() const;
  void makeInline(bool isInline = true);
  [[nodiscard]] bool isPublic() const;
  void makePublic(bool isPublic = true);
  [[nodiscard]] bool isHeap() const;
  void makeHeap(bool isHeap = true);

  // Queries on the type
  [[nodiscard]] bool isPtr() const;
  [[nodiscard]] bool isPtrTo(SuperType superType) const;
  [[nodiscard]] bool isRef() const;
  [[nodiscard]] bool isRefTo(SuperType superType) const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool isArrayOf(SuperType superType) const;
  [[nodiscard]] bool isConstRef() const;
  [[nodiscard]] SuperType getSuperType() const;
  [[nodiscard]] const std::string &getSubType() const;
  [[nodiscard]] bool hasAnyGenericParts() const;
  [[nodiscard]] bool canBind(const QualType &otherType, bool isTemporary) const;
  [[nodiscard]] bool matches(const QualType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;
  [[nodiscard]] bool isSameContainerTypeAs(const QualType &other) const;
  [[nodiscard]] llvm::Type *toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const;

  // Get new type, based on this one
  [[nodiscard]] QualType toPtr(const ASTNode *node) const;
  [[nodiscard]] QualType toRef(const ASTNode *node) const;
  [[nodiscard]] QualType toArray(const ASTNode *node, size_t size, bool skipDynCheck = false) const;
  [[nodiscard]] QualType toNonConst() const;
  [[nodiscard]] QualType toConstRef(const ASTNode *node) const;
  [[nodiscard]] QualType getContained() const;

  // Overloaded operators
  friend bool operator==(const QualType &lhs, const QualType &rhs);
  friend bool operator!=(const QualType &lhs, const QualType &rhs);

  // Modify the type
  [[nodiscard]] QualType removeReferenceWrapper() const;
  [[nodiscard]] QualType replaceBaseType(const QualType &newBaseType) const;

  // Public static methods
  static void unwrapBoth(QualType &typeA, QualType &typeB);

private:
  // Private members
  std::unique_ptr<Type> type;
  // TypeSpecifiers specifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(QualType) == 8);

} // namespace spice::compiler
