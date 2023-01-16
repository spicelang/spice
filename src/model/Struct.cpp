// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/ASTBuilder.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

/**
 * Mange the struct and return the mangled string
 *
 * @return Mangled string
 */
std::string Struct::getMangledName() const {
  // Build field type string
  std::stringstream fieldTyStr;
  for (size_t i = 0; i < fieldTypes.size(); i++) {
    if (i > 0)
      fieldTyStr << "_";
    fieldTyStr << fieldTypes.at(i).getName(false, true);
  }

  // Build template type string
  std::stringstream templateTyStr;
  for (size_t i = 0; i < templateTypes.size(); i++) {
    if (i > 0)
      templateTyStr << "_";
    templateTyStr << templateTypes.at(i).getName(false, true);
  }

  // Construct mangled name
  std::stringstream mangledName("_s");
  if (!templateTypes.empty())
    mangledName << "__" << templateTyStr.str();
  mangledName << "__" << name;
  if (!fieldTypes.empty())
    mangledName << "__" << fieldTyStr.str();

  return mangledName.str();
}

/**
 * Get a string representation of the current struct
 *
 * @return String representation as struct signature
 */
std::string Struct::getSignature() const {
  std::vector<SymbolType> templateSymbolTypes;
  for (const auto &templateType : templateTypes)
    templateSymbolTypes.push_back(templateType);
  return getSignature(name, templateSymbolTypes);
}

/**
 * Checks if a struct contains template types.
 * This would imply that the struct is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Struct::hasSubstantiatedGenerics() const {
  return std::none_of(templateTypes.begin(), templateTypes.end(),
                      [](const GenericType &genericType) { return genericType.isBaseType(TY_GENERIC); });
}

/**
 * Checks if a struct has generic types present.
 * This would imply that the struct is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Struct::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Get the signature from the struct name and the concrete template types
 *
 * Example:
 * Pair<int,double>
 *
 * @param structName Struct name
 * @param concreteTemplateTypes Concrete template types
 * @return Signature
 */
std::string Struct::getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes) {
  // Build template type string
  std::stringstream templateTyStr;
  if (!concreteTemplateTypes.empty()) {
    templateTyStr << "<";
    for (size_t i = 0; i < concreteTemplateTypes.size(); i++) {
      if (i > 0)
        templateTyStr << ",";
      templateTyStr << concreteTemplateTypes.at(i).getName();
    }
    templateTyStr << ">";
  }

  return CommonUtil::getLastFragment(structName, SCOPE_ACCESS_TOKEN) + templateTyStr.str();
}

/**
 * Checks if the current struct is of infinite size.
 *
 * This can happen for structs with at least a:
 * 1) direct dependency (e.g. the struct A has a field with type A)
 * 2) indirect dependency (e.g. the struct A has a field with type B and struct B has a field with type A)
 *
 * @return Infinite size or not
 */
/*bool Struct::hasInfiniteSize(Scope *anchorScope) const { // NOLINT(misc-no-recursion)
  // Cancel recursion if we have reached the anchor again
  if (structScope == anchorScope)
    return true;

  // Set the recursion anchor scope
  if (!anchorScope)
    anchorScope = structScope;

  // Loop through all fields
  for (const SymbolType &fieldType : fieldTypes) {
    // Skip non-struct fields
    if (!fieldType.is(TY_STRUCT))
      continue;
    // Check for 1)
    if (fieldType.getStructBodyScope() == structScope)
      return true;
    // Check for 2)
    const std::string &structName = fieldType.getSubType();
    Scope *structParentScope = fieldType.getStructBodyScope()->parent;
    SymbolTableEntry *structEntry = structParentScope->lookupStrict(structName);
    StructManifestationList *structs = StructManager::getManifestationList(structParentScope, structEntry->getDeclCodeLoc());
    for (auto &[mangledName, spiceStruct] : *structs) {
      if (spiceStruct.hasInfiniteSize(anchorScope))
        return true;
    }
  }
  return false;
}*/

} // namespace spice::compiler