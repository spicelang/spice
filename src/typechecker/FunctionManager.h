// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
class Function;
class Scope;
class SymbolType;
class ASTNode;
struct CodeLoc;

using FunctionManifestationList = std::unordered_map</*mangledName=*/std::string, /*structObject=*/Function>;
using FunctionRegistry = std::unordered_map</*codeLoc=*/std::string, /*manifestations=*/FunctionManifestationList>;
using TypeMapping = std::unordered_map</*typeName=*/std::string, /*concreteType=*/SymbolType>;

class FunctionManager {
public:
  // Public methods
  static Function *insertFunction(Scope *insertScope, const Function &baseFunction,
                                  std::vector<Function *> *nodeFunctionList = nullptr);
  static void substantiateOptionalParams(const Function &baseFunction, std::vector<Function> &manifestations);
  [[nodiscard]] static FunctionManifestationList *getManifestationList(Scope *lookupScope, const CodeLoc &defCodeLoc);
  [[nodiscard]] static Function *matchFunction(Scope *matchScope, const std::string &requestedName,
                                               const SymbolType &requestedThisType,
                                               const std::vector<SymbolType> &requestedParamTypes, const ASTNode *callNode);

private:
  // Private methods
  [[nodiscard]] static Function *insertSubstantiation(Scope *insertScope, const Function &newManifestation,
                                                      const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Function &candidate, const std::string &requestedName);
  [[nodiscard]] static bool matchThisType(Function &candidate, const SymbolType &requestedThisType, TypeMapping &typeMapping);
  [[nodiscard]] static bool matchArgTypes(Function &candidate, const std::vector<SymbolType> &requestedParamTypes,
                                          TypeMapping &typeMapping);
};