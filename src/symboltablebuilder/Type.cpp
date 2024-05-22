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

#include <utility>

namespace spice::compiler {

Type::Type(SuperType superType) : typeChain({TypeChainElement{superType}}) {}

Type::Type(SuperType superType, const std::string &subType) : typeChain({TypeChainElement{superType, subType}}) {}

Type::Type(SuperType superType, const std::string &subType, uint64_t typeId, const TypeChainElementData &data,
           const QualTypeList &templateTypes)
    : typeChain({TypeChainElement(superType, subType, typeId, data, templateTypes)}) {}

Type::Type(TypeChain typeChain) : typeChain(std::move(typeChain)) {}

/**
 * Get the super type of the current type
 *
 * @return Super type
 */
SuperType Type::getSuperType() const {
  assert(!typeChain.empty());
  return typeChain.back().superType;
}

/**
 * Get the sub type of the current type
 *
 * @return Sub type
 */
const std::string &Type::getSubType() const {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM, TY_GENERIC}));
  return typeChain.back().subType;
}

/**
 * Get the array size of the current type
 *
 * @return Array size
 */
unsigned int Type::getArraySize() const {
  assert(getSuperType() == TY_ARRAY);
  return typeChain.back().data.arraySize;
}

/**
 * Get the body scope of the current type
 *
 * @return Body scope
 */
Scope *Type::getBodyScope() const {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  return typeChain.back().data.bodyScope;
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

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_PTR);

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
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

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_REF);

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
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

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.emplace_back(TY_ARRAY, TypeChainElementData{.arraySize = size});

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
const Type *Type::getContained() const {
  if (is(TY_STRING))
    return TypeRegistry::getOrInsert(TY_CHAR);

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  assert(newTypeChain.size() > 1);
  newTypeChain.pop_back();

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Replace the base type with another one
 *
 * @param newBaseType New base type
 * @return The new type
 */
const Type *Type::replaceBase(const Type *newBaseType) const {
  assert(!typeChain.empty());

  // Create new type
  TypeChain newTypeChain = newBaseType->typeChain;
  const bool doubleRef = newTypeChain.back().superType == TY_REF && typeChain.back().superType == TY_REF;
  for (size_t i = 1; i < typeChain.size(); i++)
    if (!doubleRef || i > 1)
      newTypeChain.push_back(typeChain.at(i));

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Remove reference wrapper from the current type
 *
 * @return Type without reference wrapper
 */
const Type *Type::removeReferenceWrapper() const { return isRef() ? getContained() : this; }

/**
 * Return the LLVM type for this symbol type
 *
 * @param context LLVM context
 * @return Corresponding LLVM type
 */
llvm::Type *Type::toLLVMType(SourceFile *sourceFile) const { // NOLINT(misc-no-recursion)
  assert(!typeChain.empty() && !is(TY_INVALID));
  llvm::LLVMContext &context = sourceFile->llvmModule->getContext();

  if (is(TY_DOUBLE))
    return llvm::Type::getDoubleTy(context);

  if (isOneOf({TY_INT, TY_ENUM}))
    return llvm::Type::getInt32Ty(context);

  if (is(TY_SHORT))
    return llvm::Type::getInt16Ty(context);

  if (is(TY_LONG))
    return llvm::Type::getInt64Ty(context);

  if (isOneOf({TY_CHAR, TY_BYTE}))
    return llvm::Type::getInt8Ty(context);

  if (is(TY_BOOL))
    return llvm::Type::getInt1Ty(context);

  if (isOneOf({TY_STRUCT, TY_INTERFACE})) {
    Scope *structBodyScope = getBodyScope();
    const std::string structSignature = Struct::getSignature(getSubType(), getTemplateTypes());
    SymbolTableEntry *structSymbol = structBodyScope->parent->lookupStrict(structSignature);
    assert(structSymbol != nullptr);

    // Collect concrete field types
    llvm::StructType *structType;
    std::vector<llvm::Type *> fieldTypes;
    bool isPacked = false;
    if (is(TY_STRUCT)) { // Struct
      Struct *spiceStruct = structSymbol->getQualType().getStruct(structSymbol->declNode);
      assert(spiceStruct != nullptr);
      const std::string mangledName = NameMangling::mangleStruct(*spiceStruct);
      structType = llvm::StructType::create(context, mangledName);

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
        fieldTypes.push_back(sourceFile->getLLVMType(fieldSymbol->getQualType().getType()));
      }

      // Check if the struct is declared as packed
      if (structDeclNode->attrs() && structDeclNode->attrs()->attrLst()->hasAttr(ATTR_CORE_COMPILER_PACKED))
        isPacked = structDeclNode->attrs()->attrLst()->getAttrValueByName(ATTR_CORE_COMPILER_PACKED)->boolValue;
    } else { // Interface
      Interface *spiceInterface = structSymbol->getQualType().getInterface(structSymbol->declNode);
      assert(spiceInterface != nullptr);
      const std::string mangledName = NameMangling::mangleInterface(*spiceInterface);
      structType = llvm::StructType::create(context, mangledName);

      fieldTypes.push_back(llvm::PointerType::get(context, 0));
    }

    // Set field types to struct type
    structType->setBody(fieldTypes, isPacked);

    return structType;
  }

  if (isOneOf({TY_PTR, TY_REF, TY_STRING}) || (isArray() && getArraySize() == 0))
    return llvm::PointerType::get(context, 0);

  if (isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    llvm::PointerType *ptrTy = llvm::PointerType::get(context, 0);
    return llvm::StructType::get(context, {ptrTy, ptrTy});
  }

  if (isArray()) {
    assert(getArraySize() > 0);
    llvm::Type *containedType = sourceFile->getLLVMType(getContained());
    return llvm::ArrayType::get(containedType, getArraySize());
  }

  throw CompilerError(UNHANDLED_BRANCH, "Cannot determine LLVM type of " + getName(true)); // GCOVR_EXCL_LINE
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
 * Check if the current type is a primitive type
 *
 * @return Primitive type or not
 */
bool Type::isPrimitive() const { return isOneOf({TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL}); }

/**
 * Check if the current type is a pointer type
 *
 * @return Pointer type or not
 */
bool Type::isPtr() const { return getSuperType() == TY_PTR; }

/**
 * Check if the current type is a reference type
 *
 * @return Reference type or not
 */
bool Type::isRef() const { return getSuperType() == TY_REF; }

/**
 * Check if the current type is an array type
 *
 * @return Array type or not
 */
bool Type::isArray() const { return getSuperType() == TY_ARRAY; }

/**
 * Check if the current type is of the same container type like the other type.
 * Only TY_PTR, TY_REF and TY_ARRAY are considered as container types.
 *
 * @param other Other symbol type
 * @return Same container type or not
 */
bool Type::isSameContainerTypeAs(const Type *other) const {
  const bool bothPtr = isPtr() && other->isPtr();
  const bool bothRef = isRef() && other->isRef();
  const bool bothArray = isArray() && other->isArray();
  return bothPtr || bothRef || bothArray;
}

/**
 * Retrieve the base type of the current type
 *
 * @return Base type
 */
const Type *Type::getBase() const {
  assert(!typeChain.empty());

  // Create new type chain
  const TypeChain newTypeChain = {typeChain.front()};

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Retrieve the same type, but with lambda captures
 *
 * @return Type with lambda captures
 */
const Type *Type::getWithLambdaCaptures(bool enabled) const {
  assert(getBase()->isOneOf({TY_FUNCTION, TY_PROCEDURE}));

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.front().data.hasCaptures = enabled;

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Retrieve the same type, but with the body scope removed
 *
 * @return Type with body scope removed
 */
const Type *Type::getWithBodyScope(Scope *bodyScope) const {
  assert(getBase()->isOneOf({TY_STRUCT, TY_INTERFACE}));

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.front().data.bodyScope = bodyScope;

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Retrieve the same type, but with the given template types
 *
 * @return Type with new template types
 */
const Type *Type::getWithTemplateTypes(const QualTypeList &templateTypes) const {
  assert(isOneOf({TY_STRUCT, TY_INTERFACE}));
  return getWithBaseTemplateTypes(templateTypes);
}

/**
 * Retrieve the same type, but with the given base template types
 *
 * @return Type with new base template types
 */
const Type *Type::getWithBaseTemplateTypes(const QualTypeList &templateTypes) const {
  assert(getBase()->isOneOf({TY_STRUCT, TY_INTERFACE}));

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.front().templateTypes = templateTypes;

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Retrieve the same type, but with the param and return types removed
 *
 * @return Type with param and return types removed
 */
const Type *Type::getWithFunctionParamAndReturnTypes(const QualTypeList &paramAndReturnTypes) const {
  assert(getBase()->isOneOf({TY_FUNCTION, TY_PROCEDURE}));

  // Create new type chain
  TypeChain newTypeChain = typeChain;
  newTypeChain.front().paramTypes = paramAndReturnTypes;

  // Register new type or return if already registered
  return TypeRegistry::getOrInsert(newTypeChain);
}

/**
 * Checks if the base type is generic itself or has generic parts in its template types
 *
 * @return Contains generic parts or not
 */
bool Type::hasAnyGenericParts() const { // NOLINT(misc-no-recursion)
  const Type *baseType = getBase();

  // Check if the type itself is generic
  if (baseType->is(TY_GENERIC))
    return true;

  // Check if the type has generic template types
  const auto templateTypes = baseType->getTemplateTypes();
  if (std::ranges::any_of(templateTypes, [](const QualType &t) { return t.hasAnyGenericParts(); }))
    return true;

  // Check param and return types or functions/procedures
  if (baseType->isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    const auto paramTypes = baseType->getFunctionParamAndReturnTypes();
    if (std::ranges::any_of(paramTypes, [](const QualType &t) { return t.hasAnyGenericParts(); }))
      return true;
  }

  return false; // Does not have generic parts
}

/**
 * Retrieve template types of the current type
 *
 * @return Vector of template types
 */
const QualTypeList &Type::getTemplateTypes() const { return typeChain.back().templateTypes; }

/**
 * Check if the current type is of a certain super type
 *
 * @return Applicable or not
 */
bool Type::is(SuperType superType) const { return getSuperType() == superType; }

/**
 * Check if the current type is one of a list of super types
 *
 * @return Applicable or not
 */
bool Type::isOneOf(const std::initializer_list<SuperType> &superTypes) const {
  return std::ranges::any_of(superTypes, [this](SuperType superType) { return is(superType); });
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
void Type::getName(std::stringstream &name, bool withSize) const { // NOLINT(misc-no-recursion)
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
std::string Type::getName(bool withSize) const {
  std::stringstream name;
  getName(name, withSize);
  return name.str();
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
 * Get the param types of a function or procedure type
 *
 * @return Function param types
 */
QualTypeList Type::getFunctionParamTypes() const {
  assert(isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  if (typeChain.back().paramTypes.empty())
    return {};
  return {typeChain.back().paramTypes.begin() + 1, typeChain.back().paramTypes.end()};
}

/**
 * Check if a function or procedure type has captures
 *
 * @return Has captures
 */
bool Type::hasLambdaCaptures() const {
  assert(getBase()->isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().data.hasCaptures;
}

/**
 * Get the param and return types of a function or procedure base type
 *
 * @return Function param and return types (first is return type, rest are param types)
 */
const QualTypeList &Type::getFunctionParamAndReturnTypes() const {
  assert(getBase()->isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  return typeChain.front().paramTypes;
}

/**
 * Check for the matching compatibility of two types.
 * Useful for struct and function matching as well as assignment type validation and function arg matching.
 *
 * @param otherType Type to compare against
 * @param ignoreArraySize Ignore array sizes
 * @return Matching or not
 */
bool Type::matches(const Type *otherType, bool ignoreArraySize) const {
  // If the size does not match, it is not equal
  if (typeChain.size() != otherType->typeChain.size())
    return false;

  // Compare the elements
  for (size_t i = 0; i < typeChain.size(); i++) {
    const TypeChainElement &lhsElement = typeChain.at(i);
    const TypeChainElement &rhsElement = otherType->typeChain.at(i);

    // Ignore differences in array size
    if (ignoreArraySize && lhsElement.superType == TY_ARRAY && rhsElement.superType == TY_ARRAY)
      continue;

    // Not both types are arrays -> compare them as usual
    if (lhsElement != rhsElement)
      return false;
  }

  // Ignore or compare specifiers
  return true;
}

/**
 * Remove pointers / arrays / references if both types have them as far as possible.
 * Furthermore, remove reference wrappers if possible.
 *
 * @param typeA Candidate type
 * @param typeB Requested type
 */
void Type::unwrapBoth(const Type *&typeA, const Type *&typeB) {
  // Remove reference wrapper of front type if required
  if (typeA->isRef() && !typeB->isRef())
    typeA = typeA->removeReferenceWrapper();

  // Remove reference wrapper of requested type if required
  if (!typeA->isRef() && typeB->isRef() && !typeA->getBase()->is(TY_GENERIC))
    typeB = typeB->removeReferenceWrapper();

  // Unwrap both types as far as possible
  while (typeA->isSameContainerTypeAs(typeB)) {
    typeB = typeB->getContained();
    typeA = typeA->getContained();
  }
}

/**
 * Check if two types have the same type chain depth
 *
 * @param typeA First type
 * @param typeB Second type
 * @return Same depth or not
 */
bool Type::hasSameTypeChainDepth(const Type *typeA, const Type *typeB) {
  return typeA->typeChain.size() == typeB->typeChain.size();
}

} // namespace spice::compiler