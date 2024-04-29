// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symboltablebuilder/Type.h>
#include <symboltablebuilder/TypeSpecifiers.h>

namespace spice::compiler {

class QualType {
public:
  // Constructors
  QualType() = default;
  /*ToDo: explicit*/ QualType(Type type);
  explicit QualType(SuperType superType);
  QualType(Type type, TypeSpecifiers specifiers);

  // Public methods
  [[nodiscard]] std::string getName(bool withSize = false, bool ignorePublic = false) const;
  [[nodiscard]] ALWAYS_INLINE bool is(SuperType superType) const { return type.is(superType); }
  [[nodiscard]] ALWAYS_INLINE bool isOneOf(const std::initializer_list<SuperType> &superTypes) const {
    return type.isOneOf(superTypes);
  }
  [[nodiscard]] bool isBaseType(SuperType superType) const { return type.isBaseType(superType); }
  [[nodiscard]] Type getBaseType() const { return type.getBaseType(); }

  // Getters and setters on type
  [[nodiscard]] Type &getType() { return type; }
  [[nodiscard]] const Type &getType() const { return type; }

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
  [[nodiscard]] bool isPtr() const { return type.isPtr(); }
  [[nodiscard]] bool isRef() const { return type.isRef(); }
  [[nodiscard]] bool isArray() const { return type.isArray(); }
  [[nodiscard]] bool isConstRef() const;
  [[nodiscard]] QualType toNonConst() const;
  [[nodiscard]] bool canBind(const QualType &otherType, bool isTemporary) const;
  [[nodiscard]] bool matches(const QualType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const;

  // Modify the type
  [[nodiscard]] ALWAYS_INLINE QualType removeReferenceWrapper() const {
    return isRef() ? QualType(type.getContainedTy()) : *this;
  }
  [[nodiscard]] QualType replaceBaseType(const QualType &newBaseType) const;

  // Public static methods
  static void unwrapBoth(QualType &typeA, QualType &typeB);

private:
  // Private members
  Type type;
  TypeSpecifiers specifiers;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(QualType) == 40);

} // namespace spice::compiler
