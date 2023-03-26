// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
class Function;
class Scope;
class SymbolTableEntry;
class SymbolType;
class ASTNode;
class GenericType;
struct CodeLoc;

// Typedefs
using FunctionManifestationList = std::unordered_map</*mangledName=*/std::string, /*structObject=*/Function>;
using FunctionRegistry = std::unordered_map</*codeLoc=*/std::string, /*manifestations=*/FunctionManifestationList>;

class FunctionManager {
public:
  // Friend classes
  friend class InterfaceManager;

  // Public methods
  static Function *insertFunction(Scope *insertScope, const Function &baseFunction,
                                  std::vector<Function *> *nodeFunctionList = nullptr);
  static void substantiateOptionalParams(const Function &baseFunction, std::vector<Function> &manifestations);
  [[nodiscard]] static Function createMainFunction(SymbolTableEntry *entry, const std::vector<SymbolType> &paramTypes,
                                                   ASTNode *declNode);
  [[nodiscard]] static Function *matchFunction(Scope *matchScope, const std::string &requestedName,
                                               const SymbolType &requestedThisType,
                                               const std::vector<SymbolType> &requestedParamTypes, bool strictSpecifierMatching,
                                               const ASTNode *callNode);

private:
  // Private methods
  [[nodiscard]] static Function *insertSubstantiation(Scope *insertScope, const Function &newManifestation,
                                                      const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Function &candidate, const std::string &requestedName);
  [[nodiscard]] static bool matchThisType(Function &candidate, const SymbolType &requestedThisType, TypeMapping &typeMapping,
                                          bool strictSpecifierMatching);
  [[nodiscard]] static bool matchArgTypes(Function &candidate, const std::vector<SymbolType> &requestedArgTypes,
                                          TypeMapping &typeMapping, bool strictSpecifierMatching);
  static void substantiateReturnType(Function &candidate, TypeMapping &typeMapping);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Function &candidate,
                                                                          const std::string &templateTypeName);
};

} // namespace spice::compiler