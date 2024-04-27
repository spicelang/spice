// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
class SymbolType;

/**
 * Helper class for FunctionManager and StructManager to match generic types.
 * This is a tricky task, because the types can be indirect or/and nested.
 */
class TypeMatcher {
public:
  // Typedefs
  using ResolverFct = std::function<const GenericType *(const std::string &)>;

  // Public methods
  static bool matchRequestedToCandidateTypes(const std::vector<SymbolType> &candidateType,
                                             const std::vector<SymbolType> &reqTypes, TypeMapping &typeMapping,
                                             ResolverFct &resolverFct, bool strictSpecifiers);
  static bool matchRequestedToCandidateType(SymbolType candidateType, SymbolType requestedType, TypeMapping &typeMapping,
                                            ResolverFct &resolverFct, bool strictSpecifierMatching);
  static void substantiateTypesWithTypeMapping(std::vector<SymbolType> &symbolTypes, const TypeMapping &typeMapping);
  static void substantiateTypeWithTypeMapping(SymbolType &symbolType, const TypeMapping &typeMapping);
};

} // namespace spice::compiler