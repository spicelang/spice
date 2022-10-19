// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Function.h"

#include <ast/AstNodes.h>
#include <util/CommonUtil.h>

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

  // Param type string
  std::string paramTyStr;
  for (const Param &param : paramList) {
    if (!paramTyStr.empty())
      paramTyStr += "_";
    paramTyStr += param.type.getName(false, true);
    if (param.isOptional)
      paramTyStr += "?";
  }

  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : templateTypes) {
    if (!templateTyStr.empty())
      templateTyStr += "_";
    templateTyStr += templateType.getName(false, true);
  }

  // Construct mangled name
  std::string mangledName = "_" + functionTyStr + "__" + thisTyStr + "__" + returnTyStr;
  if (!templateTyStr.empty())
    mangledName += "__" + templateTyStr;
  mangledName += "__" + name;
  if (!paramTyStr.empty())
    mangledName += "__" + paramTyStr;
  return mangledName;
}

/**
 * Get a string representation of the current function
 *
 * @return String representation as function signature
 */
std::string Function::getSignature() const {
  std::string thisTyStr;
  if (!thisType.is(TY_DYN))
    thisTyStr = thisType.getBaseType().getSubType() + ".";

  // Return type string
  std::string returnTyStr;
  if (!returnType.is(TY_DYN))
    returnTyStr = ": " + returnType.getName();

  // Parameter type string
  std::string paramTyStr;
  for (const Param &param : paramList) {
    if (!paramTyStr.empty())
      paramTyStr += ",";
    paramTyStr += param.type.getName();
    if (param.isOptional)
      paramTyStr += "?";
  }

  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : templateTypes) {
    if (!templateTyStr.empty())
      templateTyStr += ",";
    templateTyStr += templateType.getName();
  }
  if (!templateTyStr.empty())
    templateTyStr = "<" + templateTyStr + ">";

  return thisTyStr + name + templateTyStr + "(" + paramTyStr + ")" + returnTyStr;
}

/**
 * Check if the current function is a function
 *
 * @return Function or not
 */
bool Function::isFunction() const { return !returnType.is(TY_DYN) && thisType.is(TY_DYN); }

/**
 * Check if the current function is a procedure
 *
 * @return Procedure or not
 */
bool Function::isProcedure() const { return returnType.is(TY_DYN) && thisType.is(TY_DYN); }

/**
 * Check if the current function is a method function
 *
 * @return Method function or not
 */
bool Function::isMethodFunction() const { return !returnType.is(TY_DYN) && !thisType.is(TY_DYN); }

/**
 * Check if the current function is a method procedure
 *
 * @return Method procedure or not
 */
bool Function::isMethodProcedure() const { return returnType.is(TY_DYN) && !thisType.is(TY_DYN); }

/**
 * Get symbol type. Possible super types are TY_FUNCTION and TY_PROCEDURE
 *
 * @return Symbol representing the function
 */
SymbolType Function::getSymbolType() const { return SymbolType(isFunction() || isMethodFunction() ? TY_FUNCTION : TY_PROCEDURE); }

/**
 * Convert the current ambiguous function with potential optional parameters to a vector of
 * definite functions without optional parameters
 *
 * @return List of definite functions
 */
std::vector<Function> Function::substantiateOptionalParams() const {
  std::vector<Function> definiteFunctions;
  ParamList currentFunctionParamTypes;
  bool metFirstOptionalParam = false;

  for (const Param &param : paramList) {
    if (param.isOptional) {         // Met optional parameter
      if (!metFirstOptionalParam) { // Add substantiation without the optional parameter
        definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionParamTypes, templateTypes,
                                       declNode);
        metFirstOptionalParam = true;
      }
      // Add substantiation with the optional parameter
      currentFunctionParamTypes.push_back({param.type, false});
      definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionParamTypes, templateTypes, declNode);
    } else { // Met mandatory parameter
      currentFunctionParamTypes.push_back({param.type, false});
    }
  }

  if (definiteFunctions.empty())
    definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionParamTypes, templateTypes, declNode);

  return definiteFunctions;
}

/**
 * Convert the current ambiguous function with potential generic types to a definite function without generic types
 *
 * @return Substantiated function with concrete param types and without template types
 */
Function Function::substantiateGenerics(const ParamList &concreteParamList, const SymbolType &concreteThisType,
                                        const std::map<std::string, SymbolType> &concreteGenericTypes) const {
  // Substantiate return type
  SymbolType newReturnType = returnType.is(TY_GENERIC) ? concreteGenericTypes.at(returnType.getSubType()) : returnType;

  Function substantiatedFunction(name, specifiers, concreteThisType, newReturnType, concreteParamList, {}, declNode);
  substantiatedFunction.isGenericSubstantiation = true;
  return substantiatedFunction;
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