// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "QualType.h"

#include <sstream>

#include <symboltablebuilder/Type.h>

namespace spice::compiler {

QualType::QualType(SuperType superType) : type(std::make_unique<Type>(superType)), specifiers(TypeSpecifiers::of(superType)) {}
QualType::QualType(SuperType superType, const std::string &subType)
    : type(std::make_unique<Type>(superType, subType)), specifiers(TypeSpecifiers::of(superType)) {}
QualType::QualType(const Type &t, TypeSpecifiers specifiers) : type(std::make_unique<Type>(t)), specifiers(specifiers) {}

// ToDo: Delete those two later on
QualType::QualType(const QualType &other) {
  type = std::make_unique<Type>(*other.type);
  specifiers = other.specifiers;
}
QualType &QualType::operator=(const spice::compiler::QualType &other) {
  type = std::make_unique<Type>(*other.type);
  specifiers = other.specifiers;
  return *this;
}

void QualType::getName(std::stringstream &name, bool withSize, bool ignorePublic) const {
  // Append the specifiers
  const TypeSpecifiers defaultForSuperType = TypeSpecifiers::of(getBase().getSuperType());
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
    name << "unsigned ";

  // Loop through all chain elements
  type->getName(name, withSize);
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @param ignorePublic Ignore any potential public specifier
 * @return Symbol type name
 */
std::string QualType::getName(bool withSize, bool ignorePublic) const {
  std::stringstream name;
  getName(name, withSize, ignorePublic);
  return name.str();
}

bool QualType::is(SuperType superType) const { return type->is(superType); }

bool QualType::isOneOf(const std::initializer_list<SuperType> &superTypes) const { return type->isOneOf(superTypes); }

bool QualType::isBase(SuperType superType) const { return type->isBase(superType); }

QualType QualType::getBase() const {
  QualType qualType = *this;
  qualType.type = std::make_unique<Type>(type->getBase());
  return qualType;
}

void QualType::setType(const Type &newType) { type = std::make_unique<Type>(newType); }

bool QualType::isConst() const { return specifiers.isConst; }

void QualType::makeConst(bool isConst) { specifiers.isConst = isConst; }

bool QualType::isSigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  return specifiers.isSigned;
}

void QualType::makeSigned(bool isSigned) {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  specifiers.isSigned = isSigned;
}

bool QualType::isUnsigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  return specifiers.isUnsigned;
}

void QualType::makeUnsigned(bool isUnsigned) {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  specifiers.isUnsigned = isUnsigned;
}

bool QualType::isInline() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return specifiers.isInline;
}

void QualType::makeInline(bool isInline) {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  specifiers.isInline = isInline;
}

bool QualType::isPublic() const {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  return specifiers.isPublic;
}

void QualType::makePublic(bool isPublic) {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  specifiers.isPublic = isPublic;
}

bool QualType::isHeap() const { return specifiers.isHeap; }

void QualType::makeHeap(bool isHeap) { specifiers.isHeap = isHeap; }

bool QualType::isComposition() const { return specifiers.isComposition; }

void QualType::makeComposition(bool isComposition) { specifiers.isComposition = isComposition; }

bool QualType::isPtr() const { return type->isPtr(); }

bool QualType::isPtrTo(SuperType superType) const { return isPtr() && getContained().is(superType); }

bool QualType::isRef() const { return type->isRef(); }

bool QualType::isRefTo(SuperType superType) const { return isRef() && getContained().is(superType); }

bool QualType::isArray() const { return type->isArray(); }

bool QualType::isArrayOf(SuperType superType) const { return isArray() && getContained().is(superType); }

bool QualType::isConstRef() const { return isConst() && isRef(); }

SuperType QualType::getSuperType() const { return type->getSuperType(); }

const std::string &QualType::getSubType() const { return type->getSubType(); }

bool QualType::hasAnyGenericParts() const { return type->hasAnyGenericParts(); }

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
  return ignoreSpecifiers || specifiers.match(otherType.specifiers, allowConstify);
}

bool QualType::isSameContainerTypeAs(const QualType &other) const { return type->isSameContainerTypeAs(*other.type); }

/**
 * Convert the type to an LLVM type
 *
 * @param context LLVM context
 * @param accessScope Access scope
 * @return LLVM type
 */
llvm::Type *QualType::toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const {
  return type->toLLVMType(context, accessScope);
}

QualType QualType::toPtr(const ASTNode *node) const {
  QualType newType = *this;
  newType.type = std::make_unique<Type>(type->toPointer(node));
  return newType;
}

QualType QualType::toRef(const ASTNode *node) const {
  QualType newType = *this;
  newType.type = std::make_unique<Type>(type->toReference(node));
  return newType;
}

QualType QualType::toArray(const ASTNode *node, size_t size, bool skipDynCheck /*=false*/) const {
  QualType newType = *this;
  newType.type = std::make_unique<Type>(type->toArray(node, size, skipDynCheck));
  return newType;
}

QualType QualType::toNonConst() const {
  QualType qualType = *this;
  qualType.specifiers.isConst = false;
  return qualType;
}

QualType QualType::toConstRef(const ASTNode *node) const {
  QualType qualType = toRef(node);
  qualType.specifiers.isConst = true;
  return qualType;
}

QualType QualType::getContained() const {
  QualType qualType = *this;
  qualType.type = std::make_unique<Type>(type->getContainedTy());
  return qualType;
}

bool operator==(const QualType &lhs, const QualType &rhs) { return *lhs.type == *rhs.type; }

bool operator!=(const QualType &lhs, const QualType &rhs) { return !(lhs == rhs); }

QualType QualType::removeReferenceWrapper() const { return isRef() ? getContained() : *this; }

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
  TypeSpecifiers newSpecifiers = specifiers.merge(newBaseType.specifiers);
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
