// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Function.h"

#include <utility>

/**
 * Retrieve the name of the current function
 *
 * @return Function name
 */
std::string Function::getName() const { return name; }

/**
 * Retrieve the symbol specifiers of the current function
 *
 * @return Symbol specifiers
 */
SymbolSpecifiers Function::getSpecifiers() const { return specifiers; }

/**
 * Retrieve the this type of the current function
 *
 * @return This type
 */
SymbolType Function::getThisType() const { return thisType; }

/**
 * Retrieve the return type of the current function
 *
 * @return Return type
 */
SymbolType Function::getReturnType() const { return returnType; }

/**
 * Retrieve the argument types of the current function
 *
 * @return Vector of argument types
 */
std::vector<SymbolType> Function::getArgTypes() const {
  std::vector<SymbolType> newArgTypes;
  for (const auto &argType : argTypes)
    newArgTypes.push_back(argType.first);
  return newArgTypes;
}

/**
 * Retrieve the template types of the current function
 *
 * @return Vector of template types
 */
std::vector<GenericType> Function::getTemplateTypes() const { return templateTypes; }

/**
 * Mange the function and return the mangled string
 *
 * @return Mangled string
 */
std::string Function::getMangledName() const {
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
  std::string thisTyStr;
  if (!thisType.is(TY_DYN))
    thisTyStr = "_" + thisType.getBaseType().getSubType();

  // Arg type string
  std::string argTyStr;
  for (const auto &argType : argTypes) {
    argTyStr += "_" + argType.first.getName();
    if (argType.second)
      argTyStr += "?";
  }

  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : templateTypes)
    templateTyStr += "_" + templateType.getName();

  return "_" + functionTyStr + thisTyStr + templateTyStr + "_" + name + argTyStr;
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

  // Argument type string
  std::string argTyStr;
  for (const auto &argType : argTypes) {
    if (!argTyStr.empty())
      argTyStr += ",";
    argTyStr += argType.first.getName();
    if (argType.second)
      argTyStr += "?";
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

  return thisTyStr + name + templateTyStr + "(" + argTyStr + ")" + returnTyStr;
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
 * Convert the current ambiguous function with potential optional arguments to a vector of
 * definite functions without optional arguments
 *
 * @return List of definite functions
 */
std::vector<Function> Function::substantiateOptionalArgs() const {
  std::vector<Function> definiteFunctions;
  std::vector<std::pair<SymbolType, bool>> currentFunctionArgTypes;
  bool metFirstOptionalArg = false;

  for (const auto &argType : argTypes) {
    if (argType.second) {         // Met optional argument
      if (!metFirstOptionalArg) { // Add substantiation without the optional argument
        definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes, templateTypes,
                                       definitionCodeLoc);
        metFirstOptionalArg = true;
      }
      // Add substantiation with the optional argument
      currentFunctionArgTypes.emplace_back(argType.first, false);
      definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes, templateTypes,
                                     definitionCodeLoc);
    } else { // Met mandatory argument
      currentFunctionArgTypes.emplace_back(argType.first, false);
    }
  }

  if (definiteFunctions.empty())
    definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes, templateTypes,
                                   definitionCodeLoc);

  return definiteFunctions;
}

/**
 * Convert the current ambiguous function with potential generic types to a definite function without generic types
 *
 * @return Substantiated function with concrete arg types and without template types
 */
Function Function::substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes,
                                        const SymbolType &concreteThisType) const {
  std::vector<std::pair<SymbolType, bool>> newArgTypes;

  // Substantiate arg types
  for (const auto &argType : argTypes) {
    assert(!argType.second); // Optional args need to be substantiated at this point
    SymbolType newArgType = argType.first;
    SymbolType newArgBaseType = newArgType.getBaseType();
    if (newArgBaseType.is(TY_GENERIC)) {               // We have to replace it only if it is a generic type
      for (int i = 0; i < templateTypes.size(); i++) { // Go through all template types and get the respective concrete type
        if (newArgBaseType == templateTypes[i]) {
          newArgType = newArgType.replaceBaseType(concreteTemplateTypes[i]); // Use the concrete type instead of the generic one
          break;
        }
      }
    }
    newArgTypes.emplace_back(newArgType, false);
  }

  // Substantiate return type
  SymbolType newReturnType = returnType;
  if (newReturnType.is(TY_GENERIC)) {
    for (int i = 0; i < templateTypes.size(); i++) { // Go through all template types and get the respective concrete type
      if (newReturnType == templateTypes[i]) {
        newReturnType = concreteTemplateTypes[i]; // Use the concrete type instead of the generic one
        break;
      }
    }
  }

  return Function(name, specifiers, concreteThisType, newReturnType, newArgTypes, {}, definitionCodeLoc);
}

/**
 * Checks if a function contains optional arguments.
 * This would imply that the function is not substantiated by its optional arguments yet.
 *
 * @return Substantiated args or not
 */
bool Function::hasSubstantiatedArgs() const {
  return std::none_of(argTypes.begin(), argTypes.end(), [](auto t) { return t.second; });
}

/**
 * Checks if a function contains template types.
 * This would imply that the function is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Function::hasSubstantiatedGenerics() const { return templateTypes.empty(); }

/**
 * Checks if a function contains optional arguments or has generic types present.
 * This would imply that the function is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Function::isFullySubstantiated() const { return hasSubstantiatedArgs() && hasSubstantiatedGenerics(); }

/**
 * Set the function to used. The compiler only generates IR if the function is used
 */
void Function::setUsed() { used = true; }

/**
 * Check if the function is used and the compiler needs to generate IR for it
 *
 * @return Used or not
 */
bool Function::isUsed() const { return used; }

/**
 * Retrieve the definition code loc of this function
 *
 * @return Definition code location
 */
const std::string &Function::getDefinitionCodeLoc() const { return definitionCodeLoc; }