// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

// Forward declarations
class Interface;
class Scope;

// Typedefs
using InterfaceManifestationList = std::unordered_map</*mangledName=*/std::string, Interface>;
using InterfaceRegistry = std::map<CodeLoc, InterfaceManifestationList>;

class InterfaceManager {
public:
  // Public methods
  static Interface *insertInterface(Scope *insertScope, Interface &spiceInterface, std::vector<Interface *> *nodeInterfaceList);
  [[nodiscard]] static Interface *matchInterface(Scope *matchScope, const std::string &reqName,
                                                 const std::vector<SymbolType> &reqTemplateTypes, const ASTNode *node);

private:
  // Private methods
  [[nodiscard]] static Interface *insertSubstantiation(Scope *insertScope, Interface &newManifestation, const ASTNode *declNode);
  [[nodiscard]] static bool matchName(const Interface &candidate, const std::string &reqName);
  [[nodiscard]] static bool matchTemplateTypes(Interface &candidate, const std::vector<SymbolType> &reqTemplateTypes,
                                               TypeMapping &typeMapping);
  static void substantiateSignatures(Interface &candidate, TypeMapping &typeMapping);
  [[nodiscard]] static const GenericType *getGenericTypeOfCandidateByName(const Interface &candidate,
                                                                          const std::string &templateTypeName);
};

} // namespace spice::compiler