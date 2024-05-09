// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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
using StructManifestationList = std::unordered_map</*mangledName=*/std::string, Struct>;
using StructRegistry = std::map</*structId=*/std::string, /*manifestationList=*/StructManifestationList>;

class StructManager {
public:
  // Public methods
  static Struct *insertStruct(Scope *insertScope, Struct &spiceStruct, std::vector<Struct *> *nodeStructList);
  [[nodiscard]] static Struct *matchStruct(Scope *matchScope, const std::string &reqName, const QualTypeList &reqTemplateTypes,
                                           const ASTNode *node);

private:
  // Private methods
  [[nodiscard]] static Struct *insertSubstantiation(Scope *insertScope, Struct &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Struct &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchTemplateTypes(Struct &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping);
  static void substantiateFieldTypes(Struct &candidate, TypeMapping &typeMapping);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Struct &candidate,
                                                                          const std::string &templateTypeName);
};

} // namespace spice::compiler