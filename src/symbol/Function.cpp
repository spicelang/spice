// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Function.h"

std::string Function::getName() { return name; }

/**
 * Retrieve the this type of the current function
 *
 * @return This type
 */
SymbolType *Function::getThisType() { return thisType; }

/**
 * Retrieve the return type of the current function
 *
 * @return Return type
 */
SymbolType *Function::getReturnType() { return returnType; }

/**
 * Retrieve the argument types of the current function
 *
 * @return Vector of argument types
 */
std::vector<SymbolType *> Function::getArgTypes() { return argTypes; }

/**
 * Mange the function and return the mangled string
 *
 * @return Mangled string
 */
std::string Function::getMangledName() {
  // f, p, mf or mp depending on the function type
  std::string fpm = "f";
  if (isProcedure()) {
    fpm = "p";
  } else if (isMethodFunction()) {
    fpm = "mf";
  } else if (isMethodProcedure()) {
    fpm = "mp";
  }

  std::string thisTyStr;
  if (thisType)
    thisTyStr = thisType->getName() + "_";

  std::string returnTyStr;
  if (returnType)
    returnTyStr = returnType->getName() + "_";

  std::string argTyStr;
  for (auto &argType : argTypes)
    argTyStr += argType->getName() + "_";

  return "_" + fpm + "_" + thisTyStr + returnTyStr + name + "_" + argTyStr;
}

/**
 * Check if the current function is a function
 *
 * @return Function or not
 */
bool Function::isFunction() const { return returnType != nullptr && thisType == nullptr; }

/**
 * Check if the current function is a procedure
 *
 * @return Procedure or not
 */
bool Function::isProcedure() const { return returnType == nullptr && thisType == nullptr; }

/**
 * Check if the current function is a method function
 *
 * @return Method function or not
 */
bool Function::isMethodFunction() const { return returnType != nullptr && thisType != nullptr; }

/**
 * Check if the current function is a method procedure
 *
 * @return Method procedure or not
 */
bool Function::isMethodProcedure() const { return returnType == nullptr && thisType != nullptr; }