// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/AstNodes.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>

/**
 * Mange the struct and return the mangled string
 *
 * @return Mangled string
 */
std::string Struct::getMangledName() const {
  // Field type string
  std::string fieldTyStr;
  for (const auto &fieldType : fieldTypes) {
    if (!fieldTyStr.empty())
      fieldTyStr += "_";
    fieldTyStr += fieldType.getName(false, true);
  }

  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : templateTypes) {
    if (!templateTyStr.empty())
      templateTyStr += "_";
    templateTyStr += templateType.getName(false, true);
  }

  // Construct mangled name
  std::string mangledName = "_s";
  if (!templateTyStr.empty())
    mangledName += "__" + templateTyStr;
  mangledName += "__" + name;
  if (!fieldTyStr.empty())
    mangledName += "__" + fieldTyStr;
  return mangledName;
}

/**
 * Get a string representation of the current struct
 *
 * @return String representation as struct signature
 */
std::string Struct::getSignature() const {
  std::vector<SymbolType> templateSymbolTypes;
  for (const auto &templateType : templateTypes)
    templateSymbolTypes.push_back(templateType);
  return getSignature(name, templateSymbolTypes);
}

/**
 * Get symbol type. Possible super types are TY_STRUCT
 *
 * @return Symbol representing the struct
 */
SymbolType Struct::getSymbolType() const {
  std::vector<SymbolType> concreteTemplateTypes;
  if (isFullySubstantiated()) {
    for (const auto &templateType : templateTypes)
      concreteTemplateTypes.push_back(templateType);
  }
  return SymbolType(TY_STRUCT, name, {.arraySize = 0}, concreteTemplateTypes);
}

/**
 * Convert the current ambiguous struct with potential generic types to a definite struct without generic types
 *
 * @return Substantiated struct without template types
 */
Struct Struct::substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes, SymbolTable *scope) const {
  // Convert concrete template types to a list of generic types
  std::vector<GenericType> concreteTemplateTypesGeneric;
  for (const auto &concreteTemplateType : concreteTemplateTypes)
    concreteTemplateTypesGeneric.emplace_back(concreteTemplateType);

  // Substantiate field types
  std::vector<SymbolType> currentFieldTypes;
  for (int i = 0; i < fieldTypes.size(); i++) {
    SymbolTableEntry *fieldEntry = scope->lookupByIndex(i);
    if (fieldTypes[i].isBaseType(TY_GENERIC)) {        // We have to replace it only if it is a generic type
      for (int j = 0; j < templateTypes.size(); j++) { // Go through all template types and get the respective concrete type
        if (fieldTypes[i].getBaseType() == templateTypes[j]) {
          const SymbolType &newFieldType =
              fieldTypes[i].replaceBaseType(concreteTemplateTypes[j]); // Use the concrete type instead of the generic one
          fieldEntry->updateType(newFieldType, true);
          currentFieldTypes.push_back(newFieldType);
          break;
        }
      }
    } else {
      currentFieldTypes.push_back(fieldEntry->type);
    }
  }

  Struct substantiatedStruct(name, specifiers, currentFieldTypes, concreteTemplateTypesGeneric, interfaceTypes, declNode);
  substantiatedStruct.isGenericSubstantiation = true;
  return substantiatedStruct;
}

/**
 * Checks if a struct contains template types.
 * This would imply that the struct is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Struct::hasSubstantiatedGenerics() const {
  return std::none_of(templateTypes.begin(), templateTypes.end(),
                      [](const GenericType &genericType) { return genericType.isBaseType(TY_GENERIC); });
}

/**
 * Checks if a struct has generic types present.
 * This would imply that the struct is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Struct::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Get the signature from the struct name and the concrete template types
 *
 * @param structName Struct name
 * @param concreteTemplateTypes Concrete template types
 * @return Signature
 */
std::string Struct::getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes) {
  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : concreteTemplateTypes) {
    if (!templateTyStr.empty())
      templateTyStr += ",";
    templateTyStr += templateType.getName();
  }
  if (!templateTyStr.empty())
    templateTyStr = "<" + templateTyStr + ">";

  return structName + templateTyStr;
}