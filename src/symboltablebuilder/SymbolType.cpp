// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolType.h"

#include <SourceFile.h>
#include <exception/CompilerError.h>
#include <exception/SemanticError.h>
#include <irgenerator/NameMangling.h>
#include <irgenerator/StdFunctionManager.h>
#include <model/GenericType.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableEntry.h>

namespace spice::compiler {

/**
 * Get the pointer type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Pointer type of the current type
 */
SymbolType SymbolType::toPointer(const ASTNode *node) const {
  // Do not allow pointers of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");
  if (is(TY_REF))
    throw SemanticError(node, REF_POINTERS_ARE_NOT_ALLOWED, "Pointers to references are not allowed. Use pointer instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_PTR);
  return {newTypeChain, specifiers};
}

/**
 * Get the reference type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Reference type of the current type
 */
SymbolType SymbolType::toReference(const ASTNode *node) const {
  // Do not allow references of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_REFERENCES_NOT_ALLOWED, "Just use the dyn type without '&' instead");
  // Do not allow references of references
  if (is(TY_REF))
    return *this;

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_REF);
  return {newTypeChain, specifiers};
}

/**
 * Get the const reference type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Const reference type of the current type
 */
SymbolType SymbolType::toConstReference(const ASTNode *node) const {
  SymbolType constRefType = toReference(node);
  constRefType.specifiers.isConst = true;
  return constRefType;
}

/**
 * Get the array type of the current type as a new type
 *
 * @param node AST node for error messages
 * @param size Size of the array
 * @return Array type of the current type
 */
SymbolType SymbolType::toArray(const ASTNode *node, size_t size, bool skipDynCheck /*=false*/) const {
  // Do not allow arrays of dyn
  if (!skipDynCheck && typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_ARRAY, TypeChainElementData{.arraySize = size});
  return {newTypeChain, specifiers};
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
SymbolType SymbolType::getContainedTy() const {
  if (is(TY_STRING))
    return SymbolType(TY_CHAR);
  assert(typeChain.size() > 1);
  TypeChain newTypeChain = typeChain;
  newTypeChain.pop_back();
  return {newTypeChain, specifiers};
}

/**
 * Replace the subtype of the base type with another one
 *
 * @param newSubType New sub type of the base type
 * @return The new type with the adjusted type chain
 */
SymbolType SymbolType::replaceBaseSubType(const std::string &newSubType) const {
  assert(!typeChain.empty());
  // Copy the stack to not destroy the present one
  TypeChain newTypeChain = typeChain;
  // Replace the first element
  newTypeChain.front().subType = newSubType;
  // Return the new chain as a symbol type
  return {newTypeChain, specifiers};
}

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
SymbolType SymbolType::replaceBaseType(const SymbolType &newBaseType) const {
  assert(!typeChain.empty());

  // Create new type chain
  TypeChain newTypeChain = newBaseType.typeChain;
  const bool doubleRef = newTypeChain.back().superType == TY_REF && typeChain.back().superType == TY_REF;
  for (size_t i = 1; i < typeChain.size(); i++)
    if (!doubleRef || i > 1)
      newTypeChain.push_back(typeChain.at(i));

  // Create new specifiers
  TypeSpecifiers newSpecifiers = specifiers.merge(newBaseType.specifiers);

  // Return the new chain as a symbol type
  return {newTypeChain, newSpecifiers};
}

/**
 * Return the LLVM type for this symbol type
 *
 * @param context LLVM context
 * @param accessScope Access scope for structs
 * @return Corresponding LLVM type
 */
llvm::Type *SymbolType::toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const { // NOLINT(misc-no-recursion)
  assert(!typeChain.empty() && !isOneOf({TY_DYN, TY_INVALID}));

  if (is(TY_DOUBLE))
    return llvm::Type::getDoubleTy(context);

  if (is(TY_INT))
    return llvm::Type::getInt32Ty(context);

  if (is(TY_SHORT))
    return llvm::Type::getInt16Ty(context);

  if (is(TY_LONG))
    return llvm::Type::getInt64Ty(context);

  if (isOneOf({TY_CHAR, TY_BYTE}))
    return llvm::Type::getInt8Ty(context);

  if (is(TY_STRING))
    return llvm::Type::getInt8PtrTy(context);

  if (is(TY_BOOL))
    return llvm::Type::getInt1Ty(context);

  if (isOneOf({TY_STRUCT, TY_INTERFACE})) {
    Scope *structBodyScope = getBodyScope();
    const std::string structSignature = Struct::getSignature(getSubType(), getTemplateTypes());
    SymbolTableEntry *structSymbol = structBodyScope->parent->lookupStrict(structSignature);
    assert(structSymbol != nullptr);
    llvm::StructType *structType = structSymbol->getStructLLVMType();

    // If the type is not known yet, build the LLVM type
    if (!structType) {
      Struct *spiceStruct = structSymbol->getType().getStruct(structSymbol->declNode);
      assert(spiceStruct != nullptr);
      const std::string mangledName = NameMangling::mangleStruct(*spiceStruct);
      structType = llvm::StructType::create(context, mangledName);
      structSymbol->setStructLLVMType(structType);

      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      if (is(TY_STRUCT)) { // Struct
        fieldTypes.reserve(spiceStruct->fieldTypes.size());
        for (const SymbolType &fieldType : spiceStruct->fieldTypes)
          fieldTypes.push_back(fieldType.toLLVMType(context, accessScope));
      } else { // Interface
        fieldTypes.push_back(llvm::PointerType::get(context, 0));
      }

      // Set field types to struct type
      structType->setBody(fieldTypes);
    }

    return structType;
  }

  if (is(TY_ENUM))
    return llvm::Type::getInt32Ty(context);

  if (isPtr() || isRef() || (isArray() && getArraySize() == 0))
    return llvm::PointerType::get(context, 0);

  if (isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    llvm::PointerType *ptrTy = llvm::PointerType::get(context, 0);
    return llvm::StructType::get(context, {ptrTy, ptrTy});
  }

  if (isArray()) {
    llvm::Type *containedType = getContainedTy().toLLVMType(context, accessScope);
    return llvm::ArrayType::get(containedType, getArraySize());
  }

  throw CompilerError(UNHANDLED_BRANCH, "Cannot determine LLVM type of " + getName(true)); // GCOVR_EXCL_LINE
}

/**
 * Check if the current type is an iterator
 *
 * @param node ASTNode
 * @return Iterator or not
 */
bool SymbolType::isIterator(const ASTNode *node) const {
  if (!is(TY_STRUCT))
    return false;
  SymbolType genericType(TY_GENERIC, "T");
  SymbolType iteratorType(TY_INTERFACE, "Iterable", {.bodyScope = nullptr}, {genericType});
  return implements(iteratorType, node);
}

/**
 * Check if the current type is a string object
 *
 * @return String object or not
 */
bool SymbolType::isStringObj() const {
  return is(TY_STRUCT) && getOriginalSubType() == STROBJ_NAME && getBodyScope()->sourceFile->stdFile;
}

/**
 * Check if the current type is a error object
 *
 * @return Error object or not
 */
bool SymbolType::isErrorObj() const {
  return is(TY_STRUCT) && getOriginalSubType() == ERROBJ_NAME && getBodyScope()->sourceFile->stdFile;
}

/**
 * Check if the current type implements the given interface type
 *
 * @param symbolType Interface type
 * @param node ASTNode
 * @return Struct implements interface or not
 */
bool SymbolType::implements(const SymbolType &symbolType, const spice::compiler::ASTNode *node) const {
  assert(is(TY_STRUCT) && symbolType.is(TY_INTERFACE));
  Struct *spiceStruct = getStruct(node);
  assert(spiceStruct != nullptr);
  return std::ranges::any_of(spiceStruct->interfaceTypes, [&](const SymbolType &interfaceType) {
    assert(interfaceType.is(TY_INTERFACE));
    return symbolType.matches(interfaceType, false, false, true);
  });
}

/**
 * Check if the base type of the current type chain is of a certain super type
 *
 * @param superType Super type to check for
 * @return Applicable or not
 */
bool SymbolType::isBaseType(SymbolSuperType superType) const {
  assert(!typeChain.empty());
  return typeChain.front().superType == superType;
}

/**
 * Check if the current type is of the same container type like the other type.
 * Only TY_PTR, TY_REF and TY_ARRAY are considered as container types.
 *
 * @param otherType Other symbol type
 * @return Same container type or not
 */
bool SymbolType::isSameContainerTypeAs(const SymbolType &otherType) const {
  return (is(TY_PTR) && otherType.is(TY_PTR)) || (is(TY_REF) && otherType.is(TY_REF)) || (is(TY_ARRAY) && otherType.is(TY_ARRAY));
}

/**
 * Checks if the base type is generic itself or has generic parts in its template types
 *
 * @return Contains generic parts or not
 */
bool SymbolType::hasAnyGenericParts() const { // NOLINT(misc-no-recursion)
  const SymbolType &baseType = getBaseType();

  // Check if the type itself is generic
  if (baseType.is(TY_GENERIC))
    return true;

  // Check if the type has generic template types
  const auto templateTypes = baseType.getTemplateTypes();
  if (std::ranges::any_of(templateTypes, [](const SymbolType &t) { return t.hasAnyGenericParts(); }))
    return true;

  // Check param and return types or functions/procedures
  if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const auto paramTypes = baseType.getFunctionParamAndReturnTypes();
    if (std::ranges::any_of(paramTypes, [](const SymbolType &t) { return t.hasAnyGenericParts(); }))
      return true;
  }

  return false; // Does not have generic parts
}

/**
 * Set the list of templates types
 */
void SymbolType::setTemplateTypes(const std::vector<SymbolType> &templateTypes) {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  typeChain.back().templateTypes = templateTypes;
}

/**
 * Set the list of templates types of the base type
 */
void SymbolType::setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes) {
  assert(getBaseType().isOneOf({TY_STRUCT, TY_INTERFACE}));
  typeChain.front().templateTypes = templateTypes;
}

/**
 * Retrieve template types of the current type
 *
 * @return Vector of template types
 */
const std::vector<SymbolType> &SymbolType::getTemplateTypes() const { return typeChain.back().templateTypes; }

/**
 * Check if the given generic type list has a substantiation for the current (generic) type
 *
 * @param genericTypeList Generic type list
 * @return Has substantiation or not
 */
bool SymbolType::isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const {
  const SymbolType baseType = getBaseType();
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
  const std::vector<SymbolType> &baseTemplateTypes = baseType.getTemplateTypes();
  covered &= std::ranges::all_of(baseTemplateTypes, [&](const SymbolType &templateType) {
    return templateType.isCoveredByGenericTypeList(genericTypeList);
  });

  // If function/procedure, check param and return types
  if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const std::vector<SymbolType> &paramAndReturnTypes = baseType.getFunctionParamAndReturnTypes();
    covered &= std::ranges::all_of(
        paramAndReturnTypes, [&](const SymbolType &paramType) { return paramType.isCoveredByGenericTypeList(genericTypeList); });
  }

  return covered;
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
std::string SymbolType::getName(bool withSize) const { // NOLINT(misc-no-recursion)
  std::stringstream name;

  // Append the specifiers
  const TypeSpecifiers defaultForSuperType = TypeSpecifiers::of(getBaseType().getSuperType());
  if (specifiers.isPublic && !defaultForSuperType.isPublic)
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
  for (const TypeChainElement &chainElement : typeChain)
    name << chainElement.getName(withSize);

  return name.str();
}

/**
 * Set the return type of a function type
 *
 * @param returnType Function return type
 */
void SymbolType::setFunctionReturnType(const SymbolType &returnType) {
  assert(is(TY_FUNCTION));
  std::vector<SymbolType> &paramTypes = typeChain.back().paramTypes;
  if (paramTypes.empty())
    paramTypes.resize(1);
  paramTypes.at(0) = returnType;
}

/**
 * Get the return type of a function type
 *
 * @return Function return type
 */
const SymbolType &SymbolType::getFunctionReturnType() const {
  assert(is(TY_FUNCTION));
  assert(!typeChain.back().paramTypes.empty());
  return typeChain.back().paramTypes.front();
}

/**
 * Set the param types of a function or procedure type
 *
 * @param paramTypes Function param types
 */
void SymbolType::setFunctionParamTypes(const std::vector<SymbolType> &newParamTypes) {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  std::vector<SymbolType> &paramTypes = typeChain.back().paramTypes;
  // Resize param types if required
  if (paramTypes.size() < newParamTypes.size() + 1)
    paramTypes.resize(newParamTypes.size() + 1, SymbolType(TY_DYN));
  // Set the function param types
  for (size_t i = 0; i < newParamTypes.size(); i++)
    paramTypes.at(i + 1) = newParamTypes.at(i);
}

/**
 * Get the param types of a function or procedure type
 *
 * @return Function param types
 */
std::vector<SymbolType> SymbolType::getFunctionParamTypes() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  if (typeChain.back().paramTypes.empty())
    return {};
  return {typeChain.back().paramTypes.begin() + 1, typeChain.back().paramTypes.end()};
}

/**
 * Set has captures of a function or procedure type
 *
 * @param hasCaptures Has captures
 */
void SymbolType::setHasLambdaCaptures(bool hasCaptures) {
  assert(getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  typeChain.front().data.hasCaptures = hasCaptures;
}

/**
 * Check if a function or procedure type has captures
 *
 * @return Has captures
 */
bool SymbolType::hasLambdaCaptures() const {
  assert(getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().data.hasCaptures;
}

/**
 * Set the param and return types of a function or procedure base type
 *
 * @param newParamAndReturnTypes Function param and return types (first is return type, rest are param types)
 */
void SymbolType::setFunctionParamAndReturnTypes(const std::vector<SymbolType> &newParamAndReturnTypes) {
  assert(getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  typeChain.front().paramTypes = newParamAndReturnTypes;
}

/**
 * Get the param and return types of a function or procedure base type
 *
 * @return Function param and return types (first is return type, rest are param types)
 */
const std::vector<SymbolType> &SymbolType::getFunctionParamAndReturnTypes() const {
  assert(getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().paramTypes;
}

/**
 * Get the struct instance for a struct type
 *
 * @param node Accessing AST node
 * @return Struct instance
 */
Struct *SymbolType::getStruct(const ASTNode *node) const {
  assert(is(TY_STRUCT));
  Scope *structDefScope = getBodyScope()->parent;
  const std::string structName = getOriginalSubType();
  const std::vector<SymbolType> &templateTypes = getTemplateTypes();
  return StructManager::matchStruct(structDefScope, structName, templateTypes, node);
}

/**
 * Get the interface instance for an interface type
 *
 * @param node Accessing AST node
 * @return Interface instance
 */
Interface *SymbolType::getInterface(const ASTNode *node) const {
  assert(is(TY_INTERFACE));
  Scope *interfaceDefScope = getBodyScope()->parent;
  return InterfaceManager::matchInterface(interfaceDefScope, getSubType(), getTemplateTypes(), node);
}

bool operator==(const SymbolType &lhs, const SymbolType &rhs) {
  return lhs.typeChain == rhs.typeChain && lhs.specifiers == rhs.specifiers;
}

bool operator!=(const SymbolType &lhs, const SymbolType &rhs) { return !(lhs == rhs); }

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
bool SymbolType::matches(const SymbolType &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const {
  // If the size does not match, it is not equal
  if (typeChain.size() != otherType.typeChain.size())
    return false;

  // Compare the elements
  for (size_t i = 0; i < typeChain.size(); i++) {
    const SymbolType::TypeChainElement &lhsElement = typeChain.at(i);
    const SymbolType::TypeChainElement &rhsElement = otherType.typeChain.at(i);

    // Ignore differences in array size
    if (ignoreArraySize && lhsElement.superType == TY_ARRAY && rhsElement.superType == TY_ARRAY)
      continue;

    // Not both types are arrays -> compare them as usual
    if (lhsElement != rhsElement)
      return false;
  }

  // Ignore difference of specifiers
  if (ignoreSpecifiers && !isPtr() && !isRef())
    return true;

  return specifiers.match(otherType.specifiers, allowConstify);
}

/**
 * Remove pointers / arrays / references if both types have them as far as possible.
 * Furthermore, remove reference wrappers if possible.
 *
 * @param typeA Candidate type
 * @param typeB Requested type
 */
void SymbolType::unwrapBoth(SymbolType &typeA, SymbolType &typeB) {
  // Unwrap both types as far as possible
  while (typeA.isSameContainerTypeAs(typeB)) {
    typeB = typeB.getContainedTy();
    typeA = typeA.getContainedTy();
  }

  // Remove reference wrapper of front type if required
  if (typeA.isRef() && !typeB.isRef())
    typeA = typeA.removeReferenceWrapper();

  // Remove reference wrapper of requested type if required
  if (!typeA.isRef() && typeB.isRef() && !typeA.getBaseType().is(TY_GENERIC))
    typeB = typeB.removeReferenceWrapper();
}

} // namespace spice::compiler