// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Function.h"

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
 * Mange the function and return the mangled string
 *
 * @return Mangled string
 */
std::string Function::getMangledName() const {
  // f, p, mf or mp depending on the function type
  std::string fpm = "f";
  if (isProcedure()) {
    fpm = "p";
  } else if (isMethodFunction()) {
    fpm = "mf";
  } else if (isMethodProcedure()) {
    fpm = "mp";
  }

  // This type string
  std::string thisTyStr;
  if (!thisType.is(TY_DYN))
    thisTyStr = thisType.getBaseType().getSubType() + "_";

  // Arg type string
  std::string argTyStr;
  for (const auto &argType : argTypes) {
    argTyStr += "_" + argType.first.getName();
    if (argType.second)
      argTyStr += "?";
  }

  return "_" + fpm + "_" + thisTyStr + name + argTyStr;
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

  std::string returnTyStr;
  if (!returnType.is(TY_DYN))
    returnTyStr = ": " + returnType.getName();

  std::string argTyStr;
  for (int i = 0; i < argTypes.size(); i++) {
    if (i != 0)
      argTyStr += ",";
    argTyStr += argTypes[i].first.getName();
    if (argTypes[i].second)
      argTyStr += "?";
  }
  return thisTyStr + name + "(" + argTyStr + ")" + returnTyStr;
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
 * @return
 */
SymbolType Function::getSymbolType() const { return SymbolType(isFunction() || isMethodFunction() ? TY_FUNCTION : TY_PROCEDURE); }

/**
 * Convert the current ambiguous function with potential optional arguments to a vector of
 * definite functions without optional arguments
 *
 * @return List of definite functions
 */
std::vector<Function> Function::substantiate() const {
  std::vector<Function> definiteFunctions;
  std::vector<std::pair<SymbolType, bool>> currentFunctionArgTypes;
  bool metFirstOptionalArg = false;

  for (const auto &argType : argTypes) {
    if (argType.second) {         // Met optional argument
      if (!metFirstOptionalArg) { // Add substantiation without the optional argument
        definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes);
        metFirstOptionalArg = true;
      }
      // Add substantiation with the optional argument
      currentFunctionArgTypes.emplace_back(argType.first, false);
      definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes);
    } else { // Met mandatory argument
      currentFunctionArgTypes.emplace_back(argType.first, false);
    }
  }

  if (definiteFunctions.empty())
    definiteFunctions.emplace_back(name, specifiers, thisType, returnType, currentFunctionArgTypes);

  return definiteFunctions;
}

/**
 * Checks if a function contains optional arguments. This would imply that the function is not substantiated yet
 *
 * @return
 */
bool Function::isSubstantiated() const {
  for (const auto &argType : argTypes) {
    if (argType.second)
      return false;
  }
  return true;
}