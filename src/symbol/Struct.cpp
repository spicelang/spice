// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Struct.h"

#include <symbol/SymbolTable.h>

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
 * Mange the struct and return the mangled string
 *
 * @return Mangled string
 */
std::string Struct::getMangledName() const {
  // Field type string
  std::string fieldTyStr;
  for (const auto &fieldType : fieldTypes)
    fieldTyStr += "_" + fieldType.getName();

  // Template type string
  std::string templateTyStr;
  for (const auto &templateType : templateTypes)
    templateTyStr += "_" + templateType.getName();

  return "_s" + templateTyStr + "_" + name + fieldTyStr;
}

/**
 * Get a string representation of the current struct
 *
 * @return String representation as struct signature
 */
std::string Struct::getSignature() const {
  // Argument type string
  std::string fieldTyStr;
  for (const auto &fieldType : fieldTypes) {
    if (!fieldTyStr.empty())
      fieldTyStr += ",";
    fieldTyStr += fieldType.getName();
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

  return name + templateTyStr + "(" + fieldTyStr + ")";
}

/**
 * Get symbol type. Possible super types are TY_STRUCT
 *
 * @return Symbol representing the struct
 */
SymbolType Struct::getSymbolType() const { return SymbolType(TY_STRUCT, name); }

/**
 * Convert the current ambiguous struct with potential generic types to a definite struct without generic types
 *
 * @return Substantiated struct without template types
 */
Struct Struct::substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes, SymbolTable *structScope,
                                    ErrorFactory *err, const antlr4::Token &token) const {
  std::vector<SymbolType> currentFieldTypes;

  // Substantiate field types
  for (const auto &fieldType : fieldTypes) {
    SymbolType newFieldType = fieldType;
    if (fieldType.is(TY_GENERIC)) {                    // We have to replace it only if it is a generic type
      for (int i = 0; i < templateTypes.size(); i++) { // Go through all template types and get the respective concrete type
        if (fieldType == templateTypes[i]) {
          newFieldType = concreteTemplateTypes[i]; // Use the concrete type instead of the generic one
          break;
        }
      }
    }
    currentFieldTypes.push_back(newFieldType);
  }

  // Substantiate methods
  assert(symbolTable != nullptr);
  for (const auto &method : symbolTable->getFunctions()) {
    // Duplicate method
    Function newMethod = method->substantiateGenerics(concreteTemplateTypes);
    structScope->insertSubstantiatedFunction(newMethod, err, token, method->getDefinitionCodeLoc());
    structScope->copyChildBlock(method->getSignature(), newMethod.getSignature());
  }

  return Struct(name, specifiers, currentFieldTypes, {}, definitionCodeLoc);
}

/**
 * Checks if a struct contains template types.
 * This would imply that the struct is not substantiated by its generic types yet.
 *
 * @return Substantiated generics or not
 */
bool Struct::hasSubstantiatedGenerics() const { return templateTypes.empty(); }

/**
 * Checks if a struct has generic types present.
 * This would imply that the struct is not fully substantiated yet.
 *
 * @return Fully substantiated or not
 */
bool Struct::isFullySubstantiated() const { return hasSubstantiatedGenerics(); }

/**
 * Set the associated symbol table
 *
 * @param symbolTable Symbol table
 */
void Struct::setSymbolTable(SymbolTable *symbolTable) { this->symbolTable = symbolTable; }

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