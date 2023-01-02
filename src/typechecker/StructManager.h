// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <typechecker/FunctionManager.h>

namespace spice::compiler {

// Forward declarations
class Struct;
class Scope;
class SymbolType;
class ASTNode;

using StructManifestationList = std::unordered_map</*mangledName=*/std::string, /*structObject=*/Struct>;
using StructRegistry = std::unordered_map</*codeLoc=*/std::string, /*manifestations=*/StructManifestationList>;

class StructManager {
public:
  // Public methods
  [[nodiscard]] static Struct *insertStruct(Scope *insertScope, const Struct &spiceStruct);
  [[nodiscard]] static StructManifestationList *getManifestationList(Scope *lookupScope, const CodeLoc &defCodeLoc);
  [[nodiscard]] static Struct *matchStruct(Scope *matchScope, const std::string &requestedName,
                                           const std::vector<SymbolType> &requestedTemplateTypes, const ASTNode *node);

private:
  // Private methods
  [[nodiscard]] static Struct *insertSubstantiation(Scope *insertScope, const Struct &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Struct &candidate, const std::string &requestedName);
  [[nodiscard]] static bool matchTemplateTypes(Struct &candidate, const std::vector<SymbolType> &requestedTemplateTypes,
                                               TypeMapping &typeMapping);
};

} // namespace spice::compiler