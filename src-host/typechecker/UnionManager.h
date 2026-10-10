// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <symboltablebuilder/QualType.h>

namespace spice::compiler {

// Forward declarations
class Union;
class Scope;
class ASTNode;
class GenericType;

// Typedefs
using UnionManifestationList = std::map</*mangledName=*/std::string, Union>;
using UnionRegistry = std::map</*unionId=*/std::string, /*manifestationList=*/UnionManifestationList>;

class UnionManager {
public:
  // Public methods
  static Union *insert(Scope *insertScope, Union &spiceUnion, std::vector<Union *> *nodeUnionList);
  [[nodiscard]] static Union *match(Scope *matchScope, const std::string &qt, const QualTypeList &reqTemplateTypes,
                                    const ASTNode *node);
  static void cleanup();
  [[nodiscard]] static std::string dumpLookupCacheStatistics();

private:
  // Private members
  static std::unordered_map<uint64_t, Union *> lookupCache;
  static size_t lookupCacheHits;
  static size_t lookupCacheMisses;

  // Private methods
  [[nodiscard]] static Union *insertSubstantiation(Scope *insertScope, Union &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Union &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchTemplateTypes(Union &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping,
                                               const ASTNode *node);
  static void substantiateFieldTypes(Union &candidate, const TypeMapping &typeMapping, const ASTNode *node);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Union &candidate,
                                                                          const std::string &templateTypeName);
  [[nodiscard]] static uint64_t getCacheKey(const Scope *scope, const std::string &name, const QualTypeList &templateTypes);
};

} // namespace spice::compiler
