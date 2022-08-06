// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolType.h"
#include "analyzer/AnalyzerVisitor.h"

#include <stdexcept>
#include <tuple>
#include <utility>

#include <exception/ErrorFactory.h>
#include <symbol/Struct.h>
#include <symbol/SymbolTable.h>
#include <symbol/SymbolTableEntry.h>

/**
 * Retrieve the type chain of the symbol type
 *
 * @return Type chain
 */
SymbolType::TypeChain SymbolType::getTypeChain() const { return typeChain; }

/**
 * Get the pointer type of the current type as a new type
 *
 * @return Pointer type of the current type
 */
SymbolType SymbolType::toPointer(const ErrorFactory *err, const CodeLoc &codeLoc, llvm::Value *dynamicSize) const {
  // Do not allow pointers of dyn
  if (typeChain.top().superType == TY_DYN)
    throw err->get(codeLoc, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push({TY_PTR, "", {}, dynamicSize});
  return SymbolType(newTypeChain);
}

/**
 * Get the array type of the current type as a new type
 *
 * @return Array type of the current type
 */
SymbolType SymbolType::toArray(const ErrorFactory *err, const CodeLoc &codeLoc, int size) const {
  // Do not allow arrays of dyn
  if (typeChain.top().superType == TY_DYN)
    throw err->get(codeLoc, DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.push({TY_ARRAY, std::to_string(size), {}, nullptr});
  return SymbolType(newTypeChain);
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
SymbolType SymbolType::getContainedTy() const {
  if (typeChain.empty())                                                                          // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get contained type of empty type"); // GCOV_EXCL_LINE
  if (typeChain.top().superType == TY_STRING)
    return SymbolType(TY_CHAR);
  TypeChain newTypeChain = typeChain;
  newTypeChain.pop();
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
  while (chainCopy.top().superType == TY_PTR || chainCopy.top().superType == TY_ARRAY) {
    tmp.push(chainCopy.top());
    chainCopy.pop();
  }
  chainCopy = newBaseType.typeChain; // Replace base type
  // Restore the other chain elements
  for (unsigned int i = 0; i < tmp.size(); i++) {
    chainCopy.push(tmp.top());
    tmp.pop();
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
llvm::Type *SymbolType::toLLVMType(llvm::LLVMContext &context, SymbolTable *accessScope) const {
  assert(!isOneOf({TY_DYN, TY_INVALID}));

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
    SymbolTable *structScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
    SymbolTableEntry *structSymbol = accessScope->lookup(structSignature);
    assert(structSymbol);
    size_t structFieldCount = structScope->getFieldCount();
    std::vector<llvm::Type *> elementTypes;
    elementTypes.reserve(structFieldCount);
    for (size_t i = 0; i < structFieldCount; i++) {
      SymbolTableEntry *fieldEntry = structScope->lookupByIndex(i);
      assert(fieldEntry != nullptr);
      elementTypes.push_back(fieldEntry->getType().toLLVMType(context, accessScope));
    }
    return llvm::StructType::get(context, elementTypes);
  }

  if (isPointer() || (isArray() && getArraySize() <= 0)) {
    llvm::PointerType *pointerType = getContainedTy().toLLVMType(context, accessScope)->getPointerTo();
    return static_cast<llvm::Type *>(pointerType);
  }

  if (isArray()) {
    llvm::ArrayType *arrayType = llvm::ArrayType::get(getContainedTy().toLLVMType(context, accessScope), getArraySize());
    return static_cast<llvm::Type *>(arrayType);
  }

  assert(!is(TY_GENERIC));
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
  while (chainCopy.top().superType == TY_PTR || chainCopy.top().superType == TY_ARRAY)
    chainCopy.pop();
  // Check if it is of the given superType and subType
  return chainCopy.top().superType == superType;
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
 * Retrieve the super type of the current type
 *
 * @return Super type
 */
SymbolSuperType SymbolType::getSuperType() const { return typeChain.top().superType; }

/**
 * Retrieve the sub type of the current type
 *
 * @return Sub type
 */
std::string SymbolType::getSubType() const { return typeChain.top().subType; }

/**
 * Retrieve the base type of the current type. E.g. int of int[]*[]**
 *
 * @return Base type
 */
SymbolType SymbolType::getBaseType() const {
  // Copy the stack to not destroy the present one
  TypeChain chainCopy = typeChain;
  // Unwrap the chain until the base type can be retrieved
  while (chainCopy.top().superType == TY_PTR || chainCopy.top().superType == TY_ARRAY)
    chainCopy.pop();
  // Check if it is of the given superType and subType
  return SymbolType(chainCopy);
}

/**
 * Set the list of templates types
 */
void SymbolType::setTemplateTypes(std::vector<SymbolType> templateTypes) {
  typeChain.top().templateTypes = std::move(templateTypes);
}

/**
 * Retrieve the list of template types of the current type
 *
 * @return Template types
 */
std::vector<SymbolType> SymbolType::getTemplateTypes() const { return typeChain.top().templateTypes; }

/**
 * Set the type to signed
 *
 * @param isSigned Signed or unsigned
 */
void SymbolType::setSigned(bool isSigned) { isBaseTypeSigned = isSigned; }

/**
 * Check if the current type is signed
 *
 * @return Signed or unsigned
 */
bool SymbolType::isSigned() const { return isBaseTypeSigned; }

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
std::string SymbolType::getName(bool withSize, bool mangledName) const {
  std::string name;
  TypeChain chain = typeChain;
  for (int i = 0; i < typeChain.size(); i++) {
    TypeChainElement chainElement = chain.top();
    name.insert(0, getNameFromChainElement(chainElement, withSize, mangledName));
    chain.pop();
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
int SymbolType::getArraySize() const {
  if (typeChain.top().superType != TY_ARRAY)                                                // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get size of non-array type"); // GCOV_EXCL_LINE

  return std::stoi(typeChain.top().subType);
}

/**
 * Set the dynamic array size of the current type
 */
SymbolType SymbolType::setDynamicArraySize(llvm::Value *dynamicArraySize) const {
  if (typeChain.top().superType != TY_PTR)                                                  // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get size of non-array type"); // GCOV_EXCL_LINE

  TypeChain newTypeChain = typeChain;
  newTypeChain.pop();
  newTypeChain.push({typeChain.top().superType, typeChain.top().subType, typeChain.top().templateTypes, dynamicArraySize});
  return SymbolType(newTypeChain);
}

/**
 * Retrieve the dynamic array size of the current type
 *
 * @return Dynamic array size
 */
llvm::Value *SymbolType::getDynamicArraySize() const {
  if (typeChain.top().superType != TY_PTR)                                                           // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: Cannot get dynamic sized of non-array type"); // GCOV_EXCL_LINE

  return typeChain.top().dynamicArraySize;
}

/**
 * Compares the type chains of two symbol types without taking array sizes into account
 *
 * @param lhs Lhs symbol type
 * @param rhs Rhs symbol type
 * @return Equal or not
 */
bool equalsIgnoreArraySizes(SymbolType lhs, SymbolType rhs) {
  // Compare sizes of stacks
  if (lhs.typeChain.size() != rhs.typeChain.size())
    return false;

  // Compare stack elements
  for (int i = 0; i < lhs.typeChain.size(); i++) {
    if ((lhs.typeChain.top().superType != TY_ARRAY || rhs.typeChain.top().superType != TY_ARRAY) &&
        lhs.typeChain.top() != rhs.typeChain.top()) {
      return false;
    }
    lhs.typeChain.pop();
    rhs.typeChain.pop();
  }

  return true;
}

bool operator==(const SymbolType &lhs, const SymbolType &rhs) { return lhs.typeChain == rhs.typeChain; }

bool operator!=(const SymbolType &lhs, const SymbolType &rhs) { return lhs.typeChain != rhs.typeChain; }

/**
 * Set the sub type of the top element
 *
 * @param newSubType New sub type
 */
void SymbolType::setSubType(const std::string &newSubType) { typeChain.top().subType = newSubType; }

/**
 * Get the name of a type chain element
 *
 * @param chainElement Input chain element
 * @param withSize Include size in string
 * @return Type chain element name
 */
std::string SymbolType::getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, bool mangledName) {
  switch (chainElement.superType) {
  case TY_PTR:
    return mangledName ? "ptr" : "*";
  case TY_ARRAY: {
    if (mangledName)
      return "array";
    return !withSize || chainElement.subType == "0" ? "[]" : "[" + chainElement.subType + "]";
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