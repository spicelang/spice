// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Struct.h"

/**
 * Retrieve the name of the current struct
 *
 * @return Struct name
 */
std::string Struct::getName() const { return name; }

/**
 * Retrieve the symbol specifiers of the current struct
 *
 * @return Symbol specifiers
 */
SymbolSpecifiers Struct::getSpecifiers() const { return specifiers; }

/**
 * Retrieve the field types of the current struct
 *
 * @return Vector of field types
 */
std::vector<SymbolType> Struct::getFieldTypes() const { return fieldTypes; }

/**
 * Retrieve the template types of the current struct
 *
 * @return Vector of template types
 */
std::vector<GenericType> Struct::getTemplateTypes() const { return templateTypes; }

/**
 * Convert the current ambiguous struct with potential generic types to a definite struct without generic types
 *
 * @return Substantiated struct without template types
 */
Struct Struct::substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes) const {
  std::vector<SymbolType> currentFieldTypes;

  // Substantiate field types
  for (const auto &fieldType : fieldTypes) {
    SymbolType newFieldType = fieldType;
    if (fieldType.is(TY_GENERIC)) {                    // We have to replace it only if it is a generic type
      for (int i = 0; i < templateTypes.size(); i++) { // Go through all template types and get the respective concrete type
        const SymbolType concreteArgType = concreteTemplateTypes[i];
        const SymbolType genericType = templateTypes[i];
        if (fieldType == genericType) {
          newFieldType = concreteArgType; // Use the concrete type instead of the generic one
          break;
        }
      }
    }
    currentFieldTypes.push_back(newFieldType);
  }

  // Substantiate methods
  // ToDo: implement

  return Struct(name, specifiers, currentFieldTypes, {}, definitionCodeLoc);
}

/**
 * Checks if a struct has generic types present. This would imply that the struct is not fully substantiated yet
 *
 * @return Fully substantiated or not
 */
bool Struct::isFullySubstantiated() const { return templateTypes.empty(); }

/**
 * Set the struct to used. The compiler only generates IR if the struct is used
 */
void Struct::setUsed() { used = true; }

/**
 * Check if the struct is used and the compiler needs to generate IR for it
 *
 * @return Used or not
 */
bool Struct::isUsed() const { return used; }

/**
 * Retrieve the definition code loc of this struct
 *
 * @return Definition code location
 */
const std::string &Struct::getDefinitionCodeLoc() const { return definitionCodeLoc; }