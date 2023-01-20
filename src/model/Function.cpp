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
 * Mange the function and return the mangled string
 *
 * @return Mangled string
 */
std::string Function::getMangledName() const {
  // Return 'main' if name is 'main'
  if (name == "main")
    return name;

  // f, p, mf or mp depending on the function type
  std::string functionTyStr = "f";
  if (isProcedure()) {
    functionTyStr = "p";
  } else if (isMethodFunction()) {
    functionTyStr = "mf";
  } else if (isMethodProcedure()) {
    functionTyStr = "mp";
  }

  // This type string
  std::string thisTyStr = "void";
  if (!thisType.is(TY_DYN)) {
    std::string baseSubType = thisType.getBaseType().getSubType();
    size_t startPos = baseSubType.find_last_of('.');
    startPos = startPos == std::string::npos ? 0 : startPos + 1;
    thisTyStr = baseSubType.substr(startPos);
    for (const auto &templateType : thisType.getTemplateTypes())
      thisTyStr += "_" + templateType.getName(false, true);
  }

  // Return type string
  std::string returnTyStr = "void";
  if (!returnType.is(TY_DYN))
    returnTyStr = returnType.getName(false, true);

  // Template type string
  std::stringstream templateTyStr;
  for (size_t i = 0; i < templateTypes.size(); i++) {
    if (i != 0)
      templateTyStr << "_";
    templateTyStr << templateTypes.at(i).getName(false, true);
  }

  // Param type string
  std::stringstream paramTyStr;
  for (size_t i = 0; i < paramList.size(); i++) {
    const Param &param = paramList.at(i);
    if (i > 0)
      paramTyStr << "_";
    paramTyStr << param.type.getName(false, true);
    if (param.isOptional)
      paramTyStr << "?";
  }

  // Construct mangled name
  std::stringstream mangledName;
  mangledName << "_" << functionTyStr << "__" << thisTyStr << "__" << returnTyStr;
  if (templateTyStr.rdbuf()->in_avail() > 0)
    mangledName << "__" << templateTyStr.str();
  mangledName << "__" << name;
  if (paramTyStr.rdbuf()->in_avail() > 0)
    mangledName << "__" << paramTyStr.str();
  return mangledName.str();
}

/**
 * Get a string representation of the current function.
 *
 * Example:
 * string Vector<double>.setData<double>(double)
 *
 * @return String representation as function signature
 */
std::string Function::getSignature(bool withThisType /*=true*/) const {
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
  if (!templateTypes.empty()) {
    templateTyStr << "<";
    for (size_t i = 0; i < templateTypes.size(); i++) {
      if (i > 0)
        templateTyStr << ",";
      templateTyStr << templateTypes.at(i).getName();
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
    if (templateType.isBaseType(TY_GENERIC))
      return false;
  return templateTypes.empty() && !thisType.isBaseType(TY_GENERIC) && !returnType.isBaseType(TY_GENERIC);
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