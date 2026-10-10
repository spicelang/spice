// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "Union.h"

#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <model/GenericType.h>
#include <symboltablebuilder/Scope.h>
#include <util/CommonUtil.h>

namespace spice::compiler {

static constexpr auto UNION_SCOPE_PREFIX = "union:";

Union::Union(std::string name, SymbolTableEntry *entry, Scope *scope, QualTypeList fieldTypes,
             std::vector<GenericType> templateTypes, ASTNode *declNode)
    : name(std::move(name)), templateTypes(std::move(templateTypes)), entry(entry), scope(scope), declNode(declNode),
      fieldTypes(std::move(fieldTypes)) {}

/**
 * Get a string representation of the current union
 *
 * @return String representation as union signature
 */
std::string Union::getSignature() const { return getSignature(name, getConcreteTemplateTypes()); }

/**
 * Get the signature from the union name and the concrete template types
 *
 * Example:
 * Pair<int,double>
 *
 * @param name Union name
 * @param concreteTemplateTypes Concrete template types
 * @return Signature
 */
std::string Union::getSignature(const std::string &name, const QualTypeList &concreteTemplateTypes) {
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
 * Retrieve the name of the scope, where fields are placed. This is used to navigate to the scope of the union
 * from the parent scope.
 *
 * @return Name of the union scope
 */
std::string Union::getScopeName() const {
  const std::string &appendix = isGenericSubstantiation() ? getSignature() : name;
  return UNION_SCOPE_PREFIX + appendix;
}

/**
 * Retrieve the name of the scope, where fields are placed. This is used to navigate to the scope of the
 * union from the parent scope.
 *
 * @param name Union name
 * @param concreteTemplateTypes Concrete template types
 * @return Name of the union scope
 */
std::string Union::getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes) {
  return UNION_SCOPE_PREFIX + getSignature(name, concreteTemplateTypes);
}

/**
 * Checks if a union contains generic template types.
 * This would imply that the union is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Union::hasSubstantiatedGenerics() const {
  const auto pred = [](const GenericType &genericType) { return genericType.hasAnyGenericParts(); };
  return std::ranges::none_of(templateTypes, pred);
}

/**
 * Checks if a union has generic types present.
 * This would imply that the union is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Union::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Retrieve the concrete template types of this union. For generic substantiations, the generic types are replaced
 * by the concrete types from the type mapping.
 *
 * @return Concrete template types as vector of symbol types
 */
QualTypeList Union::getConcreteTemplateTypes() const {
  QualTypeList concreteTemplateTypes;
  concreteTemplateTypes.reserve(templateTypes.size());
  for (const GenericType &genericType : templateTypes) {
    if (genericType.is(TY_GENERIC) && !typeMapping.empty()) {
      assert(typeMapping.contains(genericType.getSubType()));
      concreteTemplateTypes.push_back(typeMapping.at(genericType.getSubType()));
    } else {
      concreteTemplateTypes.push_back(genericType);
    }
  }
  return concreteTemplateTypes;
}

/**
 * Retrieve the template types as vector of symbol types
 *
 * @return Template types as vector of symbol types
 */
QualTypeList Union::getTemplateTypes() const {
  QualTypeList templateSymbolTypes;
  for (const GenericType &genericTemplateType : templateTypes)
    templateSymbolTypes.push_back(genericTemplateType);
  return templateSymbolTypes;
}

/**
 * Retrieve the declaration code location of this union
 *
 * @return Declaration code location
 */
const CodeLoc &Union::getDeclCodeLoc() const { return declNode->codeLoc; }

/**
 * Returns, if this union is a substantiation of a generic one.
 *
 * @return Generic substantiation or not
 */
bool Union::isGenericSubstantiation() const { return genericPreset != nullptr; }

} // namespace spice::compiler
