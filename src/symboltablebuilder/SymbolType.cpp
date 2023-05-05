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
  if (typeChain.back().superType == TY_REF)
    throw SemanticError(node, REF_POINTERS_ARE_NOT_ALLOWED, "Pointers to references are not allowed. Use pointer instead");

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
  // Do not allow references of references
  if (typeChain.back().superType == TY_REF)
    return *this;

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

  if (is(TY_STRUCT)) {
    Scope *structBodyScope = getBodyScope();
    const std::string structSignature = Struct::getSignature(getSubType(), getTemplateTypes());
    SymbolTableEntry *structSymbol = structBodyScope->parent->lookupStrict(structSignature);
    assert(structSymbol != nullptr);
    llvm::StructType *structType = structSymbol->getStructLLVMType();

    // If the type is not known yet, build the LLVM type
    if (structType == nullptr) {
      Struct *spiceStruct = structSymbol->getType().getStruct(structSymbol->declNode);
      assert(spiceStruct != nullptr);
      structType = llvm::StructType::create(context, spiceStruct->getMangledName());
      structSymbol->setStructLLVMType(structType);

      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      fieldTypes.reserve(spiceStruct->fieldTypes.size());
      for (const SymbolType &fieldType : spiceStruct->fieldTypes)
        fieldTypes.push_back(fieldType.toLLVMType(context, accessScope));

      // Set field types to struct type
      structType->setBody(fieldTypes);
    }

    return structType;
  }

  if (is(TY_ENUM))
    return llvm::Type::getInt32Ty(context);

  if (isPtr() || isRef() || (isArray() && getArraySize() == 0) || isOneOf({TY_FUNCTION, TY_PROCEDURE}))
    return static_cast<llvm::Type *>(llvm::PointerType::get(context, 0));

  if (isArray()) {
    llvm::Type *containedType = getContainedTy().toLLVMType(context, accessScope);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(containedType, getArraySize());
    return static_cast<llvm::Type *>(arrayType);
  }

  throw CompilerError(UNHANDLED_BRANCH, "Cannot determine LLVM type of " + getName(true));
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
  for (const SymbolType &interfaceType : spiceStruct->interfaceTypes) {
    assert(interfaceType.is(TY_INTERFACE));
    if (symbolType.matches(interfaceType, false, false, true))
      return true;
  }
  return false;
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

bool SymbolType::isCoveredByGenericTypeList(const std::vector<GenericType> &genericTypeList) const {
  const SymbolType baseType = getBaseType();
  // Check if the symbol type itself is generic
  if (baseType.is(TY_GENERIC))
    return std::any_of(genericTypeList.begin(), genericTypeList.end(), [=](const GenericType &t) { return t == baseType; });
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

  // Append the specifiers
  if (!mangledName) {
    const TypeSpecifiers defaultForSuperType = TypeSpecifiers::of(getBaseType().getSuperType());
    if (specifiers.isPublic() && !defaultForSuperType.isPublic())
      name << "public ";
    if (specifiers.isInline() && !defaultForSuperType.isInline())
      name << "inline ";
    if (specifiers.isConst() && !defaultForSuperType.isConst())
      name << "const ";
    if (specifiers.isHeap() && !defaultForSuperType.isHeap())
      name << "heap ";
    if (specifiers.isSigned() && !defaultForSuperType.isSigned())
      name << "signed ";
    if (!specifiers.isSigned() && defaultForSuperType.isSigned())
      name << "unsigned ";
  }

  // Loop through all items
  for (const TypeChainElement &chainElement : typeChain)
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
 * Check if the current type is const.
 * Only base types can be const. Pointer and references are always non-const
 */
bool SymbolType::isConst() const { return typeChain.size() == 1 && specifiers.isConst(); }

/**
 * Check if the current type is signed
 */
bool SymbolType::isSigned() const {
  assert(isOneOf({TY_INT, TY_SHORT, TY_LONG}));
  return specifiers.isSigned();
}

/**
 * Check if the current type is inline
 */
bool SymbolType::isInline() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return specifiers.isInline();
}

/**
 * Check if the current type is public
 */
bool SymbolType::isPublic() const {
  assert(isPrimitive() /* Global variables */ || isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_ENUM, TY_STRUCT, TY_INTERFACE}));
  return specifiers.isPublic();
}

/**
 * Check if the current type is heap
 */
bool SymbolType::isHeap() const {
  assert(isPrimitive() /* Local variables */ || is(TY_STRUCT));
  return specifiers.isHeap();
}

/**
 * Set the body scope of the current type
 * Available for structs and interfaces
 *
 * @param bodyScope Body scope
 */
void SymbolType::setBodyScope(Scope *bodyScope) {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  typeChain.back().data.bodyScope = bodyScope;
}

/**
 * Return the body scope of the current type
 *
 * @return Body scope
 */
Scope *SymbolType::getBodyScope() const {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  return typeChain.back().data.bodyScope;
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
    paramTypes.push_back(returnType);
  else
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
    paramTypes.resize(newParamTypes.size() + 1);
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
  if (!typeChain.back().paramTypes.empty())
    return {typeChain.back().paramTypes.begin() + 1, typeChain.back().paramTypes.end()};
  return {};
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
 * Get the name of a type chain element
 *
 * @param chainElement Input chain element
 * @param withSize Include size in string
 * @return Type chain element name
 */
std::string SymbolType::getNameFromChainElement(const TypeChainElement &chainElement, bool withSize, bool mangledName) const {
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
  case TY_FUNCTION: {
    std::stringstream paramTypesString;
    const SymbolType &baseType = getBaseType();
    for (const SymbolType &paramType : baseType.getFunctionParamTypes()) {
      if (paramTypesString.rdbuf()->in_avail() > 0)
        paramTypesString << ",";
      paramTypesString << paramType.getName(true);
    }
    const std::string returnTypeString = baseType.getFunctionReturnType().getName(true);
    return "f<" + returnTypeString + ">(" + paramTypesString.str() + ")";
  }
  case TY_PROCEDURE: {
    std::stringstream paramTypesString;
    const SymbolType &baseType = getBaseType();
    for (const SymbolType &paramType : baseType.getFunctionParamTypes()) {
      if (paramTypesString.rdbuf()->in_avail() > 0)
        paramTypesString << ",";
      paramTypesString << paramType.getName(true);
    }
    return "p(" + paramTypesString.str() + ")";
  }
  case TY_IMPORT:
    return "import";
  case TY_INVALID:    // GCOV_EXCL_LINE
    return "invalid"; // GCOV_EXCL_LINE
  }
  return "unknown"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler