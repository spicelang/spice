// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "SymbolType.h"

#include <exception/CompilerError.h>

namespace spice::compiler {

bool operator==(const SymbolType::TypeChainElement &lhs, const SymbolType::TypeChainElement &rhs) {
  // Check super type
  if (lhs.superType != rhs.superType)
    return false;

  // Check data
  switch (lhs.superType) {
  case TY_ARRAY:
    return lhs.data.arraySize == rhs.data.arraySize;
  case TY_STRUCT:    // fall-through
  case TY_INTERFACE: // fall-through
  case TY_ENUM: {
    if (lhs.superType == TY_STRUCT) {
      assert(lhs.data.bodyScope != nullptr && rhs.data.bodyScope != nullptr);
      return lhs.typeId == rhs.typeId && lhs.templateTypes == rhs.templateTypes;
    } else if (lhs.superType == TY_INTERFACE) {
      return lhs.typeId == rhs.typeId;
    } else {
      assert(lhs.data.bodyScope != nullptr && rhs.data.bodyScope != nullptr);
      return lhs.typeId == rhs.typeId && lhs.data.bodyScope == rhs.data.bodyScope;
    }
  }
  case TY_FUNCTION: // fall-through
  case TY_PROCEDURE:
    if (lhs.paramTypes.size() != rhs.paramTypes.size())
      return false;
    for (size_t i = 0; i < lhs.paramTypes.size(); i++)
      if (lhs.paramTypes.at(i) != rhs.paramTypes.at(i))
        return false;
    return true;
  case TY_GENERIC:
    return lhs.subType == rhs.subType;
  default:
    return true;
  }
}

bool operator!=(const SymbolType::TypeChainElement &lhs, const SymbolType::TypeChainElement &rhs) { return !(lhs == rhs); }

/**
 * Return the type name as string
 *
 * @param withSize Also encode array sizes
 * @return Name as string
 */
std::string SymbolType::TypeChainElement::getName(bool withSize) const {
  switch (superType) {
  case TY_PTR:
    return "*";
  case TY_REF:
    return "&";
  case TY_ARRAY:
    return withSize && data.arraySize != ARRAY_SIZE_UNKNOWN ? "[" + std::to_string(data.arraySize) + "]" : "";
  case TY_DOUBLE:
    return "double";
  case TY_INT:
    return "int";
  case TY_SHORT:
    return "short";
  case TY_LONG:
    return "long";
  case TY_BYTE:
    return "byte";
  case TY_CHAR:
    return "char";
  case TY_STRING:
    return "string";
  case TY_BOOL:
    return "bool";
  case TY_STRUCT: // fall-through
  case TY_INTERFACE: {
    std::stringstream name;
    name << subType;
    if (!templateTypes.empty()) {
      name << "<";
      for (size_t i = 0; i < templateTypes.size(); i++) {
        if (i > 0)
          name << ",";
        name << templateTypes.at(i).getName();
      }
      name << ">";
    }
    return name.str();
  }
  case TY_ENUM:
    return "enum";
  case TY_GENERIC: // fall-through
  case TY_ALIAS:
    return subType;
  case TY_DYN:
    return "dyn";
  case TY_FUNCTION: {
    std::stringstream functionName;
    functionName << "f";
    if (data.hasCaptures)
      functionName << "[]";
    functionName << "<" << paramTypes.front().getName(true) << ">(";
    for (size_t i = 1; i < paramTypes.size(); i++) {
      if (i > 1)
        functionName << ",";
      functionName << paramTypes.at(i).getName(true);
    }
    functionName << ")";
    return functionName.str();
  }
  case TY_PROCEDURE: {
    std::stringstream procedureName;
    procedureName << "p";
    if (data.hasCaptures)
      procedureName << "[]";
    procedureName << "(";
    for (size_t i = 1; i < paramTypes.size(); i++) {
      if (i > 1)
        procedureName << ",";
      procedureName << paramTypes.at(i).getName(true);
    }
    procedureName << ")";
    return procedureName.str();
  }
  case TY_IMPORT:
    return "import";
  case TY_INVALID:    // GCOV_EXCL_LINE
    return "invalid"; // GCOV_EXCL_LINE
  default:
    throw CompilerError(INTERNAL_ERROR, "Could not get name of this type chain element");
  }
}

} // namespace spice::compiler