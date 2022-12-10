// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolType.h"

#include <stdexcept>
#include <tuple>

#include <exception/SemanticError.h>
#include <irgenerator/StdFunctionManager.h>
#include <model/Struct.h>
#include <symboltablebuilder/SymbolTable.h>
#include <symboltablebuilder/SymbolTableEntry.h>

/**
 * Get the pointer type of the current type as a new type
 *
 * @return Pointer type of the current type
 */
SymbolType SymbolType::toPointer(const ASTNode *node, llvm::Value *dynamicSize) const {
  // Do not allow pointers of dyn
  if (typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push_back({TY_PTR, "", {}, {}, dynamicSize});
  return SymbolType(newTypeChain);
}

/**
 * Get the reference type of the current type as a new type
 *
 * @return Reference type of the current type
 */
SymbolType SymbolType::toReference(const ASTNode *node) const {
  // Do not allow pointers of dyn
  if (typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_REFERENCES_NOT_ALLOWED, "Just use the dyn type without '&' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push_back({TY_REF, "", {}, {}, nullptr});
  return SymbolType(newTypeChain);
}

/**
 * Get the array type of the current type as a new type
 *
 * @return Array type of the current type
 */
SymbolType SymbolType::toArray(const ASTNode *node, long size) const {
  // Do not allow arrays of dyn
  if (typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push_back({TY_ARRAY, "", {.arraySize = size}, {}, nullptr});
  return SymbolType(newTypeChain);
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
SymbolType SymbolType::getContainedTy() const {
  if (typeChain.back().superType == TY_STRING)
    return SymbolType(TY_CHAR);
  if (typeChain.size() < 2)                                                                                     // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get contained type of type with type chain < 2"); // GCOV_EXCL_LINE
  TypeChain newTypeChain = typeChain;
  newTypeChain.pop_back();
  return SymbolType(newTypeChain);
}

/**
 * Replace the subtype of the base type with another one
 *
 * @param newSubType New sub type of the base type
 * @return The new type with the adjusted type chain
 */
SymbolType SymbolType::replaceBaseSubType(const std::string &newSubType) const {
  SymbolType newBaseType = getBaseType();
  newBaseType.setSubType(newSubType);
  return replaceBaseType(newBaseType);
}

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
SymbolType SymbolType::replaceBaseType(const SymbolType &newBaseType) const {
  // Copy the stack to not destroy the present one
  TypeChain chainCopy = typeChain;
  // Reverse type chain
  TypeChain tmp;
  while (chainCopy.back().superType == TY_PTR || chainCopy.back().superType == TY_REF || chainCopy.back().superType == TY_ARRAY) {
    tmp.push_back(chainCopy.back());
    chainCopy.pop_back();
  }
  chainCopy = newBaseType.typeChain; // Replace base type
  // Restore the other chain elements
  for (unsigned int i = 0; i < tmp.size(); i++) {
    chainCopy.push_back(tmp.back());
    tmp.pop_back();
  }
  // Return the new chain as a symbol type
  return SymbolType(chainCopy);
}

/**
 * Return the LLVM type for this symbol type
 *
 * @param context LLVM context
 * @param accessScope Access scope for structs
 * @return Corresponding LLVM type
 */
llvm::Type *SymbolType::toLLVMType(llvm::LLVMContext &context, SymbolTable *accessScope) const { // NOLINT(misc-no-recursion)
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
    std::string structSignature = Struct::getSignature(getSubType(), getTemplateTypes());
    SymbolTableEntry *structSymbol = accessScope->lookup(structSignature);
    assert(structSymbol);
    llvm::Type *structType = structSymbol->getStructLLVMType();
    assert(structType);
    return structType;
  }

  if (is(TY_ENUM))
    return llvm::Type::getInt32Ty(context);

  if (isPointer() || isReference() || (isArray() && getArraySize() <= 0)) {
    llvm::PointerType *pointerType = getContainedTy().toLLVMType(context, accessScope)->getPointerTo();
    return static_cast<llvm::Type *>(pointerType);
  }

  if (isArray()) {
    llvm::Type *containedType = getContainedTy().toLLVMType(context, accessScope);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(containedType, getArraySize());
    return static_cast<llvm::Type *>(arrayType);
  }

  throw std::runtime_error("Internal compiler error: Cannot determine LLVM type of " + getName(true));
}

/**
 * Check is the current type is of type pointer
 *
 * @return Pointer or not
 */
bool SymbolType::isPointer() const { return getSuperType() == TY_PTR; }

/**
 * Check if the current type is a pointer of a certain super type
 *
 * @param elementSuperType Super type to check for
 * @return Pointer or not
 */
bool SymbolType::isPointerOf(SymbolSuperType elementSuperType) const {
  return isPointer() && getContainedTy().is(elementSuperType);
}

/**
 * Check is the current type is of type reference
 *
 * @return Reference or not
 */
bool SymbolType::isReference() const { return getSuperType() == TY_REF; }

/**
 * Check if the current type is a reference of a certain super type
 *
 * @param elementSuperType Super type to check for
 * @return Reference or not
 */
bool SymbolType::isReferenceOf(SymbolSuperType elementSuperType) const {
  return isReference() && getContainedTy().is(elementSuperType);
}

/**
 * Check is the current type is of type array
 *
 * @return Array or not
 */
bool SymbolType::isArray() const { return getSuperType() == TY_ARRAY; }

/**
 * Check if the current type is an array of a certain super type
 *
 * @param elementSuperType Super type to check for
 * @return Array of super type or not
 */
bool SymbolType::isArrayOf(SymbolSuperType elementSuperType) const { return isArray() && getContainedTy().is(elementSuperType); }

/**
 * Check if the current type is an array of the given type. Array size is ignored.
 *
 * @param otherSymbolType Symbol type
 * @return Array of contained symbol type or not
 */
bool SymbolType::isArrayOf(const SymbolType &otherSymbolType) const { return isArray() && getContainedTy() == otherSymbolType; }

/**
 * Check if the current type is of a certain super type
 *
 * @param superType Super type to check for
 * @return Applicable or not
 */
bool SymbolType::is(SymbolSuperType superType) const { return getSuperType() == superType; }

/**
 * Check if the current type is of a certain super type and sub type
 *
 * @param superType Super type to check for
 * @param subType Sub type to check for
 * @return Applicable or not
 */
bool SymbolType::is(SymbolSuperType superType, const std::string &subType) const {
  return getSuperType() == superType && getSubType() == subType;
}

/**
 * Check if the current type is one of the primitive types
 *
 * @return Primitive or not
 */
bool SymbolType::isPrimitive() const {
  return isOneOf({TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL});
}

/**
 * Check if the base type of the current type chain is of a certain super type
 *
 * @param superType Super type to check for
 * @return Applicable or not
 */
bool SymbolType::isBaseType(SymbolSuperType superType) const {
  // Copy the stack to not destroy the present one
  TypeChain chainCopy = typeChain;
  // Unwrap the chain until the base type can be retrieved
  while (chainCopy.back().superType == TY_PTR || chainCopy.back().superType == TY_REF || chainCopy.back().superType == TY_ARRAY)
    chainCopy.pop_back();
  // Check if it is of the given superType and subType
  return chainCopy.back().superType == superType;
}

/**
 * Check if the current type is amongst a collection of certain super types
 *
 * @param superTypes Vector of super types
 * @return Applicable or not
 */
bool SymbolType::isOneOf(const std::vector<SymbolSuperType> &superTypes) const {
  SymbolSuperType superType = getSuperType();
  return std::any_of(superTypes.begin(), superTypes.end(), [&superType](int type) { return type == superType; });
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
 * Retrieve the super type of the current type
 *
 * @return Super type
 */
SymbolSuperType SymbolType::getSuperType() const { return typeChain.back().superType; }

/**
 * Retrieve the sub type of the current type
 *
 * @return Sub type
 */
std::string SymbolType::getSubType() const {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
  return typeChain.back().subType;
}

/**
 * Retrieve the base type of the current type. E.g. int of int[]*[]**
 *
 * @return Base type
 */
SymbolType SymbolType::getBaseType() const {
  // Copy the stack to not destroy the present one
  TypeChain chainCopy = typeChain;
  // Unwrap the chain until the base type can be retrieved
  while (chainCopy.back().superType == TY_PTR || chainCopy.back().superType == TY_REF || chainCopy.back().superType == TY_ARRAY)
    chainCopy.pop_back();
  // Return the base type
  return SymbolType(chainCopy);
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

/**
 * Retrieve the list of template types of the current type
 *
 * @return Template types
 */
const std::vector<SymbolType> &SymbolType::getTemplateTypes() const { return typeChain.back().templateTypes; }

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
std::string SymbolType::getName(bool withSize, bool mangledName) const { // NOLINT(misc-no-recursion)
  std::string name;
  TypeChain chain = typeChain;
  for (int i = 0; i < typeChain.size(); i++) {
    TypeChainElement chainElement = chain.back();
    name.insert(0, getNameFromChainElement(chainElement, withSize, mangledName));
    chain.pop_back();
  }
  return name;
}

/**
 * Get the size of the current type
 *
 * Special cases:
 * - 0: Array size was not defined
 *
 * @return Size
 */
long SymbolType::getArraySize() const {
  if (typeChain.back().superType != TY_ARRAY)                                               // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get size of non-array type"); // GCOV_EXCL_LINE

  return typeChain.back().data.arraySize;
}

/**
 * Set the signedness of the current type
 *
 * @param value Signed or not signed
 */
void SymbolType::setSigned(bool value) {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG}));
  typeChain.back().data.numericSigned = value;
}

/**
 * Check if the current type is signed
 */
bool SymbolType::isSigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG}));
  return typeChain.back().data.numericSigned;
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

/**
 * Retrieve the dynamic array size of the current type
 *
 * @return Dynamic array size
 */
llvm::Value *SymbolType::getDynamicArraySize() const {
  if (typeChain.back().superType != TY_PTR)                                                          // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get dynamic sized of non-array type"); // GCOV_EXCL_LINE
  if (typeChain.back().data.arraySize > ARRAY_SIZE_UNKNOWN)                                          // GCOV_EXCL_LINE
    throw std::runtime_error("Cannot retrieve dynamic size of non-dynamically-sized array");         // GCOV_EXCL_LINE

  return typeChain.back().dynamicArraySize;
}

bool operator==(const SymbolType &lhs, const SymbolType &rhs) { return lhs.typeChain == rhs.typeChain; }

bool operator!=(const SymbolType &lhs, const SymbolType &rhs) { return !(lhs.typeChain == rhs.typeChain); }

/**
 * Set the sub type of the top element
 *
 * @param newSubType New sub type
 */
void SymbolType::setSubType(const std::string &newSubType) { typeChain.back().subType = newSubType; }

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
    if (chainElement.data.arraySize == ARRAY_SIZE_DYNAMIC)
      return "[n]";
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
  case TY_DYN:
    return "dyn";
  case TY_FUNCTION:
    return "function";
  case TY_PROCEDURE:
    return "procedure";
  case TY_IMPORT:
    return "import";
  case TY_GENERIC:
    return "generic(" + chainElement.subType + ")";
  case TY_INVALID:
    return "invalid"; // GCOV_EXCL_LINE
  }
  return "unknown"; // GCOV_EXCL_LINE
}