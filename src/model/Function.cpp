// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "Function.h"

#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <irgenerator/NameMangling.h>

namespace spice::compiler {

/**
 * Retrieve the parameter types of the current function
 *
 * @return Vector of parameter types
 */
std::vector<QualType> Function::getParamTypes() const {
  std::vector<QualType> newParamTypes;
  for (const Param &param : paramList)
    newParamTypes.push_back(param.type);
  return newParamTypes;
}

/**
 * Get a string representation of the current function.
 *
 * Example:
 * string Vector<double>.setData<double>(double)
 *
 * @param withThisType Include 'this' type in signature
 * @param ignorePublic Not include public modifiers in signature
 * @return String representation as function signature
 */
std::string Function::getSignature(bool withThisType /*=true*/, bool ignorePublic /*=false*/) const {
  std::vector<Type> concreteTemplateTypes;
  concreteTemplateTypes.reserve(templateTypes.size());
  for (const GenericType &genericType : templateTypes) {
    if (genericType.is(TY_GENERIC) && !typeMapping.empty()) {
      assert(typeMapping.contains(genericType.getSubType()));
      concreteTemplateTypes.push_back(typeMapping.at(genericType.getSubType()));
    } else {
      concreteTemplateTypes.push_back(genericType);
    }
  }

  return Function::getSignature(name, thisType, returnType, paramList, concreteTemplateTypes, withThisType, ignorePublic);
}

/**
 * Get a string representation of the current function.
 *
 * @param name Function name
 * @param thisType This symbol type
 * @param returnType Return symbol type
 * @param paramList Param symbol types
 * @param concreteTemplateTypes Concrete template symbol types
 * @param withThisType Include 'this' type in signature
 * @param ignorePublic Not include public modifiers in signature
 * @return Function signature
 */
std::string Function::getSignature(const std::string &name, const Type &thisType, const Type &returnType,
                                   const ParamList &paramList, const std::vector<Type> &concreteTemplateTypes,
                                   bool withThisType /*=true*/, bool ignorePublic /*=false*/) {
  std::stringstream signature;

  // Build return type string
  if (!returnType.is(TY_DYN)) {
    returnType.getName(signature, false, ignorePublic);
    signature << " ";
  }

  // Build this type string
  if (withThisType && !thisType.is(TY_DYN)) {
    signature << thisType.getBaseType().getSubType();
    const std::vector<QualType> &thisTemplateTypes = thisType.getTemplateTypes();
    if (!thisTemplateTypes.empty()) {
      signature << "<";
      for (size_t i = 0; i < thisTemplateTypes.size(); i++) {
        if (i > 0)
          signature << ",";
        signature << thisTemplateTypes.at(i).getName(false, ignorePublic);
      }
      signature << ">";
    }
    signature << MEMBER_ACCESS_TOKEN;
  }

  // Name
  signature << name;

  // Build template type string
  if (!concreteTemplateTypes.empty()) {
    signature << "<";
    for (size_t i = 0; i < concreteTemplateTypes.size(); i++) {
      if (i > 0)
        signature << ",";
      signature << concreteTemplateTypes.at(i).getName(false, ignorePublic);
    }
    signature << ">";
  }

  // Parameter type string
  signature << "(";
  for (size_t i = 0; i < paramList.size(); i++) {
    const Param &param = paramList.at(i);
    if (i > 0)
      signature << ",";
    signature << param.type.getName(false, ignorePublic);
    if (param.isOptional)
      signature << "?";
  }
  signature << ")";

  return signature.str();
}

std::string Function::getMangledName() const {
  // Use predefined mangled name if available
  if (!predefinedMangledName.empty())
    return predefinedMangledName;
  // Use function name if mangling is disabled
  if (!mangleFunctionName)
    return name;
  // Use normal name mangling
  return NameMangling::mangleFunction(*this);
}

std::string Function::getSymbolTableEntryName(const std::string &functionName, const CodeLoc &codeLoc) {
  return functionName + ":" + codeLoc.toString();
}

/**
 * Checks if a function contains optional parameters.
 * This would imply that the function is not substantiated by its optional parameters yet.
 *
 * @return Substantiated params or not
 */
bool Function::hasSubstantiatedParams() const {
  return std::ranges::none_of(paramList, [](const Param &param) { return param.isOptional; });
}

/**
 * Checks if a function contains template types.
 * This would imply that the function is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Function::hasSubstantiatedGenerics() const {
  const auto predicate = [this](const GenericType &genericType) { return typeMapping.contains(genericType.getSubType()); };
  return std::ranges::all_of(templateTypes, predicate);
}

/**
 * Checks if a function contains optional parameters or has generic types present.
 * This would imply that the function is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Function::isFullySubstantiated() const { return hasSubstantiatedParams() && hasSubstantiatedGenerics(); }

/**
 * Returns, if this function is a substantiation of a generic one.
 *
 * @return Generic substantiation or not
 */
bool Function::isGenericSubstantiation() const { return genericPreset != nullptr; }

/**
 * Retrieve the declaration code location of this function
 *
 * @return Declaration code location
 */
const CodeLoc &Function::getDeclCodeLoc() const { return declNode->codeLoc; }

} // namespace spice::compiler