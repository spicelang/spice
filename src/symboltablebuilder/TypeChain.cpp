// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChain.h"

#include <exception/CompilerError.h>

namespace spice::compiler {

bool operator==(const TypeChainElement &lhs, const TypeChainElement &rhs) {
  // Check super type
  if (lhs.superType != rhs.superType)
    return false;

  // Check data
  switch (lhs.superType) {
  case TY_ARRAY:
    return lhs.data.arraySize == rhs.data.arraySize;
  case TY_STRUCT:
    assert(lhs.data.bodyScope != nullptr && rhs.data.bodyScope != nullptr);
    return lhs.typeId == rhs.typeId && lhs.templateTypes == rhs.templateTypes;
  case TY_INTERFACE:
    return lhs.typeId == rhs.typeId;
  case TY_ENUM:
    assert(lhs.data.bodyScope != nullptr && rhs.data.bodyScope != nullptr);
    return lhs.typeId == rhs.typeId && lhs.data.bodyScope == rhs.data.bodyScope;
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

bool operator!=(const TypeChainElement &lhs, const TypeChainElement &rhs) { return !(lhs == rhs); }

void TypeChainElement::getName(std::stringstream &name, bool withSize, bool ignorePublic, bool withAliases) const {
  switch (superType) {
  case TY_PTR:
    name << "*";
    break;
  case TY_REF:
    name << "&";
    break;
  case TY_ARRAY:
    name << "[" << (withSize && data.arraySize != ARRAY_SIZE_UNKNOWN ? std::to_string(data.arraySize) : "") << "]";
    break;
  case TY_DOUBLE:
    name << "double";
    break;
  case TY_INT:
    name << "int";
    break;
  case TY_SHORT:
    name << "short";
    break;
  case TY_LONG:
    name << "long";
    break;
  case TY_BYTE:
    name << "byte";
    break;
  case TY_CHAR:
    name << "char";
    break;
  case TY_STRING:
    name << (withAliases ? "string" : "const char*");
    break;
  case TY_BOOL:
    name << "bool";
    break;
  case TY_STRUCT: // fall-through
  case TY_INTERFACE:
    name << subType;
    if (!templateTypes.empty()) {
      name << "<";
      for (size_t i = 0; i < templateTypes.size(); i++) {
        if (i > 0)
          name << ",";
        templateTypes.at(i).getName(name, withSize, ignorePublic);
      }
      name << ">";
    }
    break;
  case TY_ENUM:
    name << "enum(" << subType << ")";
    break;
  case TY_GENERIC: // fall-through
  case TY_ALIAS:
    name << subType;
    break;
  case TY_DYN:
    name << "dyn";
    break;
  case TY_FUNCTION: {
    name << "f";
    if (data.hasCaptures)
      name << "[]";
    if (!paramTypes.empty()) {
      name << "<";
      paramTypes.front().getName(name, true, ignorePublic);
      name << ">";
    }
    name << "(";
    for (size_t i = 1; i < paramTypes.size(); i++) {
      if (i > 1)
        name << ",";
      paramTypes.at(i).getName(name, true, ignorePublic);
    }
    name << ")";
    break;
  }
  case TY_PROCEDURE: {
    name << "p";
    if (data.hasCaptures)
      name << "[]";
    name << "(";
    for (size_t i = 1; i < paramTypes.size(); i++) {
      if (i > 1)
        name << ",";
      paramTypes.at(i).getName(name, true, ignorePublic);
    }
    name << ")";
    break;
  }
  case TY_IMPORT:
    name << "import";
    break;
  case TY_INVALID:     // GCOV_EXCL_LINE
    name << "invalid"; // GCOV_EXCL_LINE
    break;
  default:
    throw CompilerError(INTERNAL_ERROR, "Could not get name of this type chain element");
  }
}

/**
 * Return the type name as string
 *
 * @param withSize Also encode array sizes
 * @param ignorePublic Ignore public qualifier
 * @param withAliases Print aliases as is and not decompose them
 * @return Name as string
 */
std::string TypeChainElement::getName(bool withSize, bool ignorePublic, bool withAliases) const {
  std::stringstream name;
  getName(name, withSize, ignorePublic, withAliases);
  return name.str();
}

} // namespace spice::compiler