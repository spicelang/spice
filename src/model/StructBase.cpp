// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "StructBase.h"

#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <typechecker/TypeMatcher.h>
#include <util/CommonUtil.h>

namespace spice::compiler {

/**
 * Get a string representation of the current struct
 *
 * @return String representation as struct signature
 */
std::string StructBase::getSignature() const {
  std::vector<QualType> templateSymbolTypes;
  templateSymbolTypes.reserve(templateTypes.size());
  for (const GenericType &genericType : templateTypes) {
    if (genericType.is(TY_GENERIC) && !typeMapping.empty()) {
      assert(typeMapping.contains(genericType.getSubType()));
      templateSymbolTypes.push_back(typeMapping.at(genericType.getSubType()));
    } else {
      templateSymbolTypes.push_back(genericType);
    }
  }

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
std::string StructBase::getSignature(const std::string &name, const std::vector<QualType> &concreteTemplateTypes) {
  // Build template type string
  std::stringstream templateTyStr;
  if (!concreteTemplateTypes.empty()) {
    templateTyStr << "<";
    for (size_t i = 0; i < concreteTemplateTypes.size(); i++) {
      if (i > 0)
        templateTyStr << ",";
      templateTyStr << concreteTemplateTypes.at(i).getName(false, true);
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
bool StructBase::hasSubstantiatedGenerics() const {
  return std::ranges::none_of(templateTypes, [](const GenericType &genericType) { return genericType.hasAnyGenericParts(); });
}

/**
 * Checks if a struct has generic types present.
 * This would imply that the struct is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool StructBase::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Retrieve the template types as vector of symbol types
 *
 * @return Template types as vector of symbol types
 */
std::vector<QualType> StructBase::getTemplateTypes() const {
  std::vector<QualType> templateSymbolTypes;
  for (const GenericType &genericTemplateType : templateTypes)
    templateSymbolTypes.push_back(genericTemplateType);
  return templateSymbolTypes;
}

/**
 * Retrieve the declaration code location of this struct
 *
 * @return Declaration code location
 */
const CodeLoc &StructBase::getDeclCodeLoc() const { return declNode->codeLoc; }

/**
 * Returns, if this struct is a substantiation of a generic one.
 *
 * @return Generic substantiation or not
 */
bool StructBase::isGenericSubstantiation() const { return genericPreset != nullptr; }

} // namespace spice::compiler