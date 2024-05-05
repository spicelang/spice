// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "Type.h"

#include <SourceFile.h>
#include <ast/Attributes.h>
#include <exception/CompilerError.h>
#include <exception/SemanticError.h>
#include <global/TypeRegistry.h>
#include <irgenerator/NameMangling.h>
#include <irgenerator/StdFunctionManager.h>
#include <model/GenericType.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableEntry.h>

namespace spice::compiler {

Type::Type(SuperType superType) : typeChain({TypeChainElement{superType}}), specifiers(TypeSpecifiers::of(superType)) {}

Type::Type(const QualType &qualType) : typeChain(qualType.getType().typeChain), specifiers(qualType.getType().specifiers) {}

Type::Type(SuperType superType, const std::string &subType)
    : typeChain({TypeChainElement{superType, subType}}), specifiers(TypeSpecifiers::of(superType)) {}

Type::Type(SuperType superType, const std::string &subType, uint64_t typeId, const Type::TypeChainElementData &data,
           const std::vector<QualType> &templateTypes)
    : typeChain({TypeChainElement(superType, subType, typeId, data, templateTypes)}), specifiers(TypeSpecifiers::of(superType)) {}

Type::Type(const TypeChain &types) : typeChain(types), specifiers(TypeSpecifiers::of(types.front().superType)) {}

Type::Type(TypeChain types, TypeSpecifiers specifiers) : typeChain(std::move(types)), specifiers(specifiers) {}

/**
 * Get the pointer type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Pointer type of the current type
 */
Type Type::toPointer(const ASTNode *node) const {
  // Do not allow pointers of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");
  if (isRef())
    throw SemanticError(node, REF_POINTERS_ARE_NOT_ALLOWED, "Pointers to references are not allowed. Use pointer instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_PTR);
  return {newTypeChain, specifiers};
}

/**
 * Get the pointer type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Pointer type of the current type
 */
const Type *Type::toPtr(const ASTNode *node) const {
  // Do not allow pointers of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");
  if (isRef())
    throw SemanticError(node, REF_POINTERS_ARE_NOT_ALLOWED, "Pointers to references are not allowed. Use pointer instead");

  // Create new type
  Type newType = *this;
  newType.typeChain.emplace_back(TY_PTR);

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newType);
}

/**
 * Get the reference type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Reference type of the current type
 */
Type Type::toReference(const ASTNode *node) const {
  // Do not allow references of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_REFERENCES_NOT_ALLOWED, "Just use the dyn type without '&' instead");
  // Do not allow references of references
  if (isRef())
    throw SemanticError(node, MULTI_REF_NOT_ALLOWED, "References to references are not allowed");

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_REF);
  return {newTypeChain, specifiers};
}

/**
 * Get the reference type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Reference type of the current type
 */
const Type *Type::toRef(const ASTNode *node) const {
  // Do not allow references of dyn
  if (is(TY_DYN))
    throw SemanticError(node, DYN_REFERENCES_NOT_ALLOWED, "Just use the dyn type without '&' instead");
  // Do not allow references of references
  if (isRef())
    throw SemanticError(node, MULTI_REF_NOT_ALLOWED, "References to references are not allowed");

  // Create new type
  Type newType = *this;
  newType.typeChain.emplace_back(TY_REF);

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newType);
}

/**
 * Get the const reference type of the current type as a new type
 *
 * @param node AST node for error messages
 * @return Const reference type of the current type
 */
Type Type::toConstReference(const ASTNode *node) const {
  Type constRefType = toReference(node);
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
Type Type::toArray(const ASTNode *node, unsigned int size, bool skipDynCheck /*=false*/) const {
  // Do not allow arrays of dyn
  if (!skipDynCheck && typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_ARRAY, TypeChainElementData{.arraySize = size});
  return {newTypeChain, specifiers};
}
/**
 * Get the array type of the current type as a new type
 *
 * @param node AST node for error messages
 * @param size Size of the array
 * @return Array type of the current type
 */
const Type *Type::toArr(const ASTNode *node, unsigned int size, bool skipDynCheck /*=false*/) const {
  // Do not allow arrays of dyn
  if (!skipDynCheck && typeChain.back().superType == TY_DYN)
    throw SemanticError(node, DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

  // Create new type
  Type newType = *this;
  newType.typeChain.emplace_back(TY_ARRAY, TypeChainElementData{.arraySize = size});

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newType);
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
Type Type::getContainedTy() const {
  if (is(TY_STRING))
    return Type(TY_CHAR);
  assert(typeChain.size() > 1);
  TypeChain newTypeChain = typeChain;
  newTypeChain.pop_back();
  return {newTypeChain, specifiers};
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
const Type *Type::getContained() const {
  if (is(TY_STRING))
    return TypeRegistry::getOrInsert(TY_CHAR);

  // Create new type
  Type newType = *this;
  assert(newType.typeChain.size() > 1);
  newType.typeChain.pop_back();

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newType);
}

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
Type Type::replaceBaseType(const Type &newBaseType) const {
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
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
const Type *Type::replaceBase(const Type &newBaseType) const {
  assert(!typeChain.empty());

  // Create new type
  Type newType = *this;
  TypeChain &newTypeChain = newType.typeChain;
  const bool doubleRef = newTypeChain.back().superType == TY_REF && typeChain.back().superType == TY_REF;
  for (size_t i = 1; i < typeChain.size(); i++)
    if (!doubleRef || i > 1)
      newTypeChain.push_back(typeChain.at(i));

  // Create new specifiers
  newType.specifiers = specifiers.merge(newBaseType.specifiers);

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newType);
}

/**
 * Return the LLVM type for this symbol type
 *
 * @param context LLVM context
 * @param accessScope Access scope for structs
 * @return Corresponding LLVM type
 */
llvm::Type *Type::toLLVMType(llvm::LLVMContext &context, Scope *accessScope) const { // NOLINT(misc-no-recursion)
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
    return llvm::PointerType::get(context, 0);

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
      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      bool isPacked = false;
      if (is(TY_STRUCT)) { // Struct
        Struct *spiceStruct = structSymbol->getQualType().getType().getStruct(structSymbol->declNode);
        assert(spiceStruct != nullptr);
        const std::string mangledName = NameMangling::mangleStruct(*spiceStruct);
        structType = llvm::StructType::create(context, mangledName);
        structSymbol->setStructLLVMType(structType);

        const size_t totalFieldCount = spiceStruct->scope->getFieldCount();
        fieldTypes.reserve(totalFieldCount);

        // If the struct has no interface types, but a vtable was requested, add another ptr field type
        assert(structSymbol->declNode->isStructDef());
        auto structDeclNode = spice_pointer_cast<StructDefNode *>(structSymbol->declNode);
        if (!structDeclNode->hasInterfaces && structDeclNode->emitVTable)
          fieldTypes.push_back(llvm::PointerType::get(context, 0));

        // Collect all field types
        for (size_t i = 0; i < totalFieldCount; i++) {
          const SymbolTableEntry *fieldSymbol = spiceStruct->scope->symbolTable.lookupStrictByIndex(i);
          assert(fieldSymbol != nullptr);
          fieldTypes.push_back(fieldSymbol->getQualType().toLLVMType(context, accessScope));
        }

        // Check if the struct is declared as packed
        if (structDeclNode->attrs() && structDeclNode->attrs()->attrLst()->hasAttr(ATTR_CORE_COMPILER_PACKED))
          isPacked = structDeclNode->attrs()->attrLst()->getAttrValueByName(ATTR_CORE_COMPILER_PACKED)->boolValue;
      } else { // Interface
        Interface *spiceInterface = structSymbol->getQualType().getType().getInterface(structSymbol->declNode);
        assert(spiceInterface != nullptr);
        const std::string mangledName = NameMangling::mangleInterface(*spiceInterface);
        structType = llvm::StructType::create(context, mangledName);
        structSymbol->setStructLLVMType(structType);

        fieldTypes.push_back(llvm::PointerType::get(context, 0));
      }

      // Set field types to struct type
      structType->setBody(fieldTypes, isPacked);
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
bool Type::isIterator(const ASTNode *node) const {
  if (!is(TY_STRUCT))
    return false;
  Type genericType(TY_GENERIC, "T");
  Type iteratorType(TY_INTERFACE, IITERATOR_NAME, TYPE_ID_ITERATOR_INTERFACE, {.bodyScope = nullptr}, {genericType});
  return implements(iteratorType, node);
}

/**
 * Check if the current type is an iterable
 * - Arrays are always considered iterable
 * - Otherwise the type must be a struct that implements the iterator interface
 *
 * @param node ASTNode
 * @return Iterable or not
 */
bool Type::isIterable(const ASTNode *node) const {
  if (isArray())
    return true; // Arrays are always considered iterable
  if (!is(TY_STRUCT))
    return false;
  Type genericType(TY_GENERIC, "T");
  Type iteratorType(TY_INTERFACE, IITERATOR_NAME, TYPE_ID_ITERABLE_INTERFACE, {.bodyScope = nullptr}, {genericType});
  return implements(iteratorType, node);
}

/**
 * Check if the current type is a string object
 *
 * @return String object or not
 */
bool Type::isStringObj() const { return is(TY_STRUCT) && getSubType() == STROBJ_NAME && getBodyScope()->sourceFile->stdFile; }

/**
 * Check if the current type is a error object
 *
 * @return Error object or not
 */
bool Type::isErrorObj() const { return is(TY_STRUCT) && getSubType() == ERROBJ_NAME && getBodyScope()->sourceFile->stdFile; }

/**
 * Check if the current type implements the given interface type
 *
 * @param symbolType Interface type
 * @param node ASTNode
 * @return Struct implements interface or not
 */
bool Type::implements(const Type &symbolType, const ASTNode *node) const {
  assert(is(TY_STRUCT) && symbolType.is(TY_INTERFACE));
  Struct *spiceStruct = getStruct(node);
  assert(spiceStruct != nullptr);
  return std::ranges::any_of(spiceStruct->interfaceTypes, [&](const Type &interfaceType) {
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
bool Type::isBase(SuperType superType) const {
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
bool Type::isSameContainerTypeAs(const Type &otherType) const {
  return (isPtr() && otherType.isPtr()) || (isRef() && otherType.isRef()) || (isArray() && otherType.isArray());
}

/**
 * Retrieve the base type of the current type
 *
 * @return Base type
 */
Type Type::getBase() const {
  assert(!typeChain.empty());
  return Type({typeChain.front()}, specifiers);
}

/**
 * Checks if the base type is generic itself or has generic parts in its template types
 *
 * @return Contains generic parts or not
 */
bool Type::hasAnyGenericParts() const { // NOLINT(misc-no-recursion)
  const Type &baseType = getBase();

  // Check if the type itself is generic
  if (baseType.is(TY_GENERIC))
    return true;

  // Check if the type has generic template types
  const auto templateTypes = baseType.getTemplateTypes();
  if (std::ranges::any_of(templateTypes, [](const Type &t) { return t.hasAnyGenericParts(); }))
    return true;

  // Check param and return types or functions/procedures
  if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const auto paramTypes = baseType.getFunctionParamAndReturnTypes();
    if (std::ranges::any_of(paramTypes, [](const Type &t) { return t.hasAnyGenericParts(); }))
      return true;
  }

  return false; // Does not have generic parts
}

/**
 * Set the list of templates types
 */
void Type::setTemplateTypes(const std::vector<QualType> &templateTypes) {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  typeChain.back().templateTypes = templateTypes;
}

/**
 * Set the list of templates types of the base type
 */
void Type::setBaseTemplateTypes(const std::vector<QualType> &templateTypes) {
  assert(getBase().isOneOf({TY_STRUCT, TY_INTERFACE}));
  typeChain.front().templateTypes = templateTypes;
}

/**
 * Retrieve template types of the current type
 *
 * @return Vector of template types
 */
const std::vector<QualType> &Type::getTemplateTypes() const { return typeChain.back().templateTypes; }

/**
 * Check if the given generic type list has a substantiation for the current (generic) type
 *
 * @param genericTypeList Generic type list
 * @return Has substantiation or not
 */
bool Type::isCoveredByGenericTypeList(std::vector<GenericType> &genericTypeList) const {
  const Type baseType = getBase();
  // Check if the symbol type itself is generic
  if (baseType.is(TY_GENERIC)) {
    return std::ranges::any_of(genericTypeList, [&](GenericType &t) {
      if (baseType.matches(t.getType(), true, true, true)) {
        t.used = true;
        return true;
      }
      return false;
    });
  }

  // If the type is non-generic check template types
  bool covered = true;
  // Check template types
  const std::vector<QualType> &baseTemplateTypes = baseType.getTemplateTypes();
  auto lambda = [&](const QualType &templateType) { return templateType.getType().isCoveredByGenericTypeList(genericTypeList); };
  covered &= std::ranges::all_of(baseTemplateTypes, lambda);

  // If function/procedure, check param and return types
  if (baseType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const std::vector<QualType> &paramAndReturnTypes = baseType.getFunctionParamAndReturnTypes();
    covered &= std::ranges::all_of(paramAndReturnTypes,
                                   [&](const Type &paramType) { return paramType.isCoveredByGenericTypeList(genericTypeList); });
  }

  return covered;
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @param ignorePublic Ignore any potential public specifier
 * @return Symbol type name
 */
void Type::getName(std::stringstream &name, bool withSize, bool ignorePublic) const { // NOLINT(misc-no-recursion)
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
  for (const TypeChainElement &chainElement : typeChain)
    name << chainElement.getName(withSize);
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @param ignorePublic Ignore any potential public specifier
 * @return Symbol type name
 */
std::string Type::getName(bool withSize, bool ignorePublic) const {
  std::stringstream name;
  getName(name, withSize, ignorePublic);
  return name.str();
}

/**
 * Set the return type of a function type
 *
 * @param returnType Function return type
 */
void Type::setFunctionReturnType(const QualType &returnType) {
  assert(is(TY_FUNCTION));
  std::vector<QualType> &paramTypes = typeChain.back().paramTypes;
  if (paramTypes.empty())
    paramTypes.resize(1);
  paramTypes.at(0) = returnType;
}

/**
 * Get the return type of a function type
 *
 * @return Function return type
 */
const QualType &Type::getFunctionReturnType() const {
  assert(is(TY_FUNCTION));
  assert(!typeChain.back().paramTypes.empty());
  return typeChain.back().paramTypes.front();
}

/**
 * Set the param types of a function or procedure type
 *
 * @param paramTypes Function param types
 */
void Type::setFunctionParamTypes(const std::vector<QualType> &newParamTypes) {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  std::vector<QualType> &paramTypes = typeChain.back().paramTypes;
  // Resize param types if required
  if (paramTypes.size() < newParamTypes.size() + 1)
    paramTypes.resize(newParamTypes.size() + 1, QualType(TY_DYN));
  // Set the function param types
  for (size_t i = 0; i < newParamTypes.size(); i++)
    paramTypes.at(i + 1) = newParamTypes.at(i);
}

/**
 * Get the param types of a function or procedure type
 *
 * @return Function param types
 */
std::vector<QualType> Type::getFunctionParamTypes() const {
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
void Type::setHasLambdaCaptures(bool hasCaptures) {
  assert(getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  typeChain.front().data.hasCaptures = hasCaptures;
}

/**
 * Check if a function or procedure type has captures
 *
 * @return Has captures
 */
bool Type::hasLambdaCaptures() const {
  assert(getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().data.hasCaptures;
}

/**
 * Set the param and return types of a function or procedure base type
 *
 * @param newParamAndReturnTypes Function param and return types (first is return type, rest are param types)
 */
void Type::setFunctionParamAndReturnTypes(const std::vector<QualType> &newParamAndReturnTypes) {
  assert(getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  typeChain.front().paramTypes = newParamAndReturnTypes;
}

/**
 * Get the param and return types of a function or procedure base type
 *
 * @return Function param and return types (first is return type, rest are param types)
 */
const std::vector<QualType> &Type::getFunctionParamAndReturnTypes() const {
  assert(getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().paramTypes;
}

/**
 * Get the struct instance for a struct type
 *
 * @param node Accessing AST node
 * @return Struct instance
 */
Struct *Type::getStruct(const ASTNode *node) const {
  assert(is(TY_STRUCT));
  Scope *structDefScope = getBodyScope()->parent;
  const std::string structName = getSubType();
  const std::vector<QualType> &templateTypes = getTemplateTypes();
  return StructManager::matchStruct(structDefScope, structName, templateTypes, node);
}

/**
 * Get the interface instance for an interface type
 *
 * @param node Accessing AST node
 * @return Interface instance
 */
Interface *Type::getInterface(const ASTNode *node) const {
  assert(is(TY_INTERFACE));
  Scope *interfaceDefScope = getBodyScope()->parent;
  const std::string structName = getSubType();
  const std::vector<QualType> &templateTypes = getTemplateTypes();
  return InterfaceManager::matchInterface(interfaceDefScope, structName, templateTypes, node);
}

bool operator==(const Type &lhs, const Type &rhs) { return lhs.typeChain == rhs.typeChain && lhs.specifiers == rhs.specifiers; }

bool operator!=(const Type &lhs, const Type &rhs) { return !(lhs == rhs); }

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
bool Type::matches(const Type &otherType, bool ignoreArraySize, bool ignoreSpecifiers, bool allowConstify) const {
  // If the size does not match, it is not equal
  if (typeChain.size() != otherType.typeChain.size())
    return false;

  // Compare the elements
  for (size_t i = 0; i < typeChain.size(); i++) {
    const Type::TypeChainElement &lhsElement = typeChain.at(i);
    const Type::TypeChainElement &rhsElement = otherType.typeChain.at(i);

    // Ignore differences in array size
    if (ignoreArraySize && lhsElement.superType == TY_ARRAY && rhsElement.superType == TY_ARRAY)
      continue;

    // Not both types are arrays -> compare them as usual
    if (lhsElement != rhsElement)
      return false;
  }

  // Ignore or compare specifiers
  return ignoreSpecifiers || specifiers.match(otherType.specifiers, allowConstify);
}

/**
 * Check for the matching compatibility of two types in terms of interface implementation.
 * Useful for function matching as well as assignment type validation and function arg matching.
 *
 * @param otherType Type to compare against
 * @return Matching or not
 */
bool Type::matchesInterfaceImplementedByStruct(const Type &otherType) const {
  if (!is(TY_INTERFACE) || !otherType.is(TY_STRUCT))
    return false;

  // Check if the rhs is a struct type that implements the lhs interface type
  const Struct *spiceStruct = otherType.getStruct(nullptr);
  assert(spiceStruct != nullptr);
  const auto pred = [&](const Type &interfaceType) { return matches(interfaceType, false, false, true); };
  return std::ranges::any_of(spiceStruct->interfaceTypes, pred);
}

/**
 * Check if a certain input type can be bound (assigned) to the current type.
 *
 * @param inputType Type, which should be bound to the current type
 * @param isTemporary Is the input type a temporary type
 * @return Can be bound or not
 */
bool Type::canBind(const Type &inputType, bool isTemporary) const {
  return !isTemporary || inputType.isRef() || !isRef() || isConstRef();
}

/**
 * Remove pointers / arrays / references if both types have them as far as possible.
 * Furthermore, remove reference wrappers if possible.
 *
 * @param typeA Candidate type
 * @param typeB Requested type
 */
void Type::unwrapBoth(Type &typeA, Type &typeB) {
  // Remove reference wrapper of front type if required
  if (typeA.isRef() && !typeB.isRef())
    typeA = typeA.removeReferenceWrapper();

  // Remove reference wrapper of requested type if required
  if (!typeA.isRef() && typeB.isRef() && !typeA.getBase().is(TY_GENERIC))
    typeB = typeB.removeReferenceWrapper();

  // Unwrap both types as far as possible
  while (typeA.isSameContainerTypeAs(typeB)) {
    typeB = typeB.getContainedTy();
    typeA = typeA.getContainedTy();
  }
}

} // namespace spice::compiler