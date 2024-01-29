// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>
#include <typechecker/ExprResult.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

// Forward declarations
class Function;
class Scope;
class SymbolTableEntry;
class SymbolType;
class ASTNode;
class GenericType;

// Typedefs
using FunctionManifestationList = std::unordered_map</*mangledName=*/std::string, Function>;
using FunctionRegistry = std::map<CodeLoc, FunctionManifestationList>;
using Arg = std::pair</*type=*/SymbolType, /*isTemporary=*/bool>;
using ArgList = std::vector<Arg>;

enum class MatchResult : uint8_t {
  MATCHED,
  SKIP_MANIFESTATION,
  SKIP_FUNCTION,
};

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
  [[nodiscard]] static const Function *lookupFunction(Scope *matchScope, const std::string &reqName,
                                                      const SymbolType &reqThisType, const ArgList &reqArgs,
                                                      bool strictSpecifierMatching);
  static Function *matchFunction(Scope *matchScope, const std::string &reqName, const SymbolType &reqThisType,
                                 const ArgList &reqArgs, const std::vector<SymbolType> &reqTemplateTypes,
                                 bool strictSpecifierMatching, const ASTNode *callNode);

private:
  // Private methods
  [[nodiscard]] static Function *insertSubstantiation(Scope *insertScope, const Function &newManifestation,
                                                      const ASTNode *declNode);
  [[nodiscard]] static MatchResult matchManifestation(Function &candidate, Scope *&matchScope, const std::string &reqName,
                                                      const SymbolType &reqThisType, const ArgList &reqArgs,
                                                      TypeMapping &typeMapping, bool strictSpecifierMatching,
                                                      bool &forceSubstantiation, const ASTNode *callNode);
  [[nodiscard]] static bool matchName(const Function &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchThisType(Function &candidate, const SymbolType &reqThisType, TypeMapping &typeMapping,
                                          bool strictSpecifierMatching);
  [[nodiscard]] static bool matchArgTypes(Function &candidate, const ArgList &reqArgs, TypeMapping &typeMapping,
                                          bool strictSpecifierMatching, bool &needsSubstantiation, const ASTNode *callNode);
  static void substantiateReturnType(Function &candidate, TypeMapping &typeMapping);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Function &candidate,
                                                                          const std::string &templateTypeName);
};

} // namespace spice::compiler