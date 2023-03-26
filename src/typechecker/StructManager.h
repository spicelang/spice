// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
class Struct;
class Scope;
class SymbolType;
class ASTNode;
struct CodeLoc;

// Typedefs
using StructManifestationList = std::unordered_map</*mangledName=*/std::string, /*structObject=*/Struct>;
using StructRegistry = std::unordered_map</*codeLoc=*/std::string, /*manifestations=*/StructManifestationList>;

class StructManager {
public:
  // Public methods
  [[nodiscard]] static Struct *insertStruct(Scope *insertScope, Struct &spiceStruct, std::vector<Struct *> *nodeStructList);
  [[nodiscard]] static Struct *matchStruct(Scope *matchScope, const std::string &requestedName,
                                           const std::vector<SymbolType> &requestedTemplateTypes, const ASTNode *node);

private:
  // Private methods
  [[nodiscard]] static Struct *insertSubstantiation(Scope *insertScope, Struct &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Struct &candidate, const std::string &requestedName);
  [[nodiscard]] static bool matchTemplateTypes(Struct &candidate, const std::vector<SymbolType> &requestedTemplateTypes,
                                               TypeMapping &typeMapping);
  static void substantiateFieldTypes(Struct &candidate, TypeMapping &typeMapping);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Struct &candidate,
                                                                          const std::string &templateTypeName);
};

} // namespace spice::compiler