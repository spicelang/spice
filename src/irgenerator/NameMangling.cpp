// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "NameMangling.h"

#include <exception/CompilerError.h>
#include <model/Function.h>
#include <model/Interface.h>
#include <model/Struct.h>
#include <symboltablebuilder/SymbolTableEntry.h>
#include <util/CommonUtil.h>

namespace spice::compiler {

/**
 * Mangle a function or procedure.
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param spiceFunc Input function
 * @return Mangled name
 */
std::string NameMangling::mangleFunction(const Function &spiceFunc) {
  // Check if we have a predefined mangled name, specified by e.g. a function attribute
  if (!spiceFunc.predefinedMangledName.empty())
    return spiceFunc.predefinedMangledName;

  // Check if mangling is required
  if (!spiceFunc.mangleFunctionName)
    return spiceFunc.name;

  std::stringstream mangledName;
  mangledName << "_Z";

  // This type
  if (spiceFunc.isMethod()) {
    mangledName << "N";
    mangleType(mangledName, spiceFunc.thisType);
  }

  // Function name
  const std::string name = spiceFunc.name + spiceFunc.mangleSuffix;
  mangledName << name.length() << name;

  // Template types
  bool isSelfGeneric = !spiceFunc.templateTypes.empty();
  if (spiceFunc.isMethod())
    isSelfGeneric = spiceFunc.templateTypes.size() > spiceFunc.thisType.getTemplateTypes().size();
  if (isSelfGeneric) {
    mangledName << "I";
    // Template types themselves
    for (const GenericType &genericTemplateType : spiceFunc.templateTypes) {
      assert(spiceFunc.typeMapping.contains(genericTemplateType.getSubType()));
      const QualType &actualType = spiceFunc.typeMapping.at(genericTemplateType.getSubType());
      mangleType(mangledName, actualType);
    }
    mangledName << "E";

    // Insert second end marker to end the nested type
    if (spiceFunc.isMethod())
      mangledName << "E";

    // Return type
    if (spiceFunc.isFunction())
      mangleType(mangledName, spiceFunc.returnType);
    else
      mangledName << "v";
  } else if (spiceFunc.isMethod()) {
    mangledName << "E";
  }

  // Parameter types
  for (const auto &[qualType, isOptional] : spiceFunc.paramList) {
    assert(!isOptional);
    mangleType(mangledName, qualType);
  }
  if (spiceFunc.paramList.empty())
    mangledName << "v";

#ifndef NDEBUG
  const TypeMapping &typeMapping = spiceFunc.typeMapping;
  const bool returnTypeIsFctOrProc = spiceFunc.returnType.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE});
  const auto paramPredicate = [](const Param &p) { return p.qualType.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}); };
  const bool paramTypeIsFctOrProc = std::ranges::any_of(spiceFunc.paramList, paramPredicate);
  const auto templateTypePredicate = [&](const GenericType &t) {
    if (t.is(TY_GENERIC)) {
      assert(typeMapping.contains(t.getSubType()));
      return typeMapping.at(t.getSubType()).getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE});
    }
    return t.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE});
  };
  const bool templateTypeIsFctOrProc = std::ranges::any_of(spiceFunc.templateTypes, templateTypePredicate);
  if (!returnTypeIsFctOrProc && !paramTypeIsFctOrProc && !templateTypeIsFctOrProc)
    assert(CommonUtil::isValidMangledName(mangledName.str()));
#endif

  return mangledName.str();
}

/**
 * Mangle a struct
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param spiceStruct Input struct
 * @return Mangled name
 */
std::string NameMangling::mangleStruct(const Struct &spiceStruct) { return "struct." + spiceStruct.name; }

/**
 * Mangle an interface
 *
 * @param spiceInterface Input interface
 * @return Mangled name
 */
std::string NameMangling::mangleInterface(const Interface &spiceInterface) { return "interface." + spiceInterface.name; }

/**
 * Mangle a fully qualified name like e.g. test::s1::calledMethod to 4test2s112calledMethod
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param out Output string stream
 * @param name Input name
 * @param nestedType True if the name is a nested type
 * @return Mangled name
 */
void NameMangling::mangleName(std::stringstream &out, const std::string &name, bool &nestedType) {
  std::vector<std::string> fragments;
  std::istringstream ss(name);
  std::string token;

  while (std::getline(ss, token, ':')) {
    std::istringstream subStream(token);
    std::string subToken;
    while (std::getline(subStream, subToken, '/')) {
      fragments.push_back(subToken);
    }
  }

  // Start a nested type if needed. The caller needs to emit the end marker.
  if (fragments.size() > 1) {
    out << "N";
    nestedType = true;
  }

  // Process each fragment and append it to the result
  for (const std::string &fragment : fragments) {
    int fragmentLength = static_cast<int>(fragment.length());
    out << std::to_string(fragmentLength) << fragment;
  }
}

/**
 * Mangle a symbol qualType
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param out Output string stream
 * @param qualType Input symbol qualType
 * @return Mangled name
 */
void NameMangling::mangleType(std::stringstream &out, QualType qualType) { // NOLINT(*-no-recursion)
  const Type *type = qualType.getType();
  assert(!qualType.hasAnyGenericParts());

  // Unwrap qualType chain
  assert(!type->typeChain.empty());
  for (size_t i = type->typeChain.size() - 1; i >= 1; i--)
    mangleTypeChainElement(out, type->typeChain.at(i), false);

  // Qualifiers
  assert(qualType.getQualifiers().isSigned == !qualType.getQualifiers().isUnsigned);
  const bool signedness = qualType.getQualifiers().isSigned;
  if (qualType.getQualifiers().isConst && type->typeChain.size() > 1)
    out << "K";

  // Base chain element
  mangleTypeChainElement(out, type->typeChain.front(), signedness);
}

/**
 * Mangle a type chain element
 *
 * @param out Output string stream
 * @param chainElement Input type chain element
 * @param signedness Signedness of the type
 * @return Mangled name
 */
void NameMangling::mangleTypeChainElement(std::stringstream &out, const TypeChainElement &chainElement, bool signedness) {
  switch (chainElement.superType) {
  case TY_PTR:
    out << "P";
    break;
  case TY_ARRAY:
    if (chainElement.data.arraySize == ARRAY_SIZE_UNKNOWN)
      out << "P";
    else
      out << "A" << chainElement.data.arraySize << "_";
    break;
  case TY_REF:
    out << "R";
    break;
  case TY_DOUBLE:
    assert(signedness && "Unsigned double types are forbidden");
    out << "d";
    break;
  case TY_INT:
    out << (signedness ? "i" : "j");
    break;
  case TY_SHORT:
    out << (signedness ? "s" : "t");
    break;
  case TY_LONG:
    out << (signedness ? "l" : "m");
    break;
  case TY_BYTE:
    out << (signedness ? "a" : "h");
    break;
  case TY_CHAR:
    out << "c";
    break;
  case TY_STRING:
    out << "PKc";
    break;
  case TY_BOOL:
    assert(!signedness && "Signed bool types are forbidden");
    out << "b";
    break;
  case TY_STRUCT: // fall-through
  case TY_INTERFACE: {
    bool nestedType = false;
    mangleName(out, chainElement.subType, nestedType);
    if (!chainElement.templateTypes.empty()) {
      out << "I";
      for (const QualType &templateType : chainElement.templateTypes)
        mangleType(out, templateType);
      out << "E";
    }
    if (nestedType)
      out << "E";
    break;
  }
  case TY_ENUM: {
    bool nestedType = false;
    mangleName(out, chainElement.subType, nestedType);
    if (nestedType)
      out << "E";
    break;
  }
  case TY_FUNCTION: {
    out << (chainElement.data.hasCaptures ? "PFC" : "PF");
    for (const QualType &paramType : chainElement.paramTypes)
      mangleType(out, paramType);
    out << "E";
    break;
  }
  case TY_PROCEDURE: {
    out << (chainElement.data.hasCaptures ? "PFCv" : "PFv");
    for (size_t i = 1; i < chainElement.paramTypes.size(); i++)
      mangleType(out, chainElement.paramTypes.at(i));
    out << "E";
    break;
  }
  default:                                                                                                // GCOV_EXCL_LINE
    const std::string message = "Type " + chainElement.getName(false, true, true) + " cannot be mangled"; // GCOV_EXCL_LINE
    throw CompilerError(INTERNAL_ERROR, message);                                                         // GCOV_EXCL_LINE
  }
}

std::string NameMangling::mangleTypeInfoName(const StructBase *structBase) {
  std::stringstream out;
  out << "_ZTS";
  mangleType(out, structBase->entry->getQualType());
  return out.str();
}

std::string NameMangling::mangleTypeInfoValue(const std::string &value) { return std::to_string(value.size()) + value; }

std::string NameMangling::mangleTypeInfo(const StructBase *structBase) {
  std::stringstream out;
  out << "_ZTI";
  mangleType(out, structBase->entry->getQualType());
  return out.str();
}

std::string NameMangling::mangleVTable(const StructBase *structBase) {
  std::stringstream out;
  out << "_ZTV";
  mangleType(out, structBase->entry->getQualType());
  return out.str();
}

std::string NameMangling::mangleVTable(const std::string &typeName) {
  return "_ZTV" + std::to_string(typeName.size()) + typeName;
}

} // namespace spice::compiler