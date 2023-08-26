// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Interface.h"

namespace spice::compiler {

/**
 * Get a string representation of the current interface
 *
 * @return String representation as interface signature
 */
std::string Interface::getSignature() const {
  std::vector<SymbolType> templateSymbolTypes;
  for (const auto &templateType : templateTypes)
    templateSymbolTypes.push_back(templateType);
  return getSignature(name, templateSymbolTypes);
}

/**
 * Get the signature from the interface name and the concrete template types
 *
 * Example:
 * Comparable<int,int>
 *
 * @param name Interface name
 * @param concreteTemplateTypes Concrete template types
 * @return Signature
 */
std::string Interface::getSignature(const std::string &name, const std::vector<SymbolType> &concreteTemplateTypes) {
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
 * Checks if a interface contains template types.
 * This would imply that the interface is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Interface::hasSubstantiatedGenerics() const {
  return std::ranges::none_of(templateTypes, [](const GenericType &genericType) { return genericType.hasAnyGenericParts(); });
}

/**
 * Checks if a interface has generic types present.
 * This would imply that the interface is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Interface::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Retrieve the template types as vector of symbol types
 *
 * @return Template types as vector of symbol types
 */
std::vector<SymbolType> Interface::getTemplateTypes() const {
  std::vector<SymbolType> templateSymbolTypes;
  for (const GenericType &genericTemplateType : templateTypes)
    templateSymbolTypes.push_back(genericTemplateType);
  return templateSymbolTypes;
}

} // namespace spice::compiler