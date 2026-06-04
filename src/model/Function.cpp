// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "Function.h"

#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <irgenerator/NameMangling.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

Function::Function(std::string name, SymbolTableEntry *entry, const QualType &thisType, const QualType &returnType,
                   ParamList paramList, std::vector<GenericType> templateTypes, ASTNode *declNode)
    : name(std::move(name)), thisType(thisType), returnType(returnType), paramList(std::move(paramList)),
      templateTypes(std::move(templateTypes)), entry(entry), declNode(declNode) {}

/**
 * Retrieve the parameter types of the current function
 *
 * @return Vector of parameter types
 */
QualTypeList Function::getParamTypes() const {
  QualTypeList newParamTypes;
  for (const auto &[qualType, isOptional] : paramList)
    newParamTypes.push_back(qualType);
  return newParamTypes;
}

/**
 * Get a string representation of the current function.
 *
 * Example:
 * string Vector<double>.setData<double>(double)
 *
 * @param withThisType Include 'this' type in signature
 * @param withTemplateTypes Include concrete template types in the signature
 * @param withTypeAliases Print type aliases as is and not decompose them
 * @param withSize Print array sizes
 * @return String representation as function signature
 */
std::string Function::getSignature(bool withThisType /*=true*/, bool withTemplateTypes /*=true*/, bool withTypeAliases /*=true*/,
                                   bool withSize /*=false*/) const {
  QualTypeList concreteTemplateTypes;
  if (withTemplateTypes) {
    concreteTemplateTypes.reserve(templateTypes.size());
    for (const GenericType &genericType : templateTypes) {
      if (genericType.is(TY_GENERIC) && !typeMapping.empty()) {
        const std::string &subType = genericType.getSubType();
        assert(typeMapping.contains(subType));
        concreteTemplateTypes.push_back(typeMapping.at(subType));
      } else {
        concreteTemplateTypes.push_back(genericType);
      }
    }
  }

  return getSignature(name, thisType, returnType, paramList, concreteTemplateTypes, true, withThisType, true, withTypeAliases,
                      withSize);
}

/**
 * Get a string representation of the current function.
 *
 * @param name Function name
 * @param thisType This symbol type
 * @param returnType Return symbol type
 * @param paramList Param symbol types
 * @param concreteTemplateTypes Concrete template symbol types
 * @param withReturnType Include return type in signature
 * @param withThisType Include 'this' type in signature
 * @param ignorePublic Not include public modifiers in signature
 * @param withTypeAliases Print type aliases as is and not decompose them
 * @param withSize Print array sizes
 * @return Function signature
 */
std::string Function::getSignature(const std::string &name, const QualType &thisType, const QualType &returnType,
                                   const ParamList &paramList, const QualTypeList &concreteTemplateTypes,
                                   bool withReturnType /*=true*/, bool withThisType /*=true*/, bool ignorePublic /*=false*/,
                                   bool withTypeAliases /*=true*/, bool withSize /*=false*/) {
  std::stringstream signature;

  // Build return type string
  if (withReturnType) {
    if (returnType.is(TY_DYN)) {
      signature << "p ";
    } else {
      signature << "f<";
      returnType.getName(signature, withSize, ignorePublic, withTypeAliases);
      signature << "> ";
    }
  }

  // Build this type string
  if (withThisType && !thisType.is(TY_DYN)) {
    signature << thisType.getBase().getSubType();
    const QualTypeList &thisTemplateTypes = thisType.getTemplateTypes();
    if (!thisTemplateTypes.empty()) {
      signature << "<";
      for (size_t i = 0; i < thisTemplateTypes.size(); i++) {
        if (i > 0)
          signature << ",";
        signature << thisTemplateTypes.at(i).getName(withSize, ignorePublic, withTypeAliases);
      }
      signature << ">";
    }
    signature << MEMBER_ACCESS_TOKEN;
  }

  // Name
  signature << name;

  // Build template type string
  if (!concreteTemplateTypes.empty()) {
    signature << "<";
    for (size_t i = 0; i < concreteTemplateTypes.size(); i++) {
      if (i > 0)
        signature << ",";
      signature << concreteTemplateTypes.at(i).getName(withSize, ignorePublic, withTypeAliases);
    }
    signature << ">";
  }

  // Parameter type string
  signature << "(";
  for (size_t i = 0; i < paramList.size(); i++) {
    const auto &[qualType, isOptional] = paramList.at(i);
    if (i > 0)
      signature << ",";
    signature << qualType.getName(withSize, ignorePublic, withTypeAliases);
    if (isOptional)
      signature << "?";
  }
  signature << ")";

  return signature.str();
}

/**
 * Get the name of the scope, where members and the body of the function live
 *
 * @return Scope name
 */
std::string Function::getScopeName() const { return getSignature(false, true, false, true); }

/**
 * Get the mangled name of the function
 *
 * @return Mangled name
 */
std::string Function::getMangledName() const {
  // Use predefined mangled name if available
  if (!predefinedMangledName.empty())
    return predefinedMangledName;
  // Use function name if mangling is disabled
  if (!mangleFunctionName)
    return name;
  // Use normal name mangling
  return NameMangling::mangleFunction(*this);
}

/**
 * Get the name of the symbol table entry of the function
 *
 * @param functionName Function name
 * @param codeLoc Code location
 * @return Symbol table entry name
 */
std::string Function::getSymbolTableEntryName(const std::string &functionName, const CodeLoc &codeLoc) {
  return functionName + ":" + codeLoc.toString();
}

/**
 * Get the name of the symbol table entry of the default constructor of a struct
 *
 * @param structCodeLoc Code location of the struct
 * @return Symbol table entry name
 */
std::string Function::getSymbolTableEntryNameDefaultCtor(const CodeLoc &structCodeLoc) {
  return "default_" + std::string(CTOR_FUNCTION_NAME) + ":" + structCodeLoc.toString();
}

/**
 * Get the name of the symbol table entry of the default copy constructor of a struct
 *
 * @param structCodeLoc Code location of the struct
 * @return Symbol table entry name
 */
std::string Function::getSymbolTableEntryNameDefaultCopyCtor(const CodeLoc &structCodeLoc) {
  return "default_copy" + std::string(CTOR_FUNCTION_NAME) + ":" + structCodeLoc.toString();
}

/**
 * Get the name of the symbol table entry of the default move constructor of a struct
 *
 * @param structCodeLoc Code location of the struct
 * @return Symbol table entry name
 */
std::string Function::getSymbolTableEntryNameDefaultMoveCtor(const CodeLoc &structCodeLoc) {
  return "default_move" + std::string(CTOR_FUNCTION_NAME) + ":" + structCodeLoc.toString();
}

/**
 * Get the name of the symbol table entry of the default destructor of a struct
 *
 * @param structCodeLoc Code location of the struct
 * @return Symbol table entry name
 */
std::string Function::getSymbolTableEntryNameDefaultDtor(const CodeLoc &structCodeLoc) {
  return "default_" + std::string(DTOR_FUNCTION_NAME) + ":" + structCodeLoc.toString();
}

/**
 * Checks if a function contains optional parameters.
 * This would imply that the function is not substantiated by its optional parameters yet.
 *
 * @return Substantiated params or not
 */
bool Function::hasSubstantiatedParams() const {
  return std::ranges::none_of(paramList, [](const Param &param) { return param.isOptional; });
}

/**
 * Checks if a function contains template types.
 * This would imply that the function is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Function::hasSubstantiatedGenerics() const {
  const auto predicate = [this](const GenericType &genericType) { return typeMapping.contains(genericType.getSubType()); };
  return std::ranges::all_of(templateTypes, predicate);
}

/**
 * Checks if a function contains optional parameters or has generic types present.
 * This would imply that the function is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Function::isFullySubstantiated() const { return hasSubstantiatedParams() && hasSubstantiatedGenerics(); }

/**
 * Returns, if this function is a substantiation of a generic one.
 *
 * @return Generic substantiation or not
 */
bool Function::isGenericSubstantiation() const { return genericPreset != nullptr; }

/**
 * Retrieve the declaration code location of this function
 *
 * @return Declaration code location
 */
const CodeLoc &Function::getDeclCodeLoc() const { return declNode->codeLoc; }

} // namespace spice::compiler