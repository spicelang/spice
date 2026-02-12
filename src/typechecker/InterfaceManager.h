// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <symboltablebuilder/QualType.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

// Forward declarations
class Interface;
class Scope;
class ASTNode;
class GenericType;
using TypeMapping = std::unordered_map</*typeName=*/std::string, /*concreteType=*/QualType>;

// Typedefs
using InterfaceManifestationList = std::map</*mangledName=*/std::string, Interface>;
using InterfaceRegistry = std::map<CodeLoc, InterfaceManifestationList>;

class InterfaceManager {
public:
  // Public methods
  static Interface *insert(Scope *insertScope, Interface &spiceInterface, std::vector<Interface *> *nodeInterfaceList);
  [[nodiscard]] static Interface *match(Scope *matchScope, const std::string &reqName, const QualTypeList &reqTemplateTypes,
                                        const ASTNode *node);
  static void cleanup();
  [[nodiscard]] static std::string dumpLookupCacheStatistics();

private:
  // Private members
  static std::unordered_map<uint64_t, Interface *> lookupCache;
  static size_t lookupCacheHits;
  static size_t lookupCacheMisses;

  // Private methods
  [[nodiscard]] static Interface *insertSubstantiation(Scope *insertScope, Interface &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Interface &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchTemplateTypes(Interface &candidate, const QualTypeList &reqTemplateTypes,
                                               TypeMapping &typeMapping, const ASTNode *node);
  static void substantiateSignatures(Interface &candidate, const TypeMapping &typeMapping, const ASTNode *node);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Interface &candidate,
                                                                          const std::string &templateTypeName);
  [[nodiscard]] static uint64_t getCacheKey(const Scope *scope, const std::string &name, const QualTypeList &templateTypes);
};

} // namespace spice::compiler