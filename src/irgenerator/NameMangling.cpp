// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "NameMangling.h"

#include <exception/CompilerError.h>
#include <model/Function.h>
#include <model/Struct.h>

namespace spice::compiler {

/**
 * Mangle a function or procedure.
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param function Input function
 * @return Mangled name
 */
std::string NameMangling::mangleFunction(const Function &spiceFunc) {
  // Check if we have a predefined mangled name, specified by e.g. a function attribute
  if (!spiceFunc.predefinedMangledName.empty())
    return spiceFunc.predefinedMangledName;

  // Check if mangling is required
  if (!spiceFunc.mangleFunctionName)
    return spiceFunc.name;

  bool needsEndMarker = false;
  std::stringstream mangledName;
  mangledName << "_Z";

  // This type
  if (spiceFunc.isMethod()) {
    mangledName << "N";
    mangleType(mangledName, spiceFunc.thisType);
    needsEndMarker = true;
  }

  // Function name
  mangledName << spiceFunc.name.length() << spiceFunc.name;

  // Template types
  if (!spiceFunc.templateTypes.empty()) {
    mangledName << "I";
    // Template types themselves
    for (const GenericType &genericTemplateType : spiceFunc.templateTypes) {
      assert(spiceFunc.typeMapping.contains(genericTemplateType.getSubType()));
      const SymbolType &actualType = spiceFunc.typeMapping.at(genericTemplateType.getSubType());
      mangleType(mangledName, actualType);
    }
    mangledName << "E";

    // Return type
    if (spiceFunc.isFunction())
      mangleType(mangledName, spiceFunc.returnType);
    else
      mangledName << "v";
  } else if (needsEndMarker) {
    mangledName << "E";
  }

  // Parameter types
  for (const Param &param : spiceFunc.paramList) {
    assert(!param.isOptional);
    mangleType(mangledName, param.type);
  }
  if (spiceFunc.paramList.empty())
    mangledName << "v";

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
 * Mangle a symbol type
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param type Input symbol type
 * @return Mangled name
 */
std::string NameMangling::mangleType(const SymbolType &type) {
  std::stringstream mangledName;
  mangleType(mangledName, type);
  return mangledName.str();
}

/**
 * Mangle a symbol type
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * @param out Output string stream
 * @param type Input symbol type
 * @return Mangled name
 */
void NameMangling::mangleType(std::stringstream &out, const SymbolType &type) {
  // Unwrap type chain
  assert(!type.typeChain.empty());
  for (size_t i = type.typeChain.size() - 1; i >= 1; i--)
    mangleTypeChainElement(out, type.typeChain.at(i), false);

  // Specifiers
  const bool signedness = type.specifiers.isSigned();
  if (type.specifiers.isConst() && type.typeChain.size() > 1)
    out << "K";

  // Base chain element
  mangleTypeChainElement(out, type.typeChain.front(), signedness);
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
    if (chainElement.data.arraySize == 0)
      out << "P";
    else
      out << "A" << chainElement.data.arraySize;
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
    out << (signedness ? "c" : "h");
    break;
  case TY_STRING:
    out << "Pc";
    break;
  case TY_BOOL:
    assert(!signedness && "Signed bool types are forbidden");
    out << "b";
    break;
  case TY_STRUCT:
  case TY_INTERFACE:
  case TY_ENUM: {
    std::string fqName = chainElement.subType;
    CommonUtil::replaceAll(fqName, "::", ".");
    out << fqName.length() << fqName; // <length><name>
    if (!chainElement.templateTypes.empty()) {
      out << "I";
      for (const SymbolType &templateType : chainElement.templateTypes)
        mangleType(out, templateType);
      out << "E";
    }
    break;
  }
  case TY_FUNCTION: {
    out << (chainElement.data.hasCaptures ? "PFC" : "PF");
    for (const SymbolType &paramType : chainElement.paramTypes)
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
  case TY_GENERIC:
    out << chainElement.subType.size() << chainElement.subType; // <length><name>
    break;
  default:
    throw CompilerError(INTERNAL_ERROR, "Type " + chainElement.getName(false) + " cannot be mangled");
  }
}

} // namespace spice::compiler