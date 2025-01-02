// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
struct CodeLoc;
struct ExprResult;
class Function;
class Scope;
class SymbolTableEntry;
class Type;
class ASTNode;
class GenericType;
class TypeChecker;

// Typedefs
using FunctionManifestationList = std::unordered_map</*mangledName=*/std::string, Function>;
using FunctionRegistry = std::map</*fctId=*/std::string, /*manifestationList=*/FunctionManifestationList>;
using Arg = std::pair</*type=*/QualType, /*isTemporary=*/bool>;
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
  static Function *insert(Scope *insertScope, const Function &baseFunction, std::vector<Function *> *nodeFunctionList = nullptr);
  static void substantiateOptionalParams(const Function &baseFunction, std::vector<Function> &manifestations);
  [[nodiscard]] static Function createMainFunction(SymbolTableEntry *entry, const QualTypeList &paramTypes, ASTNode *declNode);
  [[nodiscard]] static const Function *lookup(Scope *matchScope, const std::string &reqName, const QualType &reqThisType,
                                              const ArgList &reqArgs, bool strictSpecifierMatching);
  static Function *match(TypeChecker *typeChecker, Scope *matchScope, const std::string &reqName, const QualType &reqThisType,
                         const ArgList &reqArgs, const QualTypeList &templateTypeHints, bool strictSpecifierMatching,
                         const ASTNode *callNode);
  static void clear();

private:
  // Private members
  static std::unordered_map<uint64_t, Function *> lookupCache;

  // Private methods
  [[nodiscard]] static Function *insertSubstantiation(Scope *insertScope, const Function &newManifestation,
                                                      const ASTNode *declNode);
  [[nodiscard]] static MatchResult matchManifestation(Function &candidate, Scope *&matchScope, const std::string &reqName,
                                                      const QualType &reqThisType, const ArgList &reqArgs,
                                                      TypeMapping &typeMapping, bool strictSpecifierMatching,
                                                      bool &forceSubstantiation, const ASTNode *callNode);
  [[nodiscard]] static bool matchName(const Function &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchThisType(Function &candidate, const QualType &reqThisType, TypeMapping &typeMapping,
                                          bool strictSpecifierMatching, const ASTNode *callNode);
  [[nodiscard]] static bool matchArgTypes(Function &candidate, const ArgList &reqArgs, TypeMapping &typeMapping,
                                          bool strictSpecifierMatching, bool &needsSubstantiation, const ASTNode *callNode);
  static void substantiateReturnType(Function &candidate, const TypeMapping &typeMapping, const ASTNode *callNode);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Function &candidate,
                                                                          const std::string &templateTypeName);
  [[nodiscard]] static uint64_t getCacheKey(Scope *scope, const std::string &name, const QualType &thisType, const ArgList &args,
                                            const QualTypeList &templateTypes);
};

} // namespace spice::compiler