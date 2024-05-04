// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "QualType.h"

#include <sstream>

#include <symboltablebuilder/Type.h>

namespace spice::compiler {

QualType::QualType(Type t) : type(std::make_unique<Type>(std::move(t))) /*, specifiers(this->type->specifiers)*/ {}
QualType::QualType(SuperType superType) : type(std::make_unique<Type>(superType)) {}
QualType::QualType(Type t, TypeSpecifiers specifiers) : type(std::make_unique<Type>(std::move(t))) /*, specifiers(specifiers)*/ {}

// ToDo: Delete those two later on
QualType::QualType(const QualType &other) {
  type = std::make_unique<Type>(*other.type);
  // specifiers = other.specifiers;
}
QualType &QualType::operator=(const spice::compiler::QualType &other) {
  type = std::make_unique<Type>(*other.type);
  // specifiers = other.specifiers;
  return *this;
}

void QualType::getName(std::stringstream &name, bool withSize, bool ignorePublic) const {
  // Append the specifiers
  /*const TypeSpecifiers defaultForSuperType = TypeSpecifiers::of(type->getSuperType());
  if (!ignorePublic && specifiers.isPublic && !defaultForSuperType.isPublic)
    name << "public ";
  if (specifiers.isInline && !defaultForSuperType.isInline)
    name << "inline ";
  if (specifiers.isComposition && !defaultForSuperType.isComposition)
    name << "compose ";
  if (specifiers.isConst && !defaultForSuperType.isConst)
    name << "const ";
  if (specifiers.isHeap && !defaultForSuperType.isHeap)
    name << "heap ";
  if (specifiers.isSigned && !defaultForSuperType.isSigned)
    name << "signed ";
  if (!specifiers.isSigned && defaultForSuperType.isSigned)
    name << "unsigned ";*/

  // Loop through all chain elements
  type->getName(name, withSize, ignorePublic);
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @param ignorePublic Ignore any potential public specifier
 * @return Symbol type name
 */
std::string QualType::getName(bool withSize, bool ignorePublic) const { // NOLINT(misc-no-recursion)
  std::stringstream name;
  getName(name, withSize, ignorePublic);
  return name.str();
}

bool QualType::is(SuperType superType) const { return type->is(superType); }

bool QualType::isOneOf(const std::initializer_list<SuperType> &superTypes) const { return type->isOneOf(superTypes); }

bool QualType::isBaseType(SuperType superType) const { return type->isBaseType(superType); }

QualType QualType::getBaseType() const { return type->getBaseType(); }

void QualType::setType(const Type &newType) { type = std::make_unique<Type>(newType); }

bool QualType::isConst() const {
  return type->specifiers.isConst;
  // return specifiers.isConst;
}

void QualType::makeConst(bool isConst) {
  type->specifiers.isConst = isConst;
  // specifiers.isConst = isConst;
}

bool QualType::isSigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  return type->specifiers.isSigned;
  // return specifiers.isSigned;
}

void QualType::makeSigned(bool isSigned) {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  type->specifiers.isSigned = isSigned;
  // specifiers.isSigned = isSigned;
}

bool QualType::isInline() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return type->specifiers.isInline;
  // return specifiers.isInline;
}

void QualType::makeInline(bool isInline) {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  type->specifiers.isInline = isInline;
  // specifiers.isInline = isInline;
}

bool QualType::isPublic() const {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  return type->specifiers.isPublic;
  // return specifiers.isPublic;
}

void QualType::makePublic(bool isPublic) {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  type->specifiers.isPublic = isPublic;
  // specifiers.isPublic = isPublic;
}

bool QualType::isHeap() const {
  return type->specifiers.isHeap;
  // return specifiers.isHeap;
}

void QualType::makeHeap(bool isHeap) {
  type->specifiers.isHeap = isHeap;
  // specifiers.isHeap = isHeap;
}

bool QualType::isPtr() const { return type->isPtr(); }
bool QualType::isRef() const { return type->isRef(); }
bool QualType::isArray() const { return type->isArray(); }

bool QualType::isConstRef() const { return isConst() && type->isRef(); }

QualType QualType::toNonConst() const {
  QualType qualType = *this;
  qualType.type->specifiers.isConst = false;
  return qualType;
}

/**
 * Check if a certain input type can be bound (assigned) to the current type->
 *
 * @param inputType Qualified type, which should be bound to the current type
 * @param isTemporary Is the input type a temporary type
 * @return Can be bound or not
 */
bool QualType::canBind(const QualType &inputType, bool isTemporary) const {
  return !isTemporary || inputType.type->isRef() || !type->isRef() || isConstRef();
}

/**
 * Check for the matching compatibility of two types.
 * Useful for struct and function matching as well as assignment type validation and function arg matching.
 *
 * @param otherType Type to compare against
 * @param ignoreArraySize Ignore array sizes
 * @param ignoreSpecifiers Ignore specifiers, except for pointer and reference types
 * @param allowConstify Match when the types are the same, but the lhs type is more const restrictive than the rhs type
 * @return Matching or not
 */
bool QualType::matches(const QualType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const {
  // Compare type
  if (!type->matches(*otherType.type, ignoreArraySize, ignoreSpecifiers, allowConstify))
    return false;

  // Ignore or compare specifiers
  return ignoreSpecifiers || type->specifiers.match(otherType.type->specifiers, allowConstify);
}

bool operator==(const QualType &lhs, const QualType &rhs) { return *lhs.type == *rhs.type; }

bool operator!=(const QualType &lhs, const QualType &rhs) { return !(lhs == rhs); }

QualType QualType::removeReferenceWrapper() const { return isRef() ? QualType(type->getContainedTy()) : *this; }

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
QualType QualType::replaceBaseType(const QualType &newBaseType) const {
  // Create new type
  Type newType = type->replaceBaseType(newBaseType.getType());
  // Create new specifiers
  TypeSpecifiers newSpecifiers = type->specifiers.merge(newBaseType.type->specifiers);
  // Return the new qualified type
  return {newType, newSpecifiers};
}

/**
 * Remove pointers / arrays / references if both types have them as far as possible.
 * Furthermore, remove reference wrappers if possible.
 *
 * @param typeA Candidate type
 * @param typeB Requested type
 */
void QualType::unwrapBoth(QualType &typeA, QualType &typeB) { Type::unwrapBoth(*typeA.type, *typeB.type); }

} // namespace spice::compiler
