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
class Struct;
class Scope;
class Type;
class ASTNode;
class GenericType;

// Typedefs
using StructManifestationList = std::map</*mangledName=*/std::string, Struct>;
using StructRegistry = std::map</*structId=*/std::string, /*manifestationList=*/StructManifestationList>;

class StructManager {
public:
  // Public methods
  static Struct *insert(Scope *insertScope, Struct &spiceStruct, std::vector<Struct *> *nodeStructList);
  [[nodiscard]] static Struct *match(Scope *matchScope, const std::string &qt, const QualTypeList &reqTemplateTypes,
                                     const ASTNode *node);
  static void cleanup();
  [[nodiscard]] static std::string dumpLookupCacheStatistics();

private:
  // Private members
  static std::unordered_map<uint64_t, Struct *> lookupCache;
  static size_t lookupCacheHits;
  static size_t lookupCacheMisses;

  // Private methods
  [[nodiscard]] static Struct *insertSubstantiation(Scope *insertScope, Struct &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Struct &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchTemplateTypes(Struct &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping,
                                               const ASTNode *node);
  static void substantiateFieldTypes(Struct &candidate, const TypeMapping &typeMapping, const ASTNode *node);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Struct &candidate,
                                                                          const std::string &templateTypeName);
  [[nodiscard]] static uint64_t getCacheKey(const Scope *scope, const std::string &name, const QualTypeList &templateTypes);
};

} // namespace spice::compiler