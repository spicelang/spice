// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/ASTBuilder.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

/**
 * Mangle the struct and return the mangled string
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
 * Get the signature from the struct name and the concrete template types
 *
 * Example:
 * Pair<int,double>
 *
 * @param name Struct name
 * @param concreteTemplateTypes Concrete template types
 * @return Signature
 */
std::string Struct::getSignature(const std::string &name, const std::vector<SymbolType> &concreteTemplateTypes) {
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

  return CommonUtil::getLastFragment(name, SCOPE_ACCESS_TOKEN) + templateTyStr.str();
}

/**
 * Checks if a struct contains template types.
 * This would imply that the struct is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Struct::hasSubstantiatedGenerics() const {
  return std::none_of(templateTypes.begin(), templateTypes.end(),
                      [](const GenericType &genericType) { return genericType.hasAnyGenericParts(); });
}

/**
 * Checks if a struct has generic types present.
 * This would imply that the struct is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Struct::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Retrieve the template types as vector of symbol types
 *
 * @return Template types as vector of symbol types
 */
std::vector<SymbolType> Struct::getTemplateTypes() const {
  std::vector<SymbolType> templateSymbolTypes;
  for (const GenericType &genericTemplateType : templateTypes)
    templateSymbolTypes.push_back(genericTemplateType);
  return templateSymbolTypes;
}

/**
 * Checks at least one field is a reference.
 * This is used to prohibit constant instantiations.
 *
 * @return Has reference as field type or not
 */
bool Struct::hasReferenceFields() const {
  return std::any_of(fieldTypes.begin(), fieldTypes.end(), [](const SymbolType &fieldType) { return fieldType.isRef(); });
}

} // namespace spice::compiler