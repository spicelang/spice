// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "QualType.h"

#include <sstream>

#include <SourceFile.h>
#include <global/TypeRegistry.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <symboltablebuilder/Type.h>
#include <typechecker/InterfaceManager.h>
#include <typechecker/StructManager.h>

namespace spice::compiler {

QualType::QualType(SuperType superType) : type(TypeRegistry::getOrInsert(superType)), specifiers(TypeSpecifiers::of(superType)) {}
QualType::QualType(SuperType superType, const std::string &subType)
    : type(TypeRegistry::getOrInsert(superType, subType)), specifiers(TypeSpecifiers::of(superType)) {}
QualType::QualType(const Type *type, TypeSpecifiers specifiers) : type(type), specifiers(specifiers) {}

/**
 * Get the super type of the underlying type
 *
 * @return Super type
 */
SuperType QualType::getSuperType() const { return type->getSuperType(); }

/**
 * Get the subtype of the underlying type
 *
 * @return Subtype
 */
const std::string &QualType::getSubType() const { return type->getSubType(); }

/**
 * Get the array size of the underlying type
 *
 * @return Array size
 */
unsigned int QualType::getArraySize() const { return type->getArraySize(); }

/**
 * Get the body scope of the underlying type
 *
 * @return Body scope
 */
Scope *QualType::getBodyScope() const { return type->getBodyScope(); }

/**
 * Get the function parameter types of the underlying type
 *
 * @return Function parameter types
 */
const QualType &QualType::getFunctionReturnType() const { return type->getFunctionReturnType(); }

/**
 * Get the function parameter types of the underlying type
 *
 * @return Function parameter types
 */
QualTypeList QualType::getFunctionParamTypes() const { return type->getFunctionParamTypes(); }

/**
 * Get the function parameter and return types of the underlying type
 *
 * @return Function parameter and return types
 */
const QualTypeList &QualType::getFunctionParamAndReturnTypes() const { return type->getFunctionParamAndReturnTypes(); }

/**
 * Check if the underlying type has lambda captures
 *
 * @return Has lambda captures or not
 */
bool QualType::hasLambdaCaptures() const { return type->hasLambdaCaptures(); }

/**
 * Get the template types of the underlying type
 *
 * @return Template types
 */
const QualTypeList &QualType::getTemplateTypes() const { return type->getTemplateTypes(); }

/**
 * Get the struct instance for a struct type
 *
 * @param node Accessing AST node
 * @return Struct instance
 */
Struct *QualType::getStruct(const ASTNode *node) const {
  assert(is(TY_STRUCT));
  Scope *structDefScope = getBodyScope()->parent;
  const std::string structName = getSubType();
  const QualTypeList &templateTypes = getTemplateTypes();
  return StructManager::match(structDefScope, structName, templateTypes, node);
}

/**
 * Get the interface instance for an interface type
 *
 * @param node Accessing AST node
 * @return Interface instance
 */
Interface *QualType::getInterface(const ASTNode *node) const {
  assert(is(TY_INTERFACE));
  Scope *interfaceDefScope = getBodyScope()->parent;
  const std::string structName = getSubType();
  const QualTypeList &templateTypes = getTemplateTypes();
  return InterfaceManager::match(interfaceDefScope, structName, templateTypes, node);
}

/**
 * Check if the underlying type is of a certain super type
 *
 * @param superType Super type
 * @return Is of super type or not
 */
bool QualType::is(SuperType superType) const { return type->is(superType); }

/**
 * Check if the underlying type is one of a list of super types
 *
 * @param superTypes List of super types
 * @return Is one of the super types or not
 */
bool QualType::isOneOf(const std::initializer_list<SuperType> &superTypes) const { return type->isOneOf(superTypes); }

/**
 * Check if the base type of the underlying type is a certain super type
 *
 * @param superType Super type
 * @return Is base type or not
 */
bool QualType::isBase(SuperType superType) const { return type->isBase(superType); }

/**
 * Check if the underlying type is a primitive type
 *
 * @return Primitive or not
 */
bool QualType::isPrimitive() const { return type->isPrimitive(); }

/**
 * Check if the underlying type is a pointer
 *
 * @return Pointer or not
 */
bool QualType::isPtr() const { return type->isPtr(); }

/**
 * Check if the underlying type is a pointer to a certain super type
 *
 * @param superType Super type
 * @return Pointer to super type or not
 */
bool QualType::isPtrTo(SuperType superType) const { return isPtr() && getContained().is(superType); }

/**
 * Check if the underlying type is a reference
 *
 * @return Reference or not
 */
bool QualType::isRef() const { return type->isRef(); }

/**
 * Check if the underlying type is a reference to a certain super type
 *
 * @param superType Super type
 * @return Reference to super type or not
 */
bool QualType::isRefTo(SuperType superType) const { return isRef() && getContained().is(superType); }

/**
 * Check if the underlying type is an array
 *
 * @return Array or not
 */
bool QualType::isArray() const { return type->isArray(); }

/**
 * Check if the underlying type is an array of a certain super type
 *
 * @param superType Super type
 * @return Array of super type or not
 */
bool QualType::isArrayOf(SuperType superType) const { return isArray() && getContained().is(superType); }

/**
 * Check if the underlying type is a const reference
 *
 * @return Const reference or not
 */
bool QualType::isConstRef() const { return isConst() && isRef(); }

/**
 * Check if the current type is an iterator
 *
 * @param node ASTNode
 * @return Iterator or not
 */
bool QualType::isIterator(const ASTNode *node) const {
  // The type must be a struct that implements the iterator interface
  if (!is(TY_STRUCT))
    return false;

  const QualType genericType(TY_GENERIC, "T");
  static constexpr TypeChainElementData data = {.bodyScope = nullptr};
  const Type *itType = TypeRegistry::getOrInsert(TY_INTERFACE, IITERATOR_NAME, TYPE_ID_ITERATOR_INTERFACE, data, {genericType});
  const QualType iteratorQualType(itType, TypeSpecifiers::of(TY_INTERFACE));
  return doesImplement(iteratorQualType, node);
}

/**
 * Check if the current type is an iterable
 * - Arrays are always considered iterable
 * - Otherwise the type must be a struct that implements the iterator interface
 *
 * @param node ASTNode
 * @return Iterable or not
 */
bool QualType::isIterable(const ASTNode *node) const {
  // Arrays are always considered iterable
  if (isArray())
    return true;
  // Otherwise the type must be a struct that implements the iterator interface
  if (!is(TY_STRUCT))
    return false;

  const QualType genericType(TY_GENERIC, "T");
  static constexpr TypeChainElementData data = {.bodyScope = nullptr};
  const Type *itType = TypeRegistry::getOrInsert(TY_INTERFACE, IITERATOR_NAME, TYPE_ID_ITERABLE_INTERFACE, data, {genericType});
  const QualType iteratorQualType(itType, TypeSpecifiers::of(TY_INTERFACE));
  return doesImplement(iteratorQualType, node);
}

/**
 * Check if the current type is a string object
 *
 * @return String object or not
 */
bool QualType::isStringObj() const {
  return is(TY_STRUCT) && getSubType() == STROBJ_NAME && getBodyScope()->sourceFile->isStdFile;
}

/**
 * Check if the current type is an error object
 *
 * @return Error object or not
 */
bool QualType::isErrorObj() const {
  return is(TY_STRUCT) && getSubType() == ERROBJ_NAME && getBodyScope()->sourceFile->isStdFile;
}

/**
 * Check if the current type has any generic parts
 *
 * @return Generic parts or not
 */
bool QualType::hasAnyGenericParts() const { return type->hasAnyGenericParts(); }

/**
 * Check if copying an instance of the current type would require calling other copy ctors.
 * If this function return true, the type can be copied by calling memcpy.
 *
 * @param node Accessing ASTNode
 * @return Trivially copyable or not
 */
bool QualType::isTriviallyCopyable(const ASTNode *node) const { // NOLINT(*-no-recursion)
  // Heap-allocated values may not be copied via memcpy
  if (specifiers.isHeap)
    return false;

  // References can't be copied at all
  if (is(TY_REF))
    return false;

  // In case of an array, the item type is determining the copy triviality
  if (is(TY_ARRAY))
    return getBase().isTriviallyCopyable(node);

  // In case of a struct, the member types determine the copy triviality
  if (is(TY_STRUCT)) {
    // If the struct has a copy ctor, it is a non-trivially copyable one
    const Struct *spiceStruct = getStruct(node);

    // If the struct itself has a copy ctor, it is not trivially copyable
    const std::vector args = {Arg(toConstRef(node), false)};
    const Function *copyCtor = FunctionManager::lookup(spiceStruct->scope, CTOR_FUNCTION_NAME, *this, args, true);
    if (copyCtor != nullptr)
      return false;

    // Check if all member types are trivially copyable
    const auto pred = [&](const QualType &fieldType) { return fieldType.isTriviallyCopyable(node); }; // NOLINT(*-no-recursion)
    return std::ranges::all_of(spiceStruct->fieldTypes, pred);
  }

  return true;
}

/**
 * Check if the current type implements the given interface type
 *
 * @param symbolType Interface type
 * @param node Accessing ASTNode
 * @return Struct implements interface or not
 */
bool QualType::doesImplement(const QualType &symbolType, const ASTNode *node) const {
  assert(is(TY_STRUCT) && symbolType.is(TY_INTERFACE));
  const Struct *spiceStruct = getStruct(node);
  assert(spiceStruct != nullptr);
  return std::ranges::any_of(spiceStruct->interfaceTypes, [&](const QualType &interfaceType) {
    assert(interfaceType.is(TY_INTERFACE));
    return symbolType.matches(interfaceType, false, false, true);
  });
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
  if (!type->matches(otherType.type, ignoreArraySize))
    return false;

  // Ignore or compare specifiers
  return ignoreSpecifiers || specifiers.match(otherType.specifiers, allowConstify);
}

/**
 * Check for the matching compatibility of two types in terms of interface implementation.
 * Useful for function matching as well as assignment type validation and function arg matching.
 *
 * @param structType Type to compare against
 * @return Matching or not
 */
bool QualType::matchesInterfaceImplementedByStruct(const QualType &structType) const {
  if (!is(TY_INTERFACE) || !structType.is(TY_STRUCT))
    return false;

  // Check if the rhs is a struct type that implements the lhs interface type
  const Struct *spiceStruct = structType.getStruct(nullptr);
  assert(spiceStruct != nullptr);
  const auto pred = [&](const QualType &interfaceType) { return matches(interfaceType, false, false, true); };
  return std::ranges::any_of(spiceStruct->interfaceTypes, pred);
}

/**
 * Check if the current type is the same container type as another type.
 * Container types include arrays, pointers, and references.
 *
 * @param other Other type
 * @return Same container type or not
 */
bool QualType::isSameContainerTypeAs(const QualType &other) const { return type->isSameContainerTypeAs(other.type); }

/**
 * Check if the current type is a self-referencing struct type
 *
 * @return Self-referencing struct type or not
 */
bool QualType::isSelfReferencingStructType(const QualType *typeToCompareWith) const { // NOLINT(*-no-recursion)
  if (!is(TY_STRUCT))
    return false;

  // If no type was set by a previous iteration, we set it to the current type
  if (typeToCompareWith == nullptr)
    typeToCompareWith = this;

  Scope *baseTypeBodyScope = getBodyScope();
  for (size_t i = 0; i < baseTypeBodyScope->getFieldCount(); i++) {
    const SymbolTableEntry *field = baseTypeBodyScope->lookupField(i);
    // Check if the base type of the field matches with the current type, which is also a base type
    // If yes, this is a self-referencing struct type
    if (field->getQualType().getBase() == *typeToCompareWith)
      return true;

    // If the field is a struct, check if it is a self-referencing struct type
    if (field->getQualType().isSelfReferencingStructType(typeToCompareWith))
      return true;
  }
  return false;
}

/**
 * Check if the given generic type list has a substantiation for the current (generic) type
 *
 * @param genericTypeList Generic type list
 * @return Has substantiation or not
 */
bool QualType::isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const { // NOLINT(*-no-recursion)
  const QualType baseType = getBase();
  // Check if the symbol type itself is generic
  if (baseType.is(TY_GENERIC)) {
    return std::ranges::any_of(genericTypeList, [&](GenericType &t) {
      if (baseType.matches(t, true, true, true)) {
        t.used = true;
        return true;
      }
      return false;
    });
  }

  // If the type is non-generic check template types
  bool covered = true;
  // Check template types
  const QualTypeList &baseTemplateTypes = baseType.getTemplateTypes();
  auto outerPred = [&](const QualType &templateType) { // NOLINT(*-no-recursion)
    return templateType.isCoveredByGenericTypeList(genericTypeList);
  };
  covered &= std::ranges::all_of(baseTemplateTypes, outerPred);

  // If function/procedure, check param and return types
  if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const QualTypeList &paramAndReturnTypes = baseType.getFunctionParamAndReturnTypes();
    const auto innerPred = [&](const QualType &paramType) { // NOLINT(*-no-recursion)
      return paramType.isCoveredByGenericTypeList(genericTypeList);
    };
    covered &= std::ranges::all_of(paramAndReturnTypes, innerPred);
  }

  return covered;
}

/**
 * Check if the current type needs de-allocation
 *
 * @return Needs de-allocation or not
 */
bool QualType::needsDeAllocation() const {
  if (!isHeap())
    return false;
  // We only need de-allocation, if we directly point to a heap-allocated type
  // e.g. for heap TestStruct** we don't need to de-allocate, since it is a non-owning pointer to an owning pointer
  return isPtr() && !isPtrTo(TY_PTR);
}

/**
 * Get the name of the symbol type as a string
 *
 * @param name Name stream
 * @param withSize Include the array size for sized types
 * @param ignorePublic Ignore any potential public specifier
 */
void QualType::getName(std::stringstream &name, bool withSize, bool ignorePublic) const {
  // Append the specifiers
  const TypeSpecifiers defaultForSuperType = TypeSpecifiers::of(getBase().getSuperType());
  if (!ignorePublic && specifiers.isPublic && !defaultForSuperType.isPublic)
    name << "public ";
  if (specifiers.isComposition && !defaultForSuperType.isComposition)
    name << "compose ";
  if (specifiers.isConst && !defaultForSuperType.isConst && type->typeChain.size() > 1)
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

/**
 * Convert the type to an LLVM type
 *
 * @param sourceFile Source file
 * @return LLVM type
 */
llvm::Type *QualType::toLLVMType(SourceFile *sourceFile) const { return sourceFile->getLLVMType(type); }

/**
 * Retrieve the pointer type to this type
 *
 * @param node ASTNode
 * @return New type
 */
QualType QualType::toPtr(const ASTNode *node) const {
  QualType newType = *this;
  newType.type = type->toPtr(node);
  return newType;
}

/**
 * Retrieve the reference type to this type
 *
 * @param node ASTNode
 * @return New type
 */
QualType QualType::toRef(const ASTNode *node) const {
  QualType newType = *this;
  newType.type = type->toRef(node);
  return newType;
}

/**
 * Retrieve the const reference type of this type
 *
 * @param node ASTNode
 * @return New type
 */
QualType QualType::toConstRef(const ASTNode *node) const {
  QualType qualType = toRef(node);
  qualType.makeConst();
  return qualType;
}

/**
 * Retrieve the array type of this type
 *
 * @param node ASTNode
 * @param size Array size
 * @param skipDynCheck Skip dynamic check
 * @return New type
 */
QualType QualType::toArray(const ASTNode *node, size_t size, bool skipDynCheck /*=false*/) const {
  QualType newType = *this;
  newType.type = type->toArr(node, size, skipDynCheck);
  return newType;
}

/**
 * Retrieve the non-const type of this type
 *
 * @return New type
 */
QualType QualType::toNonConst() const {
  QualType qualType = *this;
  qualType.specifiers.isConst = false;
  return qualType;
}

/**
 * Retrieve the contained type of this type
 * This works on pointers, arrays, references and strings (which alias with char*)
 *
 * @return New type
 */
QualType QualType::getContained() const {
  assert(isOneOf({TY_PTR, TY_ARRAY, TY_REF, TY_STRING}));
  QualType qualType = *this;
  qualType.type = type->getContained();
  return qualType;
}

/**
 * Retrieve the base type of this type
 *
 * @return New type
 */
QualType QualType::getBase() const {
  QualType qualType = *this;
  qualType.type = type->getBase();
  return qualType;
}

/**
 * Remove reference of this type, if it is a reference
 *
 * @return New type
 */
QualType QualType::removeReferenceWrapper() const { return isRef() ? getContained() : *this; }

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
QualType QualType::replaceBaseType(const QualType &newBaseType) const {
  // Create new type
  const Type *newType = type->replaceBase(newBaseType.getType());
  // Create new specifiers
  TypeSpecifiers newSpecifiers = specifiers.merge(newBaseType.specifiers);
  // Return the new qualified type
  return {newType, newSpecifiers};
}

/**
 * Retrieve the same type, but with lambda captures enabled
 *
 * @return Same type with lambda captures
 */
QualType QualType::getWithLambdaCaptures(bool enabled /*=true*/) const {
  // Create new type
  const Type *newType = type->getWithLambdaCaptures(enabled);
  // Return the new qualified type
  return {newType, specifiers};
}

/**
 * Retrieve the same type, but with a new body scope
 *
 * @return Same type with body scope
 */
QualType QualType::getWithBodyScope(Scope *bodyScope) const {
  // Create new type
  const Type *newType = type->getWithBodyScope(bodyScope);
  // Return the new qualified type
  return {newType, specifiers};
}

/**
 * Retrieve the same type, but with new template types
 *
 * @param templateTypes New template types
 * @return Same type with new template types
 */
QualType QualType::getWithTemplateTypes(const QualTypeList &templateTypes) const {
  // Create new type
  const Type *newType = type->getWithTemplateTypes(templateTypes);
  // Return the new qualified type
  return {newType, specifiers};
}

/**
 * Retrieve the same type, but with new base template types
 *
 * @param templateTypes New base template types
 * @return Same type with new base template types
 */
QualType QualType::getWithBaseTemplateTypes(const QualTypeList &templateTypes) const {
  // Create new type
  const Type *newType = type->getWithBaseTemplateTypes(templateTypes);
  // Return the new qualified type
  return {newType, specifiers};
}

/**
 * Retrieve the same type, but with new function parameter and return types
 *
 * @param paramAndReturnTypes New parameter types
 * @return Same type with new parameter types
 */
QualType QualType::getWithFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes) const {
  // Create new type
  const Type *newType = type->getWithFunctionParamAndReturnTypes(paramAndReturnTypes);
  // Return the new qualified type
  return {newType, specifiers};
}

QualType QualType::getWithFunctionParamAndReturnTypes(const QualType &returnType, const QualTypeList &paramTypes) const {
  QualTypeList paramAndReturnTypes = paramTypes;
  paramAndReturnTypes.insert(paramAndReturnTypes.begin(), returnType);
  return getWithFunctionParamAndReturnTypes(paramAndReturnTypes);
}

/**
 * Check if the current type is marked const
 *
 * @return Is const or not
 */
bool QualType::isConst() const { return specifiers.isConst; }

/**
 * Check if the current type is marked signed
 *
 * @return Is signed or not
 */
bool QualType::isSigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  return specifiers.isSigned;
}

/**
 * Check if the current type is marked unsigned
 *
 * @return Is unsigned or not
 */
bool QualType::isUnsigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  return specifiers.isUnsigned;
}

/**
 * Check if the current type is marked inline
 *
 * @return Is inline or not
 */
bool QualType::isInline() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return specifiers.isInline;
}

/**
 * Check if the current type is marked public
 *
 * @return Is public or not
 */
bool QualType::isPublic() const {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  return specifiers.isPublic;
}

/**
 * Check if the current type is marked heap
 *
 * @return Is heap or not
 */
bool QualType::isHeap() const { return specifiers.isHeap; }

/**
 * Check if the current type is marked as composition
 *
 * @return Is composition or not
 */
bool QualType::isComposition() const { return specifiers.isComposition; }

/**
 * Make the current type const
 *
 * @param isConst Is const or not
 */
void QualType::makeConst(bool isConst) { specifiers.isConst = isConst; }

/**
 * Make the current type unsigned
 *
 * @param isUnsigned Is unsigned or not
 */
void QualType::makeUnsigned(bool isUnsigned) {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_BOOL}));
  specifiers.isSigned = !isUnsigned;
  specifiers.isUnsigned = isUnsigned;
}

/**
 * Make the current type public
 *
 * @param isPublic Is public or not
 */
void QualType::makePublic(bool isPublic) {
  assert(type->isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  specifiers.isPublic = isPublic;
}

/**
 * Make the current type heap
 *
 * @param isHeap Is heap or not
 */
void QualType::makeHeap(bool isHeap) { specifiers.isHeap = isHeap; }

/**
 * Check if two types are equal
 *
 * @param lhs Left-hand side type
 * @param rhs Right-hand side type
 * @return Equal or not
 */
bool operator==(const QualType &lhs, const QualType &rhs) { return lhs.type == rhs.type; }

/**
 * Check if two types are not equal
 *
 * @param lhs Left-hand side type
 * @param rhs Right-hand side type
 * @return Not equal or not
 */
bool operator!=(const QualType &lhs, const QualType &rhs) { return !(lhs == rhs); }

/**
 * Remove pointers / arrays / references if both types have them as far as possible.
 * Furthermore, remove reference wrappers if possible.
 *
 * @param typeA Candidate type
 * @param typeB Requested type
 */
void QualType::unwrapBoth(QualType &typeA, QualType &typeB) { Type::unwrapBoth(typeA.type, typeB.type); }

} // namespace spice::compiler
