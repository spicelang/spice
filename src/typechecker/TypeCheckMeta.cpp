// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitParamLst(ParamLstNode *node) {
  NamedParamList namedParams;
  bool metOptional = false;

  for (DeclStmtNode *param : node->params) {
    // Visit param
    const auto paramType = std::any_cast<QualType>(visit(param));

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (paramType.is(TY_DYN)) {
      softError(node, FCT_PARAM_IS_TYPE_DYN, "Type of parameter '" + param->varName + "' is invalid");
      continue;
    }

    // Ensure that no optional param comes after a mandatory param
    if (param->hasAssignment) {
      metOptional = true;
    } else if (metOptional) {
      softError(param, INVALID_PARAM_ORDER, "Mandatory parameters must go before any optional parameters");
      continue;
    }

    // Add parameter to named param list
    namedParams.push_back({param->varName.c_str(), paramType, metOptional});
  }

  return namedParams;
}

std::any TypeChecker::visitField(FieldNode *node) {
  auto fieldType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_QT(fieldType)

  if (TernaryExprNode *defaultValueNode = node->defaultValue) {
    const QualType defaultValueType = std::any_cast<ExprResult>(visit(defaultValueNode)).type;
    HANDLE_UNRESOLVED_TYPE_QT(defaultValueType)
    if (!fieldType.matches(defaultValueType, false, true, true))
      SOFT_ERROR_QT(node, FIELD_TYPE_NOT_MATCHING, "Type of the default values does not match the field type")
  }

  return fieldType;
}

std::any TypeChecker::visitSignature(SignatureNode *node) {
  const bool isFunction = node->signatureType == SignatureNode::SignatureType::TYPE_FUNCTION;

  // Retrieve function template types
  std::vector<GenericType> usedGenericTypes;
  if (node->hasTemplateTypes) {
    for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
      // Visit template type
      auto templateType = std::any_cast<QualType>(visit(dataType));
      if (templateType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);
      // Check if it is a generic type
      if (!templateType.is(TY_GENERIC)) {
        softError(dataType, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        return static_cast<std::vector<Function *> *>(nullptr);
      }
      // Convert generic symbol type to generic type
      const GenericType *genericType = rootScope->lookupGenericTypeStrict(templateType.getSubType());
      assert(genericType != nullptr);
      usedGenericTypes.push_back(*genericType);
    }
  }

  // Visit return type
  QualType returnType(TY_DYN);
  if (isFunction) {
    returnType = std::any_cast<QualType>(visit(node->returnType));
    if (returnType.is(TY_UNRESOLVED))
      return static_cast<std::vector<Function *> *>(nullptr);

    if (!returnType.isCoveredByGenericTypeList(usedGenericTypes))
      softError(node->returnType, GENERIC_TYPE_NOT_IN_TEMPLATE,
                "Generic return type not included in the template type list of the function");
  }

  // Visit params
  QualTypeList paramTypes;
  ParamList paramList;
  if (node->hasParams) {
    paramList.reserve(node->paramTypeLst->dataTypes.size());
    for (DataTypeNode *param : node->paramTypeLst->dataTypes) {
      auto paramType = std::any_cast<QualType>(visit(param));
      if (paramType.is(TY_UNRESOLVED))
        return static_cast<std::vector<Function *> *>(nullptr);

      // Check if the type is present in the template for generic types
      if (!paramType.isCoveredByGenericTypeList(usedGenericTypes)) {
        softError(node->paramTypeLst, GENERIC_TYPE_NOT_IN_TEMPLATE,
                  "Generic param type not included in the template type list of the function");
        continue;
      }

      paramTypes.push_back(paramType);
      paramList.push_back({paramType, false});
    }
  }

  // Build signature object
  const Function signature(node->methodName, nullptr, QualType(TY_DYN), returnType, paramList, usedGenericTypes, node);

  // Add signature to current scope
  Function *manifestation = FunctionManager::insert(currentScope, signature, &node->signatureManifestations);
  manifestation->entry = node->entry;
  manifestation->used = true;

  // Prepare signature type
  const SuperType superType = isFunction ? TY_FUNCTION : TY_PROCEDURE;
  QualType signatureType = QualType(superType).getWithFunctionParamAndReturnTypes(returnType, paramTypes);
  signatureType.setQualifiers(node->signatureQualifiers);

  // Set entry to signature type
  assert(node->entry != nullptr);
  node->entry->updateType(signatureType, false);
  node->entry->used = true;

  return &node->signatureManifestations;
}

std::any TypeChecker::visitDataType(DataTypeNode *node) {
  // Visit base data type
  auto type = std::any_cast<QualType>(visit(node->baseDataType));
  HANDLE_UNRESOLVED_TYPE_QT(type)

  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    auto [modifierType, hasSize, hardcodedSize, sizeVarName] = tmQueue.front();

    // Only the outermost array can have an unknown size
    if (type.isArray() && type.getArraySize() == ARRAY_SIZE_UNKNOWN)
      SOFT_ERROR_QT(node, ARRAY_SIZE_INVALID,
                    "Usage of incomplete array type. Only the outermost array type may have unknown size")

    switch (modifierType) {
    case DataTypeNode::TypeModifierType::TYPE_PTR: {
      type = type.toPtr(node);
      break;
    }
    case DataTypeNode::TypeModifierType::TYPE_REF: {
      type = type.toRef(node);
      break;
    }
    case DataTypeNode::TypeModifierType::TYPE_ARRAY: {
      const std::string &varName = sizeVarName;
      if (!varName.empty()) {
        const SymbolTableEntry *globalVar = rootScope->lookupStrict(varName);
        if (!globalVar)
          SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + varName + "' ")
        if (!globalVar->getQualType().isConst())
          SOFT_ERROR_QT(node, EXPECTED_CONST_VARIABLE, "The size of the array must be known at compile time")
        if (!globalVar->getQualType().is(TY_INT))
          SOFT_ERROR_QT(node, OPERATOR_WRONG_DATA_TYPE, "Expected variable of type int")
        hardcodedSize = globalVar->declNode->getCompileTimeValue().intValue;
      }

      if (hasSize && hardcodedSize <= 1)
        SOFT_ERROR_QT(node, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated")
      type = type.toArr(node, hardcodedSize);
      break;
    }
    default:                                                               // GCOV_EXCL_LINE
      throw CompilerError(UNHANDLED_BRANCH, "Modifier type fall-through"); // GCOV_EXCL_LINE
    }
    tmQueue.pop();
  }

  // Attach the qualifiers to the type
  if (node->qualifierLst) {
    const QualType baseType = type.getBase();
    for (const QualifierNode *qualifier : node->qualifierLst->qualifiers) {
      if (qualifier->type == QualifierNode::QualifierType::TY_CONST) {
        type.getQualifiers().isConst = true;
      } else if (qualifier->type == QualifierNode::QualifierType::TY_SIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_QT(qualifier, QUALIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this qualifier on type " + baseType.getName(false))
        type.getQualifiers().isSigned = true;
        type.getQualifiers().isUnsigned = false;
      } else if (qualifier->type == QualifierNode::QualifierType::TY_UNSIGNED) {
        if (!baseType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BYTE, TY_CHAR, TY_GENERIC}))
          SOFT_ERROR_QT(qualifier, QUALIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this qualifier on type " + baseType.getName(false))
        type.getQualifiers().isSigned = false;
        type.getQualifiers().isUnsigned = true;
      } else if (qualifier->type == QualifierNode::QualifierType::TY_HEAP) {
        // Heap variables can only be pointers
        if (!type.removeReferenceWrapper().isOneOf({TY_PTR, TY_ARRAY, TY_STRING}))
          SOFT_ERROR_QT(qualifier, QUALIFIER_AT_ILLEGAL_CONTEXT,
                        "The heap qualifier can only be applied to symbols of pointer type, you provided " +
                            baseType.getName(false))

        type.getQualifiers().isHeap = true;
      } else if (qualifier->type == QualifierNode::QualifierType::TY_COMPOSITION && node->isFieldType) {
        if (!type.is(TY_STRUCT))
          SOFT_ERROR_QT(qualifier, QUALIFIER_AT_ILLEGAL_CONTEXT, "The compose qualifier can only be used on plain struct fields")
        type.getQualifiers().isComposition = true;
      } else if (qualifier->type == QualifierNode::QualifierType::TY_PUBLIC && (node->isFieldType || node->isGlobalType)) {
        type.getQualifiers().isPublic = true;
      } else {
        auto entryName = "local variable";
        if (node->isGlobalType)
          entryName = "global variable";
        else if (node->isFieldType)
          entryName = "field";
        else if (node->isParamType)
          entryName = "param";
        else if (node->isReturnType)
          entryName = "return variable";
        SOFT_ERROR_QT(qualifier, QUALIFIER_AT_ILLEGAL_CONTEXT,
                      "Cannot use this qualifier on a " + std::string(entryName) + " definition")
      }
    }
  }

  return node->setEvaluatedSymbolType(type, manIdx);
}

std::any TypeChecker::visitBaseDataType(BaseDataTypeNode *node) {
  switch (node->type) {
  case BaseDataTypeNode::Type::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(QualType(TY_DOUBLE), manIdx);
  case BaseDataTypeNode::Type::TYPE_INT:
    return node->setEvaluatedSymbolType(QualType(TY_INT), manIdx);
  case BaseDataTypeNode::Type::TYPE_SHORT:
    return node->setEvaluatedSymbolType(QualType(TY_SHORT), manIdx);
  case BaseDataTypeNode::Type::TYPE_LONG:
    return node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx);
  case BaseDataTypeNode::Type::TYPE_BYTE:
    return node->setEvaluatedSymbolType(QualType(TY_BYTE), manIdx);
  case BaseDataTypeNode::Type::TYPE_CHAR:
    return node->setEvaluatedSymbolType(QualType(TY_CHAR), manIdx);
  case BaseDataTypeNode::Type::TYPE_STRING:
    return node->setEvaluatedSymbolType(QualType(TY_STRING), manIdx);
  case BaseDataTypeNode::Type::TYPE_BOOL:
    return node->setEvaluatedSymbolType(QualType(TY_BOOL), manIdx);
  case BaseDataTypeNode::Type::TYPE_CUSTOM: {
    const auto customType = std::any_cast<QualType>(visit(node->customDataType));
    HANDLE_UNRESOLVED_TYPE_QT(customType)
    return node->setEvaluatedSymbolType(customType, manIdx);
  }
  case BaseDataTypeNode::Type::TYPE_FUNCTION: {
    const auto functionType = std::any_cast<QualType>(visit(node->functionDataType));
    HANDLE_UNRESOLVED_TYPE_QT(functionType)
    return node->setEvaluatedSymbolType(functionType, manIdx);
  }
  default:
    return node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx);
  }
}

std::any TypeChecker::visitCustomDataType(CustomDataTypeNode *node) {
  // It is a struct type -> get the access scope
  const std::string firstFragment = node->typeNameFragments.front();

  // Check this type requires a runtime module
  if (node->typeNameFragments.size() == 1)
    ensureLoadedRuntimeForTypeName(firstFragment);

  // A type can either be a single fragment like "Test" or multiple fragments "a.b.Test", which means it is imported.
  bool isImported = node->typeNameFragments.size() > 1;
  if (const QualType *genericType = rootScope->lookupGenericTypeStrict(firstFragment)) {
    assert(!isImported);
    // Take the concrete replacement type for the name of this generic type if available
    const QualType &symbolType = typeMapping.contains(firstFragment) ? typeMapping.at(firstFragment) : *genericType;

    // Check if the replacement requires a runtime module
    if (symbolType.is(TY_STRUCT))
      ensureLoadedRuntimeForTypeName(symbolType.getSubType());

    return node->setEvaluatedSymbolType(symbolType, manIdx);
  }

  // Check if the type exists in the exported names registry
  const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqTypeName);
  if (!registryEntry)
    SOFT_ERROR_QT(node, UNKNOWN_DATATYPE, "Unknown datatype '" + node->fqTypeName + "'")
  assert(registryEntry->targetEntry != nullptr && registryEntry->targetScope != nullptr);
  SymbolTableEntry *entry = registryEntry->targetEntry;
  assert(entry != nullptr);
  entry->used = true;
  Scope *defScope = registryEntry->targetScope->parent;
  QualType entryType = entry->getQualType();

  // Enums can early-return
  if (entryType.is(TY_ENUM))
    return QualType(TY_INT);

  if (entryType.isOneOf({TY_STRUCT, TY_INTERFACE})) {
    assert(dynamic_cast<DataTypeNode *>(node->parent->parent) != nullptr);

    // Collect the concrete template types
    bool allTemplateTypesConcrete = true;
    QualTypeList templateTypes;
    if (node->templateTypeLst) {
      assert(defScope != nullptr);
      isImported = defScope->isImportedBy(rootScope);

      templateTypes.reserve(node->templateTypeLst->dataTypes.size());
      for (DataTypeNode *dataType : node->templateTypeLst->dataTypes) {
        auto templateType = std::any_cast<QualType>(visit(dataType));
        HANDLE_UNRESOLVED_TYPE_QT(templateType)
        if (entryType.is(TY_GENERIC)) {
          allTemplateTypesConcrete = false;
        } else if (isImported) {
          // Introduce the local type to the imported source file
          [[maybe_unused]] QualType importedType = mapLocalTypeToImportedScopeType(defScope, templateType);
          assert(importedType.is(templateType.getSuperType()));
        }
        templateTypes.push_back(templateType);
      }
      entryType = entryType.getWithTemplateTypes(templateTypes);
    }

    // Check if struct is defined before the current code location, if defined in the same source file
    const CodeLoc &declCodeLoc = entry->declNode->codeLoc;
    const CodeLoc &codeLoc = node->codeLoc;
    if (declCodeLoc.sourceFile->filePath == codeLoc.sourceFile->filePath && declCodeLoc > codeLoc) {
      if (entryType.is(TY_STRUCT)) {
        SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_STRUCT, "Structs must be defined before usage")
      } else {
        assert(entryType.is(TY_INTERFACE));
        SOFT_ERROR_QT(node, REFERENCED_UNDEFINED_INTERFACE, "Interfaces must be defined before usage")
      }
    }

    if (allTemplateTypesConcrete) { // Only do the next step, if we have concrete template types
      // Set the struct/interface instance to used, if found
      // Here, it is allowed to accept, that the struct/interface cannot be found, because there are self-referencing ones
      if (entryType.is(TY_STRUCT)) {
        const std::string structName = node->typeNameFragments.back();
        if (const Struct *spiceStruct = StructManager::match(defScope, structName, templateTypes, node))
          entryType = entryType.getWithBodyScope(spiceStruct->scope);
      } else {
        assert(entryType.is(TY_INTERFACE));
        const std::string interfaceName = node->typeNameFragments.back();
        if (const Interface *spiceInterface = InterfaceManager::match(defScope, interfaceName, templateTypes, node))
          entryType = entryType.getWithBodyScope(spiceInterface->scope);
      }
    }

    return node->setEvaluatedSymbolType(entryType, manIdx);
  }

  if (entryType.is(TY_ALIAS))
    return node->setEvaluatedSymbolType(entryType.getAliased(entry), manIdx);

  // We tried everything to resolve it, but this type is still unknown
  const bool isInvalid = entryType.is(TY_INVALID);
  SOFT_ERROR_QT(node, EXPECTED_TYPE, isInvalid ? "Used type before declared" : "Expected type, but got " + entryType.getName())
}

std::any TypeChecker::visitFunctionDataType(FunctionDataTypeNode *node) {
  // Visit return type
  QualType returnType(TY_DYN);
  if (node->isFunction) {
    returnType = std::any_cast<QualType>(visit(node->returnType));
    HANDLE_UNRESOLVED_TYPE_QT(returnType)
    if (returnType.is(TY_DYN))
      SOFT_ERROR_ER(node->returnType, UNEXPECTED_DYN_TYPE, "Function types cannot have return type dyn")
  }

  // Visit param types
  QualTypeList paramTypes;
  if (const TypeLstNode *paramTypeListNode = node->paramTypeLst; paramTypeListNode != nullptr) {
    for (DataTypeNode *paramTypeNode : paramTypeListNode->dataTypes) {
      auto paramType = std::any_cast<QualType>(visit(paramTypeNode));
      HANDLE_UNRESOLVED_TYPE_QT(returnType)
      paramTypes.push_back(paramType);
    }
  }

  // Build function type
  const SuperType superType = node->isFunction ? TY_FUNCTION : TY_PROCEDURE;
  const QualType functionType = QualType(superType).getWithFunctionParamAndReturnTypes(returnType, paramTypes);

  return node->setEvaluatedSymbolType(functionType, manIdx);
}

} // namespace spice::compiler
