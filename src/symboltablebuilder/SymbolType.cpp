// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolType.h"

#include <exception/CompilerError.h>
#include <exception/SemanticError.h>
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
  if (typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push_back({TY_PTR, "", {}, {}});
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
  if (typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_REFERENCES_NOT_ALLOWED, "Just use the dyn type without '&' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push_back({TY_REF, "", {}, {}});
  return {newTypeChain, specifiers};
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
  newTypeChain.push_back({TY_ARRAY, "", {.arraySize = size}, {}});
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
  TypeChain chainCopy = typeChain;
  // Replace the first element
  chainCopy.front().subType = newSubType;
  // Return the new chain as a symbol type
  return {chainCopy, specifiers};
}

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
SymbolType SymbolType::replaceBaseType(const SymbolType &newBaseType) const {
  assert(!typeChain.empty());
  TypeChain newTypeChain = newBaseType.typeChain;
  for (size_t i = 1; i < typeChain.size(); i++)
    newTypeChain.push_back(typeChain.at(i));
  // Return the new chain as a symbol type
  return {newTypeChain, newBaseType.specifiers};
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

  if (is(TY_STRUCT)) {
    Scope *structBodyScope = getStructBodyScope();
    const std::string structSignature = Struct::getSignature(getSubType(), getTemplateTypes());
    SymbolTableEntry *structSymbol = structBodyScope->parent->lookupStrict(structSignature);
    assert(structSymbol != nullptr);
    llvm::Type *structType = structSymbol->getStructLLVMType();
    assert(structType != nullptr);
    return structType;
  }

  if (is(TY_ENUM))
    return llvm::Type::getInt32Ty(context);

  if (isPtr() || isRef() || (isArray() && getArraySize() == 0)) {
    llvm::PointerType *pointerType = getContainedTy().toLLVMType(context, accessScope)->getPointerTo();
    return static_cast<llvm::Type *>(pointerType);
  }

  if (isArray()) {
    llvm::Type *containedType = getContainedTy().toLLVMType(context, accessScope);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(containedType, getArraySize());
    return static_cast<llvm::Type *>(arrayType);
  }

  throw CompilerError(UNHANDLED_BRANCH, "Cannot determine LLVM type of " + getName(true));
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
  return std::any_of(templateTypes.begin(), templateTypes.end(), [](const SymbolType &t) { return t.hasAnyGenericParts(); });
}

/**
 * Set the list of templates types
 */
void SymbolType::setTemplateTypes(const std::vector<SymbolType> &templateTypes) {
  assert(is(TY_STRUCT));
  typeChain.back().templateTypes = templateTypes;
}

/**
 * Set the list of templates types of the base type
 */
void SymbolType::setBaseTemplateTypes(const std::vector<SymbolType> &templateTypes) {
  assert(isBaseType(TY_STRUCT));
  typeChain.front().templateTypes = templateTypes;
}

bool SymbolType::isCoveredByGenericTypeList(const std::vector<GenericType> &genericTypeList) const {
  const SymbolType baseType = getBaseType();
  // Check if the symbol type itself is generic
  if (baseType.is(TY_GENERIC))
    return std::any_of(genericTypeList.begin(), genericTypeList.end(), [=](const GenericType &t) { return t == baseType; });
  // Check if all template types are covered
  auto &baseTemplateTypes = baseType.getTemplateTypes();
  return std::all_of(baseTemplateTypes.begin(), baseTemplateTypes.end(),
                     [&](const SymbolType &templateType) { return templateType.isCoveredByGenericTypeList(genericTypeList); });
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
std::string SymbolType::getName(bool withSize, bool mangledName) const { // NOLINT(misc-no-recursion)
  std::stringstream name;

  if (!mangledName) {
    // Loop through all specifiers
    if (specifiers.isPublic())
      name << "public ";
    if (specifiers.isInline())
      name << "inline ";
    if (specifiers.isConst())
      name << "const ";
    if (specifiers.isHeap())
      name << "heap ";
    if (!specifiers.isSigned())
      name << "unsigned ";
  }

  // Copy the chain to not destroy the present one
  TypeChain chainCopy = typeChain;
  // Loop through all items
  for (const TypeChainElement &chainElement : chainCopy)
    name << getNameFromChainElement(chainElement, withSize, mangledName);

  return name.str();
}

/**
 * Get the size of the current type
 *
 * Special cases:
 * - 0: Array size was not defined
 *
 * @return Size
 */
size_t SymbolType::getArraySize() const {
  assert(getSuperType() == TY_ARRAY);
  return typeChain.back().data.arraySize;
}

/**
 * Set the struct body scope of the current type
 *
 * @param bodyScope Struct body scope
 */
void SymbolType::setStructBodyScope(Scope *bodyScope) {
  assert(is(TY_STRUCT));
  typeChain.back().data.structBodyScope = bodyScope;
}

/**
 * Return the struct body scope of the current type
 *
 * @return Struct body scope
 */
Scope *SymbolType::getStructBodyScope() const {
  assert(is(TY_STRUCT));
  return typeChain.back().data.structBodyScope;
}

bool operator==(const SymbolType &lhs, const SymbolType &rhs) { return lhs.typeChain == rhs.typeChain; }

bool operator!=(const SymbolType &lhs, const SymbolType &rhs) { return !(lhs == rhs); }

/**
 * Compare two symbol types, ignoring array sizes.
 * This can be used for function argument matching or similar
 *
 * @param lhs Other symbol type
 * @return Equal or not
 */
bool SymbolType::equalsIgnoreArraySize(const SymbolType &otherType) const {
  // If the size does not match, it is not equal
  if (typeChain.size() != otherType.typeChain.size())
    return false;
  // Compare the elements
  for (size_t i = 0; i < typeChain.size(); i++) {
    const SymbolType::TypeChainElement &lhsElement = typeChain.at(i);
    const SymbolType::TypeChainElement &rhsElement = otherType.typeChain.at(i);

    // Ignore differences in array size
    if (lhsElement.superType == TY_ARRAY && rhsElement.superType == TY_ARRAY)
      continue;

    // Not both types are arrays -> compare them as usual
    if (lhsElement != rhsElement)
      return false;
  }
  return true;
}

/**
 * Get the name of a type chain element
 *
 * @param chainElement Input chain element
 * @param withSize Include size in string
 * @return Type chain element name
 */
std::string SymbolType::getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, // NOLINT(misc-no-recursion)
                                                bool mangledName) {
  switch (chainElement.superType) {
  case TY_PTR:
    return mangledName ? "ptr" : "*";
  case TY_REF:
    return mangledName ? "ref" : "&";
  case TY_ARRAY: {
    if (mangledName)
      return "array";
    if (!withSize || chainElement.data.arraySize == ARRAY_SIZE_UNKNOWN)
      return "[]";
    return "[" + std::to_string(chainElement.data.arraySize) + "]";
  }
  case TY_DOUBLE:
    return "double";
  case TY_INT:
    return "int";
  case TY_SHORT:
    return "short";
  case TY_LONG:
    return "long";
  case TY_BYTE:
    return "byte";
  case TY_CHAR:
    return "char";
  case TY_STRING:
    return "string";
  case TY_BOOL:
    return "bool";
  case TY_STRUCT: {
    std::string templateStr;
    if (!chainElement.templateTypes.empty()) {
      for (const auto &templateType : chainElement.templateTypes) {
        if (!templateStr.empty())
          templateStr += ",";
        templateStr += templateType.getName();
      }
      templateStr = "<" + templateStr + ">";
    }
    return chainElement.subType + templateStr;
  }
  case TY_INTERFACE:
    return "interface(" + chainElement.subType + ")";
  case TY_ENUM:
    return "enum";
  case TY_GENERIC:
    return "generic(" + chainElement.subType + ")";
  case TY_ALIAS:
    return "alias(" + chainElement.subType + ")";
  case TY_DYN:
    return "dyn";
  case TY_FUNCTION:
    return "function";
  case TY_PROCEDURE:
    return "procedure";
  case TY_IMPORT:
    return "import";
  case TY_INVALID:    // GCOV_EXCL_LINE
    return "invalid"; // GCOV_EXCL_LINE
  }
  return "unknown"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler