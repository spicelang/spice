// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Function.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

/**
 * Retrieve the parameter types of the current function
 *
 * @return Vector of parameter types
 */
std::vector<SymbolType> Function::getParamTypes() const {
  std::vector<SymbolType> newParamTypes;
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
 * @return String representation as function signature
 */
std::string Function::getSignature(bool withThisType /*=true*/) const {
  std::vector<SymbolType> templateSymbolTypes;
  templateSymbolTypes.reserve(templateTypes.size());
  for (const GenericType &genericType : templateTypes)
    templateSymbolTypes.push_back(genericType);

  return Function::getSignature(name, thisType, returnType, paramList, templateSymbolTypes, withThisType);
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
 * @return Function signature
 */
std::string Function::getSignature(const std::string &name, const SymbolType &thisType, const SymbolType &returnType,
                                   const ParamList &paramList, const std::vector<SymbolType> &concreteTemplateTypes,
                                   bool withThisType /*=true*/) {
  // Build this type string
  std::stringstream thisTyStr;
  if (withThisType && !thisType.is(TY_DYN)) {
    thisTyStr << thisType.getBaseType().getSubType();
    const std::vector<SymbolType> &thisTemplateTypes = thisType.getTemplateTypes();
    if (!thisTemplateTypes.empty()) {
      thisTyStr << "<";
      for (size_t i = 0; i < thisTemplateTypes.size(); i++) {
        if (i > 0)
          thisTyStr << ",";
        thisTyStr << thisTemplateTypes.at(i).getName();
      }
      thisTyStr << ">";
    }
    thisTyStr << MEMBER_ACCESS_TOKEN;
  }

  // Build return type string
  std::string returnTyStr;
  if (!returnType.is(TY_DYN))
    returnTyStr = returnType.getName() + " ";

  // Parameter type string
  std::stringstream paramTyStr;
  for (size_t i = 0; i < paramList.size(); i++) {
    const Param &param = paramList.at(i);
    if (i > 0)
      paramTyStr << ",";
    paramTyStr << param.type.getName();
    if (param.isOptional)
      paramTyStr << "?";
  }

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

  return returnTyStr + thisTyStr.str() + name + templateTyStr.str() + "(" + paramTyStr.str() + ")";
}

/**
 * Checks if a function contains optional parameters.
 * This would imply that the function is not substantiated by its optional parameters yet.
 *
 * @return Substantiated params or not
 */
bool Function::hasSubstantiatedParams() const {
  return std::none_of(paramList.begin(), paramList.end(), [](auto t) { return t.isOptional; });
}

/**
 * Checks if a function contains template types.
 * This would imply that the function is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Function::hasSubstantiatedGenerics() const {
  for (const SymbolType &templateType : thisType.getTemplateTypes())
    if (templateType.hasAnyGenericParts())
      return false;
  return templateTypes.empty() && !thisType.hasAnyGenericParts() && !returnType.hasAnyGenericParts();
}

/**
 * Checks if a function contains optional parameters or has generic types present.
 * This would imply that the function is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Function::isFullySubstantiated() const { return hasSubstantiatedParams() && hasSubstantiatedGenerics(); }

/**
 * Retrieve the declaration code location of this function
 *
 * @return Declaration code location
 */
const CodeLoc &Function::getDeclCodeLoc() const { return declNode->codeLoc; }

} // namespace spice::compiler